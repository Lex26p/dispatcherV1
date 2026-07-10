# scada_api

## Назначение

`scada_api` — backend-модуль API foundation Dispatcher.

Модуль отвечает за базовую модель API endpoint-ов, route registry, response model, mapper foundation и read endpoint model.

На текущем этапе модуль не запускает реальный HTTP-сервер.

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
        -> ApiReadEndpointCatalog
        -> ApiResponse

Mapper foundation:

    domain value / timestamp / primitive
        -> ApiMapper
        -> DTO-friendly string / result / issue

Read endpoint foundation:

    ApiReadResourceType
        -> ApiReadEndpointDefinition
        -> ApiReadEndpointCatalog

## Границы ответственности

`scada_api` отвечает за:

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
- `ApiReadEndpointCatalog`;
- регистрацию будущих route definitions;
- описание будущих read endpoints;
- базовую response model;
- foundation mapper-слой;
- стандартное форматирование timestamp;
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
- вызов repository;
- вызов domain services;
- WebSocket;
- SSE;
- authorization;
- frontend serving;
- PostgreSQL access.

## API endpoint foundation

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

## API response foundation

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

## Mapper foundation

Mapper foundation нужен для будущего преобразования domain model в DTO.

Примеры будущих преобразований:

    ObjectNode -> ObjectNodeDto
    Device -> DeviceDto
    Tag -> TagDto
    TagCurrentValue -> RuntimeValueDto
    HistorySample -> HistorySampleDto
    EventRecord -> EventRecordDto
    AlarmRecord -> AlarmRecordDto

На текущем этапе конкретные domain-to-DTO мапперы не добавляются.

Добавлен только общий foundation.

### ApiMappingStatus

Статус mapping operation.

Поддерживаются значения:

- `NotMapped`;
- `Mapped`;
- `Skipped`;
- `InvalidInput`;
- `UnsupportedType`;
- `Failed`.

### ApiTimestampFormat

Формат timestamp для будущих DTO.

Поддерживаются значения:

- `Iso8601Utc`;
- `UnixMilliseconds`;
- `EmptyForDefault`.

### ApiMapperOptions

Опции mapper-а.

Поля:

- `timestamp_format`;
- `include_empty_fields`;
- `include_internal_ids`;
- `strict`.

### ApiMappingIssue

Одна проблема mapping operation.

Поля:

- `status`;
- `field`;
- `message`.

### ApiMappingResult

Результат mapping operation.

Поля:

- `status`;
- `issues`;
- `message`.

## Read endpoint foundation

Read endpoint foundation описывает будущие endpoint-ы чтения.

Он нужен, чтобы заранее зафиксировать API surface, но не добавлять реальные controllers.

### ApiReadResourceType

Тип читаемого ресурса.

Поддерживаются значения:

- `SystemHealth`;
- `SystemModules`;
- `Objects`;
- `ObjectTree`;
- `Devices`;
- `Tags`;
- `RuntimeValues`;
- `HistorySamples`;
- `Events`;
- `ActiveAlarms`;
- `AlarmRules`;
- `Unknown`.

### ApiReadEndpointKind

Тип read endpoint.

Поддерживаются значения:

- `Collection`;
- `SingleResource`;
- `Tree`;
- `Query`;
- `StreamPreview`.

### ApiReadQueryOptions

Опции read query.

Поля:

- `limit`;
- `offset`;
- `max_limit`;
- `include_metadata`;
- `include_disabled`;
- `include_children`;
- `include_raw_values`.

Методы:

- `normalized_limit()`;
- `has_offset()`.

### ApiReadFilter

Общий фильтр будущих read endpoint-ов.

Поля:

- `id`;
- `object_id`;
- `device_id`;
- `tag_id`;
- `source_id`;
- `correlation_id`;
- `state`;
- `quality`;
- `severity`;
- `priority`;
- `from_timestamp`;
- `to_timestamp`.

Методы:

