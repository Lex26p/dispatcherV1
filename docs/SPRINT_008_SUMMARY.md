# Sprint 008 Summary

## Название спринта

Events and Alarms Foundation

## Статус

Closed

## Диапазон шагов

Шаги 56–61.

---

# Цель спринта

Создать фундамент событий и аварий Dispatcher.

Sprint 008 должен был подготовить цепочку:

    Runtime / Historian / Polling / Devices -> Events -> Alarms -> future Event Manager / Alarm Manager / UI

---

# Выполненные шаги

## Шаг 56

Создан модуль `scada_events`.

Результат:

- создан каталог `backend/libs/scada_events`;
- добавлен `EventId`;
- добавлен `EventTimestamp`;
- добавлен `EventCategory`;
- добавлен `EventSeverity`;
- добавлен `EventSourceType`;
- добавлен `EventRecord`;
- добавлена функция `make_event_record()`;
- добавлен `get_event_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 57

Создан модуль `scada_alarms`.

Результат:

- создан каталог `backend/libs/scada_alarms`;
- добавлен `AlarmId`;
- добавлен `AlarmTimestamp`;
- добавлен `AlarmSourceType`;
- добавлен `AlarmSeverity`;
- добавлен `AlarmPriority`;
- добавлен `AlarmState`;
- добавлен `AlarmRecord`;
- добавлена функция `make_alarm_record()`;
- добавлена функция `to_event_severity()`;
- добавлен `get_alarm_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 58

Добавлен alarm lifecycle foundation.

Результат:

- добавлен `AlarmTransitionType`;
- добавлен `AlarmTransitionValidationCode`;
- добавлен `AlarmTransitionRequest`;
- добавлен `AlarmTransitionRecord`;
- добавлен `AlarmTransitionResult`;
- добавлена функция `can_apply_alarm_transition()`;
- добавлена функция `apply_alarm_transition()`.

Поддержаны базовые transitions:

- `Activate`;
- `Acknowledge`;
- `Clear`;
- `Close`;
- `Shelve`;
- `Unshelve`;
- `Suppress`;
- `Unsuppress`;
- `Reactivate`.

---

## Шаг 59

Добавлен alarm rules foundation.

Результат:

- добавлен `AlarmRuleId`;
- добавлен `AlarmRuleType`;
- добавлен `AlarmRuleState`;
- добавлен `AlarmRuleComparison`;
- добавлен `AlarmRuleEvaluationStatus`;
- добавлен `AlarmRule`;
- добавлен `AlarmRuleEvaluationResult`;
- добавлена функция `evaluate_alarm_rule()`.

Поддержаны типы правил:

- `NumericThreshold`;
- `QualityEquals`;
- `QualityNotGood`;
- `QualityBad`.

С этого шага `scada_alarms` зависит от `scada_tags`.

---

## Шаг 60

Добавлены DTO, repository-интерфейсы и черновая миграция.

Результат в `scada_contracts`:

- добавлены DTO events;
- добавлены DTO alarms.

Результат в `scada_events`:

- добавлен `IEventRecordRepository`.

Результат в `scada_alarms`:

- добавлен `IAlarmRepository`;
- добавлен `IAlarmTransitionRepository`;
- добавлен `IAlarmRuleRepository`.

Результат в `database/migrations`:

- создан `0007_events_alarms.sql`.

Миграция описывает таблицы:

- `event_records`;
- `alarm_records`;
- `alarm_transitions`;
- `alarm_rules`.

---

## Шаг 61

Обновлена документация и закрыт Sprint 008.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_008_SUMMARY.md`.

---

# Итоговые артефакты

## Backend events layer

Основной модуль:

- `backend/libs/scada_events`.

Основные сущности:

- `EventId`;
- `EventTimestamp`;
- `EventCategory`;
- `EventSeverity`;
- `EventSourceType`;
- `EventRecord`.

Repository-интерфейсы:

- `IEventRecordRepository`.

---

## Backend alarms layer

Основной модуль:

- `backend/libs/scada_alarms`.

Основные сущности:

- `AlarmId`;
- `AlarmRuleId`;
- `AlarmTimestamp`;
- `AlarmSourceType`;
- `AlarmSeverity`;
- `AlarmPriority`;
- `AlarmState`;
- `AlarmRecord`;
- `AlarmTransitionType`;
- `AlarmTransitionValidationCode`;
- `AlarmTransitionRequest`;
- `AlarmTransitionRecord`;
- `AlarmTransitionResult`;
- `AlarmRuleType`;
- `AlarmRuleState`;
- `AlarmRuleComparison`;
- `AlarmRuleEvaluationStatus`;
- `AlarmRule`;
- `AlarmRuleEvaluationResult`.

Repository-интерфейсы:

- `IAlarmRepository`;
- `IAlarmTransitionRepository`;
- `IAlarmRuleRepository`.

---

## Contracts

Основной модуль:

- `backend/libs/scada_contracts`.

Добавлены DTO events:

- `EventRecordDto`;
- `EventListDto`;
- `EventValidationIssueDto`;
- `EventValidationResultDto`.

Добавлены DTO alarms:

- `AlarmRecordDto`;
- `AlarmTransitionDto`;
- `AlarmRuleDto`;
- `AlarmRuleEvaluationResultDto`;
- `AlarmListDto`;
- `AlarmValidationIssueDto`;
- `AlarmValidationResultDto`.

---

## Database

Добавлена миграция:

- `database/migrations/0007_events_alarms.sql`.

Миграция описывает таблицы:

- `event_records`;
- `alarm_records`;
- `alarm_transitions`;
- `alarm_rules`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_008_SUMMARY.md`.

---

# Что не входило в Sprint 008

В Sprint 008 сознательно не входили:

- Event Manager;
- Alarm Manager;
- PostgreSQL repository implementation;
- runtime integration;
- historian integration;
- EventBus integration;
- WebSocket delivery;
- HTTP API;
- frontend alarm journal;
- frontend active alarm panel;
- unit-тесты.

---

# Архитектурный результат

После Sprint 008 Dispatcher имеет foundation Events and Alarms layer.

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
    scada_events
        ↓
    scada_alarms
        ↓
    future API / realtime gateway / UI

Контрактный слой развивается отдельно:

    scada_contracts
        ↓
    future API / frontend / integrations

---

# Текущий flow после Sprint 008

Текущий доступный event flow:

    make_event_record()
        ↓
    EventRecord
        ↓
    future IEventRecordRepository / Event Manager

Текущий доступный alarm lifecycle flow:

    AlarmRecord
        ↓
    AlarmTransitionRequest
        ↓
    apply_alarm_transition()
        ↓
    AlarmTransitionResult
        ↓
    AlarmTransitionRecord

Текущий доступный alarm rule flow:

    TagCurrentValue
        ↓
    AlarmRule
        ↓
    evaluate_alarm_rule()
        ↓
    AlarmRuleEvaluationResult
        ↓
    optional AlarmRecord

Пока flow не пишет данные в PostgreSQL и не интегрирован автоматически с runtime.

---

# Следующий спринт

## Sprint 009 — API and Realtime Gateway

Предварительная цель:

Создать минимальный backend API и realtime gateway foundation.

Ожидаемые направления:

- HTTP API foundation;
- runtime values API;
- objects/devices/tags read API;
- history query API draft;
- events/alarms read API draft;
- WebSocket/SSE realtime gateway foundation;
- DTO mapper foundation;
- application composition;
- server startup flow.

---

# Статус закрытия

Sprint 008 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.
