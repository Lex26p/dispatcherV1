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
- `SimulatorProtocolDriver`;
- `ProtocolDriverRegistry`;
- `create_default_protocol_driver_registry()`;
- общий контракт read;
- общий контракт batch read;
- общий контракт write;
- общий контракт проверки соединения;
- общий контракт проверки адреса;
- базовый simulator driver;
- registry протокольных драйверов;
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

### ProtocolDriverRegistry

Registry протокольных драйверов.

Он хранит драйверы по `DeviceProtocol`.

Методы:

- `register_driver()`;
- `replace_driver()`;
- `unregister_driver()`;
- `has_driver()`;
- `find_driver()`;
- `protocols()`;
- `drivers()`;
- `size()`;
- `empty()`;
- `clear()`.

Registry нужен для будущего polling layer.

Polling layer не должен создавать драйверы напрямую.

Он должен запросить нужный драйвер по протоколу устройства.

Пример будущей логики:

    DeviceProtocol::Simulator -> SimulatorProtocolDriver
    DeviceProtocol::ModbusTcp -> future ModbusTcpDriver
    DeviceProtocol::Snmp -> future SnmpDriver

### create_default_protocol_driver_registry

Функция создает registry с драйверами по умолчанию.

На текущем этапе туда добавляется:

- `SimulatorProtocolDriver`.

Позже туда можно будет добавить:

- Modbus TCP driver;
- SNMP driver;
- другие built-in drivers.

## SimulatorProtocolDriver

`SimulatorProtocolDriver` — базовый локальный драйвер симулятора.

Он реализует `IProtocolDriver` и не выполняет сетевой обмен.

Назначение:

- проверять общий контракт `IProtocolDriver`;
- проверять будущий polling без реального оборудования;
- проверять batch read;
- проверять dry run write;
- проверять обработку качества и сообщений;
- использоваться как безопасный тестовый источник значений.

## Simulator address rules

На текущем этапе simulator driver поддерживает простые адреса:

- `constant.true`;
- `constant.false`;
- `constant.zero`;
- `constant.one`;
- `status`;
- `counter`;
- `temperature`;
- `random`.

Примеры:

    constant.true -> bool true
    constant.false -> bool false
    constant.zero -> int32 0
    constant.one -> int32 1
    status -> string OK
    counter -> int64 incrementing value
    temperature -> double pseudo temperature
    random -> double pseudo random value

Если адрес неизвестен, драйвер возвращает числовое значение на основе внутреннего счетчика чтений.

## Почему registry нужен до polling scheduler

Polling scheduler не должен знать конкретные классы драйверов.

Он должен работать через `IProtocolDriver`.

Registry дает единый способ получить драйвер:

    protocol -> driver

Это позволит позже добавить новые протоколы без переписывания polling scheduler.

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

Simulator driver уже поддерживает dry run.

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
- `include/scada_protocols/protocol_driver_registry.h`
- `include/scada_protocols/protocol_module.h`
- `include/scada_protocols/drivers/simulator_protocol_driver.h`
- `src/protocol_capabilities.cpp`
- `src/protocol_driver_registry.cpp`
- `src/protocol_requests.cpp`
- `src/protocol_results.cpp`
- `src/protocol_module.cpp`
- `src/simulator_protocol_driver.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_protocols`.
- Добавлен `ProtocolCapabilities`.
- Добавлены request-модели.
- Добавлены result-модели.
- Добавлен `IProtocolDriver`.
- Добавлен `SimulatorProtocolDriver`.
- Добавлены базовые правила simulator-адресов.
- Добавлен `ProtocolDriverRegistry`.
- Добавлен `create_default_protocol_driver_registry()`.
- Добавлен `get_protocol_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем общий контракт протокольных драйверов Dispatcher.
- Добавляем безопасный локальный simulator driver.
- Добавляем registry драйверов.
- Готовим основу для polling без реального оборудования.

### Нужно доделать

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