# scada_historian

## Назначение

`scada_historian` — backend-модуль истории значений Dispatcher.

Модуль отвечает за подготовку, буферизацию и будущую запись исторических значений тегов.

На текущем этапе реализована базовая модель исторического sample и основа принятия решения об архивировании.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Historian находится после runtime-слоя:

    Polling -> Runtime -> Historian -> future PostgreSQL / TimescaleDB -> History API

## Границы ответственности

`scada_historian` отвечает за:

- `HistorySampleId`;
- `HistorySample`;
- `ArchiveDecisionReason`;
- `ArchiveDecisionOptions`;
- `ArchiveDecision`;
- создание history sample из `TagCurrentValue`;
- принятие решения об архивировании;
- подготовку к historian buffer;
- подготовку к batch write;
- подготовку к history query;
- подготовку к PostgreSQL / TimescaleDB storage.

## Что модуль не должен делать сейчас

На текущем шаге `scada_historian` не должен выполнять:

- запись в базу данных;
- подключение к PostgreSQL;
- применение миграций;
- буферизацию;
- batch write;
- history query;
- EventBus publish;
- Alarm evaluation;
- HTTP API;
- UI-логику.

## Основные сущности

### HistorySampleId

Типизированный идентификатор исторического sample.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

На текущем этапе sample id может быть пустым до момента сохранения.

Будущая repository-реализация сможет назначать id при записи.

### HistorySample

`HistorySample` — модель одной исторической записи значения тега.

Поля:

- `id`;
- `tag_id`;
- `value_type`;
- `quality`;
- `source`;
- `raw_value`;
- `engineering_value`;
- `timestamp`;
- `source_timestamp`;
- `server_timestamp`;
- `change_counter`;
- `engineering_unit`;
- `message`.

Методы:

- `has_sample_id()`;
- `has_tag_id()`;
- `has_value()`;
- `has_good_quality()`;
- `has_message()`;
- `has_valid_identity()`.

### make_history_sample_from_current_value

Функция создает `HistorySample` из `TagCurrentValue`.

Она копирует:

- `tag_id`;
- `value_type`;
- `quality`;
- `source`;
- `raw_value`;
- `engineering_value`;
- `timestamp`;
- `source_timestamp`;
- `server_timestamp`;
- `change_counter`.

Дополнительно можно передать:

- `engineering_unit`;
- `message`.

## Archive decision foundation

Archive decision отвечает на вопрос:

    нужно ли сохранить текущее значение в историю?

На текущем этапе решение принимает функция:

    decide_archive()

Функция учитывает:

- `TagArchivePolicy`;
- текущий `TagCurrentValue`;
- предыдущий `HistorySample`;
- `ArchiveDecisionOptions`.

### ArchiveDecisionReason

Причина принятого решения.

Поддерживаются значения:

- `None`;
- `InvalidInput`;
- `ArchiveDisabled`;
- `NoValue`;
- `FirstSample`;
- `AlwaysPolicy`;
- `ValueChanged`;
- `QualityChanged`;
- `ValueAndQualityChanged`;
- `DeadbandExceeded`;
- `DeadbandNotExceeded`;
- `PeriodicIntervalElapsed`;
- `PeriodicIntervalNotElapsed`;
- `AlarmTriggered`;
- `AlarmNotTriggered`;
- `Throttled`;
- `NoChange`.

### ArchiveDecisionOptions

Опции решения об архивировании.

Поля:

- `periodic_interval_ms`;
- `min_interval_ms`;
- `deadband`;
- `alarm_triggered`;
- `archive_first_sample`.

### ArchiveDecision

Результат решения об архивировании.

Поля:

- `tag_id`;
- `policy`;
- `reason`;
- `should_archive`;
- `first_sample`;
- `value_changed`;
- `quality_changed`;
- `throttled`;
- `message`.

Методы:

- `should_archive_sample()`;
- `is_first_sample()`;
- `has_value_changed()`;
- `has_quality_changed()`;
- `is_throttled()`;
- `has_message()`.

## Поддержанные политики архивирования

### Disabled

Архивирование отключено.

Результат:

    should_archive = false

### AlwaysButThrottled

Архивировать каждый принятый sample, если не сработал `min_interval_ms`.

Результат:

    should_archive = true

### OnChange

Архивировать, если изменилось `engineering_value`.

### OnQualityChange

Архивировать, если изменилось качество.

### OnChangeWithDeadband

Для числовых значений архивировать, если абсолютная разница между предыдущим и текущим `engineering_value` больше или равна `deadband`.