- `has_id()`;
- `has_object_id()`;
- `has_device_id()`;
- `has_tag_id()`;
- `has_source_id()`;
- `has_correlation_id()`;
- `has_state()`;
- `has_quality()`;
- `has_severity()`;
- `has_priority()`;
- `has_time_range()`;
- `has_any_filter()`.

### ApiReadEndpointDefinition

Описание будущего read endpoint.

Поля:

- `resource_type`;
- `kind`;
- `endpoint`;
- `supports_filtering`;
- `supports_pagination`;
- `supports_time_range`;
- `supports_realtime`;
- `returns_collection`.

Методы:

- `has_endpoint()`;
- `is_valid()`.

### ApiReadEndpointCatalog

Каталог будущих read endpoint definitions.

Методы:

- `empty()`;
- `size()`;
- `definitions()`;
- `register_definition()`;
- `find_by_resource_type()`;
- `find_by_area()`;
- `clear()`.

На текущем этапе catalog не вызывает handlers и repositories.

### create_default_api_read_endpoint_catalog

Создает начальный каталог будущих read endpoint definitions:

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

Эти endpoint-ы пока не являются реальными HTTP endpoint-ами.

Они нужны для стабилизации будущей структуры API.

## Почему read endpoint model без controllers

Controllers требуют:

- application services;
- repositories;
- authorization;
- serialization;
- error mapping;
- composition root.

Эти слои еще не готовы.

На текущем этапе мы фиксируем API surface, но не реализуем execution.

## Почему read filters являются строковыми

`ApiReadFilter` находится на границе API.

На этой границе значения еще приходят из HTTP query string.

Поэтому фильтры представлены строками.

Позже controller или mapper layer будет превращать их в типизированные domain filters.

## Почему scada_api не зависит от domain modules

На этом этапе `scada_api` не должен зависеть от всех доменных модулей сразу.

Если сразу добавить прямые зависимости, API module станет God-module.

Поэтому сейчас `scada_api` зависит только от:

- `scada_common`;
- `scada_core`;
- `scada_contracts`.

Конкретные domain integrations будут добавляться через application composition и service interfaces.

## Почему timestamp форматируется здесь

Почти все будущие API DTO содержат timestamps:

- runtime values;
- history samples;
- events;
- alarms;
- alarm transitions.

Нужно единое место, где определяется API-представление времени.

Основной формат:

    ISO-8601 UTC

Пример будущего значения:

    2026-07-09T12:34:56.789Z

## Почему API foundation без HTTP-сервера

На текущем этапе важно зафиксировать архитектуру API до выбора сетевой реализации.

Это позволяет:

- не привязаться преждевременно к конкретной библиотеке;
- описать route model;
- описать response model;
- подготовить DTO mapper layer;
- подготовить read endpoint model;
- оставить возможность использовать Boost.Beast, uWebSockets, Crow или другой транспорт позже.

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

## Основные файлы

- `include/scada_api/api_endpoint.h`
- `include/scada_api/api_response.h`
- `include/scada_api/api_route_registry.h`
- `include/scada_api/api_mapper.h`
- `include/scada_api/api_read_endpoint.h`
- `include/scada_api/api_module.h`
- `src/api_endpoint.cpp`
- `src/api_response.cpp`
- `src/api_route_registry.cpp`
- `src/api_mapper.cpp`
- `src/api_read_endpoint.cpp`
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
- Добавлен API mapper foundation.
- Добавлен timestamp formatter.
- Добавлен read endpoint model foundation.
- Добавлен `ApiReadEndpointCatalog`.
- Добавлен `create_default_api_read_endpoint_catalog()`.
- Добавлен `get_api_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем API foundation.
- Готовим route model.
- Готовим response model.
- Готовим mapper foundation.
- Готовим read endpoint model.
- Готовим будущую API surface.

### Нужно доделать

- Добавить realtime gateway foundation.
- Добавить application composition.
- Добавить gateway startup foundation.
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
- Repository calls.
- Frontend.
