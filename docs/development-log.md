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

### Шаг 1

Создана базовая проектная спецификация.

### Шаг 2

Создан план Sprint 001.

### Шаг 3

Создан список отложенных возможностей.

### Шаг 4

Создан roadmap первой версии.

### Шаг 5

Настроен CMakePresets под Visual Studio 2026.

### Шаг 6

Создана базовая структура репозитория.

Основные каталоги:

- `backend`;
- `frontend`;
- `database`;
- `docs`;
- `scripts`;
- `tools`;
- `tests`.

### Шаг 7

Создано приложение `dispatcher_server`.

### Шаг 8

Созданы базовые backend-библиотеки:

- `scada_common`;
- `scada_contracts`;
- `scada_core`.

### Шаг 9

В `scada_common` добавлены базовые общие типы.

### Шаг 10

В `scada_core` добавлены базовые core-типы.

### Шаг 11

Создана начальная документация сервисов.

### Шаг 12

Sprint 001 закрыт.

Обновлены:

- `development-log`;
- `known-limitations`.

Сборка и запуск прошли успешно.

## Результат Sprint 001

Создан минимальный технический фундамент Dispatcher.

Проект собирается, запускается и имеет базовую модульную структуру.

---

# Sprint 002 — Object Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую объектную модель Dispatcher.

Объектная модель должна стать фундаментом для будущих модулей:

- устройств;
- тегов;
- аварий;
- событий;
- дашбордов;
- мнемосхем;
- прав доступа;
- зон ответственности;
- ППР.

## Выполнено

### Шаг 13 — scada_objects

Добавлен модуль:

- `backend/libs/scada_objects`.

Добавлены основные элементы объектной модели:

- `ObjectType`;
- `ObjectNode`;
- `ObjectPath`;
- `get_object_module_info()`.

Модуль подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 14 — ObjectTree

Добавлен `ObjectTree`.

Добавлена базовая валидация дерева объектов.

Проверяются:

- пустой `id`;
- пустой `code`;
- пустой `name`;
- дублирующиеся `id`;
- отсутствующий `parent_id`;
- `parent_id == id`;
- циклические ссылки.

Сборка и запуск прошли успешно.

### Шаг 15 — ObjectHierarchyRules

Добавлены правила допустимой иерархии объектов.

Добавлены проверки:

- недопустимый тип корневого объекта;
- недопустимая связь parent/child.

Сборка и запуск прошли успешно.

### Шаг 16 — ResponsibilityZone

Добавлена сущность:

- `ResponsibilityZone`.

Зона ответственности связывает:

- объекты;
- пользователей;
- роли.

Добавлена базовая валидация зоны ответственности.

Сборка и запуск прошли успешно.

### Шаг 17 — Typed object identifiers

Добавлен общий шаблон:

- `EntityId<Tag>`.

Добавлены типизированные идентификаторы:

- `ObjectId`;
- `ResponsibilityZoneId`.

Сборка и запуск прошли успешно.

### Шаг 18 — Object DTO contracts

В модуль `scada_contracts` добавлены DTO-контракты объектной модели.

Добавлены:

- `ObjectNodeDto`;
- `ObjectTreeDto`;
- `ObjectTreeValidationIssueDto`;
- `ObjectTreeValidationResultDto`;
- `ResponsibilityZoneDto`;
- `ResponsibilityZoneValidationIssueDto`;
- `ResponsibilityZoneValidationResultDto`.

Сборка и запуск прошли успешно.

### Шаг 19 — Object repository interfaces

Добавлены repository-интерфейсы объектной модели:

- `IObjectRepository`;
- `IResponsibilityZoneRepository`.

Реализация PostgreSQL пока не добавлялась.

Сборка и запуск прошли успешно.

### Шаг 20 — Object database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0001_object_model.sql`.

Миграция описывает будущие таблицы:

- `object_nodes`;
- `responsibility_zones`;
- `responsibility_zone_objects`;
- `responsibility_zone_users`;
- `responsibility_zone_roles`.

Сборка и запуск прошли успешно.

### Шаг 21 — Sprint 002 docs update and close

Обновлена документация и закрыт Sprint 002.

Создан:

- `docs/SPRINT_002_SUMMARY.md`.

Сборка и запуск прошли успешно.

## Итог Sprint 002

Создан фундамент объектной модели Dispatcher.

---

# Sprint 003 — Device Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель устройств Dispatcher.

Модель устройств должна стать фундаментом для будущих модулей:

- тегов;
- polling;
- protocol drivers;
- диагностики связи;
- аварий связи;
- команд управления;
- истории;
- API;
- UI карточек устройств.

