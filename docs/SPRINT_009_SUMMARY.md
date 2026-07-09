# Sprint 009 Summary

## Название спринта

API and Realtime Gateway

## Статус

Closed

## Диапазон шагов

Шаги 62–67.

---

# Цель спринта

Создать foundation backend API и realtime gateway.

Sprint 009 должен был подготовить цепочку:

    Domain modules / DTO
        -> API foundation
        -> Read endpoint model
        -> Realtime foundation
        -> Application composition
        -> future HTTP/WebSocket transport

---

# Выполненные шаги

## Шаг 62

Создан модуль `scada_api`.

Результат:

- создан каталог `backend/libs/scada_api`;
- добавлен `ApiHttpMethod`;
- добавлен `ApiEndpointArea`;
- добавлен `ApiEndpoint`;
- добавлен `ApiRouteRegistry`;
- добавлен `ApiResponseStatus`;
- добавлен `ApiResponse`;
- добавлен `create_default_api_route_registry()`;
- добавлен `get_api_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

Начальный route registry содержит будущие endpoint definitions:

- `/api/system/health`;
- `/api/system/modules`;
- `/api/runtime/values`;
- `/api/history/query`;
- `/api/events`;
- `/api/alarms/active`;
- `/api/realtime`.

Реальный HTTP-сервер не добавлялся.

---

## Шаг 63

Добавлен API mapper foundation.

Результат:

- добавлен `ApiMappingStatus`;
- добавлен `ApiTimestampFormat`;
- добавлен `ApiMapperOptions`;
- добавлен `ApiMappingIssue`;
- добавлен `ApiMappingResult`;
- добавлена функция `make_mapping_success()`;
- добавлена функция `make_mapping_error()`;
- добавлена функция `format_timestamp()`;
- добавлена функция `format_bool()`;
- добавлена функция `format_uint64()`.

Конкретные domain-to-DTO мапперы не добавлялись.

---

## Шаг 64

Добавлен API read endpoint model foundation.

Результат:

- добавлен `ApiReadResourceType`;
- добавлен `ApiReadEndpointKind`;
- добавлен `ApiReadQueryOptions`;
- добавлен `ApiReadFilter`;
- добавлен `ApiReadEndpointDefinition`;
- добавлен `ApiReadEndpointCatalog`;
- добавлена функция `create_default_api_read_endpoint_catalog()`.

Начальный read endpoint catalog содержит будущие read definitions:

- `/api/system/health`;
- `/api/system/modules`;
- `/api/objects/tree`;
- `/api/objects`;
- `/api/devices`;
- `/api/tags`;
- `/api/runtime/values`;
- `/api/history/query`;
- `/api/events`;
- `/api/alarms/active`;
- `/api/alarms/rules`.

Реальные controllers, handlers и repository calls не добавлялись.

---

## Шаг 65

Создан модуль `scada_realtime`.

Результат:

- создан каталог `backend/libs/scada_realtime`;
- добавлен `RealtimeMessageId`;
- добавлен `RealtimeClientId`;
- добавлен `RealtimeSubscriptionId`;
- добавлен `RealtimeTimestamp`;
- добавлен `RealtimeChannelType`;
- добавлен `RealtimeMessageType`;
- добавлен `RealtimePayloadFormat`;
- добавлен `RealtimeDeliveryMode`;
- добавлен `RealtimeSubscriptionState`;
- добавлен `RealtimeMessage`;
- добавлен `RealtimeSubscription`;
- добавлена функция `make_realtime_message()`;
- добавлена функция `make_realtime_subscription()`;
- добавлен `get_realtime_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

Реальный WebSocket/SSE transport не добавлялся.

---

## Шаг 66

Создан модуль `scada_app`.

Результат:

- создан каталог `backend/libs/scada_app`;
- добавлен `GatewayMode`;
- добавлен `GatewayStartupStatus`;
- добавлен `GatewayStartupOptions`;
- добавлен `GatewayStartupPlan`;
- добавлен `ApplicationComposition`;
- добавлена функция `create_gateway_startup_plan()`;
- добавлена функция `create_default_application_composition()`;
- добавлен `get_app_module_info()`;
- модуль подключен к CMake;
- модуль подключен к `dispatcher_server`.

`scada_app` связывает:

- API route registry;
- API read endpoint catalog;
- realtime foundation;
- gateway startup plan.

Реальный запуск HTTP/WebSocket сервера не добавлялся.

---

## Шаг 67

Обновлена документация и закрыт Sprint 009.

Результат:

- обновлен `docs/development-log.md`;
- обновлен `docs/known-limitations.md`;
- создан `docs/SPRINT_009_SUMMARY.md`.

---

# Итоговые артефакты

## Backend API layer

Основной модуль:

- `backend/libs/scada_api`.

Основные сущности:

- `ApiHttpMethod`;
- `ApiEndpointArea`;
- `ApiEndpoint`;
- `ApiRouteRegistry`;
- `ApiResponseStatus`;
- `ApiResponse`;
- `ApiMappingStatus`;
- `ApiTimestampFormat`;
- `ApiMapperOptions`;
- `ApiMappingIssue`;
- `ApiMappingResult`;
- `ApiReadResourceType`;
- `ApiReadEndpointKind`;
- `ApiReadQueryOptions`;
- `ApiReadFilter`;
- `ApiReadEndpointDefinition`;
- `ApiReadEndpointCatalog`.

Основные factory/helper функции:

- `create_default_api_route_registry()`;
- `make_success_response()`;
- `make_error_response()`;
- `make_mapping_success()`;
- `make_mapping_error()`;
- `format_timestamp()`;
- `format_bool()`;
- `format_uint64()`;
- `create_default_api_read_endpoint_catalog()`;
- `get_api_module_info()`.

---

## Backend realtime layer

Основной модуль:

- `backend/libs/scada_realtime`.

Основные сущности:

- `RealtimeMessageId`;
- `RealtimeClientId`;
- `RealtimeSubscriptionId`;
- `RealtimeTimestamp`;
- `RealtimeChannelType`;
- `RealtimeMessageType`;
- `RealtimePayloadFormat`;
- `RealtimeDeliveryMode`;
- `RealtimeSubscriptionState`;
- `RealtimeMessage`;
- `RealtimeSubscription`.

Основные factory/helper функции:

- `make_realtime_message()`;
- `make_realtime_subscription()`;
- `get_realtime_module_info()`.

---

## Application composition layer

Основной модуль:

- `backend/libs/scada_app`.

Основные сущности:

- `GatewayMode`;
- `GatewayStartupStatus`;
- `GatewayStartupOptions`;
- `GatewayStartupPlan`;
- `ApplicationComposition`.

Основные factory/helper функции:

- `create_gateway_startup_plan()`;
- `create_default_application_composition()`;
- `get_app_module_info()`.

---

## Dispatcher server

`dispatcher_server` теперь регистрирует backend-модули:

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
- `scada_realtime`;
- `scada_app`.

---

## Documentation

Обновлены документы:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_009_SUMMARY.md`.

---

# Что не входило в Sprint 009

В Sprint 009 сознательно не входили:

- настоящий HTTP server;
- настоящий WebSocket server;
- SSE server;
- JSON serialization;
- REST controllers;
- request handlers;
- repository implementations;
- authorization;
- OpenAPI;
- frontend.

---

# Архитектурный результат

После Sprint 009 Dispatcher имеет foundation API and realtime gateway layer.

Текущая архитектурная цепочка:

    scada_common
        ↓
    scada_core
        ↓
    domain modules
        ↓
    scada_contracts
        ↓
    scada_api
        ↓
    scada_realtime
        ↓
    scada_app
        ↓
    future HTTP/WebSocket transport

Доменные модули:

    scada_objects
    scada_devices
    scada_tags
    scada_protocols
    scada_polling
    scada_runtime
    scada_historian
    scada_events
    scada_alarms

---

# Текущий flow после Sprint 009

Текущий API foundation flow:

    ApiEndpoint
        ↓
    ApiRouteRegistry
        ↓
    ApiReadEndpointCatalog
        ↓
    future controller / handler

Текущий response foundation flow:

    ApiResponseStatus
        ↓
    ApiResponse
        ↓
    future HTTP response serializer

Текущий mapper foundation flow:

    domain value / primitive / timestamp
        ↓
    ApiMapper foundation
        ↓
    future DTO mapper

Текущий realtime foundation flow:

    RealtimeMessage
        ↓
    RealtimeSubscription
        ↓
    future Realtime Gateway

Текущий application composition flow:

    ApiRouteRegistry
        +
    ApiReadEndpointCatalog
        +
    GatewayStartupOptions
        ↓
    ApplicationComposition
        ↓
    GatewayStartupPlan

---

# Следующий спринт

## Sprint 010 — Minimal Operator UI and MVP stabilization

Предварительная цель:

Создать минимальный frontend/operator UI foundation и стабилизировать MVP-контур.

Ожидаемые направления:

- Blazor WebAssembly project foundation;
- MudBlazor foundation;
- minimal layout;
- system/modules page;
- object/device/tag placeholder pages;
- runtime values placeholder page;
- events/alarms placeholder pages;
- frontend API client placeholder;
- MVP stabilization docs;
- final MVP limitations.

---

# Статус закрытия

Sprint 009 считается закрытым после выполнения условий:

- проект собирается;
- `dispatcher_server` запускается;
- документация обновлена;
- изменения закоммичены;
- изменения отправлены в `master`.