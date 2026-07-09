# scada_tags

## Назначение

`scada_tags` — backend-модуль модели тегов Dispatcher.

Тег в Dispatcher — это именованная точка данных, которая связывает объект, устройство, адрес в протоколе, тип значения, качество, архивирование и будущую логику отображения.

Теги являются центральной сущностью SCADA-системы.

Базовая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

## Границы ответственности

`scada_tags` отвечает за:

- `TagId`;
- `Tag`;
- `TagType`;
- `TagValueType`;
- `TagQuality`;
- `TagArchivePolicy`;
- `TagAddress`;
- связь тега с `ObjectId`;
- связь тега с `DeviceId`;
- базовую идентичность тега;
- базовую модель адресации тега;
- базовую модель инженерного преобразования;
- подготовку к будущему polling, runtime values, historian и alarms.

## Что модуль не должен делать

`scada_tags` не должен содержать:

- protocol drivers;
- Modbus TCP реализацию;
- SNMP реализацию;
- polling engine;
- текущие значения тегов;
- запись истории;
- аварийную логику;
- команды управления;
- HTTP API;
- PostgreSQL-реализацию;
- UI-логику.

## Основные сущности

### TagId

Типизированный идентификатор тега.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

### TagType

Тип тега.

Поддерживаются значения:

- `DeviceTag`;
- `ManualTag`;
- `CalculatedTag`;
- `SystemTag`;
- `VirtualTag`;
- `CommandTag`;
- `SimulationTag`;
- `ExternalTag`.

### TagValueType

Тип значения тега.

Поддерживаются значения:

- `Boolean`;
- `Int32`;
- `Int64`;
- `Float`;
- `Double`;
- `String`;
- `Enum`;
- `Json`.

### TagQuality

Качество значения тега.

Поддерживаются значения:

- `Good`;
- `Uncertain`;
- `Bad`;
- `CommunicationError`;
- `Timeout`;
- `DeviceError`;
- `ProtocolError`;
- `OutOfRange`;
- `Manual`;
- `Simulation`;
- `Maintenance`;
- `Stale`;
- `Disabled`;
- `NotInitialized`.

### TagArchivePolicy

Политика архивирования тега.

Поддерживаются значения:

- `Disabled`;
- `OnChange`;
- `Periodic`;
- `OnChangeWithDeadband`;
- `OnQualityChange`;
- `OnAlarm`;
- `AlwaysButThrottled`.

### TagAddress

Базовая модель адреса тега.

Поля:

- `protocol`;
- `address`;
- `protocol_address_json`;
- `bit_index`;
- `byte_offset`;
- `mask`;
- `shift`;
- `bit_length`.

`address` — человекочитаемый адрес.

`protocol_address_json` — расширенное протокольное описание адреса.

Это нужно, чтобы поддержать разные протоколы без переписывания общей модели тега.

Примеры будущих адресов:

    ModbusTcp: holding_register:40001
    SNMP: 1.3.6.1.2.1.1.3.0
    Simulator: random.temperature.room01

### Tag

Основная доменная сущность тега.

Поля:

- `id`;
- `object_id`;
- `device_id`;
- `type`;
- `value_type`;
- `archive_policy`;
- `code`;
- `name`;
- `description`;
- `engineering_unit`;
- `scale`;
- `offset`;
- `address`;
- `is_enabled`.

## Связь с объектной моделью

Тег может быть связан с объектом через `ObjectId`.

Пример:

    ObjectNode: Насосная станция 1
        Tag: Pump01.State

Связь с объектной моделью нужна для:

- отображения тегов в дереве объекта;
- дашбордов;
- мнемосхем;
- аварий;
- прав доступа;
- зон ответственности;
- ППР.

## Связь с устройством

Тег может быть связан с устройством через `DeviceId`.

Пример:

    Device: PLC-01
        Tag: Pump01.State
        Tag: Pump01.Current
        Tag: Pump01.Alarm

Связь с устройством нужна для:

- будущего polling;
- protocol drivers;
- диагностики;
- группировки тегов устройства;
- команд управления.

## Инженерное преобразование

На текущем этапе в теге заложены поля:

- `scale`;
- `offset`;
- `engineering_unit`.

Формула будущего преобразования:

    engineering_value = raw_value * scale + offset

Фактическая обработка значений будет добавлена позже в модуле runtime values.

## Адресация и extraction

`TagAddress` сразу закладывает поля для будущего извлечения значения:

- bit;
- byte offset;
- mask;
- shift;
- bit length.

Это нужно для:

- Modbus-регистров;
- битовых флагов;
- packed values;
- enum;
- boolean extraction.

## Основные файлы

- `include/scada_tags/tag_ids.h`
- `include/scada_tags/tag_type.h`
- `include/scada_tags/tag_value_type.h`
- `include/scada_tags/tag_quality.h`
- `include/scada_tags/tag_archive_policy.h`
- `include/scada_tags/tag_address.h`
- `include/scada_tags/tag.h`
- `include/scada_tags/tag_module.h`
- `src/tag_type.cpp`
- `src/tag_value_type.cpp`
- `src/tag_quality.cpp`
- `src/tag_archive_policy.cpp`
- `src/tag_address.cpp`
- `src/tag.cpp`
- `src/tag_module.cpp`

## Зависимости

- `scada_common`
- `scada_core`
- `scada_objects`
- `scada_devices`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_tags`.
- Добавлен `TagId`.
- Добавлен `TagType`.
- Добавлен `TagValueType`.
- Добавлен `TagQuality`.
- Добавлен `TagArchivePolicy`.
- Добавлен `TagAddress`.
- Добавлен `Tag`.
- Добавлен `get_tag_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем базовую модель тегов Dispatcher.

### Нужно доделать

- Добавить модель текущего значения тега.
- Добавить валидацию тегов.
- Добавить DTO-контракты тегов.
- Добавить repository-интерфейсы тегов.
- Добавить черновую миграцию БД.
- Добавить API тегов позже.
- Добавить UI карточки тега позже.

### Расширить позже

- Modbus address parser.
- SNMP OID parser.
- Simulator address parser.
- Tag groups.
- Polling groups.
- Alarm rules.
- History policies.
- Deadband settings.
- Engineering conversion service.
- Tag calculation engine.
- Command tags.
- Manual value workflow.

### Не входит в этот шаг

- Текущие значения тегов.
- Реальный polling.
- Protocol drivers.
- Historian.
- Alarm engine.
- PostgreSQL.
- HTTP API.
- Frontend.