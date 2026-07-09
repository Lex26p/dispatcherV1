# scada_api

## Назначение

`scada_api` — backend-модуль API foundation Dispatcher.

Модуль отвечает за базовую модель API endpoint-ов, route registry и response model.

На текущем шаге модуль не запускает реальный HTTP-сервер.

## Базовая цепочка

Будущая цепочка API:

    HTTP request
        -> API route
        -> controller / handler
        -> application service
        -> domain module
        -> DTO
        -> API response

Текущий foundation:

    ApiEndpoint
        -> ApiRouteRegistry
        -> ApiResponse

## Границы ответственности

`scada_api` отвечает за:

- `ApiHttpMethod`;
- `ApiEndpointArea`;
- `ApiEndpoint`;
- `ApiRouteRegistry`;
- `ApiResponseStatus`;
- `ApiResponse`;
- регистрацию будущих route definitions;
- базовую response model;
- подготовку к HTTP API;
- подготовку к realtime gateway;
- подготовку к DTO mapper layer.

## Что модуль не должен делать сейчас

На текущем шаге `scada_api` не должен выполнять:

- открытие TCP-порта;
- запуск HTTP listener;
- обработку HTTP request;
- JSON serialization;
- routing на уровне сетевой библиотеки;
- controller execution;
- WebSocket;
- SSE;
- authorization;
- frontend serving.

## Основные сущности

### ApiHttpMethod

Метод будущего HTTP endpoint.

Поддерживаются значения:

- `Get`;
- `Post`;
- `Put`;
- `Patch`;
- `Delete`.

### ApiEndpointArea

Логическая область endpoint.

Поддерживаются значения:

- `System`;
- `Objects`;
- `Devices`;
- `Tags`;
- `Runtime`;
- `Historian`;
- `Events`;
- `Alarms`;
- `Realtime`;
- `Unknown`.

### ApiEndpoint

Описание будущего endpoint.

Поля:

- `method`;
- `area`;
- `path`;
- `name`;
- `description`;
- `requires_authentication`;
- `realtime_capable`.

Методы:

- `has_path()`;
- `has_name()`;
- `has_description()`;
- `is_public()`;
- `is_valid()`.

### ApiRouteRegistry

In-memory registry описаний endpoint-ов.

Методы:

- `empty()`;
- `size()`;
- `endpoints()`;
- `register_endpoint()`;
- `find_by_path()`;
- `find_by_area()`;
- `clear()`.

На текущем этапе registry хранит только описания route.

Он не выполняет handler dispatch.

### create_default_api_route_registry

Создает начальный набор будущих endpoint definitions:

- `/api/system/health`;
- `/api/system/modules`;
- `/api/runtime/values`;
- `/api/history/query`;
- `/api/events`;
- `/api/alarms/active`;
- `/api/realtime`.

Эти endpoint-ы пока не являются реальными HTTP endpoint-ами.

Они нужны для стабилизации будущей структуры API.

### ApiResponseStatus

Статус будущего API response.

Поддерживаются значения:

- `Ok`;
- `Created`;
- `Accepted`;
- `NoContent`;
- `BadRequest`;
- `Unauthorized`;
- `Forbidden`;
- `NotFound`;
- `Conflict`;
- `ValidationError`;
- `InternalError`;
- `NotImplemented`.

### ApiResponse

Базовая response model.

Поля:

- `status`;
- `success`;
- `http_status_code`;
- `correlation_id`;
- `message`;
- `body`.

Методы:

- `is_success()`;
- `has_correlation_id()`;
- `has_message()`;
- `has_body()`.

## Почему API foundation без HTTP-сервера

На текущем этапе важно зафиксировать архитектуру API до выбора сетевой реализации.

Это позволяет:

- не привязаться преждевременно к конкретной библиотеке;
- описать route model;
- описать response model;
- подготовить DTO mapper layer;
- оставить возможность использовать Boost.Beast, uWebSockets, Crow или другой транспорт позже.

## Почему body пока строка

На текущем этапе JSON serialization еще не добавлена.

`body` хранится строкой как transport-neutral payload placeholder.

Позже появятся:

- JSON mapper;
- typed request DTO;
- typed response DTO;
- serialization layer.

## Почему route registry не содержит handlers

Handlers требуют application services, repositories и composition root.

Пока этих слоев нет.

На текущем этапе registry хранит только route metadata.

Будущий API layer сможет добавить handler binding отдельно.

## Почему realtime указан как endpoint area

Realtime gateway будет отдельным слоем, но должен быть виден в общей API surface.

Поэтому `Realtime` добавлен как endpoint area.

Фактический WebSocket/SSE foundation появится отдельным шагом.

## Зависимости

`scada_api` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_contracts`.

Причины:

- нужны общие типы;
- нужен `ModuleInfo`;
- API будет работать с DTO contracts.

На текущем шаге `scada_api` не зависит от доменных модулей напрямую.

Это сделано намеренно, чтобы API layer не стал God-module.

## Основные файлы

- `include/scada_api/api_endpoint.h`
- `include/scada_api/api_response.h`
- `include/scada_api/api_route_registry.h`
- `include/scada_api/api_module.h`
- `src/api_endpoint.cpp`
- `src/api_response.cpp`
- `src/api_route_registry.cpp`
- `src/api_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_api`.
- Добавлен `ApiHttpMethod`.
- Добавлен `ApiEndpointArea`.
- Добавлен `ApiEndpoint`.
- Добавлен `ApiRouteRegistry`.
- Добавлен `ApiResponseStatus`.
- Добавлен `ApiResponse`.
- Добавлен `create_default_api_route_registry()`.
- Добавлен `get_api_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем API foundation.
- Готовим route model.
- Готовим response model.
- Готовим будущую API surface.
- Готовим переход к mapper foundation.

### Нужно доделать

- Добавить API mapper foundation.
- Добавить read endpoint model.
- Добавить realtime gateway foundation.
- Добавить application composition.
- Добавить настоящий HTTP transport позже.

### Расширить позже

- HTTP listener.
- Request parser.
- Response serializer.
- JSON serialization.
- OpenAPI.
- Controllers.
- Middleware.
- Authorization.
- Rate limiting.
- WebSocket.
- SSE.
- API tests.

### Не входит в этот шаг

- HTTP server.
- WebSocket.
- JSON serialization.
- Controllers.
- Frontend.