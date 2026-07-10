# Sprint 015 Plan

## Название

Object, Device and Tag Read API Foundation

## Статус

Completed

## Этап

Post-MVP application API expansion.

## Диапазон шагов

Шаги 108–116.

## Дата закрытия

2026-07-10

## Цель

Создать первый прикладной read-only vertical slice Dispatcher:

    domain models
        ->
    application read service
        ->
    HTTP endpoints
        ->
    JSON contracts
        ->
    typed frontend client
        ->
    real operator pages

Sprint подключает Objects, Devices и Tags pages к реальному backend без добавления CRUD, persistence или authentication.

## Архитектурные границы

### Domain modules

Модули:

    scada_objects
    scada_devices
    scada_tags

остаются владельцами domain entities, enums и validation behavior.

Они не зависят от:

- HTTP;
- Drogon;
- JsonCpp;
- frontend DTO;
- pagination;
- query strings.

### scada_api

`scada_api` владеет transport-neutral read contracts:

- Object read models;
- Object tree read models;
- Device read models;
- Tag read models;
- collection page model;
- resource query models;
- configuration read service interface;
- domain-to-read-model mapping.

`scada_api` не формирует JSON и не знает о Drogon.

### scada_app

`scada_app` предоставляет development implementation:

    DevelopmentConfigurationReadService

Источник данных:

    deterministic development snapshot

Snapshot временный и позднее должен быть заменен repository-backed implementation.

Development data не находится:

- в HTTP endpoint source files;
- во frontend;
- в JavaScript;
- в appsettings;
- в transport DTO.

### scada_http

`scada_http` отвечает за:

- HTTP routes;
- query parsing;
- percent decoding;
- duplicate parameter detection;
- unknown parameter detection;
- scalar validation;
- вызов read service;
- JSON serialization;
- API error envelope;
- correlation ID;
- response headers.

### Frontend

Frontend использует:

    IConfigurationApiClient
    ConfigurationHttpApiClient

Razor pages не используют `HttpClient` напрямую.

Frontend не содержит configuration demo-data.

## Read service boundary

Интерфейс:

    IConfigurationReadService

Операции:

    read_objects
    read_object_tree
    read_devices
    read_tags

Service возвращает transport-neutral read models.

Service не возвращает:

- JsonValue;
- Drogon types;
- HTTP responses;
- frontend models.

## Реализованные HTTP endpoints

### Objects collection

    GET /api/objects

Query parameters:

    id
    parentId
    includeChildren
    limit
    offset

### Object tree

    GET /api/objects/tree

Query parameters:

    id
    parentId

### Devices

    GET /api/devices

Query parameters:

    id
    objectId
    state
    includeDisabled
    limit
    offset

### Tags

    GET /api/tags

Query parameters:

    id
    objectId
    deviceId
    includeDisabled
    limit
    offset

## Pagination contract

List endpoints возвращают:

    {
      "objects": [],
      "count": 0,
      "total": 0,
      "offset": 0,
      "limit": 1000,
      "hasMore": false
    }

Имя массива зависит от resource:

    objects
    devices
    tags

Значения:

- `count` — количество элементов текущей страницы;
- `total` — количество элементов после filters до pagination;
- `offset` — фактическое смещение;
- `limit` — фактический limit;
- `hasMore` — существуют ли элементы после текущей страницы.

## Query validation

Некорректный query возвращает:

    HTTP 400
    error.code = invalid_query

Error details содержат:

    field
    reason

Проверяются:

- malformed percent encoding;
- duplicate parameters;
- unknown parameters;
- invalid Boolean;
- invalid unsigned integer;
- пустые identifier filters;
- limit равный нулю;
- limit выше maximum;
- unsupported Device state.

Валидный filter без совпадений возвращает:

    HTTP 200

с пустой collection.

## Ordering

List responses имеют детерминированный порядок:

    code
    id

Object tree:

- сохраняет hierarchy;
- сортирует siblings по `code`, затем `id`.

