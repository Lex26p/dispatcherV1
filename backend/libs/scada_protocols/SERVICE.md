# scada_protocols

## Назначение

`scada_protocols` — backend-модуль протокольных контрактов Dispatcher.

Модуль задает общий интерфейс для драйверов связи с оборудованием и внешними источниками данных.

Протокольный слой нужен, чтобы Dispatcher мог работать с разными источниками данных через единый контракт.

Стартовые целевые протоколы:

- Modbus TCP;
- SNMP;
- Simulator.

Будущие возможные протоколы:

- OPC UA;
- MQTT;
- REST polling;
- WebSocket;
- gRPC;
- BACnet;
- Siemens S7;
- IEC 60870-5-104;
- DNP3;
- SQL/file sources;
- корпоративные протоколы.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Протокольный слой находится между устройствами и тегами:

    Device + TagAddress -> IProtocolDriver -> raw value -> TagCurrentValue

## Границы ответственности

`scada_protocols` отвечает за:

- `IProtocolDriver`;
- `ProtocolCapabilities`;
- `ProtocolConnectionCheckRequest`;
- `ProtocolConnectionCheckResult`;
- `ProtocolAddressTestRequest`;
- `ProtocolAddressTestResult`;
- `ProtocolReadRequest`;
- `ProtocolReadResult`;
- `ProtocolBatchReadRequest`;
- `ProtocolBatchReadResult`;
- `ProtocolWriteRequest`;
- `ProtocolWriteResult`;
- общий контракт read;
- общий контракт batch read;
- общий контракт write;
- общий контракт проверки соединения;
- общий контракт проверки адреса;
- подготовку к future protocol drivers.

## Что модуль не должен делать

`scada_protocols` не должен содержать:

- polling scheduler;
- runtime TagValueStore;
- historian;
- alarm engine;
- HTTP API;
- PostgreSQL repository;
- UI-логику;
- hardcoded business logic объекта;
- hardcoded business logic дашбордов.

## Что пока не реализовано

На текущем шаге не реализованы:

- Modbus TCP driver;
- SNMP driver;
- Simulator driver;
- реальные TCP/UDP подключения;
- Boost.Asio;
- retry policy;
- reconnect policy;
- connection pool;
- rate limiting;
- polling groups.

## Основные сущности

### ProtocolCapabilities

Описание возможностей драйвера.

Поля:

- `protocol`;
- `supports_read`;
- `supports_write`;
- `supports_batch_read`;
- `supports_connection_check`;
- `supports_address_test`;
- `supports_subscriptions`;
- `max_batch_size`;
- `default_timeout_ms`;
- `supported_value_types`.

### ProtocolConnectionCheckRequest

Запрос проверки соединения.

Поля:

- `device_id`;
- `connection`;
- `timeout_ms`;
- `correlation_id`.

### ProtocolConnectionCheckResult

Результат проверки соединения.

Поля:

- `success`;
- `quality`;
- `latency_ms`;
- `message`;
- `timestamp`.

### ProtocolAddressTestRequest

Запрос проверки адреса тега.

Поля:

- `device_id`;
- `connection`;
- `address`;
- `timeout_ms`;
- `correlation_id`.

### ProtocolAddressTestResult

Результат проверки адреса тега.

Поля:

- `success`;
- `quality`;
- `normalized_address`;
- `message`;
- `timestamp`.

### ProtocolReadRequest

Запрос чтения одного тега.

Поля:

- `device_id`;
- `connection`;
- `tag_id`;
- `address`;
- `timeout_ms`;
- `correlation_id`.

### ProtocolReadResult

Результат чтения одного тега.

Поля:

- `tag_id`;
- `success`;
- `raw_value`;
- `quality`;
- `latency_ms`;
- `message`;
- `source_timestamp`;
- `server_timestamp`.

### ProtocolBatchReadRequest

Запрос batch-чтения.

Поля:

- `device_id`;
- `connection`;
- `items`;
- `timeout_ms`;
- `correlation_id`.

### ProtocolBatchReadResult

Результат batch-чтения.

Поля:

- `success`;
- `items`;
- `message`;
- `server_timestamp`.

### ProtocolWriteRequest

Запрос записи значения.

Поля:

