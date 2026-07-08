# scada_devices

## Назначение

`scada_devices` — backend-модуль модели устройств Dispatcher.

Модуль отвечает за описание устройств, их связь с объектной моделью и базовые параметры подключения.

Устройство в Dispatcher — это техническая сущность, через которую система получает данные или отправляет команды.

Примеры устройств:

- PLC;
- контроллер;
- счетчик;
- даталоггер;
- SNMP-устройство;
- симулятор;
- внешний источник данных.

## Границы ответственности

`scada_devices` отвечает за:

- `DeviceId`;
- `Device`;
- `DeviceProtocol`;
- `DeviceState`;
- `DeviceConnection`;
- `IpMode`;
- связь устройства с `ObjectId`;
- базовую идентичность устройства;
- базовую модель сетевого подключения;
- подготовку к будущему polling и protocol drivers.

## Что модуль не должен делать

`scada_devices` не должен содержать:

- реализацию Modbus TCP;
- реализацию SNMP;
- polling engine;
- текущие значения тегов;
- историю тегов;
- аварийную логику;
- команды управления;
- HTTP API;
- PostgreSQL-реализацию;
- UI-логику.

## Основные сущности

### DeviceId

Типизированный идентификатор устройства.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

### DeviceProtocol

Базовый протокол устройства.

На текущем этапе поддерживаются значения:

- `ModbusTcp`;
- `Snmp`;
- `Simulator`;
- `Custom`.

### DeviceState

Жизненное состояние устройства.

Поддерживаются значения:

- `Draft`;
- `Commissioning`;
- `Active`;
- `Maintenance`;
- `Disabled`;
- `OutOfService`;
- `Decommissioned`;
- `Simulation`.

### IpMode

Режим сетевого адреса.

Поддерживаются значения:

- `IPv4`;
- `IPv6`;
- `Auto`.

### DeviceConnection

Базовая модель подключения.

Поля:

- `protocol`;
- `host`;
- `port`;
- `ip_mode`;
- `network_interface`.

### Device

Основная доменная сущность устройства.

Поля:

- `id`;
- `object_id`;
- `code`;
- `name`;
- `description`;
- `protocol`;
- `state`;
- `connection`.

## Связь с объектной моделью

Устройство может быть привязано к объекту через `ObjectId`.

Пример:

    ObjectNode: Насосная станция 1
        Device: PLC-01

Связь с объектной моделью нужна для:

- отображения устройства в дереве объекта;
- будущих тегов;
- аварий;
- дашбордов;
- прав доступа;
- зон ответственности;
- ППР.

## IPv4 и IPv6

Сразу закладывается поддержка:

- IPv4;
- IPv6;
- автоматического режима;
- DNS-имени;
- сетевого интерфейса сервера.

Фактическая проверка адресов и сетевые подключения будут реализованы позже.

## Основные файлы

- `include/scada_devices/device_ids.h`
- `include/scada_devices/device_protocol.h`
- `include/scada_devices/device_state.h`
- `include/scada_devices/device_connection.h`
- `include/scada_devices/device.h`
- `include/scada_devices/device_module.h`
- `src/device_protocol.cpp`
- `src/device_state.cpp`
- `src/device_connection.cpp`
- `src/device.cpp`
- `src/device_module.cpp`

## Зависимости

- `scada_common`
- `scada_core`
- `scada_objects`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_devices`.
- Добавлен `DeviceId`.
- Добавлен `DeviceProtocol`.
- Добавлен `DeviceState`.
- Добавлен `IpMode`.
- Добавлен `DeviceConnection`.
- Добавлен `Device`.
- Добавлен `get_device_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем базовую модель устройств Dispatcher.

### Нужно доделать

- Добавить валидацию устройства.
- Добавить правила состояний устройства.
- Добавить DTO-контракты устройства.
- Добавить repository-интерфейсы устройств.
- Добавить черновую миграцию БД.
- Добавить модель диагностики устройства.
- Добавить API устройств позже.
- Добавить UI карточки устройства позже.

### Расширить позже

- Modbus TCP connection settings.
- SNMP connection settings.
- OPC UA connection settings.
- MQTT connection settings.
- Simulator device settings.
- Проверка связи.
- Тестовый опрос.
- Диагностика качества связи.
- Статистика обмена.
- Привязка тегов.
- Привязка команд.

### Не входит в этот шаг

- Реальный сетевой обмен.
- Polling.
- Protocol drivers.
- PostgreSQL.
- HTTP API.
- Frontend.