# scada_api

## Назначение

`scada_api` — backend-модуль API foundation Dispatcher.

Модуль отвечает за базовую модель API endpoint-ов, route registry, response model и mapper foundation.

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
        -> ApiResponse

Mapper foundation:

    domain value / timestamp / primitive
        -> ApiMapper
        -> DTO-friendly string / result / issue

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
- регистрацию будущих route definitions;
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
- WebSocket;
- SSE;
- authorization;
- frontend serving;
- PostgreSQL access;
- domain service orchestration.

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

На текущем шаге конкретные domain-to-DTO мапперы не добавляются.

Добавляется только общий foundation.

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

Методы:

- `is_success()`;
- `has_issues()`;
- `has_message()`.

### make_mapping_success

Создает успешный `ApiMappingResult`.

### make_mapping_error

Создает ошибочный `ApiMappingResult` с одной issue.

### format_timestamp

Форматирует `std::chrono::system_clock::time_point` для DTO/API.

Поддерживает:

- ISO-8601 UTC;
- Unix milliseconds;
- empty string для default timestamp.

### format_bool

Форматирует bool как:

- `true`;
- `false`.

### format_uint64

Форматирует `std::uint64_t` в строку.

## Почему mapper foundation без domain dependencies

На этом шаге `scada_api` не должен зависеть от всех доменных модулей сразу.

Если сразу добавить мапперы для всех доменных моделей, `scada_api` начнет зависеть от:

- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_runtime`;
- `scada_historian`;
- `scada_events`;
- `scada_alarms`.

Это может быстро превратить API module в God-module.

Поэтому сначала добавлен нейтральный foundation:

    mapping status
    mapping result
    timestamp formatting
    primitive formatting
    mapper options

Конкретные мапперы будут добавляться постепенно рядом с endpoint models.

## Почему timestamp форматируется здесь

Почти все будущие API DTO содержат timestamps:

- runtime values;
- history samples;
- events;
- alarms;
- alarm transitions.

Нужно единое место, где определяется API-представление времени.

На текущем этапе выбран основной формат:

    ISO-8601 UTC

Пример будущего значения:

    2026-07-09T12:34:56.789Z

## Почему body в ApiResponse пока строка

На текущем этапе JSON serialization еще не добавлена.

`body` хранится строкой как transport-neutral payload placeholder.

Позже появятся:

- JSON mapper;
- typed request DTO;
- typed response DTO;
- serialization layer.

## Почему API foundation без HTTP-сервера

На текущем этапе важно зафиксировать архитектуру API до выбора сетевой реализации.

Это позволяет:

- не привязаться преждевременно к конкретной библиотеке;
- описать route model;
- описать response model;
- подготовить DTO mapper layer;
- оставить возможность использовать Boost.Beast, uWebSockets, Crow или другой транспорт позже.

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
- `include/scada_api/api_mapper.h`
- `include/scada_api/api_module.h`
- `src/api_endpoint.cpp`
- `src/api_response.cpp`
- `src/api_route_registry.cpp`
- `src/api_mapper.cpp`
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
- Добавлен `get_api_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем API foundation.
- Готовим route model.
- Готовим response model.
- Готовим mapper foundation.
- Готовим будущую API surface.

### Нужно доделать

- Добавить read endpoint model.
- Добавить DTO mapper functions для конкретных read endpoints.
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