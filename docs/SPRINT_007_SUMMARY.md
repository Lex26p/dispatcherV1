# Sprint 007 Summary

## Название спринта

Historian Foundation

## Статус

Closed

## Диапазон шагов

Шаги 50–55.

---

# Цель спринта

Создать фундамент истории значений тегов.

Sprint 007 должен был подготовить цепочку:

    RuntimeValueEvent / TagCurrentValue -> archive decision -> HistorianBuffer -> batch write -> future PostgreSQL / TimescaleDB

---

# Выполненные шаги

## Шаг 50

Создан модуль `scada_historian`.

Результат:

- создан каталог `backend/libs/scada_historian`;
- добавлен `HistorySampleId`;
- добавлен `HistorySample`;
- добавлена функция `make_history_sample_from_current_value()`;
- добавлен `get_historian_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 51

Добавлен archive decision foundation.

Результат:

- добавлен `ArchiveDecisionReason`;
- добавлен `ArchiveDecisionOptions`;
- добавлен `ArchiveDecision`;
- добавлена функция `decide_archive()`.

Решение об архивировании учитывает:

- `TagArchivePolicy`;
- текущий `TagCurrentValue`;
- предыдущий `HistorySample`;
- `ArchiveDecisionOptions`.

Поддержаны политики:

- `Disabled`;
- `AlwaysButThrottled`;
- `OnChange`;
- `OnQualityChange`;
- `OnChangeWithDeadband`;
- `Periodic`;
- `OnAlarm`.

---

## Шаг 52

Добавлены historian buffer и batch write contract.

Результат:

- добавлен `HistorianBufferOptions`;
- добавлен `HistorianBufferStats`;
- добавлен `HistorianBufferPushResult`;
- добавлен `HistorianBuffer`;
- добавлен `HistoryBatch`;
- добавлен `HistoryBatchWriteStatus`;
- добавлен `HistoryBatchWriteResult`;
- добавлен `IHistoryBatchWriter`.

Текущая цепочка:

    HistorySample -> HistorianBuffer -> HistoryBatch -> IHistoryBatchWriter

---

## Шаг 53

Добавлена модель запроса истории.

Результат:

- добавлен `HistoryQueryTimeRange`;
- добавлен `HistoryQueryFilter`;
- добавлен `HistoryQueryOptions`;
- добавлен `HistoryQuery`;
- добавлен `HistoryQueryValidationIssue`;
- добавлен `HistoryQueryValidationResult`;
- добавлен `HistoryQueryResult`;
- добавлена функция `validate_history_query()`.

Модель готовит будущие запросы истории по:

- тегам;
- времени;
- quality;
- value type;
- source;
- limit;
- offset;
- order.

---

## Шаг 54

Добавлены historian DTO, repository-интерфейс и черновая миграция.

Результат:

- добавлен `HistorySampleDto`;
- добавлен `ArchiveDecisionDto`;
- добавлен `HistoryBatchDto`;
- добавлен `HistoryBatchWriteResultDto`;
- добавлен `HistoryQueryDto`;
- добавлен `HistoryQueryResultDto`;
- добавлен `HistoryValidationIssueDto`;
- добавлен `HistoryValidationResultDto`;
- добавлен `IHistorySampleRepository`;
- создан `database/migrations/0006_tag_history.sql`.

Миграция описывает таблицу:

- `tag_history_samples`.

---

## Шаг 55

Обновлена документация и закрыт Sprint 007.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_007_SUMMARY.md`.

---

# Итоговые артефакты

## Backend historian layer

Основной модуль:

- `backend/libs/scada_historian`

Основные сущности:

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
- `HistoryQueryResult`.

Repository-интерфейсы:

- `IHistorySampleRepository`.

---

## Contracts

Основной модуль:

- `backend/libs/scada_contracts`

Добавлены DTO:

- `HistorySampleDto`;
- `ArchiveDecisionDto`;
- `HistoryBatchDto`;
- `HistoryBatchWriteResultDto`;
- `HistoryQueryDto`;
- `HistoryQueryResultDto`;
- `HistoryValidationIssueDto`;
- `HistoryValidationResultDto`.

---

## Database

Добавлена миграция:

- `database/migrations/0006_tag_history.sql`.

Миграция описывает таблицу:

- `tag_history_samples`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_007_SUMMARY.md`.

---

# Что не входило в Sprint 007

В Sprint 007 сознательно не входили:

- PostgreSQL writer;
- TimescaleDB hypertable;
- migration runner;
- Runtime -> Historian integration;
- EventBus integration;
- Alarm integration;
- History API;
- charts API;
- frontend;
- unit-тесты.

---

# Архитектурный результат

После Sprint 007 Dispatcher имеет foundation Historian layer.

Текущая архитектурная цепочка:

    scada_common
        ↓
    scada_core
        ↓
    scada_objects
        ↓
    scada_devices
        ↓
    scada_tags
        ↓
    scada_protocols
        ↓
    scada_polling
        ↓
    scada_runtime
        ↓
    scada_historian
        ↓
    future events / alarms / API / UI

Контрактный слой развивается отдельно:

    scada_contracts
        ↓
    future API / frontend / integrations

---

# Текущий flow после Sprint 007

Текущий доступный historian flow:

    TagCurrentValue
        ↓
    ArchiveDecision
        ↓
    HistorySample
        ↓
    HistorianBuffer
        ↓
    HistoryBatch
        ↓
    IHistoryBatchWriter

Пока flow не пишет данные в PostgreSQL и не интегрирован автоматически с runtime.

---

# Следующий спринт

## Sprint 008 — Events and Alarms Foundation

Предварительная цель:

Создать фундамент событий и аварий.

Ожидаемые направления:

- `scada_events`;
- `scada_alarms`;
- event model;
- alarm model;
- alarm severity;
- alarm priority;
- alarm lifecycle;
- alarm transition model;
- alarm rules foundation;
- event/alarm DTO;
- repository interfaces;
- database migration draft.

---

# Статус закрытия

Sprint 007 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.
