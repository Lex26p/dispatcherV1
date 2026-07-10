# Dispatcher Frontend Configuration API Client

## Статус

Foundation создан в Sprint 015, шаг 113.

## Назначение

Typed frontend client предоставляет read access к:

- objects;
- object tree;
- devices;
- tags.

Основной interface:

    IConfigurationApiClient

HTTP implementation:

    ConfigurationHttpApiClient

## Operations

    GetObjectsAsync
    GetObjectTreeAsync
    GetDevicesAsync
    GetTagsAsync

Все operations поддерживают:

    CancellationToken

## Query requests

Созданы:

    ObjectReadRequest
    ObjectTreeReadRequest
    DeviceReadRequest
    TagReadRequest

Query builder:

- сохраняет стабильный порядок parameters;
- пропускает null;
- поддерживает явный false;
- использует URI escaping;
- использует invariant integer formatting;
- не скрывает пустые значения от backend validation.

## Transport DTO

Созданы:

    ObjectCollectionApiResponse
    ObjectApiDto
    ObjectTreeApiResponse
    ObjectTreeNodeApiDto
    DeviceCollectionApiResponse
    DeviceApiDto
    DeviceConnectionApiDto
    TagCollectionApiResponse
    TagApiDto

DTO отражают backend JSON contracts.

## View models

Созданы:

    ConfigurationPage<T>
    ObjectViewModel
    ObjectTreeViewModel
    ObjectTreeNodeViewModel
    DeviceViewModel
    DeviceConnectionViewModel
    TagViewModel

Razor pages не должны работать непосредственно с transport DTO.

## Error behavior

Configuration client переиспользует Dispatcher frontend error conventions.

Поддерживаются:

- HTTP status errors;
- backend error code;
- correlation ID из body;
- correlation ID из response header;
- connection failure;
- timeout;
- caller cancellation;
- malformed JSON;
- null JSON;
- unsupported response content;
- invalid HttpClient configuration.

Ошибки представлены через:

    DispatcherApiException

## Correlation ID

Response correlation ID читается из:

    X-Correlation-Id

Для backend error envelope приоритет имеет:

    error.correlationId

Header используется как fallback.

## DI

Registration:

    IConfigurationApiClient
        -> ConfigurationHttpApiClient

Configuration client всегда использует HTTP backend.

Frontend mock configuration dataset не создается.

## Pages

На шаге 113 client еще не используется Razor pages.

Подключение выполняется:

- Objects page — шаг 114;
- Devices page — шаг 115;
- Tags page — шаг 115.

## Tests

Покрыты:

- Object DTO mapping;
- recursive Object tree mapping;
- Device connection mapping;
- Tag engineering mapping;
- query path generation;
- URI escaping;
- successful Objects response;
- successful Object tree response;
- successful Devices response;
- successful Tags response;
- error envelope;
- backend error code;
- correlation ID;
- malformed JSON;
- null JSON;
- connection failure;
- timeout;
- caller cancellation;
- missing HttpClient base address.
