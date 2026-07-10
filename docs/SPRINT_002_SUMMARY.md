# Sprint 002 Summary

## Название спринта

Object Model Foundation

## Статус

Closed

## Диапазон шагов

Шаги 13–21.

---

# Цель спринта

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

---

# Выполненные шаги

## Шаг 13

Добавлен модуль `scada_objects`.

Результат:

- создана структура модуля;
- добавлен `ObjectType`;
- добавлен `ObjectNode`;
- добавлен `ObjectPath`;
- добавлен `get_object_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

---

## Шаг 14

Добавлен `ObjectTree`.

Результат:

- добавлено хранение дерева объектов;
- добавлен поиск объекта;
- добавлено получение корневых объектов;
- добавлено получение дочерних объектов;
- добавлена базовая валидация дерева.

---

## Шаг 15

Добавлены правила допустимой иерархии объектов.

Результат:

- добавлены root-правила;
- добавлены parent/child-правила;
- добавлена проверка недопустимого root-типа;
- добавлена проверка недопустимой parent/child-связи.

---

## Шаг 16

Добавлены зоны ответственности.

Результат:

- добавлен `ResponsibilityZone`;
- добавлена связь зоны с объектами;
- добавлена связь зоны с пользователями;
- добавлена связь зоны с ролями;
- добавлена базовая валидация зоны ответственности.

---

## Шаг 17

Добавлены типизированные идентификаторы объектов.

Результат:

- добавлен `EntityId<Tag>`;
- добавлен `ObjectId`;
- добавлен `ResponsibilityZoneId`;
- объектная модель переведена на доменные идентификаторы.

---

## Шаг 18

Добавлены DTO-контракты объектной модели.

Результат:

- добавлен `ObjectNodeDto`;
- добавлен `ObjectTreeDto`;
- добавлен `ObjectTreeValidationIssueDto`;
- добавлен `ObjectTreeValidationResultDto`;
- добавлен `ResponsibilityZoneDto`;
- добавлен `ResponsibilityZoneValidationIssueDto`;
- добавлен `ResponsibilityZoneValidationResultDto`.

---

## Шаг 19

Добавлены repository-интерфейсы объектной модели.

Результат:

- добавлен `IObjectRepository`;
- добавлен `IResponsibilityZoneRepository`;
- реализация хранения отложена на будущие спринты.

---

## Шаг 20

Добавлена черновая миграция объектной модели.

Результат:

- создан `database/migrations/0001_object_model.sql`;
- описана таблица `object_nodes`;
- описана таблица `responsibility_zones`;
- описаны таблицы связей зон ответственности.

---

## Шаг 21

Обновлена документация и закрыт Sprint 002.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_002_SUMMARY.md`.

---

# Итоговые артефакты

## Backend

Основной модуль:

- `backend/libs/scada_objects`

Основные сущности:

- `ObjectId`;
- `ResponsibilityZoneId`;
- `ObjectType`;
- `ObjectNode`;
- `ObjectTree`;
- `ResponsibilityZone`.

Основные правила:

- проверка дерева объектов;
- проверка допустимой иерархии;
- проверка зон ответственности.

Основные repository-интерфейсы:

- `IObjectRepository`;
- `IResponsibilityZoneRepository`.

---

## Contracts

Основной модуль:

- `backend/libs/scada_contracts`

Добавлены DTO:

- `ObjectNodeDto`;
- `ObjectTreeDto`;
- `ObjectTreeValidationIssueDto`;
- `ObjectTreeValidationResultDto`;
- `ResponsibilityZoneDto`;
- `ResponsibilityZoneValidationIssueDto`;
- `ResponsibilityZoneValidationResultDto`.

---

## Database

Добавлена миграция:

- `database/migrations/0001_object_model.sql`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_002_SUMMARY.md`.

---

# Что не входило в Sprint 002

В Sprint 002 сознательно не входили:

- PostgreSQL-подключение;
- реализация repository;
- HTTP API;
- frontend;
- устройства;
- теги;
- polling;
- аварии;
- история;
- авторизация;
- unit-тесты.

---

# Архитектурный результат

После Sprint 002 Dispatcher имеет базовую объектную модель.

Текущая архитектурная цепочка:

    scada_common
        ↓
    scada_core
        ↓
    scada_objects
        ↓
    scada_contracts
        ↓
    future API / database / frontend

---

# Следующий спринт

## Sprint 003 — Device Model Foundation

Предварительная цель:

Создать базовую модель устройств.

Ожидаемые направления:

- `scada_devices`;
- `DeviceId`;
- `DeviceType`;
- `Device`;
- `DeviceConnection`;
- IPv4/IPv6/DNS;
- состояние устройства;
- диагностика подключения;
- repository-интерфейсы устройств;
- черновая SQL-миграция устройств.

---

# Статус закрытия

Sprint 002 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.
