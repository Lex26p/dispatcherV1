# Sprint 005 Summary

## Название спринта

Communication and Polling Foundation

## Статус

Closed

## Диапазон шагов

Шаги 36–42.

---

# Цель спринта

Создать базовую архитектуру коммуникаций и опроса Dispatcher.

Sprint 005 должен был заложить фундамент для будущих модулей:

- Modbus TCP;
- SNMP;
- Simulator;
- polling scheduler;
- runtime values;
- diagnostics;
- historian;
- alarms.

---

# Выполненные шаги

## Шаг 36

Добавлены контракты протокольных драйверов.

Результат:

- создан модуль `scada_protocols`;
- добавлен `ProtocolCapabilities`;
- добавлен `ProtocolConnectionCheckRequest`;
- добавлен `ProtocolConnectionCheckResult`;
- добавлен `ProtocolAddressTestRequest`;
- добавлен `ProtocolAddressTestResult`;
- добавлен `ProtocolReadRequest`;
- добавлен `ProtocolReadResult`;
- добавлен `ProtocolBatchReadRequest`;
- добавлен `ProtocolBatchReadResult`;
- добавлен `ProtocolWriteRequest`;
- добавлен `ProtocolWriteResult`;
- добавлен `IProtocolDriver`;
- добавлен `get_protocol_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 37

Добавлен simulator protocol driver.

Результат:

- добавлен `SimulatorProtocolDriver`;
- реализован `IProtocolDriver`;
- добавлена проверка simulator connection;
- добавлена проверка simulator address;
- добавлено чтение одного значения;
- добавлено batch read;
- добавлена dry run запись;
- добавлены простые simulator address rules.

Simulator driver не выполняет сетевой обмен.

Он используется как безопасный локальный источник значений для проверки polling foundation.

---

## Шаг 38

Добавлен registry протокольных драйверов.

Результат:

- добавлен `ProtocolDriverRegistry`;
- добавлен `create_default_protocol_driver_registry()`;
- добавлена регистрация драйвера;
- добавлена замена драйвера;
- добавлено удаление драйвера;
- добавлен поиск драйвера по `DeviceProtocol`;
- default registry создает `SimulatorProtocolDriver`.

Registry нужен, чтобы polling scheduler не зависел от конкретных классов драйверов.

---

## Шаг 39

Добавлена модель polling task и polling group.

Результат:

- создан модуль `scada_polling`;
- добавлен `PollingTaskId`;
- добавлен `PollingGroupId`;
- добавлен `PollingTaskState`;
- добавлен `PollingGroupMode`;
- добавлен `PollingTask`;
- добавлен `PollingGroup`;
- добавлен `get_polling_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 40

Добавлена основа polling scheduler.

Результат:

- добавлен `PollingExecutionStatus`;
- добавлен `PollingTaskExecutionResult`;
- добавлен `PollingGroupExecutionResult`;
- добавлен `PollingSchedulerOptions`;
- добавлен `PollingScheduler`;
- добавлен один синхронный запуск polling group;
- добавлен выбор batch/sequential execution;
- добавлена работа через `ProtocolDriverRegistry`.

Scheduler пока не запускает фоновые потоки, таймеры и не пишет значения в runtime storage.

---

## Шаг 41

Добавлены DTO, repository-интерфейсы и черновая миграция polling.

Результат:

- добавлен `PollingAddressDto`;
- добавлен `PollingTaskDto`;
- добавлен `PollingGroupDto`;
- добавлен `PollingTaskExecutionResultDto`;
- добавлен `PollingGroupExecutionResultDto`;
- добавлен `PollingValidationIssueDto`;
- добавлен `PollingTaskValidationResultDto`;
- добавлен `PollingGroupValidationResultDto`;
- добавлен `IPollingTaskRepository`;
- добавлен `IPollingGroupRepository`;
- создан `database/migrations/0004_polling_model.sql`.

---

## Шаг 42

Обновлена документация и закрыт Sprint 005.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_005_SUMMARY.md`.

---

# Итоговые артефакты

## Backend protocol layer

Основной модуль:

- `backend/libs/scada_protocols`

Основные сущности:

- `ProtocolCapabilities`;
- `ProtocolConnectionCheckRequest`;
- `ProtocolConnectionCheckResult`;
- `ProtocolAddressTestRequest`;
- `ProtocolAddressTestResult`;
- `ProtocolReadRequest`;
- `ProtocolReadResult`;
- `ProtocolBatchReadRequest`;
- `ProtocolBatchReadResult`;
- `ProtocolWriteRequest`;
- `ProtocolWriteResult`;
- `IProtocolDriver`;
- `SimulatorProtocolDriver`;
- `ProtocolDriverRegistry`.

---

## Backend polling layer

Основной модуль:

- `backend/libs/scada_polling`

Основные сущности:

- `PollingTaskId`;
- `PollingGroupId`;
- `PollingTaskState`;
- `PollingGroupMode`;
- `PollingTask`;
- `PollingGroup`;
- `PollingExecutionStatus`;
- `PollingTaskExecutionResult`;
- `PollingGroupExecutionResult`;
- `PollingSchedulerOptions`;
- `PollingScheduler`.

Repository-интерфейсы:

- `IPollingTaskRepository`;
- `IPollingGroupRepository`.

---

## Contracts

Основной модуль:

- `backend/libs/scada_contracts`

Добавлены DTO:

- `PollingAddressDto`;
- `PollingTaskDto`;
- `PollingGroupDto`;
- `PollingTaskExecutionResultDto`;
- `PollingGroupExecutionResultDto`;
- `PollingValidationIssueDto`;
- `PollingTaskValidationResultDto`;
- `PollingGroupValidationResultDto`.

---

## Database

Добавлена миграция:

- `database/migrations/0004_polling_model.sql`.

Миграция описывает таблицы:

- `polling_groups`;
- `polling_tasks`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_005_SUMMARY.md`.

---

# Что не входило в Sprint 005

В Sprint 005 сознательно не входили:

- реальный Modbus TCP driver;
- реальный SNMP driver;
- Boost.Asio network layer;
- фоновые polling threads;
- timers;
- бесконечный polling loop;
- runtime TagValueStore;
- запись текущих значений;
- EventBus integration;
- Historian integration;
- Alarm integration;
- PostgreSQL repository implementation;
- HTTP API;
- frontend;
- unit-тесты.

---

# Архитектурный результат

После Sprint 005 Dispatcher имеет фундамент коммуникаций и опроса.

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
    future runtime values / historian / alarms

Контрактный слой развивается отдельно:

    scada_contracts
        ↓
    future API / frontend / integrations

---

# Текущий flow после Sprint 005

Текущий доступный архитектурный flow:

    PollingGroup
        ↓
    PollingTask
        ↓
    ProtocolDriverRegistry
        ↓
    IProtocolDriver
        ↓
    SimulatorProtocolDriver
        ↓
    ProtocolReadResult
        ↓
    PollingGroupExecutionResult

Пока flow останавливается на результате polling execution.

Следующий спринт должен добавить runtime values layer.

---

# Следующий спринт

## Sprint 006 — Runtime Values and Data Engine

Предварительная цель:

Создать runtime-слой текущих значений тегов.

Ожидаемые направления:

- `scada_runtime`;
- `TagValueStore`;
- applying `ProtocolReadResult` to `TagCurrentValue`;
- quality handling;
- last good value update;
- change detection;
- value conversion;
- engineering transform;
- basic runtime events;
- integration point between polling and runtime values.

---

# Статус закрытия

Sprint 005 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.