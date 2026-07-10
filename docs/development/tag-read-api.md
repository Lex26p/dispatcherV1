# Dispatcher Tag Read API

## Статус

Foundation создан в Sprint 015, шаг 112.

## Endpoint

    GET /api/tags

## Query parameters

Поддерживаются:

    id
    objectId
    deviceId
    includeDisabled
    limit
    offset

Примеры:

    GET /api/tags?id=tag-temperature
    GET /api/tags?objectId=room-control
    GET /api/tags?deviceId=device-modbus-1
    GET /api/tags?includeDisabled=true
    GET /api/tags?limit=2&offset=1

## Default disabled behavior

По умолчанию endpoint скрывает tags с:

    enabled = false

Для включения используется:

    includeDisabled=true

Archive policy `Disabled` не означает, что configuration tag выключен.

Например SystemTag может иметь:

    archivePolicy = Disabled
    enabled = true

## Collection response

Формат:

    {
      "tags": [],
      "count": 0,
      "total": 0,
      "offset": 0,
      "limit": 1000,
      "hasMore": false
    }

`count` — количество элементов текущей страницы.

`total` — количество элементов после filters и до pagination.

## Tag item

Формат:

    {
      "id": "tag-temperature",
      "objectId": "room-control",
      "deviceId": "device-modbus-1",
      "type": "DeviceTag",
      "valueType": "Double",
      "archivePolicy": "OnChange",
      "code": "TEMPERATURE",
      "name": "Room temperature",
      "description": "Control room air temperature.",
      "engineeringUnit": "degC",
      "scale": 0.1,
      "offset": 0.0,
      "address": "40001",
      "enabled": true
    }

## Null relationships

Отсутствующие relationships сериализуются как JSON null:

    objectId: null
    deviceId: null

Пустые строки не используются вместо отсутствующих relationships.

## Tag type values

Текущие стабильные string values:

    DeviceTag
    ManualTag
    CalculatedTag
    SystemTag
    VirtualTag
    CommandTag
    SimulationTag
    ExternalTag

## Value type values

Текущие стабильные string values:

    Boolean
    Int32
    Int64
    Float
    Double
    String
    Enum
    Json

## Archive policy values

Текущие стабильные string values:

    Disabled
    OnChange
    Periodic
    OnChangeWithDeadband
    OnQualityChange
    OnAlarm
    AlwaysButThrottled

## Engineering configuration

Tag response содержит:

    engineeringUnit
    scale
    offset
    address

Current value и quality не входят в configuration contract.

## Ordering

Tags сортируются до pagination:

    code
    id

Source-container order и будущий database row order не являются API order.

## Empty result

Валидный filter без совпадений возвращает:

    HTTP 200

с пустым `tags` array.

## Query validation

Отклоняются:

- unknown parameters;
- duplicate parameters;
- malformed percent encoding;
- пустой id;
- пустой objectId;
- пустой deviceId;
- invalid Boolean;
- invalid unsigned integer;
- limit равный нулю;
- limit выше maximum.

Ошибки используют:

    HTTP 400
    error.code = invalid_query

Error envelope содержит correlation ID и machine-readable details.

## Security

Tag configuration response не содержит:

- current command values;
- credentials;
- secrets;
- access tokens;
- exception messages;
- protocol credentials.

## Composition

Development provider создается в:

    dispatcher_server

Endpoint зависит только от:

    IConfigurationReadService

`scada_http` не зависит от concrete development provider.

## Ограничения

На шаге 112 отсутствуют:

- create;
- update;
- delete;
- single-resource path;
- type filter;
- valueType filter;
- archivePolicy filter;
- current value;
- quality;
- timestamp;
- history;
- alarm state;
- realtime updates;
- persistence;
- authorization;
- frontend Tags integration.
