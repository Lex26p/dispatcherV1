# scada_historian

## Назначение

`scada_historian` — backend-модуль истории значений Dispatcher.

Модуль отвечает за подготовку, буферизацию и будущую запись исторических значений тегов.

На текущем шаге реализована только базовая модель исторического sample.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Historian находится после runtime-слоя:

    Polling -> Runtime -> Historian -> future PostgreSQL / TimescaleDB -> History API

## Границы ответственности

`scada_historian` отвечает за:

- `HistorySampleId`;
- `HistorySample`;
- создание history sample из `TagCurrentValue`;
- подготовку к archive decision;
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

## Почему HistorySample отделен от TagCurrentValue

`TagCurrentValue` — это текущее состояние тега.

`HistorySample` — это историческая запись.

Они похожи, но имеют разные роли.

`TagCurrentValue` может перезаписываться много раз.

`HistorySample` должен быть append-only записью для будущей истории.

## Почему HistorySample не является RuntimeValueEvent

`RuntimeValueEvent` описывает факт изменения.

`HistorySample` описывает значение, которое можно сохранить в историю.

Пример:

    RuntimeValueEvent:
        Tag changed from quality Good to Timeout

    HistorySample:
        tag_id = Pump01.Current
        engineering_value = 12.7
        quality = Good
        timestamp = 10:00:02

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

## Почему пока нет archive decision

Не каждое изменение должно попадать в историю.

Решение о записи зависит от:

- `TagArchivePolicy`;
- изменения значения;
- изменения качества;
- deadband;
- периодического режима;
- аварийного режима;
- throttling.

Эти правила будут добавлены следующим шагом.

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
- нужен `TagValueSource`.

## Основные файлы

- `include/scada_historian/history_ids.h`
- `include/scada_historian/history_sample.h`
- `include/scada_historian/historian_module.h`
- `src/history_sample.cpp`
- `src/historian_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_historian`.
- Добавлен `HistorySampleId`.
- Добавлен `HistorySample`.
- Добавлена функция `make_history_sample_from_current_value()`.
- Добавлен `get_historian_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Historian.
- Готовим модель исторической записи.
- Готовим основу для archive decision и buffer.

### Нужно доделать

- Добавить archive decision foundation.
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

- Archive decision.
- Buffer.
- Batch writer.
- PostgreSQL.
- TimescaleDB.
- HTTP API.
- Frontend.