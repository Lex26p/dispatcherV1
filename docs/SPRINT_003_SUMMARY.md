# Sprint 003 Summary

## Название спринта

Device Model Foundation

## Статус

Closed

## Диапазон шагов

Шаги 22–28.

---

# Цель спринта

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

# Выполненные шаги

## Шаг 22

Добавлен модуль `scada_devices`.

Результат:

- создана структура модуля;
- добавлен `DeviceId`;
- добавлен `DeviceProtocol`;
- добавлен `DeviceState`;
- добавлен `IpMode`;
- добавлен `DeviceConnection`;
- добавлен `Device`;
- добавлен `get_device_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 23

Добавлены валидация устройств и правила подключения.

Результат:

- добавлен `DeviceValidationCode`;
- добавлен `DeviceValidationIssue`;
- добавлен `DeviceValidationResult`;
- добавлен `validate_device()`;
- добавлен `requires_network_endpoint()`;
- добавлен `is_runtime_enabled_state()`;
- добавлена проверка идентичности устройства;
- добавлена проверка привязки к объекту;
- добавлена проверка совпадения протоколов;
- добавлена проверка `host`;
- добавлена проверка `port`;
- добавлена проверка runtime endpoint.

---

## Шаг 24

Добавлена модель диагностики устройств.

Результат:

- добавлен `DeviceDiagnostics`;
- добавлен `DeviceCommunicationStatus`;
- добавлен `DeviceHealthStatus`;
- добавлены счетчики успешных запросов;
- добавлены счетчики ошибочных запросов;
- добавлены счетчики таймаутов;
- добавлены счетчики переподключений;
- добавлена последняя задержка обмена;
- добавлена последняя ошибка.

---

## Шаг 25

Добавлены DTO-контракты устройств.

Результат:

- добавлен `DeviceConnectionDto`;
- добавлен `DeviceDto`;
- добавлен `DeviceListDto`;
- добавлен `DeviceValidationIssueDto`;
- добавлен `DeviceValidationResultDto`;
- добавлен `DeviceDiagnosticsDto`;
- сохранено правило независимости `scada_contracts` от `scada_devices`.

---

## Шаг 26

Добавлены repository-интерфейсы устройств.

Результат:

- добавлен `IDeviceRepository`;
- добавлен `IDeviceDiagnosticsRepository`;
- реализация хранения отложена на будущие спринты.

---

## Шаг 27

Добавлена черновая миграция устройств.

Результат:

- создан `database/migrations/0002_device_model.sql`;
- описана таблица `devices`;
- описана таблица `device_diagnostics`;
- добавлена связь `devices.object_id` с `object_nodes.id`.

---

## Шаг 28

Обновлена документация и закрыт Sprint 003.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_003_SUMMARY.md`.

---

# Итоговые артефакты

## Backend

Основной модуль:

- `backend/libs/scada_devices`

Основные сущности:

- `DeviceId`;
- `Device`;
- `DeviceProtocol`;
- `DeviceState`;
- `DeviceConnection`;
- `IpMode`;
- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`.

Основные правила:

- проверка идентичности устройства;
- проверка привязки устройства к объекту;
- проверка совпадения протоколов;
- проверка необходимости сетевого endpoint;
- проверка `host`;
- проверка `port`;
- проверка runtime endpoint.

Основные repository-интерфейсы:

- `IDeviceRepository`;
- `IDeviceDiagnosticsRepository`.

---

## Contracts

Основной модуль:

- `backend/libs/scada_contracts`

Добавлены DTO:

- `DeviceConnectionDto`;
- `DeviceDto`;
- `DeviceListDto`;
- `DeviceValidationIssueDto`;
- `DeviceValidationResultDto`;
- `DeviceDiagnosticsDto`.

---

## Database

Добавлена миграция:

- `database/migrations/0002_device_model.sql`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_003_SUMMARY.md`.

---

# Что не входило в Sprint 003

В Sprint 003 сознательно не входили:

- Modbus TCP driver;
- SNMP driver;
- Polling Engine;
- реальные TCP/UDP подключения;
- Boost.Asio;
- теги;
- текущие значения;
- аварии связи;
- история диагностики;
- PostgreSQL-подключение;
- реализация repository;
- HTTP API;
- frontend;
- unit-тесты.

---

# Архитектурный результат

После Sprint 003 Dispatcher имеет базовую модель устройств.

Текущая архитектурная цепочка:

    scada_common
        ↓
    scada_core
        ↓
    scada_objects
        ↓
    scada_devices
        ↓
    future tags / polling / protocol drivers

Контрактный слой развивается отдельно:

    scada_contracts
        ↓
    future API / frontend / integrations

---

# Следующий спринт

## Sprint 004 — Tag Model Foundation

Предварительная цель:

Создать базовую модель тегов.

Ожидаемые направления:

- `scada_tags`;
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

# Статус закрытия

Sprint 003 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.