Для нечисловых значений fallback-поведение такое же, как у `OnChange`.

### Periodic

Архивировать, если прошел `periodic_interval_ms` относительно предыдущего sample.

### OnAlarm

Архивировать, если в `ArchiveDecisionOptions` передан признак `alarm_triggered`.

Полный Alarm Engine будет добавлен позже.

## Почему first sample архивируется отдельно

Если в истории нет предыдущего sample, Historian должен иметь возможность сохранить стартовое значение.

Это нужно для:

- начального отображения графиков;
- диагностики;
- восстановления контекста;
- будущих агрегатов.

Поведение можно отключить через:

    archive_first_sample = false

## Почему есть throttling

Даже если политика требует частой записи, система должна иметь защиту от чрезмерного количества samples.

`min_interval_ms` позволяет ограничить минимальный интервал между архивными записями.

Это особенно важно для политик:

- `AlwaysButThrottled`;
- `OnChange`;
- `OnChangeWithDeadband`.

## Почему archive decision не пишет в историю

Archive decision только принимает решение.

Запись должна выполняться будущими слоями:

    ArchiveDecision -> HistorianBuffer -> BatchWriter -> Repository

Это разделение нужно, чтобы отдельно тестировать правила архивирования и отдельно оптимизировать запись.

## Почему HistorySample отделен от TagCurrentValue

`TagCurrentValue` — это текущее состояние тега.

`HistorySample` — это историческая запись.

Они похожи, но имеют разные роли.

`TagCurrentValue` может перезаписываться много раз.

`HistorySample` должен быть append-only записью для будущей истории.

## Почему HistorySample не является RuntimeValueEvent

`RuntimeValueEvent` описывает факт изменения.

`HistorySample` описывает значение, которое можно сохранить в историю.

В будущем Event Manager и Historian будут разными подсистемами.

## Почему timestamp несколько

В SCADA важно различать несколько временных меток:

- `timestamp` — основная временная метка sample;
- `source_timestamp` — время источника, если источник его предоставил;
- `server_timestamp` — время обработки на сервере.

Для некоторых протоколов source timestamp может отсутствовать.

В таком случае runtime слой может использовать server timestamp.

## Почему есть raw_value и engineering_value

`raw_value` — значение до инженерного преобразования.

`engineering_value` — значение после применения scale / offset / transform.

Для диагностики и будущих расширений важно сохранять оба значения.

## Почему пока нет buffer

Historian не должен писать каждое значение в БД отдельным SQL-запросом.

Для производительности нужен buffer и batch writer.

Это будет добавлено отдельными шагами.

## Почему пока нет TimescaleDB

На первом этапе готовим обычную PostgreSQL-совместимую модель.

TimescaleDB можно будет подключить позже как расширение для time-series.

## Зависимости

`scada_historian` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_tags`.

Причины:

- нужны typed ids;
- нужен `ModuleInfo`;
- нужен `TagId`;
- нужен `TagCurrentValue`;
- нужен `TagValuePayload`;
- нужен `TagQuality`;
- нужен `TagValueSource`;
- нужен `TagArchivePolicy`.

## Основные файлы

- `include/scada_historian/history_ids.h`
- `include/scada_historian/history_sample.h`
- `include/scada_historian/archive_decision.h`
- `include/scada_historian/historian_module.h`
- `src/history_sample.cpp`
- `src/archive_decision.cpp`
- `src/historian_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_historian`.
- Добавлен `HistorySampleId`.
- Добавлен `HistorySample`.
- Добавлена функция `make_history_sample_from_current_value()`.
- Добавлен `ArchiveDecisionReason`.
- Добавлен `ArchiveDecisionOptions`.
- Добавлен `ArchiveDecision`.
- Добавлена функция `decide_archive()`.
- Добавлен `get_historian_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Historian.
- Готовим модель исторической записи.
- Готовим правила принятия решения об архивировании.
- Готовим основу для historian buffer и batch writer.

### Нужно доделать

- Добавить historian buffer.
- Добавить batch write contract.
- Добавить history query model.
- Добавить historian DTO.
- Добавить repository-интерфейсы.
- Добавить миграцию истории тегов.

### Расширить позже

- PostgreSQL writer.
- TimescaleDB hypertable.
- Retention policies.
- Compression policies.
- Downsampling.
- Aggregates.
- History API.
- Chart data API.
- Export.
- Diagnostics.
- Metrics.

### Не входит в этот шаг

- Buffer.
- Batch writer.
- PostgreSQL.
- TimescaleDB.
- HTTP API.
- Frontend.