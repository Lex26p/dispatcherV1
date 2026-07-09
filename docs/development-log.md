# Dispatcher Development Log

## Назначение документа

Этот документ фиксирует ход разработки Dispatcher по спринтам и шагам.

Документ нужен для:

- отслеживания выполненных задач;
- понимания текущего состояния проекта;
- фиксации архитектурных решений;
- подготовки следующих спринтов;
- восстановления контекста разработки.

---

# Sprint 001 — Project Foundation

## Статус

Закрыт.

## Цель спринта

Создать технический фундамент проекта Dispatcher.

## Выполнено

### Шаги 1–12

Создана базовая проектная документация, структура репозитория, CMake-настройки, приложение `dispatcher_server` и базовые backend-модули:

- `scada_common`;
- `scada_contracts`;
- `scada_core`.

Добавлены базовые общие типы, core-типы, модульная регистрация и начальная документация сервисов.

## Итог Sprint 001

Создан минимальный технический фундамент Dispatcher.

Проект собирается, запускается и имеет базовую модульную структуру.

---

# Sprint 002 — Object Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую объектную модель Dispatcher.

## Выполнено

### Шаги 13–21

Добавлен модуль:

- `scada_objects`.

Реализованы:

- `ObjectId`;
- `ResponsibilityZoneId`;
- `ObjectType`;
- `ObjectNode`;
- `ObjectPath`;
- `ObjectTree`;
- `ObjectHierarchyRules`;
- `ResponsibilityZone`;
- DTO объектной модели;
- repository-интерфейсы объектной модели;
- миграция `0001_object_model.sql`.

## Итог Sprint 002

Создан фундамент объектной модели Dispatcher.

---

# Sprint 003 — Device Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель устройств Dispatcher.

## Выполнено

### Шаги 22–28

Добавлен модуль:

- `scada_devices`.

Реализованы:

