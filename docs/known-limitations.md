# Dispatcher Known Limitations

## Назначение документа

Этот документ фиксирует известные ограничения текущей версии Dispatcher.

Ограничения фиксируются явно, чтобы не путать:

- уже реализованные возможности;
- временные заготовки;
- архитектурные решения;
- будущие задачи.

---

# Общие ограничения

## Проект находится на ранней стадии

Dispatcher пока находится на этапе формирования backend-фундамента и подготовки MVP-контуров.

На текущем этапе система еще не является готовой SCADA-платформой.

## Нет production-ready функциональности

Текущая версия не предназначена для промышленной эксплуатации.

Пока реализуются базовые архитектурные модули, доменные модели, API foundation и realtime foundation.

## Нет полноценного запуска системы

`dispatcher_server` запускается и выводит информацию о модулях.

Но он пока не выполняет:

- полноценный фоновый опрос оборудования;
- подключение к PostgreSQL;
- реальный HTTP API;
- реальный WebSocket/SSE gateway;
- авторизацию;
- архивирование в БД;
- обработку аварий через Alarm Manager;
- управление командами;
- frontend serving.

---

# Ограничения backend

## Нет реального HTTP API

В `scada_api` уже есть:

- API route model;
- API response model;
- API mapper foundation;
- API read endpoint model;
- route registry;
- read endpoint catalog.

Но пока нет:

- HTTP listener;
- REST controllers;
- request parser;
- response serializer;
- middleware;
- OpenAPI;
- authorization;
- real endpoint execution.

## Нет реального realtime transport

В `scada_realtime` уже есть:

- realtime message model;
- realtime subscription model;
- channel types;
- delivery modes;
- subscription states.

Но пока нет:

- WebSocket server;
- SSE server;
- socket clients;
- subscription registry;
- topic router;
- message dispatcher;
- ping/pong;
- reconnect;
- backpressure;
- delivery guarantees.

## Нет подключения к PostgreSQL

SQL-миграции уже существуют как черновики.

Но приложение пока не умеет:

- подключаться к PostgreSQL;
- выполнять миграции;
- читать данные из БД;
- сохранять данные в БД.

## Нет реализации репозиториев

Добавлены только интерфейсы.

Для объектной модели:

- `IObjectRepository`;
- `IResponsibilityZoneRepository`.

Для модели устройств:

- `IDeviceRepository`;
- `IDeviceDiagnosticsRepository`.

Для модели тегов:

- `ITagRepository`;
- `ITagCurrentValueRepository`.

Для polling:

- `IPollingTaskRepository`;
- `IPollingGroupRepository`.

Для runtime:

- `IRuntimeValueSnapshotRepository`;
- `IRuntimeEventRepository`.

Для historian:

- `IHistorySampleRepository`.

Для events:

- `IEventRecordRepository`.

Для alarms:

- `IAlarmRepository`;
- `IAlarmTransitionRepository`;
- `IAlarmRuleRepository`.

Но пока нет реализаций:

- in-memory repository;
- PostgreSQL repository;
- TimescaleDB repository;
- mock repository;
- file-based repository.

## Нет полноценного composition root

Добавлен модуль `scada_app`, который умеет создавать:

- `ApplicationComposition`;
- `GatewayStartupOptions`;
- `GatewayStartupPlan`.

Но пока нет настоящего composition root, который создает:

- repositories;
- services;
- background workers;
- HTTP transport;
- realtime transport;
- database connections;
- module lifecycle;
- shutdown lifecycle.

## Нет dependency injection контейнера

Модули пока подключаются статически через CMake и вручную регистрируются в `dispatcher_server`.

Полноценный DI/service container будет добавлен позже.

---

# Ограничения объектной модели

## Object Model является базовой

Объектная модель уже содержит:

- `ObjectId`;
- `ResponsibilityZoneId`;
- `ObjectType`;
- `ObjectNode`;
- `ObjectTree`;
- `ResponsibilityZone`;
- правила иерархии;
- базовую валидацию;
- DTO;
- repository-интерфейсы;
- SQL-черновик.

Но пока нет:

