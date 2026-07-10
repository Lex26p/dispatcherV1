# Dispatcher Current Status

## Статус документа

Актуально после завершения:

    Sprint 015 — Object, Device and Tag Read API Foundation

Дата фиксации:

    2026-07-10

Этот документ является каноническим кратким описанием текущего технического состояния Dispatcher.

Исторический MVP status:

    docs/MVP_STATUS.md

История разработки:

    docs/development-log.md

Текущие ограничения:

    docs/known-limitations.md

## Общий статус проекта

Dispatcher находится на стадии:

    Post-MVP application foundation

Закрыты:

- MVP foundation;
- HTTP transport foundation;
- frontend System integration;
- engineering baseline;
- первый read-only configuration vertical slice.

Dispatcher пока не является production-ready SCADA-платформой.

## Рабочие vertical slices

### System vertical slice

    Blazor System page
        ->
    ISystemApiClient
        ->
    SystemHttpApiClient
        ->
    HTTP
        ->
    DrogonHttpServer
        ->
    HttpRouteDispatcher
        ->
    System endpoints
        ->
    JsonValue
        ->
    frontend DTO mapping
        ->
    operator UI

### Configuration vertical slice

    Object, Device and Tag domain models
        ->
    IConfigurationReadService
        ->
    DevelopmentConfigurationReadService
        ->
    Object, Device and Tag HTTP endpoints
        ->
    JsonValue
        ->
    ConfigurationHttpApiClient
        ->
    transport DTO
        ->
    frontend view models
        ->
    real operator pages

## Реальные HTTP endpoints

Доступны:

    GET /api/system/health
    GET /api/system/modules
    GET /api/objects
    GET /api/objects/tree
    GET /api/devices
    GET /api/tags

## Configuration query contracts

### Objects

    id
    parentId
    includeChildren
    limit
    offset

### Object tree

    id
    parentId

### Devices

    id
    objectId
    state
    includeDisabled
    limit
    offset

### Tags

    id
    objectId
    deviceId
    includeDisabled
    limit
    offset

## Backend

### Технологии

- C++20;
- CMake;
- vcpkg manifest mode;
- Drogon;
- JsonCpp как private implementation dependency;
- Catch2 v3;
- CTest.

### Модули

Сформированы:

    scada_common
    scada_contracts
    scada_core
    scada_objects
    scada_devices
    scada_tags
    scada_protocols
    scada_polling
    scada_runtime
    scada_historian
    scada_events
    scada_alarms
    scada_api
    scada_http
    scada_realtime
    scada_app

Application:

    dispatcher_server

### HTTP transport

Реализованы:

- HTTP methods;
- HTTP statuses;
- endpoint metadata;
- exact-path router;
- Drogon adapter;
- request validation;
- development CORS;
- OPTIONS preflight;
- structured JSON serialization;
- framework error conversion;
- API error envelope;
- correlation ID;
- safe handler-exception conversion.

### JSON foundation

Публичный transport type:

    dispatcher::http::JsonValue

Поддерживаются:

- object;
- array;
- null;
- string;
- bool;
- signed 64-bit integer;
- unsigned 64-bit integer;
- double;
- nested values;
- compact serialization.

JsonCpp types не входят в публичные Dispatcher headers.

### Query foundation

Reusable strict parser поддерживает:

- allowed parameter list;
- percent decoding;
- malformed encoding detection;
- duplicate detection;
- unknown parameter detection;
- Boolean parsing;
- unsigned integer parsing.

### Configuration application service

Интерфейс:

    IConfigurationReadService

Операции:

    read_objects
    read_object_tree
    read_devices
    read_tags

Текущая implementation:

    DevelopmentConfigurationReadService

Источник:

    deterministic development snapshot

Snapshot содержит:

- пять объектов;
- четыре устройства;
- шесть тегов.

### Ordering and pagination

Collection ordering:

    code
    id

Object tree siblings:

    code
    id

Collection metadata:

    count
    total
    offset
    limit
    hasMore

### Error contract

Формат:

    {
      "error": {
        "code": "invalid_query",
        "message": "The query string is invalid.",
        "correlationId": "request-id",
        "details": {
          "field": "limit",
          "reason": "must_be_positive"
        }
      }
    }

Correlation header:

    X-Correlation-Id

## Frontend

### Технологии

- .NET 10;
- Blazor WebAssembly;
- MudBlazor;
- xUnit.

### Язык

Пользовательский UI по умолчанию использует русский язык.

Английский язык остается будущей локализацией.

### Typed API clients

Реализованы:

    ISystemApiClient
    SystemHttpApiClient
    MockSystemApiClient
    IConfigurationApiClient
    ConfigurationHttpApiClient

Legacy demo client остается только для страниц, которые еще не подключены к реальному backend.

### Реальные страницы

Реальные backend data используют:

    /system
    /objects
    /devices
    /tags

