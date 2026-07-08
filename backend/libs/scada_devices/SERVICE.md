# scada_devices

## Назначение

`scada_devices` — backend-модуль модели устройств Dispatcher.

Модуль отвечает за описание устройств, их связь с объектной моделью, базовые параметры подключения, базовую валидацию устройства, модель диагностики и repository-интерфейсы.

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
- `DeviceValidationCode`;
- `DeviceValidationResult`;
- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`;
- `IDeviceRepository`;
- `IDeviceDiagnosticsRepository`;
- связь устройства с `ObjectId`;
- базовую идентичность устройства;
- базовую модель сетевого подключения;
- базовую проверку корректности устройства;
- базовую модель диагностики устройства;
- repository-интерфейсы устройств;
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
- SQL-запросы;
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

## Валидация устройства

На текущем этапе добавлена базовая валидация устройства.

Проверяются:

- заполнена ли идентичность устройства;
- привязано ли устройство к объекту перед выходом из `Draft`;
- совпадает ли `Device::protocol` и `DeviceConnection::protocol`;
- нужен ли устройству сетевой endpoint;
- заполнен ли `host`;
- указан ли корректный `port`;
- может ли устройство работать в runtime.

## Диагностика устройства

На текущем этапе добавлена базовая модель диагностики устройства.

`DeviceDiagnostics` хранит:

- идентификатор устройства;
- статус связи;
- статус здоровья;
- количество успешных запросов;
- количество ошибочных запросов;
- количество таймаутов;
- количество переподключений;
- последнюю задержку обмена;
- последнюю ошибку.

Диагностика пока не обновляется автоматически.

Позже ее будут обновлять:

- Polling Engine;
- Protocol Drivers;
- Device Manager;
- Diagnostics Service.

## DeviceCommunicationStatus

Статус связи устройства.

Поддерживаются значения:

- `Unknown`;
- `NotConfigured`;
- `Online`;
- `Degraded`;
- `Offline`;
- `Timeout`;
- `ProtocolError`;
- `Disabled`.

## DeviceHealthStatus

Общее состояние здоровья устройства.

Поддерживаются значения:

- `Unknown`;
- `Healthy`;
- `Warning`;
- `Critical`;
- `Offline`;
- `Disabled`.

## Repository-интерфейсы

На текущем этапе добавлены repository-интерфейсы устройств.

### IDeviceRepository

Интерфейс хранения устройств.

Отвечает за:

- получение всех устройств;
- поиск устройства по `DeviceId`;
- поиск устройства по `code`;
- получение устройств по `ObjectId`;
- получение устройств по `DeviceProtocol`;
- получение устройств по `DeviceState`;
- сохранение устройства;
- удаление устройства.

### IDeviceDiagnosticsRepository

Интерфейс хранения диагностики устройств.

Отвечает за:

- получение всей диагностики;
- поиск диагностики по `DeviceId`;
- сохранение диагностики;
- удаление диагностики.

Repository-интерфейсы не содержат SQL и не знают, где именно хранятся данные.

Будущие реализации могут быть разными:

- in-memory repository для тестов;
- PostgreSQL repository для основной системы;
- file-based repository для отладки;
- mock repository для unit-тестов.

## Правила подключения

Сетевой endpoint требуется для:

- `ModbusTcp`;
- `Snmp`;
- `Custom`.

Сетевой endpoint не требуется для:

- `Simulator`.

Для сетевого endpoint обязательны:

- `host`;
- `port > 0`.

`host` может быть:

- IPv4-адресом;
- IPv6-адресом;
- DNS-именем.

Фактическая проверка формата IPv4/IPv6/DNS будет добавлена позже.

## Runtime-состояния

Устройство считается готовым к runtime, если его состояние:

- `Active`;
- `Simulation`.

Остальные состояния пока не считаются runtime-ready:

- `Draft`;
- `Commissioning`;
- `Maintenance`;
- `Disabled`;
- `OutOfService`;
- `Decommissioned`.

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
- `include/scada_devices/device_validation.h`
- `include/scada_devices/device_diagnostics.h`
- `include/scada_devices/device_module.h`
- `include/scada_devices/repository/IDeviceRepository.h`
- `include/scada_devices/repository/IDeviceDiagnosticsRepository.h`
- `src/device_protocol.cpp`
- `src/device_state.cpp`
- `src/device_connection.cpp`
- `src/device.cpp`
- `src/device_validation.cpp`
- `src/device_diagnostics.cpp`
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
- Добавлена базовая валидация устройства.
- Добавлен `DeviceValidationCode`.
- Добавлен `DeviceValidationResult`.
- Добавлена проверка идентичности устройства.
- Добавлена проверка привязки устройства к объекту.
- Добавлена проверка совпадения протоколов.
- Добавлена проверка необходимости сетевого endpoint.
- Добавлена проверка `host`.
- Добавлена проверка `port`.
- Добавлена проверка runtime endpoint.
- Добавлен `DeviceDiagnostics`.
- Добавлен `DeviceCommunicationStatus`.
- Добавлен `DeviceHealthStatus`.
- Добавлены базовые счетчики диагностики устройства.
- Добавлены методы проверки состояния диагностики.
- Добавлен `IDeviceRepository`.
- Добавлен `IDeviceDiagnosticsRepository`.
- Добавлен `get_device_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем базовую модель устройств Dispatcher.
- Закладываем правила конфигурации устройства.
- Закладываем модель диагностики устройства.
- Готовим модель устройств к будущему хранению в БД.

### Нужно доделать

- Добавить правила переходов состояний устройства.
- Добавить черновую миграцию БД.
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
- PostgreSQL-реализация repository.
- In-memory repository для тестов.

### Не входит в этот шаг

- Реальный сетевой обмен.
- Polling.
- Protocol drivers.
- SQL.
- PostgreSQL.
- HTTP API.
- Frontend.