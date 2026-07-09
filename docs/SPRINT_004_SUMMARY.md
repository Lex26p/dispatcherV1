# Sprint 004 Summary

## Название спринта

Tag Model Foundation

## Статус

Closed

## Диапазон шагов

Шаги 29–35.

---

# Цель спринта

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

# Выполненные шаги

## Шаг 29

Добавлен модуль `scada_tags`.

Результат:

- создана структура модуля;
- добавлен `TagId`;
- добавлен `TagType`;
- добавлен `TagValueType`;
- добавлен `TagQuality`;
- добавлен `TagArchivePolicy`;
- добавлен `TagAddress`;
- добавлен `Tag`;
- добавлен `get_tag_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 30

Добавлена модель текущего значения тега.

Результат:

- добавлен `TagValueSource`;
- добавлен `TagValuePayload`;
- добавлен `TagCurrentValue`;
- добавлены временные метки значения;
- добавлено текущее качество;
- добавлен source значения;
- добавлен `last_good_value`;
- добавлен `change_counter`.

---

## Шаг 31

Добавлены валидация тегов и правила адресации.

Результат:

- добавлен `TagValidationCode`;
- добавлен `TagValidationIssue`;
- добавлен `TagValidationResult`;
- добавлен `validate_tag()`;
- добавлен `requires_object()`;
- добавлен `requires_device()`;
- добавлен `requires_address()`;
- добавлен `supports_extraction()`;
- добавлен `is_valid_engineering_transform()`;
- добавлена проверка идентичности тега;
- добавлена проверка привязки к объекту;
- добавлена проверка привязки к устройству;
- добавлена проверка адреса;
- добавлена проверка engineering transform;
- добавлена проверка bit extraction.

---

## Шаг 32

Добавлены DTO-контракты тегов.

Результат:

- добавлен `TagAddressDto`;
- добавлен `TagDto`;
- добавлен `TagListDto`;
- добавлен `TagCurrentValueDto`;
- добавлен `TagValidationIssueDto`;
- добавлен `TagValidationResultDto`;
- сохранено правило независимости `scada_contracts` от `scada_tags`.

---

## Шаг 33

Добавлены repository-интерфейсы тегов.

Результат:

- добавлен `ITagRepository`;
- добавлен `ITagCurrentValueRepository`;
- реализация хранения отложена на будущие спринты.

---

## Шаг 34

Добавлена черновая миграция тегов.

Результат:

- создан `database/migrations/0003_tag_model.sql`;
- описана таблица `tags`;
- описана таблица `tag_current_values`;
- добавлена связь `tags.object_id` с `object_nodes.id`;
- добавлена связь `tags.device_id` с `devices.id`.

---

## Шаг 35

Обновлена документация и закрыт Sprint 004.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_004_SUMMARY.md`.

---

# Итоговые артефакты

## Backend

Основной модуль:

- `backend/libs/scada_tags`

Основные сущности:

- `TagId`;
- `Tag`;
- `TagType`;
- `TagValueType`;
- `TagQuality`;
- `TagArchivePolicy`;
- `TagAddress`;
- `TagCurrentValue`;
- `TagValueSource`;
- `TagValuePayload`.

Основные правила:

- проверка идентичности тега;
- проверка привязки тега к объекту;
- проверка привязки тега к устройству;
- проверка необходимости адреса;
- проверка engineering transform;
- проверка bit extraction;
- проверка совместимости extraction с типом значения.

Основные repository-интерфейсы:

- `ITagRepository`;
- `ITagCurrentValueRepository`.

---

## Contracts

Основной модуль:

- `backend/libs/scada_contracts`

Добавлены DTO:

- `TagAddressDto`;
- `TagDto`;
- `TagListDto`;
- `TagCurrentValueDto`;
- `TagValidationIssueDto`;
- `TagValidationResultDto`.

---

## Database

Добавлена миграция:

- `database/migrations/0003_tag_model.sql`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_004_SUMMARY.md`.

---

# Что не входило в Sprint 004

В Sprint 004 сознательно не входили:

- Polling Engine;
- Modbus TCP driver;
- SNMP driver;
- runtime TagValueStore;
- реальные значения из устройств;
- historian;
- tag history;
- alarm rules;
- command execution;
- PostgreSQL-подключение;
- реализация repository;
- HTTP API;
- frontend;
- unit-тесты.

---

# Архитектурный результат

После Sprint 004 Dispatcher имеет базовую модель тегов.

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
    future polling / runtime values / historian / alarms

Контрактный слой развивается отдельно:

    scada_contracts
        ↓
    future API / frontend / integrations

---

# Следующий спринт

## Sprint 005 — Communication and Polling Foundation

Предварительная цель:

Создать базовую архитектуру коммуникаций и опроса.

Ожидаемые направления:

- `scada_protocols`;
- `IProtocolDriver`;
- `ProtocolCapabilities`;
- `ProtocolReadRequest`;
- `ProtocolReadResult`;
- `ProtocolWriteRequest`;
- `ProtocolWriteResult`;
- batch read;
- connection check;
- address test;
- simulator driver;
- polling task model;
- polling group model;
- polling scheduler foundation.

---

# Статус закрытия

Sprint 004 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.