# scada_historian

## Назначение

`scada_historian` — backend-модуль истории значений Dispatcher.

Модуль отвечает за подготовку, буферизацию и будущую запись исторических значений тегов.

На текущем этапе реализованы:

- базовая модель исторического sample;
- основа принятия решения об архивировании;
- in-memory historian buffer;
- модель history batch;
- контракт будущей пакетной записи;
- модель запроса истории.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Historian находится после runtime-слоя:

    Polling -> Runtime -> Historian -> future PostgreSQL / TimescaleDB -> History API

Текущая внутренняя цепочка Historian:

    TagCurrentValue -> ArchiveDecision -> HistorySample -> HistorianBuffer -> HistoryBatch -> IHistoryBatchWriter

Будущая цепочка чтения истории:

    HistoryQuery -> IHistoryRepository -> HistoryQueryResult -> future API / UI charts

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
- `HistoryQueryTimeRange`;
- `HistoryQueryFilter`;
- `HistoryQueryOptions`;
- `HistoryQuery`;
- `HistoryQueryValidationIssue`;
- `HistoryQueryValidationResult`;
- `HistoryQueryResult`;
- создание history sample из `TagCurrentValue`;
- принятие решения об архивировании;
- накопление samples в buffer;
- подготовку batch для будущей записи;
- контракт будущего batch writer;
- модель запроса истории;
- подготовку к PostgreSQL / TimescaleDB storage;
- подготовку к History API.

## Что модуль не должен делать сейчас

На текущем шаге `scada_historian` не должен выполнять:

- запись в базу данных;
- чтение из базы данных;
- подключение к PostgreSQL;
- применение миграций;
- реальный batch write;
- выполнение history query;
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

## Historian buffer

`HistorianBuffer` — in-memory buffer исторических samples.

Он нужен, чтобы Historian не писал каждое значение в БД отдельным запросом.

На текущем этапе buffer хранит samples только в памяти процесса.

После остановки приложения buffer теряется.

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

`IHistoryBatchWriter` — интерфейс будущей пакетной записи history samples.

Метод:

- `write_batch()`.

На текущем этапе реализации writer нет.

## History query model

History query model описывает будущий запрос к истории тегов.

На текущем этапе query model не выполняет чтение из базы данных.

Он только описывает параметры запроса и результат.

### HistoryQueryOrder

Порядок сортировки истории.

Поддерживаются значения:

- `Ascending`;
- `Descending`.

### HistoryQueryTimeRange

Диапазон времени.

Поля:

- `from`;
- `to`.

Методы:

- `has_from()`;
- `has_to()`;
- `has_range()`;
- `is_valid()`.

Если указаны оба значения, `from` должен быть меньше или равен `to`.

### HistoryQueryFilter

Фильтры запроса.

Поля:

- `tag_ids`;
- `quality`;
- `value_type`;
- `source`.

Методы:

- `has_tag_filter()`;
- `has_quality_filter()`;
- `has_value_type_filter()`;
- `has_source_filter()`;
- `has_any_filter()`.

### HistoryQueryOptions

Опции запроса.

Поля:

- `order`;
- `limit`;
- `offset`;
- `max_limit`;
- `include_raw_value`;
- `include_message`.

Методы:

- `normalized_limit()`;
- `has_offset()`.

`normalized_limit()` защищает future API от слишком больших запросов.

### HistoryQuery

Полный доменный запрос истории.

Поля:

- `time_range`;
- `filter`;
- `options`.

Методы:

- `has_time_range()`;
- `has_filters()`;
- `is_valid()`.

### HistoryQueryValidationIssue

Одна ошибка валидации запроса.

Поля:

- `code`;
- `message`.

### HistoryQueryValidationResult

Результат валидации history query.

Поля:

- `issues`.

Методы:

- `is_valid()`;
- `has_issues()`.

### HistoryQueryResult

Результат будущего выполнения history query.

Поля:

- `samples`;
- `total_count`;
- `truncated`;
- `message`.

Методы:

- `empty()`;
- `size()`;
- `has_message()`.

## Почему HistoryQuery отделен от DTO

`HistoryQuery` — доменная модель backend.

DTO для API будут добавлены отдельно в `scada_contracts`.

Это нужно, чтобы:

- backend мог использовать типизированные идентификаторы;
- API мог использовать строки;
- domain model не зависела от JSON;
- mapper-слой можно было добавить позже.

## Почему query пока не выполняется

На текущем этапе еще нет:

- history repository;
- PostgreSQL implementation;
- TimescaleDB implementation;
- schema migration for tag history;
- API layer.

Поэтому query model только описывает будущий запрос.

## Почему есть max_limit

History API может вернуть очень много данных.

`max_limit` нужен для защиты:

- API;
- UI;
- базы данных;
- памяти процесса.

Будущий API сможет устанавливать свой максимум.

## Почему есть include_raw_value и include_message

Для графиков обычно нужен только engineering value.

Для диагностики может понадобиться raw value и message.

Эти флаги позволяют будущему repository/API оптимизировать объем данных.

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
- `include/scada_historian/history_query.h`
- `include/scada_historian/historian_module.h`
- `src/history_sample.cpp`
- `src/archive_decision.cpp`
- `src/history_batch.cpp`
- `src/historian_buffer.cpp`
- `src/history_query.cpp`
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
- Добавлен history query model.
- Добавлен `get_historian_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Historian.
- Готовим модель исторической записи.
- Готовим правила принятия решения об архивировании.
- Готовим buffer и batch write contract.
- Готовим модель чтения истории.

### Нужно доделать

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