## Выполнено

### Шаг 22 — scada_devices

Добавлен модуль:

- `backend/libs/scada_devices`.

Добавлены:

- `DeviceId`;
- `DeviceProtocol`;
- `DeviceState`;
- `IpMode`;
- `DeviceConnection`;
- `Device`;
- `get_device_module_info()`.

Модуль подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 23 — Device validation and connection rules

Добавлена базовая валидация устройства.

Добавлены:

- `DeviceValidationCode`;
- `DeviceValidationIssue`;
- `DeviceValidationResult`;
- `validate_device()`;
- `requires_network_endpoint()`;
- `is_runtime_enabled_state()`.

Сборка и запуск прошли успешно.

### Шаг 24 — Device diagnostics model

Добавлена модель диагностики устройства.

Добавлены:

- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`.

Сборка и запуск прошли успешно.

### Шаг 25 — Device DTO contracts

В `scada_contracts` добавлены DTO-контракты устройств.

Добавлены:

- `DeviceConnectionDto`;
- `DeviceDto`;
- `DeviceListDto`;
- `DeviceValidationIssueDto`;
- `DeviceValidationResultDto`;
- `DeviceDiagnosticsDto`.

Сборка и запуск прошли успешно.

### Шаг 26 — Device repository interfaces

Добавлены repository-интерфейсы устройств:

- `IDeviceRepository`;
- `IDeviceDiagnosticsRepository`.

Реализация хранения пока не добавлялась.

Сборка и запуск прошли успешно.

### Шаг 27 — Device database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0002_device_model.sql`.

Миграция описывает будущие таблицы:

- `devices`;
- `device_diagnostics`.

Сборка и запуск прошли успешно.

### Шаг 28 — Sprint 003 docs update and close

Обновлена документация и закрыт Sprint 003.

Создан:

- `docs/SPRINT_003_SUMMARY.md`.

Сборка и запуск прошли успешно.

## Итог Sprint 003

Создан фундамент модели устройств Dispatcher.

---

# Sprint 004 — Tag Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель тегов Dispatcher.

Модель тегов должна стать фундаментом для будущих модулей:

- polling;
- protocol drivers;
- runtime values;
- historian;
- alarms;
- dashboards;
- mimic diagrams;
- commands;
- API;
- UI карточек тегов.

## Выполнено

### Шаг 29 — scada_tags

Добавлен модуль:

- `backend/libs/scada_tags`.

Добавлены:

- `TagId`;
- `TagType`;
- `TagValueType`;
- `TagQuality`;
- `TagArchivePolicy`;
- `TagAddress`;
- `Tag`;
- `get_tag_module_info()`.

Модуль подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 30 — Tag current value model

Добавлена модель текущего значения тега.

Добавлены:

- `TagValueSource`;
- `TagValuePayload`;
- `TagCurrentValue`.

Runtime-хранилище текущих значений пока не добавлялось.

Сборка и запуск прошли успешно.

### Шаг 31 — Tag validation and addressing rules

Добавлена базовая валидация тегов.

Добавлены:

- `TagValidationCode`;
- `TagValidationIssue`;
- `TagValidationResult`;
- `validate_tag()`;
- `requires_object()`;
- `requires_device()`;
- `requires_address()`;
- `supports_extraction()`;
- `is_valid_engineering_transform()`.

Сборка и запуск прошли успешно.

### Шаг 32 — Tag DTO contracts

В `scada_contracts` добавлены DTO-контракты тегов.

Добавлены:

- `TagAddressDto`;
- `TagDto`;
- `TagListDto`;
- `TagCurrentValueDto`;
- `TagValidationIssueDto`;
- `TagValidationResultDto`.

Сборка и запуск прошли успешно.

### Шаг 33 — Tag repository interfaces

Добавлены repository-интерфейсы тегов:

- `ITagRepository`;
- `ITagCurrentValueRepository`.

Реализация хранения пока не добавлялась.

Сборка и запуск прошли успешно.

### Шаг 34 — Tag database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0003_tag_model.sql`.

Миграция описывает будущие таблицы:

- `tags`;
- `tag_current_values`.

Tag history и Historian пока не добавлялись.

Сборка и запуск прошли успешно.

### Шаг 35 — Sprint 004 docs update and close

Обновлена документация и закрыт Sprint 004.

Создан:

- `docs/SPRINT_004_SUMMARY.md`.

Сборка и запуск прошли успешно.

## Итог Sprint 004

Создан фундамент модели тегов Dispatcher.

---

# Sprint 005 — Communication and Polling Foundation

