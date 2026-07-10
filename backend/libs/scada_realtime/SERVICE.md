# scada_realtime

## Назначение

`scada_realtime` — backend-модуль realtime gateway foundation Dispatcher.

Модуль отвечает за базовую модель realtime-сообщений, каналов доставки и подписок.

На текущем этапе модуль не запускает WebSocket или SSE сервер.

## Базовая цепочка

Будущая realtime-цепочка:

    Domain event / runtime update / alarm update
        -> RealtimeMessage
        -> Realtime Gateway
        -> WebSocket / SSE clients

Текущий foundation:

    RealtimeMessage
        -> RealtimeChannelType
        -> RealtimeDeliveryMode
        -> RealtimeSubscription

## Границы ответственности

`scada_realtime` отвечает за:

- `RealtimeMessageId`;
- `RealtimeClientId`;
- `RealtimeSubscriptionId`;
- `RealtimeTimestamp`;
- `RealtimeChannelType`;
- `RealtimeMessageType`;
- `RealtimePayloadFormat`;
- `RealtimeDeliveryMode`;
- `RealtimeSubscriptionState`;
- `RealtimeMessage`;
- `RealtimeSubscription`;
- создание realtime message;
- создание realtime subscription;
- подготовку к future WebSocket gateway;
- подготовку к future SSE gateway;
- подготовку к future client subscription registry.

## Что модуль не должен делать сейчас

На текущем шаге `scada_realtime` не должен выполнять:

- открытие TCP-порта;
- запуск WebSocket server;
- запуск SSE server;
- управление реальными socket-клиентами;
- отправку сообщений по сети;
- JSON serialization;
- authorization;
- replay retained messages;
- backpressure;
- retry;
- delivery guarantees;
- frontend serving.

## Основные сущности

### RealtimeMessageId

Типизированный идентификатор realtime message.

Построен поверх общего шаблона:

    dispatcher::common::EntityId<Tag>

### RealtimeClientId

Типизированный идентификатор realtime client.

На текущем этапе клиент является доменной моделью будущего подключения.

Реальных сетевых клиентов пока нет.

### RealtimeSubscriptionId

Типизированный идентификатор realtime subscription.

### RealtimeTimestamp

Временная метка realtime-сообщений и подписок.

На текущем этапе используется:

    std::chrono::system_clock::time_point

### RealtimeChannelType

Канал realtime-сообщения.

Поддерживаются значения:

- `System`;
- `Runtime`;
- `Historian`;
- `Events`;
- `Alarms`;
- `Diagnostics`;
- `Unknown`.

### RealtimeMessageType

Тип realtime-сообщения.

Поддерживаются значения:

- `Heartbeat`;
- `SystemStatus`;
- `RuntimeValueChanged`;
- `HistorySampleWritten`;
- `EventCreated`;
- `AlarmCreated`;
- `AlarmUpdated`;
- `AlarmCleared`;
- `AlarmAcknowledged`;
- `AlarmClosed`;
- `DiagnosticsUpdated`;
- `Error`;
- `Unknown`.

### RealtimePayloadFormat

Формат payload.

Поддерживаются значения:

- `Json`;
- `Text`;
- `Binary`;
- `Empty`.

На текущем этапе payload хранится строкой.

### RealtimeDeliveryMode

Режим доставки.

Поддерживаются значения:

- `Broadcast`;
- `Channel`;
- `Client`;
- `Subscription`.

### RealtimeMessage

Базовая модель realtime-сообщения.

Поля:

- `id`;
- `channel`;
- `type`;
- `payload_format`;
- `delivery_mode`;
- `client_id`;
- `subscription_id`;
- `topic`;
- `correlation_id`;
- `payload`;
- `timestamp`;
- `sequence_number`;
- `retained`.

Методы:

- `has_message_id()`;
- `has_client_id()`;
- `has_subscription_id()`;
- `has_topic()`;
- `has_correlation_id()`;
- `has_payload()`;
- `has_timestamp()`;
- `is_broadcast()`;
- `is_valid()`.

### make_realtime_message

Создает базовое realtime-сообщение.

Принимает:

- channel;
- type;
- topic;
- payload.

Автоматически выставляет timestamp.

### RealtimeSubscriptionState

Состояние подписки.

Поддерживаются значения:

- `Active`;
- `Paused`;
- `Closed`.

### RealtimeSubscription

Базовая модель подписки клиента.

Поля:

- `id`;
- `client_id`;
- `channel`;
- `topic`;
- `filter`;
- `state`;
- `created_at`;
- `last_message_at`;
- `delivered_count`;
- `dropped_count`;
- `include_snapshot`.

Методы:

