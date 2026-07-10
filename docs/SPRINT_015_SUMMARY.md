# Sprint 015 Summary

## Название

Object, Device and Tag Read API Foundation

## Статус

Completed

## Шаги

Шаги 108–116.

## Дата закрытия

2026-07-10

## Этап

Post-MVP application API expansion.

## Цель

Создать первый read-only application vertical slice Dispatcher:

    domain
        ->
    application service
        ->
    HTTP
        ->
    JSON
        ->
    typed frontend client
        ->
    operator UI

## Главный результат

Objects, Devices и Tags перестали быть frontend placeholders.

Dispatcher предоставляет реальные backend configuration endpoints:

    GET /api/objects
    GET /api/objects/tree
    GET /api/devices
    GET /api/tags

Реальные данные используют страницы:

    /objects
    /devices
    /tags

Development data принадлежит backend application layer и не дублируется во frontend или HTTP endpoints.

## Архитектурный vertical slice

Реализована цепочка:

    domain entities
        ->
    domain-to-read-model mapping
        ->
    IConfigurationReadService
        ->
    DevelopmentConfigurationReadService
        ->
    resource-specific HTTP endpoint
        ->
    HttpRouteDispatcher
        ->
    DrogonHttpServer
        ->
    JsonValue
        ->
    JSON response
        ->
    ConfigurationHttpApiClient
        ->
    transport DTO
        ->
    view model mapping
        ->
    Blazor operator page

## Шаг 108 — Read API architecture

Созданы:

    docs/SPRINT_015_PLAN.md
    docs/adr/0006-configuration-read-api-boundaries.md

Зафиксированы boundaries:

- domain modules владеют domain behavior;
- `scada_api` владеет transport-neutral read contracts;
- `scada_app` владеет read-service implementation;
- `scada_http` владеет transport parsing и JSON;
- frontend использует typed client;
- development snapshot не размещается в transport или UI.

## Шаг 109 — Read contracts and provider

Добавлены:

    ObjectReadModel
    ObjectTreeNodeReadModel
    DeviceReadModel
    DeviceConnectionReadModel
    TagReadModel
    ReadCollection<T>
    ObjectTreeReadResult
    ObjectReadQuery
    DeviceReadQuery
    TagReadQuery
    IConfigurationReadService
    DevelopmentConfigurationReadService

Development snapshot содержит:

- пять объектов;
- четыре устройства;
- шесть тегов;
- несколько root и child objects;
- IPv4 configuration;
- IPv6 configuration;
- active configuration;
- maintenance configuration;
- disabled configuration;
- различные Tag types;
- различные value types;
- nullable relationships.

## Шаг 110 — Object HTTP API

Добавлены:

    GET /api/objects
    GET /api/objects/tree

Поддерживаются:

    id
    parentId
    includeChildren
    limit
    offset

Реализованы:

- strict reusable query parser;
- percent decoding;
- unknown parameter detection;
- duplicate parameter detection;
- Object collection contract;
- pagination metadata;
- nested Object tree;
- deterministic ordering;
- invalid query error envelope;
- correlation ID.

## Шаг 111 — Device HTTP API

Добавлен:

    GET /api/devices

Поддерживаются:

    id
    objectId
    state
    includeDisabled
    limit
    offset

Device response содержит:

- nullable object relationship;
- code;
- name;
- description;
- protocol;
- configuration state;
- runtimeEnabled;
- non-secret connection contract;
- IPv4 и IPv6 configuration;
- network interface.

## Шаг 112 — Tag HTTP API

Добавлен:

    GET /api/tags

Поддерживаются:

    id
    objectId
    deviceId
    includeDisabled
    limit
    offset

Tag response содержит:

- nullable Object relationship;
- nullable Device relationship;
- Tag type;
- value type;
- archive policy;
- code;
- name;
- description;
- engineering unit;
- scale;
- offset;
- address;
- enabled state.

Current value, quality, history и alarms сознательно не входят в configuration contract.

## Шаг 113 — Frontend configuration client

Добавлены:

    IConfigurationApiClient
    ConfigurationHttpApiClient

Операции:

    GetObjectsAsync
    GetObjectTreeAsync
    GetDevicesAsync
    GetTagsAsync

