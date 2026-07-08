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

Примеры допустимых иерархий:

    Site
     └── Building
          └── Floor
               └── Room
                    └── Cabinet
                         └── Rack

    Site
     └── Zone
          └── Line
               └── EquipmentGroup

    Building
     └── Room
          └── Cabinet

Сборка и запуск прошли успешно.

### Шаг 16 — ResponsibilityZone

Добавлена сущность:

- `ResponsibilityZone`.

Зона ответственности связывает:

- объекты;
- пользователей;
- роли.

Добавлена базовая валидация зоны ответственности:

- пустая идентичность;
- пустой список объектов;
- дублирующиеся объекты;
- дублирующиеся пользователи;
- дублирующиеся роли.

Сборка и запуск прошли успешно.

### Шаг 17 — Typed object identifiers

Добавлен общий шаблон:

- `EntityId<Tag>`.

Добавлены типизированные идентификаторы:

- `ObjectId`;
- `ResponsibilityZoneId`.

Объектная модель переведена с общего `Uuid` на доменные идентификаторы там, где это уже оправдано.

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

Зафиксировано разделение:

- доменная модель остается внутри backend;
- DTO используются на внешних границах системы.

Сборка и запуск прошли успешно.

### Шаг 19 — Object repository interfaces

Добавлены repository-интерфейсы объектной модели:

- `IObjectRepository`;
- `IResponsibilityZoneRepository`.

Реализация PostgreSQL пока не добавлялась.

Интерфейсы подготовлены для будущих реализаций:

- in-memory;
- PostgreSQL;
- mock;
- file-based.

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

PostgreSQL пока не подключался.

Сборка и запуск прошли успешно.

### Шаг 21 — Sprint 002 docs update and close

Обновлена документация и закрыт Sprint 002.

Обновлены:

- `docs/development-log.md`;
- `docs/known-limitations.md`.

Создан:

- `docs/SPRINT_002_SUMMARY.md`.

Сборка и запуск прошли успешно.

## Итог Sprint 002

В Sprint 002 создан фундамент объектной модели Dispatcher.

Сейчас в проекте есть:

- модуль объектной модели;
- типы объектов;
- узлы дерева объектов;
- дерево объектов;
- базовая валидация дерева;
- правила допустимой иерархии;
- зоны ответственности;
- типизированные идентификаторы объектов;
- DTO-контракты объектной модели;
- repository-интерфейсы;
- черновая SQL-миграция.

---

# Sprint 003 — Device Model Foundation

## Статус

Закрывается на шаге 28.

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

---

## Выполнено

### Шаг 22 — scada_devices

Добавлен модуль:

- `backend/libs/scada_devices`.

Добавлены базовые элементы модели устройств:

- `DeviceId`;
- `DeviceProtocol`;
- `DeviceState`;
- `IpMode`;
- `DeviceConnection`;
- `Device`;
- `get_device_module_info()`.

Модуль подключен к CMake и к `dispatcher_server`.

`dispatcher_server` теперь отображает модуль:

- `scada_devices | Devices | Running`.

Сборка и запуск прошли успешно.

---

### Шаг 23 — Device validation and connection rules

Добавлена базовая валидация устройства.

Добавлены:

- `DeviceValidationCode`;
- `DeviceValidationIssue`;
- `DeviceValidationResult`;
- `validate_device()`;
- `requires_network_endpoint()`;
- `is_runtime_enabled_state()`.

Проверяются:

- пустая идентичность устройства;
- отсутствие привязки к объекту после выхода из `Draft`;
- несовпадение `Device::protocol` и `DeviceConnection::protocol`;
- отсутствие `host`;
- некорректный `port`;
- отсутствие endpoint для runtime-устройства.

Сборка и запуск прошли успешно.

---

### Шаг 24 — Device diagnostics model

Добавлена модель диагностики устройства.

Добавлены:

- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`.

Диагностика хранит:

- идентификатор устройства;
- статус связи;
- статус здоровья;
- счетчик успешных запросов;
- счетчик ошибочных запросов;
- счетчик таймаутов;
- счетчик переподключений;
- последнюю задержку;
- последнюю ошибку.

Сборка и запуск прошли успешно.

---

### Шаг 25 — Device DTO contracts

В `scada_contracts` добавлены DTO-контракты устройств.

Добавлены:

- `DeviceConnectionDto`;
- `DeviceDto`;
- `DeviceListDto`;
- `DeviceValidationIssueDto`;
- `DeviceValidationResultDto`;
- `DeviceDiagnosticsDto`.

Зафиксировано правило:

- `scada_contracts` не зависит от `scada_devices`;
- идентификаторы в DTO передаются строками;
- enum-значения в DTO передаются строками.

Сборка и запуск прошли успешно.

---

### Шаг 26 — Device repository interfaces

Добавлены repository-интерфейсы устройств.

Добавлены:

- `IDeviceRepository`;
- `IDeviceDiagnosticsRepository`.

Интерфейсы подготовлены для будущих реализаций:

- in-memory;
- PostgreSQL;
- mock;
- file-based.

Реализация хранения пока не добавлялась.

Сборка и запуск прошли успешно.

---

### Шаг 27 — Device database migration draft

Добавлена черновая SQL-миграция:

- `database/migrations/0002_device_model.sql`.

Миграция описывает будущие таблицы:

- `devices`;
- `device_diagnostics`.

Миграция зависит от объектной модели, потому что `devices.object_id` ссылается на `object_nodes.id`.

PostgreSQL пока не подключался.

Сборка и запуск прошли успешно.

---

## Итог Sprint 003

В Sprint 003 создан фундамент модели устройств Dispatcher.

Сейчас в проекте есть:

- модуль устройств;
- типизированный идентификатор устройства;
- базовая сущность устройства;
- модель подключения устройства;
- поддержка IPv4/IPv6/Auto на уровне модели;
- базовые протоколы `ModbusTcp`, `Snmp`, `Simulator`, `Custom`;
- жизненные состояния устройства;
- базовая валидация устройства;
- модель диагностики устройства;
- DTO-контракты устройств;
- repository-интерфейсы устройств;
- черновая SQL-миграция устройств.

---

## Что сознательно не делали в Sprint 003

В Sprint 003 не добавлялись:

- Modbus TCP driver;
- SNMP driver;
- Polling Engine;
- реальные сетевые подключения;
- Boost.Asio;
- теги;
- текущие значения;
- аварии связи;
- PostgreSQL-подключение;
- реализация repository;
- HTTP API;
- frontend;
- unit-тесты.

Эти задачи будут выполняться в следующих спринтах.

---

# Текущее состояние после Sprint 003

Проект находится в состоянии:

    Dispatcher backend foundation
        +
    Object Model foundation
        +
    Device Model foundation

Текущий технический фокус завершен.

Следующий логический фокус:

    Tag Model Foundation

---

# Следующий спринт

## Sprint 004 — Tag Model Foundation

Цель Sprint 004:

Создать базовую модель тегов Dispatcher.

Планируемые направления:

- модуль `scada_tags`;
- `TagId`;
- `TagType`;
- `TagValueType`;
- `TagQuality`;
- `TagAddress`;
- `TagArchivePolicy`;
- `Tag`;
- `TagCurrentValue`;
- связь тега с устройством;
- связь тега с объектом;
- DTO-контракты тегов;
- repository-интерфейсы тегов;
- черновая SQL-миграция тегов.

---

# Общий статус проекта

## Завершено

- Sprint 001 — Project Foundation.
- Sprint 002 — Object Model Foundation.
- Sprint 003 — Device Model Foundation.

## Следующий

- Sprint 004 — Tag Model Foundation.