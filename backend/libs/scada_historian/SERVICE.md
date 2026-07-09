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
- модель запроса истории;
- repository-интерфейс history samples;
- черновая SQL-миграция tag history.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Historian находится после runtime-слоя:

    Polling -> Runtime -> Historian -> future PostgreSQL / TimescaleDB -> History API

Текущая внутренняя цепочка Historian:

    TagCurrentValue -> ArchiveDecision -> HistorySample -> HistorianBuffer -> HistoryBatch -> IHistoryBatchWriter

Будущая цепочка чтения истории:

    HistoryQuery -> IHistorySampleRepository -> HistoryQueryResult -> future API / UI charts

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
- `IHistorySampleRepository`;
- создание history sample из `TagCurrentValue`;
- принятие решения об архивировании;
- накопление samples в buffer;
- подготовку batch для будущей записи;
- контракт будущего batch writer;
- модель запроса истории;
- repository-интерфейс history samples;
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
- PostgreSQL repository implementation;
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

## Repository-интерфейс

### IHistorySampleRepository

Интерфейс будущего хранения history samples.

Методы:

- `find_by_id()`;
- `find_latest_by_tag_id()`;
- `query()`;
- `find_by_tag_id()`;
- `save()`;
- `save_batch()`;
- `remove_by_id()`;
- `remove_by_tag_id()`;
- `remove_older_than()`.

На текущем этапе реализации repository нет.

Будущие реализации:

- PostgreSQL;
- TimescaleDB;
- in-memory для тестов;
- mock для unit-тестов.

## База данных

Добавлена черновая миграция:

- `database/migrations/0006_tag_history.sql`.

Миграция описывает таблицу:

- `tag_history_samples`.

Таблица предназначена для append-only хранения истории значений тегов.

## Почему tag history отделена от runtime snapshots

`runtime_value_snapshots` хранит последнее состояние тега.

`tag_history_samples` хранит исторический временной ряд.

Пример:

    runtime_value_snapshots:
        Pump01.Current = 12.7 A

    tag_history_samples:
        10:00:00 Pump01.Current = 12.1 A
        10:00:01 Pump01.Current = 12.3 A
        10:00:02 Pump01.Current = 12.7 A

Это разные сценарии хранения и чтения.

## Почему tag history append-only

История значений должна быть надежной временной последовательностью.

Обычный сценарий — добавление новых samples.

Удаление возможно только через retention policy или административные операции.

## Почему пока нет TimescaleDB hypertable

Миграция написана как обычная PostgreSQL-схема.

Позже таблицу можно будет превратить в TimescaleDB hypertable.

Это позволит добавить:

- партиционирование по времени;
- compression;
- retention policies;
- continuous aggregates.

## Почему HistoryQuery отделен от DTO

`HistoryQuery` — доменная модель backend.

DTO для API находятся в `scada_contracts`.

Это нужно, чтобы:

- backend мог использовать типизированные идентификаторы;
- API мог использовать строки;
- domain model не зависела от JSON;
- mapper-слой можно было добавить позже.

## Почему query пока не выполняется

На текущем этапе еще нет:

- PostgreSQL implementation;
- TimescaleDB implementation;
- API layer;
- mapper layer.

Поэтому query model только описывает будущий запрос.

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
- `include/scada_historian/repository/IHistorySampleRepository.h`
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
- Добавлен `IHistorySampleRepository`.
- Добавлена миграция `0006_tag_history.sql`.
- Добавлен `get_historian_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Historian.
- Готовим модель исторической записи.
- Готовим правила принятия решения об архивировании.
- Готовим buffer и batch write contract.
- Готовим модель чтения истории.
- Готовим repository и database draft.

### Нужно доделать

- Обновить документацию и закрыть Sprint 007.
- Добавить PostgreSQL writer позже.
- Добавить TimescaleDB support позже.
- Добавить Runtime -> Historian integration позже.

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