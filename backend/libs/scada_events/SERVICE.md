# scada_events

## Назначение

`scada_events` — backend-модуль событий Dispatcher.

Модуль отвечает за базовую модель событий, которые возникают в системе.

На текущем этапе реализованы:

- фундаментальная модель события;
- repository-интерфейс event records;
- DTO events в `scada_contracts`;
- черновая SQL-миграция events/alarms.

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
- `IEventRecordRepository`;
- создание базового события;
- подготовку к future Event Manager;
- подготовку к future EventBus integration;
- подготовку к future event storage;
- подготовку к future audit trail;
- подготовку к future alarm events.

## Что модуль не должен делать сейчас

На текущем шаге `scada_events` не должен выполнять:

- хранение событий в БД;
- реализацию repository;
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

## Repository-интерфейс

### IEventRecordRepository

Интерфейс будущего хранения событий.

Методы:

- `find_by_id()`;
- `find_recent()`;
- `find_by_category()`;
- `find_by_source_id()`;
- `find_by_correlation_id()`;
- `save()`;
- `remove_by_id()`;
- `remove_older_than()`;
- `count_all()`.

На текущем этапе реализации repository нет.

Будущие реализации:

- PostgreSQL;
- in-memory для тестов;
- mock для unit-тестов.

## База данных

Добавлена черновая миграция:

- `database/migrations/0007_events_alarms.sql`.

Миграция описывает таблицу:

- `event_records`.

## Почему события отделены от аварий

Событие — это широкий факт, который произошел в системе.

Авария — это особый тип события с жизненным циклом.

Событие может быть одноразовым.

Авария может жить во времени.

Поэтому аварии реализованы отдельным модулем:

    scada_alarms

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

## Почему пока нет EventBus

Сейчас мы создаем только доменную модель события и repository-интерфейс.

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

На текущем этапе создан только foundation.

## Зависимости

`scada_events` зависит от:

- `scada_common`;
- `scada_core`.

Причины:

- нужны typed ids;
- нужен `ModuleInfo`.

## Основные файлы

- `include/scada_events/event_ids.h`;
- `include/scada_events/event_record.h`;
- `include/scada_events/event_module.h`;
- `include/scada_events/repository/IEventRecordRepository.h`;
- `src/event_record.cpp`;
- `src/event_module.cpp`.

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
- Добавлен `IEventRecordRepository`.
- Добавлены DTO events.
- Добавлена миграция events/alarms.
- Добавлен `get_event_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Нужно доделать

- Добавить Event Manager.
- Добавить EventBus integration.
- Добавить PostgreSQL repository implementation.
- Добавить Event API.
- Добавить WebSocket delivery.
- Добавить UI event journal.

### Не входит в этот шаг

- EventBus.
- Event storage implementation.
- Event API.
- WebSocket.
- Frontend.