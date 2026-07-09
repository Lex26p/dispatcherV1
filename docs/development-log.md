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

Закрывается на шаге 35.

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

---

## Выполнено

### Шаг 29 — scada_tags

Добавлен модуль:

- `backend/libs/scada_tags`.

Добавлены базовые элементы модели тегов:

- `TagId`;
- `TagType`;
- `TagValueType`;
- `TagQuality`;
- `TagArchivePolicy`;
- `TagAddress`;
- `Tag`;
- `get_tag_module_info()`.

Модуль подключен к CMake и к `dispatcher_server`.

`dispatcher_server` теперь отображает модуль:

- `scada_tags | Tags | Running`.

Сборка и запуск прошли успешно.

---

### Шаг 30 — Tag current value model

Добавлена модель текущего значения тега.

Добавлены:

- `TagValueSource`;
- `TagValuePayload`;
- `TagCurrentValue`.

Модель текущего значения содержит:

- `tag_id`;
- `value_type`;
- `quality`;
- `source`;
- `raw_value`;
- `engineering_value`;
- `timestamp`;
- `source_timestamp`;
- `server_timestamp`;
- `last_good_value`;
- `last_good_timestamp`;
- `change_counter`.

Runtime-хранилище текущих значений пока не добавлялось.

Сборка и запуск прошли успешно.

---

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

Проверяются:

- идентичность тега;
- привязка к объекту;
- привязка к устройству;
- необходимость адреса;
- корректность `scale`;
- корректность `offset`;
- корректность `bit_index`;
- корректность `bit_length`;
- корректность `shift`;
- совместимость extraction с типом значения.

Сборка и запуск прошли успешно.

---

### Шаг 32 — Tag DTO contracts

В `scada_contracts` добавлены DTO-контракты тегов.

Добавлены:

- `TagAddressDto`;
- `TagDto`;
- `TagListDto`;
- `TagCurrentValueDto`;
- `TagValidationIssueDto`;
- `TagValidationResultDto`.

Зафиксировано правило:

- `scada_contracts` не зависит от `scada_tags`;
- идентификаторы в DTO передаются строками;
- enum-значения в DTO передаются строками;
- значения тегов в DTO передаются строками;
- timestamps в DTO передаются строками.

Сборка и запуск прошли успешно.

---

### Шаг 33 — Tag repository interfaces

Добавлены repository-интерфейсы тегов.

Добавлены:

- `ITagRepository`;
- `ITagCurrentValueRepository`.

Интерфейсы подготовлены для будущих реализаций:

- in-memory;
- PostgreSQL;
- mock;
- file-based;
- runtime TagValueStore.

Реализация хранения пока не добавлялась.

Сборка и запуск прошли успешно.

---

### Шаг 34 — Tag database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0003_tag_model.sql`.

Миграция описывает будущие таблицы:

- `tags`;
- `tag_current_values`.

Миграция зависит от:

- `object_nodes`;
- `devices`.

Tag history и Historian пока не добавлялись.

Сборка и запуск прошли успешно.

---

## Итог Sprint 004

В Sprint 004 создан фундамент модели тегов Dispatcher.

Сейчас в проекте есть:

- модуль тегов;
- типизированный идентификатор тега;
- базовая сущность тега;
- модель адресации тега;
- модель типа тега;
- модель типа значения;
- модель качества;
- модель политики архивирования;
- модель текущего значения;
- source-модель текущего значения;
- value payload;
- базовая валидация тегов;
- правила адресации;
- правила extraction;
- DTO-контракты тегов;
- repository-интерфейсы тегов;
- черновая SQL-миграция тегов.

---

## Что сознательно не делали в Sprint 004

В Sprint 004 не добавлялись:

- Polling Engine;
- Modbus TCP driver;
- SNMP driver;
- runtime TagValueStore;
- реальные текущие значения из устройств;
- Historian;
- tag history;
- alarm rules;
- command execution;
- PostgreSQL-подключение;
- реализация repository;
- HTTP API;
- frontend;
- unit-тесты.

Эти задачи будут выполняться в следующих спринтах.

---

# Текущее состояние после Sprint 004

Проект находится в состоянии:

    Dispatcher backend foundation
        +
    Object Model foundation
        +
    Device Model foundation
        +
    Tag Model foundation

Текущий технический фокус завершен.

Следующий логический фокус:

    Communication and Polling Foundation

---

# Следующий спринт

## Sprint 005 — Communication and Polling Foundation

Цель Sprint 005:

Создать базовую архитектуру коммуникаций и опроса.

Планируемые направления:

- модуль `scada_protocols`;
- `IProtocolDriver`;
- protocol capabilities;
- read request / read result;
- write request / write result;
- batch read;
- connection check;
- test address;
- simulator driver;
- polling task model;
- polling group model;
- базовый polling scheduler;
- подготовка к Modbus TCP и SNMP.

---

# Общий статус проекта

## Завершено

- Sprint 001 — Project Foundation.
- Sprint 002 — Object Model Foundation.
- Sprint 003 — Device Model Foundation.
- Sprint 004 — Tag Model Foundation.

## Следующий

- Sprint 005 — Communication and Polling Foundation.