- `DeviceId`;
- `Device`;
- `DeviceProtocol`;
- `DeviceState`;
- `DeviceConnection`;
- `IpMode`;
- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`;
- валидация устройств;
- DTO устройств;
- repository-интерфейсы устройств;
- миграция `0002_device_model.sql`.

## Итог Sprint 003

Создан фундамент модели устройств Dispatcher.

---

# Sprint 004 — Tag Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель тегов Dispatcher.

## Выполнено

### Шаги 29–35

Добавлен модуль:

- `scada_tags`.

Реализованы:

- `TagId`;
- `Tag`;
- `TagType`;
- `TagValueType`;
- `TagQuality`;
- `TagArchivePolicy`;
- `TagAddress`;
- `TagCurrentValue`;
- `TagValueSource`;
- `TagValuePayload`;
- валидация тегов;
- DTO тегов;
- repository-интерфейсы тегов;
- миграция `0003_tag_model.sql`.

## Итог Sprint 004

Создан фундамент модели тегов Dispatcher.

---

# Sprint 005 — Communication and Polling Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую архитектуру коммуникаций и опроса.

## Выполнено

### Шаг 36 — Protocol driver contracts

Добавлен модуль:

- `scada_protocols`.

Реализованы:

- `ProtocolCapabilities`;
- request/result модели протокольного слоя;
- `IProtocolDriver`;
- `get_protocol_module_info()`.

### Шаг 37 — Simulator protocol driver foundation

Добавлен:

- `SimulatorProtocolDriver`.

Драйвер реализует `IProtocolDriver` и используется для безопасной проверки polling architecture без реального оборудования.

### Шаг 38 — Protocol driver registry

Добавлен:

- `ProtocolDriverRegistry`;
- `create_default_protocol_driver_registry()`.

Default registry содержит simulator driver.

### Шаг 39 — Polling task and group model

Добавлен модуль:

- `scada_polling`.

Реализованы:

- `PollingTaskId`;
- `PollingGroupId`;
- `PollingTaskState`;
- `PollingGroupMode`;
- `PollingTask`;
- `PollingGroup`.

### Шаг 40 — Polling scheduler foundation

Добавлена минимальная синхронная основа scheduler.

Реализованы:

- `PollingExecutionStatus`;
- `PollingTaskExecutionResult`;
- `PollingGroupExecutionResult`;
- `PollingSchedulerOptions`;
- `PollingScheduler`.

### Шаг 41 — Polling DTO / repository / migration draft

Добавлены:

- DTO polling;
- `IPollingTaskRepository`;
- `IPollingGroupRepository`;
- миграция `0004_polling_model.sql`.

### Шаг 42 — Sprint 005 docs update and close

Sprint 005 закрыт.

## Итог Sprint 005

Создан фундамент коммуникаций и опроса Dispatcher.

---

# Sprint 006 — Runtime Values and Data Engine

## Статус

Закрыт.

## Цель спринта

Создать runtime-слой текущих значений тегов.

## Выполнено

### Шаг 43 — scada_runtime module and TagValueStore foundation

Добавлен модуль:

- `scada_runtime`.

Реализованы:

- `TagValueStore`;
- `get_runtime_module_info()`.

### Шаг 44 — Apply ProtocolReadResult to TagCurrentValue

Добавлены:

- `RuntimeValueApplyResult`;
- `RuntimeValueApplier`.

Реализована цепочка:

    ProtocolReadResult -> TagCurrentValue -> TagValueStore

### Шаг 45 — Runtime quality and last good value handling

Добавлены runtime quality helpers.

Реализовано обновление:

- `last_good_value`;
- `last_good_timestamp`.

Good runtime quality:

- `Good`;
- `Manual`;
- `Simulation`.

### Шаг 46 — Value conversion and engineering transform

Добавлены:

- runtime value conversion;
- engineering transform;
- overload `apply_protocol_read_result(read_result, tag)`.

Реализована формула:

    engineering_value = raw_value * scale + offset

### Шаг 47 — Runtime change detection and events foundation

Добавлены:

- `RuntimeValueChangeKind`;
- `RuntimeValueChangeResult`;
- `RuntimeEventType`;
- `RuntimeValueEvent`.

`change_counter` увеличивается только при изменении значения, качества или источника.

### Шаг 48 — Runtime DTO / repository / migration draft

Добавлены:

- DTO runtime;
- `IRuntimeValueSnapshotRepository`;
- `IRuntimeEventRepository`;
- миграция `0005_runtime_values.sql`.

### Шаг 49 — Sprint 006 docs update and close

Sprint 006 закрыт.

## Итог Sprint 006

Создан foundation runtime-слоя текущих значений Dispatcher.

---

# Sprint 007 — Historian Foundation

## Статус

Закрыт.

## Цель спринта

Создать фундамент истории значений тегов.

Sprint 007 подготовил цепочку:

    RuntimeValueEvent / TagCurrentValue -> archive decision -> HistorianBuffer -> batch write -> future PostgreSQL / TimescaleDB

## Выполнено

### Шаг 50 — scada_historian module and HistorySample model

Добавлен модуль:

- `scada_historian`.

Добавлены:

- `HistorySampleId`;
- `HistorySample`;
- `make_history_sample_from_current_value()`;
- `get_historian_module_info()`.

### Шаг 51 — Archive decision foundation

Добавлена основа принятия решения об архивировании.

Добавлены:

- `ArchiveDecisionReason`;
- `ArchiveDecisionOptions`;
- `ArchiveDecision`;
- `decide_archive()`.

Поддержаны политики:

- `Disabled`;
- `AlwaysButThrottled`;
- `OnChange`;
- `OnQualityChange`;
- `OnChangeWithDeadband`;
- `Periodic`;
- `OnAlarm`.

### Шаг 52 — Historian buffer and batch write contract

Добавлен in-memory buffer и контракт будущей batch-записи.

Добавлены:

- `HistorianBuffer`;
- `HistoryBatch`;
- `HistoryBatchWriteResult`;
- `IHistoryBatchWriter`.

### Шаг 53 — History query model

Добавлена доменная модель запроса истории.

Добавлены:

- `HistoryQueryTimeRange`;
- `HistoryQueryFilter`;
- `HistoryQueryOptions`;
- `HistoryQuery`;
- `HistoryQueryResult`;
- `validate_history_query()`.

### Шаг 54 — Historian DTO / repository / migration draft

Добавлены:

- DTO historian;
- `IHistorySampleRepository`;
- миграция `0006_tag_history.sql`.

### Шаг 55 — Sprint 007 docs update and close

Sprint 007 закрыт.

## Итог Sprint 007

Создан foundation Historian.

---

# Sprint 008 — Events and Alarms Foundation

## Статус

Закрывается на шаге 61.

## Цель спринта

Создать фундамент событий и аварий Dispatcher.

Sprint 008 должен был подготовить цепочку:

    Runtime / Historian / Polling / Devices -> Events -> Alarms -> future Event Manager / Alarm Manager / UI

## Выполнено

### Шаг 56 — scada_events module and EventRecord foundation

Добавлен модуль:

- `scada_events`.

Добавлены:

- `EventId`;
- `EventTimestamp`;
- `EventCategory`;
- `EventSeverity`;
- `EventSourceType`;
- `EventRecord`;
- `make_event_record()`;
- `get_event_module_info()`.

`scada_events` подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 57 — scada_alarms module and Alarm model foundation

Добавлен модуль:

- `scada_alarms`.

Добавлены:

- `AlarmId`;
- `AlarmTimestamp`;
- `AlarmSourceType`;
- `AlarmSeverity`;
- `AlarmPriority`;
- `AlarmState`;
- `AlarmRecord`;
- `make_alarm_record()`;
- `to_event_severity()`;
- `get_alarm_module_info()`.

`scada_alarms` подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 58 — Alarm lifecycle and transitions

Добавлен lifecycle foundation аварий.

Добавлены:

- `AlarmTransitionType`;
- `AlarmTransitionValidationCode`;
- `AlarmTransitionRequest`;
- `AlarmTransitionRecord`;
- `AlarmTransitionResult`;
- `can_apply_alarm_transition()`;
- `apply_alarm_transition()`.

Поддержаны базовые переходы:

- `New -> Active`;
- `New -> Acknowledged`;
- `Active -> Acknowledged`;
- `New -> Cleared`;
- `Active -> Cleared`;
- `Acknowledged -> Cleared`;
- `Cleared -> Closed`;
- `New -> Shelved`;
- `Active -> Shelved`;
- `Acknowledged -> Shelved`;
- `Shelved -> Active`;
- `New -> Suppressed`;
- `Active -> Suppressed`;
- `Acknowledged -> Suppressed`;
- `Suppressed -> Active`;
- `Cleared -> Active`.

Сборка и запуск прошли успешно.

### Шаг 59 — Alarm rules foundation

Добавлен foundation alarm rules.

Добавлены:

- `AlarmRuleId`;
- `AlarmRuleType`;
- `AlarmRuleState`;
- `AlarmRuleComparison`;
- `AlarmRuleEvaluationStatus`;
- `AlarmRule`;
- `AlarmRuleEvaluationResult`;
- `evaluate_alarm_rule()`.

Поддержаны типы правил:

- `NumericThreshold`;
- `QualityEquals`;
- `QualityNotGood`;
- `QualityBad`.

`scada_alarms` получил зависимость от `scada_tags`, потому что alarm rules проверяют `TagCurrentValue`.

Сборка и запуск прошли успешно.

### Шаг 60 — Events/Alarms DTO / repository / migration draft

В `scada_contracts` добавлены DTO events:

- `EventRecordDto`;
- `EventListDto`;
- `EventValidationIssueDto`;
- `EventValidationResultDto`.

В `scada_contracts` добавлены DTO alarms:

- `AlarmRecordDto`;
- `AlarmTransitionDto`;
- `AlarmRuleDto`;
- `AlarmRuleEvaluationResultDto`;
- `AlarmListDto`;
- `AlarmValidationIssueDto`;
- `AlarmValidationResultDto`.

В `scada_events` добавлен repository-интерфейс:

- `IEventRecordRepository`.

В `scada_alarms` добавлены repository-интерфейсы:

- `IAlarmRepository`;
- `IAlarmTransitionRepository`;
- `IAlarmRuleRepository`.

Добавлена черновая SQL-миграция:

- `database/migrations/0007_events_alarms.sql`.

Миграция описывает таблицы:

- `event_records`;
- `alarm_records`;
- `alarm_transitions`;
- `alarm_rules`.

Сборка и запуск прошли успешно.

## Итог Sprint 008

В Sprint 008 создан foundation событий и аварий.

Сейчас в проекте есть:

- модуль `scada_events`;
- модель `EventRecord`;
- базовая классификация событий;
- модуль `scada_alarms`;
- модель `AlarmRecord`;
- severity и priority аварий;
- lifecycle transitions аварий;
- foundation alarm rules;
- DTO events;
- DTO alarms;
- repository-интерфейсы events;
- repository-интерфейсы alarms;
- черновая SQL-миграция events/alarms.

## Что сознательно не делали в Sprint 008

В Sprint 008 не добавлялись:

- Event Manager;
- Alarm Manager;
- PostgreSQL repository implementation;
- Runtime -> Events integration;
- Runtime -> Alarms integration;
- EventBus publish;
- WebSocket delivery;
- HTTP API;
- frontend alarm journal;
- frontend active alarm panel;
- unit-тесты.

Эти задачи будут выполняться в следующих спринтах.

---

# Текущее состояние после Sprint 008

Проект находится в состоянии:

    Dispatcher backend foundation
        +
    Object Model foundation
        +
    Device Model foundation
        +
    Tag Model foundation
        +
    Communication and Polling foundation
        +
    Runtime Values foundation
        +
    Historian foundation
        +
    Events and Alarms foundation

Текущий технический фокус завершен.

Следующий логический фокус:

    API and Realtime Gateway

---

# Следующий спринт

## Sprint 009 — API and Realtime Gateway

Цель Sprint 009:

Создать минимальный backend API и realtime gateway foundation.

Предварительные направления:

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

# Общий статус проекта

## Завершено

- Sprint 001 — Project Foundation.
- Sprint 002 — Object Model Foundation.
- Sprint 003 — Device Model Foundation.
- Sprint 004 — Tag Model Foundation.
- Sprint 005 — Communication and Polling Foundation.
- Sprint 006 — Runtime Values and Data Engine.
- Sprint 007 — Historian Foundation.
- Sprint 008 — Events and Alarms Foundation.

## Следующий

- Sprint 009 — API and Realtime Gateway.