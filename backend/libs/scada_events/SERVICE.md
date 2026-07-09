# scada_events

## Назначение

`scada_events` — backend-модуль событий Dispatcher.

Модуль отвечает за базовую модель событий, которые возникают в системе.

На текущем шаге реализована только фундаментальная модель события.

Аварии будут реализованы отдельным модулем `scada_alarms`.

## Базовая цепочка

Общая цепочка Dispatcher:

    объект -> устройство -> тег -> значение -> качество -> история -> авария -> виджет

Events layer будет принимать события из разных подсистем:

    Runtime -> Events
    Historian -> Events
    Polling -> Events
    Devices -> Events
    Alarms -> Events
    Commands -> Events
    Users -> Events

## Границы ответственности

`scada_events` отвечает за:

- `EventId`;
- `EventTimestamp`;
- `EventCategory`;
- `EventSeverity`;
- `EventSourceType`;
- `EventRecord`;
- создание базового события;
- подготовку к future Event Manager;
- подготовку к future EventBus integration;
- подготовку к future event storage;
- подготовку к future audit trail;
- подготовку к future alarm events.

## Что модуль не должен делать сейчас

На текущем шаге `scada_events` не должен выполнять:

- хранение событий в БД;
- публикацию в EventBus;
- доставку событий в UI;
- обработку аварий;
- квитирование аварий;
- audit logic;
- HTTP API;
- WebSocket API;
- UI-логику.

## Основные сущности

### EventId

Типизированный идентификатор события.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

На текущем этапе event id может быть пустым до момента сохранения.

Будущая repository-реализация сможет назначать id при записи.

### EventTimestamp

Временная метка события.

На текущем этапе используется:

    std::chrono::system_clock::time_point

Позже mapper/API слой будет преобразовывать timestamp в строковый формат.

### EventCategory

Категория события.

Поддерживаются значения:

- `System`;
- `Runtime`;
- `Device`;
- `Tag`;
- `Communication`;
- `Historian`;
- `Security`;
- `UserAction`;
- `Integration`;
- `Maintenance`;
- `Alarm`.

### EventSeverity

Серьезность события.

Поддерживаются значения:

- `Trace`;
- `Information`;
- `Warning`;
- `Error`;
- `Critical`.

`EventSeverity` описывает важность события.

Для аварий позже будет добавлена отдельная модель `AlarmSeverity`.

### EventSourceType

Тип источника события.

Поддерживаются значения:

- `Unknown`;
- `System`;
- `Object`;
- `Device`;
- `Tag`;
- `Runtime`;
- `Historian`;
- `Polling`;
- `Protocol`;
- `Alarm`;
- `User`;
- `Integration`;
- `Script`.

### EventRecord

Базовая модель события.

Поля:

- `id`;
- `category`;
- `severity`;
- `source_type`;
- `code`;
- `title`;
- `message`;
- `source_id`;
- `object_id`;
- `device_id`;
- `tag_id`;
- `correlation_id`;
- `timestamp`;
- `sequence_number`.

Методы:

- `has_event_id()`;
- `has_code()`;
- `has_title()`;
- `has_message()`;
- `has_source_id()`;
- `has_object_id()`;
- `has_device_id()`;
- `has_tag_id()`;
- `has_correlation_id()`;
- `has_valid_identity()`.

### make_event_record

Функция создает базовое событие.

Принимает:

- category;
- severity;
- source type;
- code;
- title;
- message.

Автоматически выставляет текущий timestamp.

## Почему события отделены от аварий

Событие — это широкий факт, который произошел в системе.

Авария — это особый тип события с жизненным циклом.

Примеры событий:

- устройство перешло в Maintenance;
- polling group выполнена;
- пользователь вошел в систему;
- runtime value изменился;
- historian batch записан;
- alarm был активирован.

Примеры аварий:

- связь с устройством потеряна;
- температура выше порога;
- насос в аварийном состоянии;
- датчик не отвечает.

Аварии требуют lifecycle:

- активна;
- подтверждена;
- нормализована;
- закрыта.

Поэтому аварии будут отдельным модулем.

## Почему source ids пока строки

EventRecord должен уметь ссылаться на разные сущности:

- объект;
- устройство;
- тег;
- пользователя;
- интеграцию;
- скрипт;
- alarm;
- внешний источник.

Чтобы не связывать `scada_events` со всеми доменными модулями сразу, source ids пока представлены строками.

Позже mapper/repository/API слой сможет валидировать и связывать эти идентификаторы с конкретными доменными сущностями.

## Почему EventSeverity не равен AlarmSeverity

EventSeverity описывает важность события.

AlarmSeverity будет описывать серьезность аварии.

Они похожи, но не полностью одинаковы.

Например:

- событие аудита может быть `Information`;
- авария связи может быть `Critical`;
- событие подтверждения аварии может быть `Information`, но сама авария остается `Critical`.

## Почему пока нет EventBus

Сейчас мы создаем только доменную модель события.

Публикация событий будет добавлена позже.

Будущая цепочка:

    Module -> EventRecord -> EventBus -> subscribers

Потенциальные subscribers:

- Historian;
- Alarm Engine;
- WebSocket Gateway;
- Audit;
- Diagnostics;
- Notification Service.

## Почему пока нет Event Manager

Event Manager будет отвечать за:

- хранение событий;
- поиск событий;
- фильтрацию;
- доставку;
- lifecycle events;
- связь с alarms;
- аудит;
- диагностику.

На текущем шаге создается только foundation.

## Зависимости

`scada_events` зависит от:

- `scada_common`;
- `scada_core`.

Причины:

- нужны typed ids;
- нужен `ModuleInfo`.

На текущем шаге `scada_events` не зависит от:

- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_runtime`;
- `scada_historian`;
- `scada_alarms`.

Это сделано намеренно, чтобы events layer оставался универсальным.

## Основные файлы

- `include/scada_events/event_ids.h`
- `include/scada_events/event_record.h`
- `include/scada_events/event_module.h`
- `src/event_record.cpp`
- `src/event_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_events`.
- Добавлен `EventId`.
- Добавлен `EventTimestamp`.
- Добавлен `EventCategory`.
- Добавлен `EventSeverity`.
- Добавлен `EventSourceType`.
- Добавлен `EventRecord`.
- Добавлена функция `make_event_record()`.
- Добавлены функции `to_string()` для enum.
- Добавлен `get_event_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем foundation Events.
- Готовим базовую модель события.
- Готовим основу для Event Manager.
- Готовим основу для Alarm events.

### Нужно доделать

- Добавить `scada_alarms`.
- Добавить alarm model.
- Добавить alarm lifecycle.
- Добавить alarm transitions.
- Добавить alarm rules foundation.
- Добавить event/alarm DTO.
- Добавить repository-интерфейсы.
- Добавить миграции.

### Расширить позже

- EventBus integration.
- Event Manager.
- Event storage.
- Event filtering.
- Audit events.
- Notification integration.
- WebSocket delivery.
- API.
- UI event journal.

### Не входит в этот шаг

- Alarms.
- EventBus.
- Event storage.
- Event API.
- WebSocket.
- Frontend.