### Objects page

Показывает:

- real Object tree;
- flat Object collection;
- total count;
- root count;
- maximum depth;
- Russian type labels;
- loading;
- empty;
- connection error;
- timeout;
- backend error;
- API error code;
- correlation ID;
- refresh;
- cancellation.

### Devices page

Показывает:

- Device collection;
- protocol;
- state;
- Object relationship;
- connection;
- IP mode;
- network interface;
- runtime state;
- includeDisabled toggle;
- summary metrics;
- loading;
- empty;
- error;
- refresh;
- cancellation.

### Tags page

Показывает:

- Tag collection;
- Tag type;
- value type;
- relationships;
- engineering unit;
- scale;
- offset;
- address;
- archive policy;
- enabled state;
- includeDisabled toggle;
- summary metrics;
- loading;
- empty;
- error;
- refresh;
- cancellation.

### Demo pages

Пока используют legacy demo-data:

    /runtime
    /events
    /alarms

## Automated tests

### Backend

Framework:

    Catch2 v3

Runner:

    CTest

Количество зарегистрированных tests:

    78

Покрыты:

- common version behavior;
- API read options;
- domain-to-read-model mapping;
- configuration provider;
- Object tree construction;
- filtering;
- pagination;
- ordering;
- HTTP methods and statuses;
- router behavior;
- CORS;
- JSON serializer;
- API errors;
- correlation ID;
- strict query parser;
- System endpoints;
- Object endpoints;
- Device endpoint;
- Tag endpoint.

Unit tests не запускают настоящий network listener.

### Frontend

Framework:

    xUnit

Покрыты:

- client options;
- URL normalization;
- DTO mapping;
- tree mapping;
- query generation;
- URI escaping;
- System client;
- Configuration client;
- HTTP errors;
- API error code;
- correlation ID;
- malformed JSON;
- null JSON;
- connection failure;
- timeout;
- caller cancellation;
- presentation helpers.

### Local integration

Скрипт:

    scripts/test-local-integration.ps1

Проверяет настоящий development HTTP listener:

- System health;
- System modules;
- Objects;
- Object tree;
- Devices;
- Tags;
- filters;
- pagination;
- disabled configuration;
- invalid query;
- unsupported query;
- correlation ID;
- CORS;
- preflight;
- rejected origin;
- non-secret Device contract.

## Build support

### Windows

Presets:

    x64-debug
    x64-release

Generator:

    Visual Studio 18 2026

Triplet:

    x64-windows

### Linux

Presets:

    linux-x64-debug
    linux-x64-release

Generator:

    Ninja

Triplet:

    x64-linux

Linux Debug и Release проверяются GitHub Actions.

## Continuous integration

Workflow:

    .github/workflows/ci.yml

Jobs:

- Repository quality;
- Windows VS 2026;
- Ubuntu 24.04.

CI проверяет:

- required repository files;
- CMake presets;
- LF line endings;
- pinned vcpkg baseline;
- Windows backend build;
- Windows CTest;
- frontend Release build;
- frontend xUnit;
- Linux Debug backend build;
- Linux Debug CTest;
- Linux Release backend build;
- Linux Release CTest;
- case-sensitive Linux filesystem.

## Line endings

Repository text files используют:

    LF

Политика закреплена через:

- `.gitattributes`;
- `.editorconfig`;
- `scripts/check-line-endings.ps1`;
- CI.

## Что пока не реализовано

Отсутствуют:

- repository-backed configuration read service;
- PostgreSQL repository implementations;
- migration runner;
- production configuration loading;
- Object, Device и Tag CRUD;
- Runtime HTTP API;
- History HTTP API;
- Events HTTP API;
- Alarms HTTP API;
- current Tag value API;
- quality API;
- real Modbus TCP driver;
- real SNMP driver;
- background polling lifecycle;
- Runtime-to-Historian pipeline;
- Runtime-to-Events pipeline;
- Runtime-to-Alarms pipeline;
- WebSocket;
- SSE;
- authentication;
- authorization;
- operator commands;
- alarm acknowledgement;
- dashboards;
- mimic diagrams;
- structured logging;
- metrics;
- production process lifecycle;
- deployment packaging.

## Production readiness

Dispatcher пока не готов к промышленной эксплуатации.

До production stage требуются:

- persistence;
- security model;
- real protocol drivers;
- service lifecycle;
- observability;
- backups and recovery;
- load tests;
- endurance tests;
- deployment automation;
- operator workflows;
- commissioning tools.

## Следующий planning checkpoint

Sprint 016 должен быть согласован отдельным plan.

Рекомендуемый следующий технический приоритет:

    Configuration Persistence Foundation

Предполагаемая цель:

    deterministic development snapshot
        ->
    repository-backed Object, Device and Tag reads

До отдельного Sprint plan это направление не считается утвержденным scope.