- `device_id`;
- `connection`;
- `tag_id`;
- `address`;
- `value`;
- `dry_run`;
- `timeout_ms`;
- `correlation_id`.

### ProtocolWriteResult

Результат записи значения.

Поля:

- `tag_id`;
- `success`;
- `dry_run`;
- `quality`;
- `latency_ms`;
- `message`;
- `timestamp`.

### IProtocolDriver

Базовый интерфейс драйвера протокола.

Методы:

- `protocol()`;
- `name()`;
- `capabilities()`;
- `check_connection()`;
- `test_address()`;
- `read()`;
- `read_batch()`;
- `write()`.

## Почему batch read обязателен в контракте

Для SCADA polling важно уметь читать группы тегов эффективно.

Даже если конкретный протокол или тестовый драйвер внутри реализует batch read как последовательный read, внешний polling layer должен работать с единым batch-контрактом.

Это особенно важно для:

- Modbus TCP;
- SNMP;
- OPC UA;
- будущих корпоративных протоколов.

## Почему write уже есть в контракте

Реальное управление командами будет отдельным Command Service.

Но protocol layer должен заранее иметь возможность записи, потому что будущий Command Service будет вызывать драйверы только через безопасный слой команд.

Прямой записи в тег через UI API не будет.

## Dry run

`ProtocolWriteRequest` содержит поле:

    dry_run

Оно нужно для будущих сценариев:

- проверка команды без выполнения;
- simulation mode;
- проверка доступности записи;
- безопасное подтверждение команды.

## Correlation ID

Запросы содержат поле:

    correlation_id

Оно нужно для будущих сценариев:

- трассировка;
- диагностика;
- аудит;
- связь polling events;
- связь command events;
- связь alarm events.

На текущем шаге это простая строка.

Позже можно будет заменить или связать ее с `CorrelationId` из `scada_common`.

## Качество результата

Protocol result использует `TagQuality`.

Протокольный слой может возвращать качества:

- `Good`;
- `CommunicationError`;
- `Timeout`;
- `DeviceError`;
- `ProtocolError`;
- `Bad`;
- `Uncertain`;
- `NotInitialized`.

Финальную обработку качества позже будет выполнять runtime values layer.

## Зависимости

`scada_protocols` зависит от:

- `scada_common`;
- `scada_core`;
- `scada_devices`;
- `scada_tags`.

Причины:

- нужны `DeviceId` и `DeviceConnection`;
- нужны `TagId`;
- нужен `TagAddress`;
- нужен `TagValuePayload`;
- нужен `TagQuality`;
- нужен `ModuleInfo`.

## Основные файлы

- `include/scada_protocols/protocol_capabilities.h`
- `include/scada_protocols/protocol_requests.h`
- `include/scada_protocols/protocol_results.h`
- `include/scada_protocols/protocol_driver.h`
- `include/scada_protocols/protocol_module.h`
- `src/protocol_capabilities.cpp`
- `src/protocol_requests.cpp`
- `src/protocol_results.cpp`
- `src/protocol_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_protocols`.
- Добавлен `ProtocolCapabilities`.
- Добавлены request-модели.
- Добавлены result-модели.
- Добавлен `IProtocolDriver`.
- Добавлен `get_protocol_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем общий контракт протокольных драйверов Dispatcher.
- Готовим основу для Simulator, Modbus TCP, SNMP и будущего polling.

### Нужно доделать

- Добавить Simulator driver.
- Добавить registry драйверов.
- Добавить polling task model.
- Добавить polling group model.
- Добавить polling scheduler foundation.
- Добавить DTO-контракты polling/protocol diagnostics.
- Добавить repository-интерфейсы polling configuration.
- Добавить черновую миграцию polling configuration.

### Расширить позже

- Modbus TCP driver.
- SNMP driver.
- Async I/O through Boost.Asio.
- Connection pooling.
- Retry policy.
- Timeout policy.
- Batch optimizer.
- Protocol diagnostics.
- Protocol metrics.
- Security model for write operations.

### Не входит в этот шаг

- Реальный Modbus TCP.
- Реальный SNMP.
- Реальный polling.
- Runtime values.
- Historian.
- Alarm engine.
- PostgreSQL.
- HTTP API.
- Frontend.