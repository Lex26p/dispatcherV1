# scada_historian

## Назначение

`scada_historian` — backend-модуль истории значений Dispatcher.

Модуль отвечает за подготовку, буферизацию и будущую запись исторических значений тегов.

На текущем этапе реализованы:

- базовая модель исторического sample;
- основа принятия решения об архивировании;
- in-memory historian buffer;
- модель history batch;
- контракт будущей пакетной записи.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Historian находится после runtime-слоя:

    Polling -> Runtime -> Historian -> future PostgreSQL / TimescaleDB -> History API

Текущая внутренняя цепочка Historian:

    TagCurrentValue -> ArchiveDecision -> HistorySample -> HistorianBuffer -> HistoryBatch -> IHistoryBatchWriter

## Границы ответственности

`scada_historian` отвечает за:

- `HistorySampleId`;
- `HistorySample`;
- `ArchiveDecisionReason`;
- `ArchiveDecisionOptions`;
- `ArchiveDecision`;
- `HistorianBufferOptions`;
- `HistorianBufferStats`;
- `HistorianBufferPushResult`;
- `HistorianBuffer`;
- `HistoryBatch`;
- `HistoryBatchWriteStatus`;
- `HistoryBatchWriteResult`;
- `IHistoryBatchWriter`;
- создание history sample из `TagCurrentValue`;
- принятие решения об архивировании;
- накопление samples в buffer;
- подготовку batch для будущей записи;
- контракт будущего batch writer;
- подготовку к history query;
- подготовку к PostgreSQL / TimescaleDB storage.

## Что модуль не должен делать сейчас

На текущем шаге `scada_historian` не должен выполнять:

- запись в базу данных;
- подключение к PostgreSQL;
- применение миграций;
- реальный batch write;
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

## Historian buffer

`HistorianBuffer` — in-memory buffer исторических samples.

Он нужен, чтобы Historian не писал каждое значение в БД отдельным запросом.

На текущем этапе buffer хранит samples только в памяти процесса.

После остановки приложения buffer теряется.

### HistorianBufferOptions

Опции buffer.

Поля:

- `max_samples`;
- `flush_threshold`;
- `drop_oldest_when_full`.

`max_samples` задает максимальное количество samples в buffer.

`flush_threshold` задает порог, после которого buffer рекомендует flush.

`drop_oldest_when_full` определяет поведение при переполнении.

Если `drop_oldest_when_full = false`, новые samples отклоняются при полном buffer.

Если `drop_oldest_when_full = true`, самый старый sample удаляется, а новый принимается.

### HistorianBufferStats

Статистика buffer.

Поля:

- `size`;
- `capacity`;
- `flush_threshold`;
- `accepted_count`;
- `rejected_count`;
- `dropped_count`;
- `drained_count`;
- `full`;
- `flush_recommended`.

### HistorianBufferPushResult

Результат добавления sample в buffer.

Поля:

- `accepted`;
- `rejected`;
- `dropped_oldest`;
- `size_after`;
- `message`.

Методы:

- `is_accepted()`;
- `is_rejected()`;
- `has_dropped_oldest()`;
- `has_message()`.

### HistorianBuffer

Основные методы:

- `options()`;
- `empty()`;
- `is_full()`;
- `should_flush()`;
- `size()`;
- `capacity()`;
- `flush_threshold()`;
- `stats()`;
- `push()`;
- `push_many()`;
- `drain()`;
- `drain_up_to()`;
- `peek_all()`;
- `clear()`.

`drain()` возвращает `HistoryBatch` и удаляет samples из buffer.

`peek_all()` возвращает копию samples без удаления.

## History batch

`HistoryBatch` — контейнер samples для будущей пакетной записи.

Поля:

- `samples`.

Методы:

- `empty()`;
- `size()`;
- `add()`;
- `clear()`.

## Batch write contract

### HistoryBatchWriteStatus

Статус будущей batch-записи.

Поддерживаются значения:

- `NotStarted`;
- `Succeeded`;
- `PartialSuccess`;
- `Failed`;
- `Skipped`.

### HistoryBatchWriteResult

Результат будущей batch-записи.

Поля:

- `status`;
- `attempted_count`;
- `written_count`;
- `failed_count`;
- `message`.

Методы:

- `is_success()`;
- `has_failures()`;
- `has_message()`.

### IHistoryBatchWriter

Интерфейс будущей пакетной записи history samples.

Метод:

- `write_batch()`.

На текущем этапе реализации writer нет.

В будущем возможны реализации:

- PostgreSQL writer;
- TimescaleDB writer;
- file writer для диагностики;
- mock writer для тестов.

## Почему buffer отделен от writer

Buffer отвечает за временное накопление samples.

Writer отвечает за запись batch во внешнее хранилище.

Это разные ответственности.

Разделение позволяет:

- тестировать buffer без БД;
- менять стратегию записи;
- использовать разные writer-реализации;
- добавить retry policy позже;
- добавить disk spool позже.

## Почему drain удаляет samples

`drain()` моделирует передачу владения batch-записью.

После drain samples больше не находятся в buffer.

В будущем при ошибке записи можно будет вернуть failed samples в buffer или disk spool.

## Почему пока нет retry

Retry зависит от конкретного writer и типа ошибки.

Это будет добавлено позже вместе с PostgreSQL writer и diagnostics.

## Почему пока нет disk spool

Disk spool нужен для важных тегов, чтобы не терять данные при проблемах с БД.

Это более сложная подсистема.

На текущем этапе делаем только in-memory buffer.

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
- `include/scada_historian/history_batch.h`
- `include/scada_historian/historian_buffer.h`
- `include/scada_historian/historian_module.h`
- `src/history_sample.cpp`
- `src/archive_decision.cpp`
- `src/history_batch.cpp`
- `src/historian_buffer.cpp`
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
- Добавлен `HistoryBatch`.
- Добавлен `HistoryBatchWriteStatus`.
- Добавлен `HistoryBatchWriteResult`.
- Добавлен `IHistoryBatchWriter`.
- Добавлен `HistorianBuffer`.
- Добавлен `get_historian_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Historian.
- Готовим модель исторической записи.
- Готовим правила принятия решения об архивировании.
- Готовим buffer и batch write contract.

### Нужно доделать

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
- Retry policy.
- Disk spool.

### Не входит в этот шаг

- PostgreSQL writer.
- TimescaleDB.
- Repository implementation.
- Runtime integration.
- HTTP API.
- Frontend.