Созданы:

- transport DTO;
- frontend view models;
- typed query request models;
- deterministic URI query builder;
- DTO-to-view-model mapping;
- backend error-envelope handling;
- API error code handling;
- correlation ID handling;
- timeout handling;
- connection failure handling;
- invalid-response handling;
- caller cancellation handling.

Frontend configuration demo-data не добавлялись.

## Шаг 114 — Real Objects page

Страница:

    /objects

получает реальные backend data.

Реализованы:

- Object tree;
- recursive tree component;
- flat Object table;
- total count;
- root count;
- maximum hierarchy depth;
- Russian Object type labels;
- loading state;
- empty state;
- no-connection state;
- timeout state;
- API error state;
- API error code;
- correlation ID;
- refresh;
- cancellation.

Frontend не реконструирует hierarchy из flat collection.

Tree строится application service.

## Шаг 115 — Real Devices and Tags pages

Страница:

    /devices

отображает:

- Device collection;
- protocol;
- configuration state;
- Object relationship;
- connection;
- IP mode;
- network interface;
- runtime state;
- disabled configuration toggle;
- summary metrics.

Страница:

    /tags

отображает:

- Tag collection;
- Tag type;
- value type;
- Object relationship;
- Device relationship;
- engineering unit;
- scale;
- offset;
- address;
- archive policy;
- enabled state;
- disabled configuration toggle;
- summary metrics.

Обе страницы поддерживают loading, empty, error, no-connection, timeout, refresh и cancellation states.

## Шаг 116 — Integration and closure

Расширен:

    scripts/test-local-integration.ps1

Smoke-test проверяет:

- System health;
- System modules;
- Objects;
- Object tree;
- filters;
- pagination;
- Devices;
- Device state;
- disabled Device configuration;
- Tags;
- disabled Tag configuration;
- invalid query;
- unsupported query;
- error details;
- correlation ID;
- CORS;
- preflight;
- rejected origin;
- отсутствие secret fields.

Актуализированы:

    docs/CURRENT_STATUS.md
    docs/known-limitations.md
    docs/MVP_STATUS.md
    docs/roadmap.md
    docs/SPRINT_015_PLAN.md
    docs/development-log.md

Создан:

    docs/SPRINT_015_SUMMARY.md

## HTTP contracts

Рабочие endpoints:

    GET /api/system/health
    GET /api/system/modules
    GET /api/objects
    GET /api/objects/tree
    GET /api/devices
    GET /api/tags

List envelope:

    items
    count
    total
    offset
    limit
    hasMore

Имя collection:

    objects
    devices
    tags

## Error contract

Invalid query:

    HTTP 400
    error.code = invalid_query

Error body содержит:

    message
    correlationId
    details.field
    details.reason

Correlation ID возвращается также в header:

    X-Correlation-Id

## Test baseline

Backend:

    78 CTest tests

Frontend:

    xUnit suite

Integration:

    scripts/test-local-integration.ps1

Все проверки проходят.

## CI baseline

Подтверждены:

- Repository quality;
- Windows VS 2026 backend build;
- Windows CTest;
- frontend Release build;
- frontend xUnit tests;
- Ubuntu 24.04 Debug build;
- Ubuntu 24.04 Debug CTest;
- Ubuntu 24.04 Release build;
- Ubuntu 24.04 Release CTest;
- LF validation.

## Что сознательно не входило

Не реализовывались:

- CRUD;
- PostgreSQL persistence;
- production configuration loading;
- authentication;
- authorization;
- commands;
- current runtime values;
- history;
- events;
- alarms;
- WebSocket;
- SSE;
- real protocol drivers;
- background polling;
- deployment packaging.

## Известная временная реализация

Configuration read service использует:

    deterministic development snapshot

Snapshot не является production persistence.

Следующая production-oriented реализация должна читать configuration через repository boundary.

## Итог

Sprint 015 завершен.

Dispatcher получил первый прикладной read-only vertical slice и реальную инженерную навигацию:

    объект
        ->
    устройство
        ->
    тег

Следующий sprint должен быть зафиксирован отдельным plan.

Рекомендуемое направление:

    Configuration Persistence Foundation
