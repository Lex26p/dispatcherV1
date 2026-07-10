# scada_app

## Назначение

`scada_app` — backend-модуль application composition Dispatcher.

Модуль отвечает за сборку foundation-конфигурации приложения:

- API route registry;
- API read endpoint catalog;
- realtime gateway startup plan;
- gateway startup options;
- application composition snapshot.

На текущем этапе модуль не запускает реальный HTTP/WebSocket сервер.

## Базовая цепочка

Текущая composition-цепочка:

    scada_api
        -> ApiRouteRegistry
        -> ApiReadEndpointCatalog
        -> scada_app

    scada_realtime
        -> Realtime foundation
        -> scada_app

    scada_app
        -> ApplicationComposition
        -> GatewayStartupPlan

Будущая runtime-цепочка:

    dispatcher_server
        -> ApplicationComposition
        -> HTTP API transport
        -> Realtime transport
        -> domain modules

## Границы ответственности

`scada_app` отвечает за:

- `GatewayMode`;
- `GatewayStartupStatus`;
- `GatewayStartupOptions`;
- `GatewayStartupPlan`;
- `ApplicationComposition`;
- создание gateway startup plan;
- создание default application composition;
- связывание API foundation и realtime foundation;
- подготовку к будущему composition root;
- подготовку к будущему startup lifecycle.

## Что модуль не должен делать сейчас

На текущем шаге `scada_app` не должен выполнять:

- запуск HTTP listener;
- запуск WebSocket server;
- запуск SSE server;
- обработку HTTP request;
- обработку WebSocket frames;
- JSON serialization;
- вызов repositories;
- вызов domain services;
- авторизацию;
- управление потоками;
- graceful shutdown;
- background workers;
- frontend serving.

## Основные сущности

### GatewayMode

Режим gateway.

Поддерживаются значения:

- `Disabled`;
- `ApiOnly`;
- `RealtimeOnly`;
- `ApiAndRealtime`.

### GatewayStartupStatus

Статус startup plan.

Поддерживаются значения:

- `NotStarted`;
- `Prepared`;
- `Running`;
- `Stopped`;
- `Failed`.

На текущем этапе реальный статус `Running` не выставляется автоматически, потому что настоящий transport еще не запускается.

### GatewayStartupOptions

Опции будущего gateway.

Поля:

- `mode`;
- `bind_address`;
- `http_port`;
- `realtime_port`;
- `api_base_path`;
- `realtime_path`;
- `enable_api`;
- `enable_realtime`;
- `require_authentication`;
- `allow_loopback_only`.

Методы:

- `has_bind_address()`;
- `has_api_base_path()`;
- `has_realtime_path()`;
- `api_enabled()`;
- `realtime_enabled()`;
- `is_valid()`.

### GatewayStartupPlan

План будущего запуска gateway.

Поля:

- `options`;
- `status`;
- `api_route_count`;
- `api_read_endpoint_count`;
- `realtime_channel_count`;
- `message`.

Методы:

- `is_prepared()`;
- `is_running()`;
- `has_message()`;
- `api_enabled()`;
- `realtime_enabled()`.

### create_gateway_startup_plan

Функция создает startup plan на основе:

- gateway options;
- количества API routes;
- количества read endpoints;
- количества realtime channels.

Если options некорректны, plan получает статус `Failed`.

Если mode равен `Disabled`, plan получает статус `Stopped`.

Если options корректны, plan получает статус `Prepared`.

### ApplicationComposition

Snapshot текущей application composition.

Поля:

- `api_routes`;
- `api_read_endpoints`;
- `gateway_startup_plan`;
- `name`;
- `environment`;
- `api_configured`;
- `realtime_configured`;
- `ready`.

Методы:

- `has_name()`;
- `has_environment()`;
- `has_api_routes()`;
- `has_api_read_endpoints()`;
- `is_ready()`;
- `api_route_count()`;
- `api_read_endpoint_count()`.

### create_default_application_composition