- `has_subscription_id()`;
- `has_client_id()`;
- `has_topic()`;
- `has_filter()`;
- `has_created_at()`;
- `has_last_message_at()`;
- `is_active()`;
- `is_closed()`;
- `is_valid()`.

### make_realtime_subscription

Создает базовую подписку клиента.

Принимает:

- client id;
- channel;
- topic.

Автоматически выставляет:

- `state = Active`;
- `created_at`.

## Почему realtime вынесен в отдельный модуль

Realtime gateway — отдельная ответственность.

Он не должен быть частью `scada_api`, потому что realtime transport живет иначе, чем request/response API.

HTTP API обычно работает по схеме:

    request -> response

Realtime работает по схеме:

    subscribe -> push messages -> keep connection open

Поэтому realtime вынесен в отдельный модуль.

## Почему WebSocket/SSE пока нет

На текущем этапе нам нужно зафиксировать доменную модель realtime-сообщений и подписок.

Реальный transport требует:

- выбора сетевой библиотеки;
- event loop;
- client registry;
- serialization;
- authorization;
- ping/pong;
- reconnect;
- backpressure;
- shutdown lifecycle.

Это будет добавляться позже.

## Почему payload пока строка

На текущем этапе JSON serialization еще не добавлена.

`payload` хранится строкой как transport-neutral placeholder.

Позже появятся:

- DTO serialization;
- JSON payload builder;
- binary payload support;
- protocol envelope.

## Почему есть retained

Некоторые realtime-сообщения могут быть сохранены как последняя известная snapshot-информация.

Например:

- последний system status;
- последняя active alarm summary;
- последняя runtime value snapshot.

На текущем этапе retained только флаг.

Хранилища retained messages пока нет.

## Почему topic является строкой

Topic должен быть гибким.

Примеры будущих topic:

- `runtime.values`;
- `runtime.values.tag.<tag_id>`;
- `alarms.active`;
- `alarms.object.<object_id>`;
- `events`;
- `system.health`.

Пока topic строковый, потому что routing rules еще не реализованы.

## Почему subscription filter пока строка

Фильтр может быть:

- query-string-like;
- JSON expression;
- DSL;
- набором key-value параметров.

Пока формат фильтра не выбран, поэтому поле хранится строкой.

## Зависимости

`scada_realtime` зависит от:

- `scada_common`;
- `scada_core`.

Причины:

- нужны typed ids;
- нужен `ModuleInfo`.

На текущем шаге `scada_realtime` не зависит от:

- `scada_api`;
- `scada_contracts`;
- `scada_runtime`;
- `scada_historian`;
- `scada_events`;
- `scada_alarms`.

Это сделано намеренно, чтобы realtime foundation оставался transport-neutral и domain-neutral.

## Основные файлы

- `include/scada_realtime/realtime_ids.h`
- `include/scada_realtime/realtime_message.h`
- `include/scada_realtime/realtime_subscription.h`
- `include/scada_realtime/realtime_module.h`
- `src/realtime_message.cpp`
- `src/realtime_subscription.cpp`
- `src/realtime_module.cpp`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_realtime`.
- Добавлен `RealtimeMessageId`.
- Добавлен `RealtimeClientId`.
- Добавлен `RealtimeSubscriptionId`.
- Добавлен `RealtimeTimestamp`.
- Добавлен `RealtimeChannelType`.
- Добавлен `RealtimeMessageType`.
- Добавлен `RealtimePayloadFormat`.
- Добавлен `RealtimeDeliveryMode`.
- Добавлен `RealtimeSubscriptionState`.
- Добавлен `RealtimeMessage`.
- Добавлен `RealtimeSubscription`.
- Добавлена функция `make_realtime_message()`.
- Добавлена функция `make_realtime_subscription()`.
- Добавлены функции `to_string()` для enum.
- Добавлен `get_realtime_module_info()`.
- Модуль подключен к CMake.
- Модуль подключен к `dispatcher_server`.

### Делаем сейчас

- Формируем realtime foundation.
- Готовим message model.
- Готовим subscription model.
- Готовим будущий realtime gateway.

### Нужно доделать

- Добавить application composition.
- Добавить gateway startup foundation.
- Добавить настоящую WebSocket/SSE реализацию позже.
- Добавить serialization позже.
- Добавить integration с runtime/events/alarms позже.

### Расширить позже

- WebSocket transport.
- SSE transport.
- Client registry.
- Subscription registry.
- Message dispatcher.
- Topic router.
- Backpressure.
- Ping/pong.
- Reconnect support.
- Retained messages.
- Authorization.
- WebSocket tests.

### Не входит в этот шаг

- WebSocket server.
- SSE server.
- HTTP server.
- Serialization.
- Frontend.
