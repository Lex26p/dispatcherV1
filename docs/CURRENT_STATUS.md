# Dispatcher Current Status

## Статус документа

Актуально после завершения Sprint 014 — Engineering Baseline.

Дата фиксации:

    2026-07-10

Этот документ является каноническим кратким описанием текущего состояния Dispatcher.

Исторический статус MVP foundation хранится в:

    docs/MVP_STATUS.md

История разработки хранится в:

    docs/development-log.md

## Общий статус проекта

Dispatcher находится на стадии:

    Post-MVP engineering foundation

MVP foundation закрыт.

Engineering baseline закрыт.

Проект имеет рабочий вертикальный срез frontend-to-backend, автоматические backend и frontend tests, а также Windows/Linux continuous integration.

Dispatcher пока не является production-ready SCADA-платформой.

## Текущий рабочий вертикальный срез

Реализована цепочка:

    Blazor WebAssembly System page
        -> ISystemApiClient
        -> SystemHttpApiClient
        -> HTTP
        -> DrogonHttpServer
        -> HttpRouteDispatcher
        -> System endpoints
        -> JsonValue
        -> JSON response
        -> frontend DTO mapping
        -> operator UI state

Реально доступны endpoints:

    GET /api/system/health
    GET /api/system/modules

Страница:

    /system

получает реальные данные backend.

## Backend

### Технологии

- C++20;
- CMake;
- vcpkg manifest mode;
- Drogon HTTP transport;
- JsonCpp как private implementation dependency `scada_http`;
- Catch2 v3;
- CTest.

### Модульная структура

В backend созданы foundation-модули:

- `scada_common`;
- `scada_contracts`;
- `scada_core`;
- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_protocols`;
- `scada_polling`;
- `scada_runtime`;
- `scada_historian`;
- `scada_events`;
- `scada_alarms`;
- `scada_api`;
- `scada_http`;
- `scada_realtime`;
- `scada_app`.

Приложение:

    dispatcher_server

компонует модули и запускает development HTTP listener.

### HTTP transport

Реализованы:

- HTTP method и status models;
- route dispatcher;
- Drogon adapter;
- System endpoint registry;
- development CORS;
- OPTIONS preflight;
- request validation;
- structured JSON serialization;
- единый API error envelope;
- correlation ID;
- безопасное преобразование handler exceptions в HTTP 500.

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

JsonCpp types не входят в публичные headers Dispatcher.

### Error contract

Ошибки возвращаются в формате:

    {
      "error": {
        "code": "not_found",
        "message": "The requested API route was not found.",
        "correlationId": "request-id",
        "details": {
          "path": "/api/missing"
        }
      }
    }

Если дополнительные сведения отсутствуют:

    "details": null

Correlation ID передается через:

    X-Correlation-Id

Валидный входящий ID сохраняется.

Отсутствующий или некорректный ID заменяется сгенерированным значением.

## Frontend

### Технологии

- .NET 10;
- Blazor WebAssembly;
- MudBlazor;
- xUnit.

### Язык интерфейса

Русский язык используется по умолчанию.

Английский язык рассматривается как будущая локализация.

### Реальная интеграция

Реальный backend использует страница:

    /system

Реализованы:

- `ISystemApiClient`;
- `SystemHttpApiClient`;
- `MockSystemApiClient`;
- transport DTO;
- DTO-to-view-model mapping;
- loading state;
- connected state;
- no-connection state;
- timeout state;
- HTTP error state;
- invalid-response state;
- empty-modules state;
- manual refresh;
- cancellation предыдущего запроса;
- чтение backend error code;
- чтение correlation ID.

### Demo и placeholder scope

Следующие страницы пока не имеют настоящих backend endpoints:

- Objects;
- Devices;
- Tags;
- Runtime;
- Events;
- Alarms.

Runtime, Events и Alarms пока используют legacy demo-client.

Objects, Devices и Tags остаются placeholder pages до Sprint 015.

## Automated tests

### Backend

Framework:

    Catch2 v3

Runner:

    CTest

Текущее количество зарегистрированных CTest tests:

    36

Покрыты:

- version information;
- HTTP methods;
- HTTP statuses;
- response factories;
- CORS options;
- server options;
- request validation;
- endpoint validation;
- route registration;
- duplicate routes;
- route dispatch;
- 400 behavior;
- 404 behavior;
- 405 behavior;
- handler exceptions;
- JSON scalar types;
- JSON escaping;
- nested JSON;
- arrays;
- System health contract;
- System modules contract;
- correlation ID;
- API error envelope.

Unit tests не запускают настоящий network listener.

### Frontend

Framework:

    xUnit

Покрыты:

- API client options;
- URL normalization;
- API path normalization;
- timeout configuration;
- real/mock mode selection;
- DTO mapping;
- successful System requests;
- HTTP error envelope;
- fallback HTTP errors;
- malformed JSON;
- null JSON;
- connection failure;
- timeout conversion;
- cancellation;
- invalid HttpClient configuration;
- error code;
- correlation ID.

### Local smoke-test

Скрипт:

    scripts/test-local-integration.ps1

Проверяет:

- System health;
- System modules;
- JSON contracts;
- development CORS;
- OPTIONS preflight;
- неизвестный origin;
- реальный HTTP listener.

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

Linux Debug и Release проверяются в GitHub Actions.

## Continuous integration

Workflow:

    .github/workflows/ci.yml

Jobs:

- Repository quality;
- Windows VS 2026;
- Ubuntu 24.04.

CI проверяет:

- обязательные repository files;
- CMake presets JSON;
- LF line endings;
- pinned vcpkg baseline;
- Windows backend build;
- Windows CTest;
- Linux Debug backend build;
- Linux Debug CTest;
- Linux Release backend build;
- Linux Release CTest;
- frontend build;
- frontend xUnit tests;
- case-sensitive Linux filesystem.

## Dependency management

Manifest:

    vcpkg.json

Прямые dependencies:

- Catch2;
- Drogon;
- JsonCpp.

vcpkg baseline закреплен.

Windows и Linux используют отдельные host installations vcpkg.

## Line endings

Repository text files используют:

    LF

Политика закреплена через:

- `.gitattributes`;
- `.editorconfig`;
- `scripts/check-line-endings.ps1`;
- CI validation.

## Что пока не реализовано

Отсутствуют:

- Object HTTP API;
- Device HTTP API;
- Tag HTTP API;
- Runtime HTTP API;
- History HTTP API;
- Events HTTP API;
- Alarms HTTP API;
- PostgreSQL repository implementations;
- production configuration loading;
- реальный Modbus TCP driver;
- реальный SNMP driver;
- background polling lifecycle;
- runtime-to-historian pipeline;
- runtime-to-events pipeline;
- runtime-to-alarms pipeline;
- WebSocket или SSE transport;
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

До production stage потребуются:

- security model;
- persistence;
- real protocol drivers;
- service lifecycle;
- observability;
- backup and recovery;
- load and endurance tests;
- deployment automation;
- operator workflows;
- commissioning tools.

## Следующий этап

Следующий спринт:

    Sprint 015 — Object, Device and Tag Read API Foundation

Цель:

- создать первые прикладные read endpoints;
- использовать сформированные JSON и error contracts;
- добавить transport DTO mapping;
- подключить Objects, Devices и Tags pages к typed API clients;
- сохранить автоматическую Windows/Linux проверку.