- хранения в БД через приложение;
- API handlers;
- UI;
- unit-тестов;
- версионирования;
- импорта;
- экспорта;
- аудита изменений;
- управления через конфигуратор.

## ResponsibilityZone пока не является системой прав доступа

`ResponsibilityZone` только хранит связи:

- объекты;
- пользователи;
- роли.

Она пока не выполняет:

- проверку прав;
- наследование прав;
- разграничение команд;
- разграничение просмотра;
- связь с рабочими местами;
- связь со сменами.

---

# Ограничения модели устройств

## Device Model является базовой

Модель устройств уже содержит:

- `DeviceId`;
- `Device`;
- `DeviceProtocol`;
- `DeviceState`;
- `DeviceConnection`;
- `IpMode`;
- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`;
- базовую валидацию устройства;
- DTO устройств;
- repository-интерфейсы;
- SQL-черновик.

Но пока нет:

- реального сетевого обмена с устройствами;
- Modbus TCP driver;
- SNMP driver;
- проверки связи через сеть;
- test poll через реальное устройство;
- API handlers;
- UI;
- PostgreSQL-реализации;
- unit-тестов.

---

# Ограничения модели тегов

## Tag Model является базовой

Модель тегов уже содержит:

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
- базовую валидацию тегов;
- DTO тегов;
- repository-интерфейсы;
- SQL-черновик.

Но пока нет:

- полноценного alarm rule management через API;
- command execution;
- API handlers;
- UI;
- PostgreSQL-реализации;
- unit-тестов.

---

# Ограничения протокольного слоя

## Protocol layer пока является foundation

Модуль `scada_protocols` уже содержит:

- `IProtocolDriver`;
- `ProtocolCapabilities`;
- request/result модели;
- `SimulatorProtocolDriver`;
- `ProtocolDriverRegistry`.

Но пока нет:

- реального Modbus TCP driver;
- реального SNMP driver;
- Boost.Asio;
- connection pooling;
- retry policy;
- reconnect policy;
- timeout policy;
- rate limiting;
- protocol diagnostics;
- protocol metrics.

## Simulator driver не является реальным устройством

`SimulatorProtocolDriver` используется только для проверки архитектуры.

Он не открывает сетевые соединения и не читает реальные устройства.

---

# Ограничения polling

## Polling является foundation

Модуль `scada_polling` уже содержит:

- `PollingTaskId`;
- `PollingGroupId`;
- `PollingTaskState`;
- `PollingGroupMode`;
- `PollingTask`;
- `PollingGroup`;
- `PollingExecutionStatus`;
- `PollingTaskExecutionResult`;
- `PollingGroupExecutionResult`;
- `PollingSchedulerOptions`;
- `PollingScheduler`;
- repository-интерфейсы polling.

Но пока нет:

- фонового polling loop;
- таймеров;
- потоков;
- async polling;
- Boost.Asio scheduling;
- automatic due calculation;
- retry policy;
- backoff policy;
- device diagnostics update.

## Polling пока не интегрирован с runtime

Runtime-слой уже умеет применять `ProtocolReadResult`.

Но автоматическая цепочка еще не собрана:

    PollingScheduler -> RuntimeValueApplier -> TagValueStore

---

# Ограничения runtime

## Runtime является foundation

Модуль `scada_runtime` уже содержит:

- `TagValueStore`;
- `RuntimeValueApplyResult`;
- `RuntimeValueApplier`;
- runtime quality helpers;
- runtime value conversion;
- engineering transform;
- runtime change detection;
- runtime event foundation;
- repository-интерфейсы snapshots/events.

Но пока нет:

- потокобезопасности;
- восстановления snapshots при старте;
- PostgreSQL repository implementation;
- EventBus publish;
- Historian integration;
- Alarm integration;
- realtime integration;
- HTTP API handlers.

## TagValueStore является in-memory хранилищем

`TagValueStore` хранит текущие значения только в памяти процесса.

После остановки приложения runtime-state теряется.

## Runtime events пока не публикуются

`RuntimeValueEvent` формируется и возвращается в `RuntimeValueApplyResult`.

Но пока нет:

- публикации в EventBus;
- подписчиков;
- доставки в realtime gateway;
- записи в Historian;
- alarm evaluation.

---

# Ограничения historian

## Historian является foundation

Модуль `scada_historian` уже содержит:

- `HistorySampleId`;
- `HistorySample`;
- `ArchiveDecision`;
- `HistorianBuffer`;
- `HistoryBatch`;
- `HistoryBatchWriteResult`;
- `IHistoryBatchWriter`;
- `HistoryQuery`;
- `HistoryQueryResult`;
- `IHistorySampleRepository`;
- DTO historian;
- SQL-черновик `0006_tag_history.sql`.

Но пока нет:

- PostgreSQL writer;
- TimescaleDB hypertable;
- реальной записи истории;
- реального чтения истории;
- repository implementation;
- Runtime -> Historian integration;
- History API handlers;
- chart data API.

## HistorianBuffer является in-memory buffer

`HistorianBuffer` хранит samples только в памяти процесса.

После остановки приложения buffer теряется.

Пока нет:

- disk spool;
- retry queue;
- durable queue;
- backpressure;
- flush scheduler;
- write worker.

---

# Ограничения events

## Events является foundation

Модуль `scada_events` уже содержит:

- `EventId`;
- `EventTimestamp`;
- `EventCategory`;
- `EventSeverity`;
- `EventSourceType`;
- `EventRecord`;
- `make_event_record()`;
- `IEventRecordRepository`;
- DTO events;
- SQL-черновик `0007_events_alarms.sql`.

Но пока нет:

- Event Manager;
- EventBus publish;
- PostgreSQL repository implementation;
- автоматической записи событий;
- подписчиков событий;
- event filtering service;
- event API handlers;
- realtime delivery;
- UI event journal.

## EventRecord пока не создается автоматически

Модель события есть, но runtime, historian, polling, alarms и другие модули пока не создают события автоматически.

Будущая цепочка:

    Module -> EventRecord -> Event Manager -> Repository / EventBus / Realtime Gateway

---

# Ограничения alarms

## Alarms является foundation

Модуль `scada_alarms` уже содержит:

- `AlarmId`;
- `AlarmRuleId`;
- `AlarmSourceType`;
- `AlarmSeverity`;
- `AlarmPriority`;
- `AlarmState`;
- `AlarmRecord`;
- `AlarmTransitionType`;
- `AlarmTransitionRequest`;
- `AlarmTransitionRecord`;
- `AlarmTransitionResult`;
- `AlarmRule`;
- `AlarmRuleEvaluationResult`;
- `evaluate_alarm_rule()`;
- repository-интерфейсы alarms;
- DTO alarms;
- SQL-черновик `0007_events_alarms.sql`.

Но пока нет:

- Alarm Manager;
- active alarm index;
- PostgreSQL repository implementation;
- автоматической интеграции runtime -> alarms;
- deduplication;
- automatic reactivation;
- automatic close policy;
- shelving policy;
- suppression policy;
- escalation;
- notifications;
- alarm API handlers;
- realtime delivery;
- UI active alarm panel.

## Alarm lifecycle пока доменный

`apply_alarm_transition()` применяет transition к копии `AlarmRecord`.

Но пока нет:

- сохранения transition в БД;
- обновления alarm repository;
- создания linked EventRecord;
- публикации в EventBus;
- audit trail;
- проверки прав пользователя.

## Alarm rules пока проверяются вручную

`evaluate_alarm_rule()` проверяет одно правило и одно текущее значение.

Пока нет слоя, который:

- загружает все rules;
- подписывается на runtime changes;
- проверяет rules автоматически;
- ищет существующие активные alarms;
- предотвращает дубли;
- создает transition history;
- закрывает аварии при нормализации.

---

# Ограничения API

## scada_api пока не является HTTP API

`scada_api` содержит foundation:

- route model;
- route registry;
- response model;
- mapper foundation;
- read endpoint model.

Но пока нет:

- HTTP listener;
- real routing;
- controllers;
- request parsing;
- response serialization;
- JSON;
- OpenAPI;
- middleware;
- authentication;
- authorization;
- rate limiting.

## API endpoint definitions не являются рабочими endpoint-ами

В `ApiRouteRegistry` и `ApiReadEndpointCatalog` описаны будущие endpoint-ы.

Но обращения по этим URL пока невозможны, потому что transport отсутствует.

## ApiResponse body пока строка

`ApiResponse.body` является placeholder.

Пока нет:

- typed response envelope;
- JSON serialization;
- content negotiation;
- streaming response;
- error response schema.

## ApiReadFilter пока строковый

Фильтры в `ApiReadFilter` представлены строками, потому что они моделируют будущие HTTP query string значения.

Пока нет:

- типизированного parsing;
- validation;
- conversion to domain query;
- repository query execution.

## Mapper foundation пока общий

Добавлены только:

- mapping status;
- mapping result;
- timestamp formatting;
- primitive formatting.

Пока нет конкретных mapper functions:

- Object -> DTO;
- Device -> DTO;
- Tag -> DTO;
- Runtime -> DTO;
- Historian -> DTO;
- Event -> DTO;
- Alarm -> DTO.

---

# Ограничения realtime

## scada_realtime пока не является gateway

`scada_realtime` содержит foundation:

- realtime message model;
- subscription model;
- channel types;
- message types;
- delivery modes.

Но пока нет:

- WebSocket server;
- SSE server;
- client registry;
- subscription registry;
- message dispatcher;
- topic router;
- retained message store;
- authorization;
- network transport.

## RealtimeMessage payload пока строка

Payload хранится строкой как transport-neutral placeholder.

Пока нет:

- JSON builder;
- binary payload support;
- schema validation;
- compression;
- serialization envelope.

## RealtimeSubscription пока не связана с клиентом сети

`RealtimeSubscription` содержит `client_id`, но реального сетевого клиента пока нет.

Пока нет:

- socket handle;
- connection state;
- remote address;
- ping/pong state;
- reconnect token;
- send queue.

---

# Ограничения application composition

## scada_app является foundation

`scada_app` уже содержит:

- `GatewayMode`;
- `GatewayStartupStatus`;
- `GatewayStartupOptions`;
- `GatewayStartupPlan`;
- `ApplicationComposition`;
- `create_gateway_startup_plan()`;
- `create_default_application_composition()`.

Но пока нет:

- настоящего service container;
- repository wiring;
- HTTP transport startup;
- WebSocket/SSE startup;
- settings loader;
- environment profiles;
- background worker lifecycle;
- graceful shutdown;
- health check execution.

## GatewayStartupPlan не запускает сервер

`GatewayStartupPlan` только описывает подготовку запуска.

Статус `Prepared` означает, что конфигурация выглядит валидной.

Реального перехода в `Running` пока нет.

---

# Ограничения контрактов

## DTO есть не для всех будущих модулей

В `scada_contracts` добавлены DTO для:

- объектов;
- дерева объектов;
- зон ответственности;
- устройств;
- диагностики устройств;
- тегов;
- текущих значений тегов;
- polling;
- runtime values;
- runtime events;
- historian;
- events;
- alarms.

Но пока нет DTO для:

- команд;
- пользователей;
- ролей;
- рабочих мест;
- панелей;
- дашбордов;
- protocol diagnostics;
- уведомлений;
- realtime messages.

## Нет mapper-слоя domain <-> DTO

Преобразование domain <-> DTO пока не реализовано.

Добавлен только нейтральный mapper foundation в `scada_api`.

---

# Ограничения базы данных

## Миграции являются черновыми

Сейчас есть черновые SQL-миграции:

- `database/migrations/0001_object_model.sql`;
- `database/migrations/0002_device_model.sql`;
- `database/migrations/0003_tag_model.sql`;
- `database/migrations/0004_polling_model.sql`;
- `database/migrations/0005_runtime_values.sql`;
- `database/migrations/0006_tag_history.sql`;
- `database/migrations/0007_events_alarms.sql`.

Они не выполняются автоматически.

## Нет механизма применения миграций

Пока отсутствует:

- migration runner;
- версия схемы БД;
- rollback;
- seed-данные;
- проверка совместимости схемы.

---

# Ограничения frontend

## Frontend пока не реализован

Каталог `frontend` существует как заготовка.

Пока нет:

- Blazor WebAssembly проекта;
- MudBlazor;
- страниц;
- компонентов;
- API-клиента;
- мнемосхем;
- дашбордов;
- графиков истории;
- журнала событий;
- журнала аварий;
- панели активных аварий;
- карточки объекта;
- карточки устройства;
- карточки тега;
- runtime value widgets.

---

# Ограничения безопасности

Пока нет:

- пользователей;
- ролей;
- сессий;
- токенов;
- прав доступа;
- аудита действий;
- политик команд;
- проверки прав на acknowledgement;
- проверки прав на command execution;
- authentication middleware;
- authorization middleware.

---

# Ограничения тестирования

Пока нет полноценного набора unit-тестов.

Сборка и запуск проверяются вручную.

Тесты будут добавляться позже по мере стабилизации модулей.

---

# Итог

После Sprint 009 проект имеет хороший backend foundation:

- object model;
- device model;
- tag model;
- protocol foundation;
- polling foundation;
- runtime foundation;
- historian foundation;
- events foundation;
- alarms foundation;
- API foundation;
- realtime foundation;
- application composition foundation.

Но Dispatcher еще не является рабочей диспетчерской системой.

Следующий важный этап:

    Sprint 010 — Minimal Operator UI and MVP stabilization
---

# Ограничения после закрытия MVP foundation

## MVP foundation не является production-ready системой

После Sprint 010 проект достиг MVP foundation, но не стал промышленной SCADA-системой.

Остаются ограничения:

- нет real HTTP API;
- нет real WebSocket/SSE gateway;
- нет PostgreSQL repository implementations;
- нет Modbus TCP driver;
- нет SNMP driver;
- нет background polling loop;
- нет Event Manager;
- нет Alarm Manager;
- нет authentication;
- нет authorization;
- нет dashboards;
- нет mimic diagrams;
- нет command execution.

## Frontend данные пока mock

Следующие frontend-экраны пока используют mock-data:

- System;
- Runtime;
- Events;
- Alarms.

Mock-data не отражает реальное состояние backend.

## Frontend API client пока placeholder

`DispatcherApiClient` пока не делает реальные HTTP-запросы.

Он нужен как стабильная точка расширения до реализации backend HTTP transport и API handlers.

## Frontend язык

После Sprint 010 принято правило:

    новый пользовательский frontend-текст пишется на русском языке

Текущие английские подписи, оставшиеся в UI foundation, считаются временным ограничением.

Английский язык будет добавлен позже как локализация, а не как основной язык интерфейса.

## Post-MVP ближайшие ограничения

Перед переходом к real integration нужно решить:

- русский UI cleanup;
- терминологию интерфейса;
- backend HTTP transport;
- JSON serialization;
- real system/modules API;
- real frontend API client;
- CORS/settings policy;
- error handling policy;
- loading states;
- empty states.

---

# Ограничения после Sprint 011

## Frontend стал русскоязычным, но локализация еще не реализована

После Sprint 011 основной пользовательский UI переведен на русский язык.

Однако полноценный localization engine пока не реализован.

Ограничения:

- нет resource files;
- нет переключения языка;
- нет culture switching;
- нет fallback language;
- нет проверки полноты локализации.

Английский язык планируется позже как дополнительная локализация.

## Route paths остаются английскими

URL routes остаются техническими:

- `/system`;
- `/objects`;
- `/devices`;
- `/tags`;
- `/runtime`;
- `/events`;
- `/alarms`.

Это принято сознательно.

Причина:

- route paths являются техническим контрактом;
- пользовательские подписи уже русские;
- русские URL могут усложнить поддержку.

## Demo-data остается временной

Несмотря на русский UI, данные на страницах пока demo/mock:

- System;
- Runtime;
- Events;
- Alarms.

Реальное подключение к backend API пока отсутствует.

## UI state foundation не заменяет real error handling

Добавлены:

- `UiStateKind`;
- `UiText`;
- `UiStatePanel`.

Но пока нет:

- реальной обработки HTTP errors;
- retry logic;
- loading state вокруг реальных запросов;
- cancellation handling;
- toast/snackbar error policy;
- global error boundary.

## Loading screen является статическим

Стартовый экран загрузки Blazor WebAssembly улучшен, но он статический.

Ограничения:

- не показывает реальный процент загрузки;
- не показывает этапы загрузки;
- не проверяет состояние backend;
- не обрабатывает offline mode;
- не содержит retry action.

## Следующий критичный gap

Главный gap после Sprint 011:

    frontend выглядит как операторский UI,
    но данные все еще не приходят из backend.

Следующий этап должен закрыть backend HTTP transport foundation.

---

## Ограничения после Sprint 012 — Backend HTTP API Transport Foundation

### Общий статус

После Sprint 012 backend имеет рабочий HTTP API foundation на базе:

    vcpkg + Drogon

Доступны первые системные endpoints:

    GET /api/system/health
    GET /api/system/modules

Frontend пока продолжает использовать demo-data.

Real frontend API integration будет выполняться в Sprint 013.

### HTTP gateway

Текущие ограничения HTTP gateway:

- HTTP server является development foundation;
- default bind address: `127.0.0.1`;
- default port: `8080`;
- HTTPS/TLS пока нет;
- CORS policy пока нет;
- authentication пока нет;
- authorization пока нет;
- OpenAPI/Swagger UI пока нет;
- request logging пока нет;
- rate limiting пока нет;
- production hardening пока нет.

### JSON error handling

Для известных Dispatcher routes используются Dispatcher responses.

Для неизвестных routes стандартный Drogon 404 response пока может возвращаться как plain text.

Ограничение:

    unified JSON error fallback для неизвестных API routes пока не реализован.

Нужно добавить позже:

- единый JSON формат ошибок;
- fallback для unknown API routes;
- 404 JSON response;
- 405 JSON response на уровне Drogon routing;
- correlation id в API errors.

### vcpkg

Проект теперь использует vcpkg manifest mode.

Ограничения:

- требуется установленный vcpkg;
- требуется корректный `VCPKG_ROOT`;
- standalone vcpkg предпочтительнее bundled Visual Studio vcpkg;
- первая сборка может занимать больше времени из-за установки Drogon и зависимостей;
- Linux triplet пока не проверялся;
- CI/CD с vcpkg пока не настроен.

### Drogon integration

Drogon интегрирован только внутри:

    backend/libs/scada_http

Ограничения:

- WebSocket/SSE пока не реализованы;
- middleware/filter pipeline пока не настроен;
- graceful full application shutdown пока минимальный;
- production configuration пока отсутствует;
- structured request logging пока отсутствует.

Архитектурное правило:

    Drogon types не должны попадать в доменные backend-модули.

### API scope

После Sprint 012 реализованы только system endpoints.

Пока нет:

- object API endpoints;
- device API endpoints;
- tag API endpoints;
- runtime API endpoints;
- history API endpoints;
- events API endpoints;
- alarms API endpoints;
- command API endpoints;
- auth API endpoints.

### Frontend

Frontend пока не подключен к реальному backend API.

Ограничения:

- System page все еще использует frontend-side mock/demo data;
- runtime/events/alarms pages используют mock/demo data;
- API base URL settings не реализованы;
- no connection state для реального backend еще не подключен;
- CORS не проверялся, потому что frontend integration еще не началась.

### Deployment

Deployment ограничения:

- Linux-сборка пока не проверялась;
- Docker пока не добавлен;
- service/systemd deployment пока не добавлен;
- production config files пока не добавлены;
- secrets/configuration management пока отсутствует.

### Следующее ожидаемое снятие ограничений

Ближайший следующий этап:

    Sprint 013 — Frontend Real API Client Integration

Ожидаемые снятия ограничений:

- frontend получит real API client settings;
- `/api/system/health` будет использоваться frontend;
- `/api/system/modules` будет использоваться frontend;
- System page перестанет быть полностью demo-only;
- появятся real loading/error/no connection states.
