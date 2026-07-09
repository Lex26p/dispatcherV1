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
- `TagCurrentValue`;
- `TagValueSource`;
- `TagValuePayload`;
- `TagValidationCode`;
- `TagValidationResult`;
- связь тега с `ObjectId`;
- связь тега с `DeviceId`;
- базовую идентичность тега;
- базовую модель адресации тега;
- базовую модель инженерного преобразования;
- базовую модель текущего значения тега;
- базовую валидацию тега;
- базовые правила адресации;
- подготовку к будущему polling, runtime values, historian и alarms.

## Что модуль не должен делать

`scada_tags` не должен содержать:

- protocol drivers;
- Modbus TCP реализацию;
- SNMP реализацию;
- polling engine;
- runtime-хранилище текущих значений;
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

### TagValueSource

Источник текущего значения тега.

Поддерживаются значения:

- `Unknown`;
- `Device`;
- `Manual`;
- `Calculation`;
- `System`;
- `Simulation`;
- `External`.

### TagValuePayload

Универсальный контейнер значения тега.

Поддерживает:

- пустое значение;
- `bool`;
- `int32`;
- `int64`;
- `float`;
- `double`;
- `string`.

`Enum` и `Json` на текущем этапе хранятся как строковое значение.

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

### TagCurrentValue

Модель текущего значения тега.

Поля:

- `tag_id`;
- `value_type`;
- `quality`;
- `source`;
- `raw_value`;
- `engineering_value`;
- `timestamp`;
- `source_timestamp`;
- `server_timestamp`;
- `last_good_value`;
- `last_good_timestamp`;
- `change_counter`.

`raw_value` — исходное значение, полученное из устройства, ручного ввода или внешнего источника.

`engineering_value` — значение после инженерного преобразования.

`last_good_value` — последнее значение с хорошим качеством.

`change_counter` — счетчик изменений текущего значения.

## Валидация тегов

На текущем этапе добавлена базовая валидация тегов.

Проверяются:

- заполнена ли идентичность тега;
- нужен ли тегу объект;
- привязан ли тег к объекту;
- нужен ли тегу устройство;
- привязан ли тег к устройству;
- нужен ли тегу адрес;
- задан ли адрес или протокольный payload;
- корректны ли `scale` и `offset`;
- корректен ли `bit_index`;
- корректен ли `bit_length`;
- корректен ли `shift`;
- поддерживает ли тип значения bit extraction.

## Правила привязки к объекту

Объект требуется для типов:

- `DeviceTag`;
- `ManualTag`;
- `CalculatedTag`;
- `VirtualTag`;
- `CommandTag`;
- `SimulationTag`;
- `ExternalTag`.

Объект не требуется для:

- `SystemTag`.

Это позволяет системным тегам описывать состояние самого Dispatcher, а не конкретного объекта.

## Правила привязки к устройству

Устройство требуется для типов:

- `DeviceTag`;
- `CommandTag`.

Устройство не требуется для типов:

- `ManualTag`;
- `CalculatedTag`;
- `SystemTag`;
- `VirtualTag`;
- `SimulationTag`;
- `ExternalTag`.

## Правила адресации

Адрес требуется для типов:

- `DeviceTag`;
- `CommandTag`;
- `ExternalTag`.

Адрес не требуется для типов:

- `ManualTag`;
- `CalculatedTag`;
- `SystemTag`;
- `VirtualTag`;
- `SimulationTag`.

Адрес может быть задан двумя способами:

- через `address`;
- через `protocol_address_json`.

## Правила extraction

Bit/mask extraction предназначен для числовых, boolean и enum-значений.

Extraction поддерживается для типов значения:

- `Boolean`;
- `Int32`;
- `Int64`;
- `Float`;
- `Double`;
- `Enum`.

Extraction не поддерживается для:

- `String`;
- `Json`.

Ограничения:

- `bit_index` должен быть в диапазоне 0..63;
- `bit_length` должен быть в диапазоне 1..64;
- `shift` должен быть в диапазоне 0..63.

## Инженерное преобразование

На текущем этапе в теге заложены поля:

- `scale`;
- `offset`;
- `engineering_unit`.

Формула будущего преобразования:

    engineering_value = raw_value * scale + offset

Валидация проверяет:

- `scale` должен быть конечным числом;
- `offset` должен быть конечным числом;
- `scale` не должен быть равен нулю.

## Текущие значения тегов

На текущем этапе добавлена только модель текущего значения.

Runtime-хранилище текущих значений пока не реализовано.

Позже появится отдельный слой, который будет хранить актуальные значения тегов в памяти:

    TagValueStore

Этот слой будет получать значения из:

- Polling Engine;
- ручного ввода;
- расчетных тегов;
- системных тегов;
- симулятора;
- внешних интеграций.

## Качество и last good value

`TagCurrentValue` хранит текущее качество значения.

Хорошими качествами на текущем этапе считаются:

- `Good`;
- `Manual`;
- `Simulation`.

Если значение хорошего качества, будущий runtime-слой сможет обновлять:

- `last_good_value`;
- `last_good_timestamp`.

На текущем этапе автоматического обновления `last_good_value` нет.

## Временные метки

`TagCurrentValue` содержит три временные метки:

- `timestamp`;
- `source_timestamp`;
- `server_timestamp`.

`timestamp` — основная временная метка значения.

`source_timestamp` — время на стороне источника, если известно.

`server_timestamp` — время получения или фиксации значения на сервере Dispatcher.

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
- `include/scada_tags/tag_current_value.h`
- `include/scada_tags/tag_validation.h`
- `include/scada_tags/tag.h`
- `include/scada_tags/tag_module.h`
- `src/tag_type.cpp`
- `src/tag_value_type.cpp`
- `src/tag_quality.cpp`
- `src/tag_archive_policy.cpp`
- `src/tag_address.cpp`
- `src/tag_current_value.cpp`
- `src/tag_validation.cpp`
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
- Добавлен `TagValueSource`.
- Добавлен `TagValuePayload`.
- Добавлен `TagCurrentValue`.
- Добавлена базовая валидация тегов.
- Добавлен `TagValidationCode`.
- Добавлен `TagValidationResult`.
- Добавлены правила привязки к объекту.
- Добавлены правила привязки к устройству.
- Добавлены правила адресации.
- Добавлены правила extraction.
- Добавлена проверка инженерного преобразования.
- Добавлены базовые методы проверки текущего значения.
- Добавлен `get_tag_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем базовую модель тегов Dispatcher.
- Закладываем модель текущего значения тега.
- Закладываем правила валидации и адресации тегов.

### Нужно доделать

- Добавить DTO-контракты тегов.
- Добавить repository-интерфейсы тегов.
- Добавить черновую миграцию БД.
- Добавить runtime-хранилище текущих значений позже.
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
- TagValueStore.
- EventBus integration.
- Historian integration.
- Alarm integration.

### Не входит в этот шаг

- Runtime-хранилище текущих значений.
- Реальный polling.
- Protocol drivers.
- Historian.
- Alarm engine.
- PostgreSQL.
- HTTP API.
- Frontend.