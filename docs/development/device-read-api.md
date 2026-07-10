# Dispatcher Device Read API

## Статус

Foundation создан в Sprint 015, шаг 111.

## Endpoint

    GET /api/devices

## Query parameters

Поддерживаются:

    id
    objectId
    state
    includeDisabled
    limit
    offset

Примеры:

    GET /api/devices?id=device-modbus-1
    GET /api/devices?objectId=room-control
    GET /api/devices?state=Maintenance
    GET /api/devices?includeDisabled=true
    GET /api/devices?limit=2&offset=1

## Device states

Допустимы стабильные case-sensitive values:

    Draft
    Commissioning
    Active
    Maintenance
    Disabled
    OutOfService
    Decommissioned
    Simulation

Некорректное значение возвращает:

    HTTP 400
    error.code = invalid_query
    details.field = state
    details.reason = unsupported_value

## Default disabled behavior

По умолчанию endpoint скрывает devices со states:

    Disabled
    Decommissioned

Для включения используется:

    includeDisabled=true

Maintenance, OutOfService и Simulation не считаются disabled configuration.

## Collection response

Формат:

    {
      "devices": [],
      "count": 0,
      "total": 0,
      "offset": 0,
      "limit": 1000,
      "hasMore": false
    }

`count` — количество элементов текущей страницы.

`total` — количество элементов после filters и до pagination.

## Device item

Формат:

    {
      "id": "device-modbus-1",
      "objectId": "room-control",
      "code": "PLC_ROOM",
      "name": "Control room PLC",
      "description": "Development Modbus TCP controller.",
      "protocol": "ModbusTcp",
      "state": "Active",
      "runtimeEnabled": true,
      "connection": {
        "host": "127.0.0.1",
        "port": 1502,
        "ipMode": "IPv4",
        "networkInterface": ""
      }
    }

Отсутствующий `objectId` сериализуется как JSON null.

## Protocol values

Current development snapshot содержит:

    ModbusTcp
    Snmp
    Simulator

Wire contract использует стабильные domain string values.

## Connection contract

Возвращаются только non-secret engineering fields:

    host
    port
    ipMode
    networkInterface

Не возвращаются:

- passwords;
- tokens;
- SNMP community strings;
- secret references;
- credentials.

## Ordering

Devices сортируются до pagination:

    code
    id

Порядок source container или будущих database rows не используется как API order.

## Empty result

Валидный filter без совпадений возвращает:

    HTTP 200

с пустым `devices` array.

## Query validation

Отклоняются:

- unknown parameters;
- duplicate parameters;
- malformed percent encoding;
- пустой id;
- пустой objectId;
- пустой state;
- unsupported state;
- invalid Boolean;
- invalid unsigned integer;
- limit равный нулю;
- limit выше maximum.

Ошибки используют единый Dispatcher error envelope и correlation ID.

## Composition

Development provider создается в:

    dispatcher_server

Endpoint зависит только от:

    IConfigurationReadService

`scada_http` не зависит от concrete development provider.

## Ограничения

На шаге 111 отсутствуют:

- create;
- update;
- delete;
- single-resource path;
- protocol filter;
- connection health;
- polling state;
- current tag values;
- persistence;
- authorization;
- frontend Devices integration.
