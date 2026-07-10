# Sprint 015 Plan

## Название

Object, Device and Tag Read API Foundation

## Статус

In Progress

## Этап

Post-MVP application API expansion.

## Диапазон шагов

Шаги 108–116.

## Цель

Создать первый прикладной read-only vertical slice Dispatcher:

    domain models
        -> application read service
        -> HTTP endpoints
        -> JSON contracts
        -> typed frontend client
        -> real operator pages

Sprint должен подключить Objects, Devices и Tags pages к реальному backend без добавления CRUD, persistence или authentication.

## Причина

После Sprint 014 проект имеет:

- модульный backend;
- Drogon HTTP transport;
- JSON serialization foundation;
- единый API error envelope;
- correlation ID;
- backend Catch2 tests;
- frontend xUnit tests;
- Windows/Linux CI;
- real System API integration.

Следующий безопасный этап — расширение read-only application API с использованием уже защищенных transport contracts.

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

`scada_api` становится владельцем transport-neutral read contracts:

- resource read models;
- collection page model;
- resource query models;
- configuration read service interface;
- domain-to-read-model mapping declarations.

`scada_api` не формирует JSON и не знает о Drogon.

### scada_app

`scada_app` предоставляет первую реализацию read service.

На этапе Sprint 015 используется deterministic development snapshot.

Snapshot временный и позднее будет заменен repository-backed implementation.

Development data не должна находиться:

- в HTTP endpoint source files;
- во frontend;
- в JavaScript;
- в appsettings;
- в transport DTO.

### scada_http

`scada_http` отвечает за:

- HTTP routes;
- query-string parsing;
- query validation;
- вызов read service;
- JSON serialization;
- API error envelope;
- correlation ID;
- response headers.

HTTP layer не создает domain entities и не владеет development snapshot.

### Frontend

Frontend получает:

    IConfigurationApiClient
    ConfigurationHttpApiClient

Client отвечает за Objects, Devices и Tags read operations.

Frontend не использует `HttpClient` непосредственно внутри Razor pages.

## Read service boundary

Планируемый интерфейс:

    IConfigurationReadService

Операции:

    read_objects
    read_object_tree
    read_devices
    read_tags

Service возвращает transport-neutral read models.

Service не возвращает:

- `JsonValue`;
- Drogon types;
- HTTP responses;
- frontend models.

## Existing read foundation

Следует переиспользовать существующие concepts:

    ApiReadResourceType
    ApiReadQueryOptions
    ApiReadFilter
    ApiReadRequest
    ApiReadEndpointCatalog

Не следует создавать вторую параллельную generic query system.

Resource-specific query structures могут агрегировать существующие generic options и filters.

## HTTP endpoints

### Objects list

    GET /api/objects

Поддерживаемые query parameters:

    id
    parentId
    includeChildren
    limit
    offset

### Object tree

    GET /api/objects/tree

Возвращает вложенные tree nodes.

### Devices

    GET /api/devices

Поддерживаемые query parameters:

    id
    objectId
    includeDisabled
    limit
    offset

### Tags

    GET /api/tags

Поддерживаемые query parameters:

    id
    objectId
    deviceId
    includeDisabled
    limit
    offset

## Dynamic path parameters

Dynamic routes вида:

    /api/devices/{id}

не входят в Sprint 015.

Получение конкретного ресурса выполняется через query filter:

    /api/devices?id=device-1

Это позволяет использовать текущий exact-path router без преждевременного добавления path-template engine.

## Pagination

Для list endpoints используется envelope:

    {
      "objects": [],
      "count": 0,
      "total": 0,
      "offset": 0,
      "limit": 1000,
      "hasMore": false
    }

Имя массива зависит от ресурса:

    objects
    devices
    tags

Значения:

- `count` — количество элементов в текущем response;
- `total` — количество элементов после filters до pagination;
- `offset` — фактическое смещение;
- `limit` — фактический limit;
- `hasMore` — существуют ли элементы после текущей страницы.

Следует переиспользовать существующие defaults и maximum limits из `ApiReadQueryOptions`, если они остаются валидными для текущего API foundation.

## Object JSON contract

Object item:

    {
      "id": "plant-1",
      "parentId": null,
      "type": "Plant",
      "code": "PLANT_1",
      "name": "Объект 1",
      "description": "",
      "isRoot": true
    }

Правила:

- отсутствующий `parentId` сериализуется как `null`;
- enum сериализуется стабильной строкой;
- пустое описание остается пустой строкой;
- порядок properties не является частью contract.

## Object tree contract

Response:

    {
      "roots": [
        {
          "id": "plant-1",
          "parentId": null,
          "type": "Plant",
          "code": "PLANT_1",
          "name": "Объект 1",
          "description": "",
          "children": []
        }
      ],
      "count": 1
    }

`count` содержит общее количество object nodes, а не только roots.

Tree строится application service, а не frontend.

## Device JSON contract

Device item:

    {
      "id": "device-1",
      "objectId": "plant-1",
      "code": "DEVICE_1",
      "name": "Устройство 1",
      "description": "",
      "protocol": "ModbusTcp",
      "state": "Active",
      "runtimeEnabled": true,
      "connection": {
        "host": "127.0.0.1",
        "port": 502,
        "ipMode": "IPv4",
        "networkInterface": ""
      }
    }

