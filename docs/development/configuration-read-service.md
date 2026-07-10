# Dispatcher Configuration Read Service

## Статус

Foundation создан в Sprint 015, шаг 109.

## Назначение

Configuration read service предоставляет transport-neutral чтение:

- objects;
- object tree;
- devices;
- tags.

Основной интерфейс:

    dispatcher::api::IConfigurationReadService

## Module ownership

### scada_api

Владеет:

- read models;
- collection result;
- resource queries;
- service interface;
- domain-to-read-model mapping;
- object tree read-model construction.

### scada_app

Владеет:

- development implementation;
- deterministic snapshot;
- filtering;
- ordering;
- pagination;
- snapshot relationship validation.

### scada_http

На шаге 109 не изменяется.

В следующих шагах будет отвечать за:

- query-string parsing;
- query validation;
- HTTP routes;
- JSON serialization;
- API error envelope.

## Read models

Созданы:

    ObjectReadModel
    ObjectTreeNodeReadModel
    DeviceConnectionReadModel
    DeviceReadModel
    TagReadModel

Read models не содержат:

- Drogon types;
- JsonCpp types;
- HttpRequest;
- HttpResponse;
- frontend DTO;
- repository implementation details.

## Collection result

Используется:

    ReadCollection<T>

Поля:

    items
    total
    offset
    limit
    has_more

Ordering выполняется до pagination.

## Queries

Используются:

    ObjectReadQuery
    DeviceReadQuery
    TagReadQuery

Queries переиспользуют:

    ApiReadQueryOptions
    ApiReadFilter

Object query дополнительно содержит:

    parent_id

## Development snapshot

Snapshot включает:

- 5 objects;
- 4 devices;
- 6 tags.

Objects:

- Site;
- Building;
- Room;
- Line;
- EquipmentGroup.

Devices:

- active Modbus TCP;
- maintenance SNMP IPv6;
- runtime simulator;
- disabled Modbus TCP.

Tags:

- Boolean;
- Float;
- Double;
- String;
- DeviceTag;
- ManualTag;
- SystemTag;
- CommandTag;
- enabled и disabled configuration.

## Validation

При создании provider проверяются:

- object tree validation;
- device domain validation;
- tag domain validation;
- duplicate device IDs;
- duplicate tag IDs;
- device-to-object relationships;
- tag-to-object relationships;
- tag-to-device relationships;
- соответствие object assignment тега и устройства.

Некорректный embedded snapshot блокирует startup через `std::logic_error`.

## Filtering

Objects:

    id
    parent_id
    include_children

Devices:

    id
    object_id
    state
    include_disabled

Tags:

    id
    tag_id
    object_id
    device_id
    include_disabled

## Disabled semantics

По умолчанию Devices скрывают states:

    Disabled
    Decommissioned

По умолчанию Tags скрывают:

    is_enabled = false

Objects не имеют disabled state в текущей domain model.

## Ordering

Objects, Devices и Tags сортируются по:

    code
    id

Object tree roots и siblings используют тот же порядок.

## Pagination

Limit нормализуется через:

    ApiReadQueryOptions::normalized_limit

Offset ограничивается размером filtered result.

Поля `total` и `has_more` вычисляются после filtering и до/после pagination соответственно.

## Factory

Composition root может использовать:

    create_development_configuration_read_service()

Factory возвращает:

    ConfigurationReadServicePtr

Concrete provider не требуется HTTP layer.

## Ограничения

Development provider:

- не является persistence;
- не загружает внешнюю configuration;
- не изменяет данные;
- не поддерживает CRUD;
- не является production source;
- не подключен к HTTP routes на шаге 109.
