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

### Шаг 15 — ObjectHierarchyRules

Добавлены правила допустимой иерархии объектов.

Добавлены проверки:

- недопустимый тип корневого объекта;
- недопустимая связь parent/child.

### Шаг 16 — ResponsibilityZone

Добавлена сущность:

- `ResponsibilityZone`.

Зона ответственности связывает:

- объекты;
- пользователей;
- роли.

Добавлена базовая валидация зоны ответственности.

### Шаг 17 — Typed object identifiers

Добавлен общий шаблон:

- `EntityId<Tag>`.

Добавлены типизированные идентификаторы:

- `ObjectId`;
- `ResponsibilityZoneId`.

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

### Шаг 19 — Object repository interfaces

Добавлены repository-интерфейсы объектной модели:

- `IObjectRepository`;
- `IResponsibilityZoneRepository`.

### Шаг 20 — Object database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0001_object_model.sql`.

Миграция описывает будущие таблицы объектной модели.

### Шаг 21 — Sprint 002 docs update and close

Обновлена документация и закрыт Sprint 002.

Создан:

- `docs/SPRINT_002_SUMMARY.md`.

## Итог Sprint 002

Создан фундамент объектной модели Dispatcher.

---

# Sprint 003 — Device Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель устройств Dispatcher.

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

### Шаг 23 — Device validation and connection rules

Добавлена базовая валидация устройства.

Добавлены:

- `DeviceValidationCode`;
- `DeviceValidationIssue`;
- `DeviceValidationResult`;
- `validate_device()`;
- `requires_network_endpoint()`;
- `is_runtime_enabled_state()`.

### Шаг 24 — Device diagnostics model

Добавлена модель диагностики устройства.

Добавлены:

- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`.

### Шаг 25 — Device DTO contracts

В `scada_contracts` добавлены DTO-контракты устройств.

Добавлены:

- `DeviceConnectionDto`;
- `DeviceDto`;
- `DeviceListDto`;
- `DeviceValidationIssueDto`;
- `DeviceValidationResultDto`;
- `DeviceDiagnosticsDto`.

### Шаг 26 — Device repository interfaces

Добавлены repository-интерфейсы устройств:

- `IDeviceRepository`;
- `IDeviceDiagnosticsRepository`.

### Шаг 27 — Device database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0002_device_model.sql`.

Миграция описывает будущие таблицы:

- `devices`;
- `device_diagnostics`.

### Шаг 28 — Sprint 003 docs update and close

Обновлена документация и закрыт Sprint 003.

Создан:

- `docs/SPRINT_003_SUMMARY.md`.

## Итог Sprint 003

Создан фундамент модели устройств Dispatcher.

---

# Sprint 004 — Tag Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель тегов Dispatcher.

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

### Шаг 30 — Tag current value model

Добавлена модель текущего значения тега.

Добавлены:

- `TagValueSource`;
- `TagValuePayload`;
- `TagCurrentValue`.

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

### Шаг 32 — Tag DTO contracts

В `scada_contracts` добавлены DTO-контракты тегов.

Добавлены:

- `TagAddressDto`;
- `TagDto`;
- `TagListDto`;
- `TagCurrentValueDto`;
- `TagValidationIssueDto`;
- `TagValidationResultDto`.

### Шаг 33 — Tag repository interfaces

Добавлены repository-интерфейсы тегов:

- `ITagRepository`;
- `ITagCurrentValueRepository`.

### Шаг 34 — Tag database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0003_tag_model.sql`.

Миграция описывает будущие таблицы:

- `tags`;
- `tag_current_values`.

### Шаг 35 — Sprint 004 docs update and close

Обновлена документация и закрыт Sprint 004.

Создан:

- `docs/SPRINT_004_SUMMARY.md`.

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

### Шаг 38 — Protocol driver registry

Добавлен registry протокольных драйверов:

- `ProtocolDriverRegistry`;
- `create_default_protocol_driver_registry()`.

Default registry содержит:

- `SimulatorProtocolDriver`.

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

### Шаг 40 — Polling scheduler foundation

Добавлена минимальная синхронная основа scheduler.

Добавлены:

- `PollingExecutionStatus`;
- `PollingTaskExecutionResult`;
- `PollingGroupExecutionResult`;
- `PollingSchedulerOptions`;
- `PollingScheduler`.

`PollingScheduler` умеет выполнить одну polling group один раз через `ProtocolDriverRegistry`.

### Шаг 41 — Polling DTO / repository / migration draft

В `scada_contracts` добавлены DTO polling.

В `scada_polling` добавлены repository-интерфейсы:

- `IPollingTaskRepository`;
- `IPollingGroupRepository`.

Добавлена черновая SQL-миграция:

- `database/migrations/0004_polling_model.sql`.

### Шаг 42 — Sprint 005 docs update and close

Обновлена документация и закрыт Sprint 005.

Создан:

- `docs/SPRINT_005_SUMMARY.md`.

## Итог Sprint 005

Создан фундамент коммуникаций и опроса Dispatcher.

---

# Sprint 006 — Runtime Values and Data Engine

## Статус

Закрывается на шаге 49.

## Цель спринта

Создать runtime-слой текущих значений тегов.

Sprint 006 должен был заложить фундамент для цепочки:

    Polling -> ProtocolReadResult -> Runtime -> TagCurrentValue -> future EventBus / Historian / Alarms

## Выполнено

### Шаг 43 — scada_runtime module and TagValueStore foundation

Добавлен модуль:

- `backend/libs/scada_runtime`.

Добавлены:

- `TagValueStore`;
- `get_runtime_module_info()`.

`TagValueStore` умеет:

- хранить `TagCurrentValue` в памяти;
- искать значение по `TagId`;
- получать все текущие значения;
- проверять наличие значения;
- обновлять или добавлять значение;
- удалять значение;
- очищать store.

Модуль подключен к CMake и к `dispatcher_server`.

Сборка и запуск прошли успешно.

### Шаг 44 — Apply ProtocolReadResult to TagCurrentValue

Добавлен первый runtime-processing слой:

- `RuntimeValueApplyResult`;
- `RuntimeValueApplier`.

Добавлено применение:

    ProtocolReadResult -> TagCurrentValue -> TagValueStore

На этом шаге `RuntimeValueApplier` начал:

- принимать `ProtocolReadResult`;
- создавать или обновлять `TagCurrentValue`;
- записывать значение в `TagValueStore`;
- переносить `raw_value`;
- временно делать `engineering_value = raw_value`;
- переносить quality;
- переносить timestamps;
- увеличивать `change_counter`.

Сборка и запуск прошли успешно.

### Шаг 45 — Runtime quality and last good value handling

Добавлены runtime quality helpers.

Добавлены правила good runtime quality:

- `Good`;
- `Manual`;
- `Simulation`.

Добавлено автоматическое обновление:

- `last_good_value`;
- `last_good_timestamp`.

`last_good_value` обновляется только при хорошем качестве и непустом значении.

При плохом качестве предыдущее последнее хорошее значение сохраняется.

Сборка и запуск прошли успешно.

### Шаг 46 — Value conversion and engineering transform

Добавлены:

- `RuntimeValueConversionResult`;
- `convert_runtime_value()`;
- `infer_runtime_value_type()`;
- `runtime_value_to_double()`;
- `runtime_value_to_string()`;
- `RuntimeEngineeringTransformOptions`;
- `RuntimeEngineeringTransformResult`;
- `apply_engineering_transform()`.

`RuntimeValueApplier` получил overload:

- `apply_protocol_read_result(read_result)`;
- `apply_protocol_read_result(read_result, tag)`.

Метод с `Tag` использует:

- `Tag.value_type`;
- `Tag.scale`;
- `Tag.offset`.

Добавлено преобразование:

    raw value -> typed raw value -> engineering value

Сборка и запуск прошли успешно.

### Шаг 47 — Runtime change detection and events foundation

Добавлены:

- `RuntimeValueChangeKind`;
- `RuntimeValueChangeResult`;
- `detect_runtime_value_change()`;
- `RuntimeEventType`;
- `RuntimeValueEvent`;
- `make_runtime_value_event()`.

Добавлена базовая логика change detection.

Runtime сравнивает:

- предыдущее `engineering_value`;
- новое `engineering_value`;
- предыдущее `quality`;
- новое `quality`;
- предыдущий `source`;
- новый `source`.

`change_counter` теперь увеличивается только при реальном изменении.

При изменении формируется `RuntimeValueEvent`.

Событие пока только возвращается в `RuntimeValueApplyResult` и не публикуется в EventBus.

Сборка и запуск прошли успешно.

### Шаг 48 — Runtime DTO / repository / migration draft

В `scada_contracts` добавлены DTO runtime:

- `RuntimeValueDto`;
- `RuntimeValueEventDto`;
- `RuntimeValueApplyResultDto`;
- `RuntimeValueSnapshotDto`;
- `RuntimeValueValidationIssueDto`;
- `RuntimeValueValidationResultDto`.

В `scada_runtime` добавлены repository-интерфейсы:

- `IRuntimeValueSnapshotRepository`;
- `IRuntimeEventRepository`.

Добавлена черновая SQL-миграция:

- `database/migrations/0005_runtime_values.sql`.

Миграция описывает будущие таблицы:

- `runtime_value_snapshots`;
- `runtime_value_events`.

Сборка и запуск прошли успешно.

## Итог Sprint 006

В Sprint 006 создан foundation runtime-слоя текущих значений Dispatcher.

Сейчас в проекте есть:

- модуль `scada_runtime`;
- in-memory `TagValueStore`;
- применение `ProtocolReadResult` к `TagCurrentValue`;
- runtime quality helpers;
- last good value handling;
- value conversion;
- engineering transform;
- change detection;
- runtime event foundation;
- runtime DTO;
- runtime repository interfaces;
- черновая SQL-миграция runtime snapshots/events.

## Что сознательно не делали в Sprint 006

В Sprint 006 не добавлялись:

- потокобезопасность `TagValueStore`;
- интеграция polling -> runtime;
- EventBus publish;
- Historian;
- Alarm Engine;
- deadband;
- archive policy evaluation;
- PostgreSQL repository implementation;
- HTTP API;
- WebSocket;
- frontend;
- unit-тесты.

Эти задачи будут выполняться в следующих спринтах.

---

# Текущее состояние после Sprint 006

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

Текущий технический фокус завершен.

Следующий логический фокус:

    Historian Foundation

---

# Следующий спринт

## Sprint 007 — Historian Foundation

Цель Sprint 007:

Создать фундамент истории значений тегов.

Предварительные направления:

- `scada_historian`;
- history sample model;
- archive decision foundation;
- historian buffer;
- batch write contract;
- history query contract;
- historian DTO;
- repository interfaces;
- database migration draft for tag history.

---

# Общий статус проекта

## Завершено

- Sprint 001 — Project Foundation.
- Sprint 002 — Object Model Foundation.
- Sprint 003 — Device Model Foundation.
- Sprint 004 — Tag Model Foundation.
- Sprint 005 — Communication and Polling Foundation.
- Sprint 006 — Runtime Values and Data Engine.

## Следующий

- Sprint 007 — Historian Foundation.