Правила:

- отсутствующий `objectId` сериализуется как `null`;
- connection fields сериализуются из domain connection settings;
- secrets и credentials в response не включаются;
- runtime status не подменяется configuration state.

## Tag JSON contract

Tag item:

    {
      "id": "tag-1",
      "objectId": "plant-1",
      "deviceId": "device-1",
      "type": "Analog",
      "valueType": "Double",
      "archivePolicy": "OnChange",
      "code": "TEMPERATURE",
      "name": "Температура",
      "description": "",
      "engineeringUnit": "°C",
      "scale": 1.0,
      "offset": 0.0,
      "address": "40001",
      "enabled": true
    }

Правила:

- отсутствующие relationship IDs сериализуются как `null`;
- current value и quality не входят в этот contract;
- history не входит в этот contract;
- protocol-specific extended configuration откладывается.

## Query validation

Некорректный query возвращает:

    HTTP 400

Error code:

    invalid_query

Error details должны содержать имя field и причину, например:

    {
      "field": "limit",
      "reason": "must_be_positive"
    }

Проверяются:

- integer parsing;
- negative offset;
- non-positive limit;
- limit above maximum;
- invalid boolean;
- пустые значения обязательного filter;
- неподдерживаемые query parameters.

Пустой результат валидного filter возвращает:

    HTTP 200

с пустым массивом, а не `404`.

## Ordering

Все list responses имеют детерминированный порядок.

Objects:

    code
    id

Devices:

    code
    id

Tags:

    code
    id

Object tree сохраняет hierarchy и сортирует siblings по:

    code
    id

Нельзя полагаться на порядок `unordered_map` или database rows без явной сортировки.

## Development snapshot

Sprint 015 использует небольшой детерминированный backend snapshot.

Snapshot должен включать:

- несколько root и child objects;
- active device;
- disabled или maintenance device;
- IPv4 device;
- IPv6-capable configuration example;
- enabled tags;
- disabled tag;
- tags разных value types;
- tags с object и device relationships.

Snapshot должен проходить domain validation.

Snapshot предназначен только для development и tests.

## Frontend client

Создается:

    IConfigurationApiClient
    ConfigurationHttpApiClient

Планируемые методы:

    GetObjectsAsync
    GetObjectTreeAsync
    GetDevicesAsync
    GetTagsAsync

Client использует:

- `DispatcherApiClientOptions`;
- `DispatcherApiException`;
- backend error envelope;
- correlation ID;
- cancellation tokens.

## Frontend pages

### Objects

Реализуются:

- loading state;
- tree view;
- flat list fallback;
- empty state;
- connection error;
- backend error;
- correlation ID display;
- refresh;
- cancellation предыдущего request.

### Devices

Реализуются:

- loading state;
- device table;
- object relationship;
- protocol;
- configuration state;
- connection summary;
- enabled/disabled indication;
- empty state;
- error state;
- refresh.

### Tags

Реализуются:

- loading state;
- tag table;
- object relationship;
- device relationship;
- value type;
- engineering unit;
- address;
- archive policy;
- enabled/disabled indication;
- empty state;
- error state;
- refresh.

## Testing strategy

### scada_api tests

Проверяют:

- domain-to-read-model mapping;
- nullable relationships;
- enum string mapping;
- pagination;
- filtering;
- includeDisabled;
- deterministic ordering;
- object tree construction.

### scada_http tests

Проверяют:

- route registration;
- query parsing;
- invalid query errors;
- correlation ID;
- object JSON contract;
- object tree JSON contract;
- device JSON contract;
- tag JSON contract;
- empty responses;
- pagination metadata.

### Frontend tests

Проверяют:

- DTO deserialization;
- DTO-to-view-model mapping;
- query path generation;
- successful responses;
- empty responses;
- backend error envelope;
- invalid JSON;
- cancellation;
- correlation ID.

### Integration

Расширяется:

    scripts/test-local-integration.ps1

Smoke-test должен проверять:

- `/api/objects`;
- `/api/objects/tree`;
- `/api/devices`;
- `/api/tags`;
- filters;
- invalid query;
- CORS;
- correlation ID.

## Security and privacy

Read contracts не должны возвращать:

- passwords;
- SNMP community strings;
- tokens;
- secret references;
- database credentials;
- internal exception messages.

Connection data в Sprint 015 содержит только non-secret engineering fields.

## Out of scope

В Sprint 015 не входят:

- create;
- update;
- delete;
- bulk editing;
- commands;
- authentication;
- authorization;
- PostgreSQL;
- migrations implementation;
- realtime values;
- value quality;
- history;
- alarms;
- events;
- WebSocket;
- pagination UI controls beyond initial foundation;
- path-template routing;
- production configuration loading.

## Выполняемые шаги

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

Integration checks, current status and Sprint 015 closure.

## Acceptance criteria

Sprint считается завершенным, когда:

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
- backend tests проходят;
- frontend tests проходят;
- local integration smoke-test проходит;
- Windows CI проходит;
- Linux Debug CI проходит;
- Linux Release CI проходит;
- документация актуализирована;
- новые warnings отсутствуют.

## Итоговый результат

После Sprint 015 Dispatcher должен иметь первый полноценный read-only application vertical slice для инженерной конфигурации объекта.