## Статус

Закрывается на шаге 42.

## Цель спринта

Создать базовую архитектуру коммуникаций и опроса.

Sprint 005 должен был заложить границы между:

- устройствами;
- тегами;
- протокольными драйверами;
- polling configuration;
- scheduler foundation;
- будущим runtime values layer.

## Выполнено

### Шаг 36 — Protocol driver contracts

Добавлен модуль:

- `backend/libs/scada_protocols`.

Добавлены базовые контракты протокольного слоя:

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
- `get_protocol_module_info()`.

Модуль подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 37 — Simulator protocol driver foundation

Добавлен первый драйвер протокольного слоя:

- `SimulatorProtocolDriver`.

Драйвер реализует:

- `IProtocolDriver`;
- `check_connection()`;
- `test_address()`;
- `read()`;
- `read_batch()`;
- `write()`.

Simulator driver не выполняет сетевой обмен.

Он нужен для проверки polling architecture без реального оборудования.

Сборка и запуск прошли успешно.

### Шаг 38 — Protocol driver registry

Добавлен registry протокольных драйверов:

- `ProtocolDriverRegistry`;
- `create_default_protocol_driver_registry()`.

Registry хранит драйверы по `DeviceProtocol`.

На текущем этапе default registry содержит:

- `SimulatorProtocolDriver`.

Сборка и запуск прошли успешно.

### Шаг 39 — Polling task and group model

Добавлен модуль:

- `backend/libs/scada_polling`.

Добавлены базовые сущности polling:

- `PollingTaskId`;
- `PollingGroupId`;
- `PollingTaskState`;
- `PollingGroupMode`;
- `PollingTask`;
- `PollingGroup`;
- `get_polling_module_info()`.

Модуль подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 40 — Polling scheduler foundation

Добавлена минимальная синхронная основа scheduler.

Добавлены:

- `PollingExecutionStatus`;
- `PollingTaskExecutionResult`;
- `PollingGroupExecutionResult`;
- `PollingSchedulerOptions`;
- `PollingScheduler`.

`PollingScheduler` умеет выполнить одну polling group один раз через `ProtocolDriverRegistry`.

Поддержаны режимы:

- sequential read;
- batch read.

Scheduler пока не запускает фоновые потоки, не использует таймеры и не пишет значения в runtime storage.

Сборка и запуск прошли успешно.

### Шаг 41 — Polling DTO / repository / migration draft

В `scada_contracts` добавлены DTO polling:

- `PollingAddressDto`;
- `PollingTaskDto`;
- `PollingGroupDto`;
- `PollingTaskExecutionResultDto`;
- `PollingGroupExecutionResultDto`;
- `PollingValidationIssueDto`;
- `PollingTaskValidationResultDto`;
- `PollingGroupValidationResultDto`.

В `scada_polling` добавлены repository-интерфейсы:

- `IPollingTaskRepository`;
- `IPollingGroupRepository`.

Добавлена черновая SQL-миграция:

- `database/migrations/0004_polling_model.sql`.

Миграция описывает будущие таблицы:

- `polling_groups`;
- `polling_tasks`.

Сборка и запуск прошли успешно.

## Итог Sprint 005

В Sprint 005 создан фундамент коммуникаций и опроса Dispatcher.

Сейчас в проекте есть:

- модуль протокольных контрактов;
- общий интерфейс драйвера протокола;
- request/result модели протокольного слоя;
- capabilities протокольного драйвера;
- simulator protocol driver;
- registry протокольных драйверов;
- модуль polling;
- модель polling task;
- модель polling group;
- модель результата выполнения polling;
- минимальный синхронный polling scheduler;
- DTO polling;
- repository-интерфейсы polling;
- черновая SQL-миграция polling configuration.

## Что сознательно не делали в Sprint 005

В Sprint 005 не добавлялись:

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

Эти задачи будут выполняться в следующих спринтах.

---

# Текущее состояние после Sprint 005

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

Текущий технический фокус завершен.

Следующий логический фокус:

    Runtime Values and Data Engine

---

# Следующий спринт

## Sprint 006 — Runtime Values and Data Engine

Цель Sprint 006:

Создать runtime-слой текущих значений тегов.

Планируемые направления:

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

# Общий статус проекта

## Завершено

- Sprint 001 — Project Foundation.
- Sprint 002 — Object Model Foundation.
- Sprint 003 — Device Model Foundation.
- Sprint 004 — Tag Model Foundation.
- Sprint 005 — Communication and Polling Foundation.

## Следующий

- Sprint 006 — Runtime Values and Data Engine.