Функция создает default composition.

Она собирает:

- `create_default_api_route_registry()`;
- `create_default_api_read_endpoint_catalog()`;
- `create_gateway_startup_plan()`.

## Почему scada_app вынесен отдельно

`scada_api` должен описывать API.

`scada_realtime` должен описывать realtime.

`dispatcher_server` должен быть точкой запуска приложения.

Но между ними нужен слой composition.

Этот слой не должен жить внутри API или realtime, потому что он связывает несколько подсистем.

Поэтому добавлен отдельный модуль:

    scada_app

## Почему это еще не настоящий composition root

Настоящий composition root позже будет отвечать за:

- создание repositories;
- создание services;
- wiring modules;
- loading configuration;
- startup lifecycle;
- shutdown lifecycle;
- background workers;
- HTTP transport;
- realtime transport;
- database connections.

На текущем этапе `scada_app` только готовит foundation.

## Почему gateway startup plan не запускает сервер

Пока не выбрана и не подключена transport-библиотека.

Startup plan нужен, чтобы:

- зафиксировать настройки gateway;
- проверить корректность options;
- посчитать route/read endpoint/realtime foundation;
- подготовить структуру будущего запуска.

Реальный запуск будет добавлен позже.

## Почему bind address по умолчанию 127.0.0.1

На раннем этапе безопаснее использовать loopback-only режим.

Это снижает риск случайного открытия незавершенного API во внешнюю сеть.

Позже настройки будут загружаться из конфигурации.

## Почему http_port и realtime_port сейчас одинаковые

На старте предполагается, что HTTP API и realtime endpoint могут жить на одном порту.

Например:

    HTTP API: /api/...
    WebSocket: /api/realtime

Позже можно будет разделить ports, если понадобится.

## Почему realtime_channel_count пока константа

В `scada_realtime` пока нет channel registry.

На текущем этапе известны базовые каналы:

- System;
- Runtime;
- Historian;
- Events;
- Alarms;
- Diagnostics.

Поэтому в composition используется foundation-count.

Позже появится реальный channel registry.

## Зависимости

`scada_app` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_api`;
- `scada_realtime`.

Причины:

- нужны общие типы;
- нужен `ModuleInfo`;
- нужны API route/read endpoint catalogs;
- нужен realtime foundation.

На текущем шаге `scada_app` не зависит от:

- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_runtime`;
- `scada_historian`;
- `scada_events`;
- `scada_alarms`.

Это сделано намеренно, чтобы composition foundation не стал преждевременно зависеть от всех доменных модулей.

## Основные файлы

- `include/scada_app/gateway_startup.h`
- `include/scada_app/application_composition.h`
- `include/scada_app/app_module.h`
- `src/gateway_startup.cpp`
- `src/application_composition.cpp`
- `src/app_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_app`.
- Добавлен `GatewayMode`.
- Добавлен `GatewayStartupStatus`.
- Добавлен `GatewayStartupOptions`.
- Добавлен `GatewayStartupPlan`.
- Добавлена функция `create_gateway_startup_plan()`.
- Добавлен `ApplicationComposition`.
- Добавлена функция `create_default_application_composition()`.
- Добавлен `get_app_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем application composition foundation.
- Готовим gateway startup foundation.
- Связываем API foundation и realtime foundation.
- Готовим будущий composition root.

### Нужно доделать

- Добавить настоящий HTTP transport позже.
- Добавить настоящий realtime transport позже.
- Добавить service wiring.
- Добавить configuration loading.
- Добавить startup/shutdown lifecycle.
- Добавить repository implementations позже.

### Расширить позже

- App settings.
- Environment profiles.
- Dependency injection.
- Service container.
- Background workers.
- Health checks.
- HTTP listener.
- WebSocket listener.
- Graceful shutdown.
- Metrics.
- Diagnostics.

### Не входит в этот шаг

- HTTP server.
- WebSocket server.
- SSE server.
- JSON serialization.
- Repositories.
- Frontend.