## Security

Read contracts не возвращают:

- passwords;
- SNMP community strings;
- tokens;
- secret references;
- database credentials;
- internal exception messages.

Device connection contract содержит только non-secret engineering fields.

## Frontend pages

Реальные backend данные используют:

    /objects
    /devices
    /tags

Реализованы:

- typed DTO;
- DTO-to-view-model mapping;
- typed query requests;
- deterministic query builder;
- loading states;
- empty states;
- no-connection states;
- timeout states;
- backend API errors;
- API error code;
- correlation ID;
- manual refresh;
- cancellation предыдущего request;
- cancellation при уничтожении component;
- Russian presentation labels.

## Выполненные шаги

### Шаг 108

Read API scope and architecture.

### Шаг 109

Configuration read contracts and development provider.

### Шаг 110

Object list and tree HTTP API.

### Шаг 111

Device read HTTP API.

### Шаг 112

Tag read HTTP API.

### Шаг 113

Frontend configuration API client and tests.

### Шаг 114

Real Objects page integration.

### Шаг 115

Real Devices and Tags page integration.

### Шаг 116

Integration checks, current status and Sprint closure.

## Testing strategy

### Backend

Используются:

    Catch2 v3
    CTest

Покрыты:

- read-model mapping;
- nullable relationships;
- filtering;
- pagination;
- deterministic ordering;
- Object tree construction;
- strict query parser;
- Object JSON;
- Object tree JSON;
- Device JSON;
- Tag JSON;
- invalid query envelope;
- correlation ID;
- route registration.

### Frontend

Используется:

    xUnit

Покрыты:

- DTO mapping;
- recursive tree mapping;
- query path generation;
- URI escaping;
- successful responses;
- backend error envelope;
- error code;
- correlation ID;
- malformed JSON;
- null JSON;
- connection failure;
- timeout;
- caller cancellation;
- presentation helpers.

### Integration

Скрипт:

    scripts/test-local-integration.ps1

Проверяет:

- System health;
- System modules;
- Object collection;
- Object filters;
- Object pagination;
- Object tree;
- Object subtree;
- Device collection;
- Device filters;
- disabled Device configuration;
- Tag collection;
- Tag filters;
- disabled Tag configuration;
- invalid query;
- unsupported query;
- correlation ID;
- CORS;
- OPTIONS preflight;
- rejected origin;
- отсутствие secret fields в Device response.

## Acceptance criteria

Выполнены:

- read service не зависит от HTTP;
- development snapshot не находится в endpoints или frontend;
- `/api/objects` работает;
- `/api/objects/tree` работает;
- `/api/devices` работает;
- `/api/tags` работает;
- filters и pagination валидируются;
- responses имеют стабильные JSON contracts;
- error responses используют общий envelope;
- responses имеют correlation ID;
- frontend использует typed client;
- Objects page получает реальные backend data;
- Devices page получает реальные backend data;
- Tags page получает реальные backend data;
- 78 CTest tests проходят;
- frontend xUnit tests проходят;
- local integration smoke-test проходит;
- Windows CI проходит;
- Linux Debug CI проходит;
- Linux Release CI проходит;
- документация актуализирована;
- новые warnings отсутствуют.

## Out of scope

В Sprint 015 не входят:

- create;
- update;
- delete;
- bulk editing;
- commands;
- authentication;
- authorization;
- PostgreSQL persistence;
- migration runner;
- realtime values;
- value quality;
- history;
- alarms;
- events;
- WebSocket;
- SSE;
- production configuration loading;
- path-template routing;
- production deployment.

## Итог

Sprint 015 завершен.

Dispatcher имеет первый полноценный read-only application vertical slice для инженерной конфигурации:

    объект
        ->
    устройство
        ->
    тег

Следующий sprint scope должен быть согласован отдельным plan.

Рекомендуемый следующий приоритет:

    repository-backed configuration persistence

Эта рекомендация не является утвержденным Sprint 016 scope.
