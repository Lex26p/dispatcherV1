# ADR 0006 — Configuration Read API Boundaries

## Статус

Accepted

## Контекст

После Sprint 014 Dispatcher имеет стабильный HTTP transport, JSON serializer, error envelope, correlation ID, tests и Windows/Linux CI.

Следующий этап должен предоставить read access к:

- objects;
- object tree;
- devices;
- tags.

Domain models уже существуют, но прямое использование domain entities в HTTP endpoints создало бы нежелательную связь между transport contract и внутренней моделью.

Также проект пока не имеет production repository implementations.

Для frontend нужны реальные backend data, но временный development dataset не должен находиться в Razor pages или HTTP endpoint functions.

## Решение

Ввести transport-neutral configuration read boundary.

Основной интерфейс:

    IConfigurationReadService

Интерфейс размещается в:

    scada_api

Он возвращает resource-specific read models и collection results.

Первая реализация размещается в:

    scada_app

и использует deterministic development snapshot.

HTTP endpoints размещаются в:

    scada_http

и зависят от interface, а не от concrete provider.

## Dependency direction

Допустимо:

    scada_api -> scada_objects
    scada_api -> scada_devices
    scada_api -> scada_tags

    scada_app -> scada_api
    scada_app -> domain modules

    scada_http -> scada_api
    scada_http -> JsonValue

    dispatcher_server -> scada_app
    dispatcher_server -> scada_http

Недопустимо:

    domain modules -> scada_api
    domain modules -> scada_http
    scada_api -> Drogon
    scada_api -> JsonCpp
    scada_app -> Drogon
    frontend -> domain C++ types

## Existing generic API foundation

Следует переиспользовать:

    ApiReadResourceType
    ApiReadQueryOptions
    ApiReadFilter
    ApiReadRequest
    ApiReadEndpointCatalog

Resource-specific contracts не должны создавать дублирующую generic query abstraction.

Если существующих fields недостаточно, они расширяются совместимо.

## Read models

Создаются отдельные read models:

    ObjectReadModel
    ObjectTreeNodeReadModel
    DeviceReadModel
    DeviceConnectionReadModel
    TagReadModel

Read models:

- являются простыми value structures;
- не содержат behavior HTTP layer;
- не содержат JsonCpp types;
- не содержат Drogon types;
- не содержат frontend-specific text;
- не раскрывают secret configuration.

## Collection result

Для list operations используется generic или эквивалентный result:

    ReadCollection<T>

Поля:

    items
    total
    offset
    limit
    has_more

HTTP layer преобразует `items` в resource-specific array property.

## Query semantics

List query содержит:

    limit
    offset
    include_disabled
    filters

Defaults и maximum limits должны быть централизованы.

HTTP parser преобразует query-string в transport-neutral query.

Service не анализирует raw URL или query-string.

## Empty result semantics

Валидный read query без совпадений возвращает успешный пустой result.

HTTP response:

    200 OK

Не используется:

    404 Not Found

`404` резервируется для отсутствующего route или будущего single-resource endpoint.

## Ordering

Read service обязан возвращать детерминированный порядок.

Сортировка выполняется до pagination.

Objects, Devices и Tags сортируются по:

    code
    id

Object tree siblings сортируются тем же способом.

## Object tree ownership

Tree строится application read service.

Frontend не должен реконструировать hierarchy из unordered flat response.

HTTP endpoint только сериализует готовое дерево.

## Development provider

До появления persistence используется deterministic development implementation.

Она:

- создается composition root;
- проходит domain validation;
- не зависит от HTTP;
- не зависит от frontend;
- возвращает те же read contracts, что будущий repository-backed provider;
- используется для local development и vertical slice validation.

Она не является production persistence.

## HTTP contracts

Endpoints:

    GET /api/objects
    GET /api/objects/tree
    GET /api/devices
    GET /api/tags

Dynamic path parameters не добавляются.

Фильтрация конкретного ID выполняется query parameter.

Это сохраняет текущий exact-path router и ограничивает scope Sprint 015.

## Pagination contract

List response содержит:

    count
    total
    offset
    limit
    hasMore

`count` равен размеру возвращенного массива.

`total` вычисляется после filters и до pagination.

## Null semantics

Отсутствующие relationships сериализуются как JSON null.

Не используются:

- пустая строка;
- строка `"null"`;
- отсутствующее property для обязательного contract field.

Примеры:

    parentId: null
    objectId: null
    deviceId: null

## Enum semantics

Enums сериализуются стабильными строковыми значениями.

Frontend не зависит от underlying integer values.

Изменение строкового enum value считается API contract change.

## Query errors

Некорректный query возвращает общий Dispatcher error envelope.

Status:

    400 Bad Request

Code:

    invalid_query

Details содержат machine-readable field и reason.

Внутренние parser exceptions клиенту не возвращаются.

## Frontend boundary

Frontend получает единый client:

    IConfigurationApiClient

Причины одного client вместо трех:

- Objects, Devices и Tags относятся к одной engineering configuration area;
- используются одинаковые query и error conventions;
- уменьшается число DI registrations;
- client можно разделить позднее без изменения backend contract.

Razor pages зависят только от interface.

## Отклоненные варианты

### Domain entities непосредственно в HTTP layer

Отклонено.

Причины:

- transport contract стал бы зависеть от внутренней domain layout;
- optional и enum semantics были бы неявными;
- будущие domain changes могли бы ломать API.

### JSON generation в scada_app

Отклонено.

Application layer не должен зависеть от transport serialization.

### Development data в endpoint functions

Отклонено.

Это смешивает transport и data source и усложняет замену на repositories.

### Development data во frontend

Отклонено.

Frontend должен проверять реальный backend vertical slice.

### Три независимых frontend clients

Отложено.

На текущем scope один configuration client имеет меньшую сложность.

### Dynamic route templates

Отложено.

Текущий read scope может быть реализован exact routes и query filters.

### CRUD в том же спринте

Отклонено.

Read contracts должны стабилизироваться до command и persistence layers.

## Последствия

Положительные:

- HTTP transport остается тонким;
- domain model не становится wire contract;
- development provider заменяем;
- frontend работает с реальным backend;
- pagination и filtering централизованы;
- contracts тестируются независимо;
- готова граница для PostgreSQL repositories.

Отрицательные:

- появляются дополнительные mapping structures;
- временный snapshot требует поддержки;
- некоторые domain fields будут представлены дважды в domain и read models;
- single-resource routes откладываются.

## Следующее развитие

После Sprint 015 read service может быть реализован через:

- PostgreSQL repositories;
- configuration cache;
- remote configuration service;
- test fixtures.

HTTP и frontend contracts при этом не должны зависеть от выбранного storage implementation.
