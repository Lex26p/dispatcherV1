# Dispatcher Development Log

## Назначение документа

Этот документ фиксирует ход разработки Dispatcher по спринтам и шагам.

Документ нужен для:

- отслеживания выполненных задач;
- понимания текущего состояния проекта;
- фиксации архитектурных решений;
- подготовки следующих спринтов;
- восстановления контекста разработки.

---

# Sprint 001 — Project Foundation

## Статус

Закрыт.

## Цель спринта

Создать технический фундамент проекта Dispatcher.

## Выполнено

### Шаги 1–12

Создана базовая проектная документация, структура репозитория, CMake-настройки, приложение `dispatcher_server` и базовые backend-модули:

- `scada_common`;
- `scada_contracts`;
- `scada_core`.

Добавлены базовые общие типы, core-типы, модульная регистрация и начальная документация сервисов.

## Итог Sprint 001

Создан минимальный технический фундамент Dispatcher.

Проект собирается, запускается и имеет базовую модульную структуру.

---

# Sprint 002 — Object Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую объектную модель Dispatcher.

## Выполнено

### Шаги 13–21

Добавлен модуль:

- `scada_objects`.

Реализованы:

- `ObjectId`;
- `ResponsibilityZoneId`;
- `ObjectType`;
- `ObjectNode`;
- `ObjectPath`;
- `ObjectTree`;
- `ObjectHierarchyRules`;
- `ResponsibilityZone`;
- DTO объектной модели;
- repository-интерфейсы объектной модели;
- миграция `0001_object_model.sql`.

## Итог Sprint 002

Создан фундамент объектной модели Dispatcher.

---

# Sprint 003 — Device Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель устройств Dispatcher.

## Выполнено

### Шаги 22–28

Добавлен модуль:

- `scada_devices`.

Реализованы:

- `DeviceId`;
- `Device`;
- `DeviceProtocol`;
- `DeviceState`;
- `DeviceConnection`;
- `IpMode`;
- `DeviceDiagnostics`;
- `DeviceCommunicationStatus`;
- `DeviceHealthStatus`;
- DTO устройств;
- repository-интерфейсы устройств;
- миграция `0002_device_model.sql`.

## Итог Sprint 003

Создан фундамент модели устройств Dispatcher.

---

# Sprint 004 — Tag Model Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую модель тегов Dispatcher.

## Выполнено

### Шаги 29–35

Добавлен модуль:

- `scada_tags`.

Реализованы:

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
- DTO тегов;
- repository-интерфейсы тегов;
- миграция `0003_tag_model.sql`.

## Итог Sprint 004

Создан фундамент модели тегов Dispatcher.

---

# Sprint 005 — Communication and Polling Foundation

## Статус

Закрыт.

## Цель спринта

Создать базовую архитектуру коммуникаций и опроса.

## Выполнено

### Шаг 36 — Protocol driver contracts

Добавлен модуль:

- `scada_protocols`.

Реализованы:

- `ProtocolCapabilities`;
- request/result модели протокольного слоя;
- `IProtocolDriver`;
- `get_protocol_module_info()`.

### Шаг 37 — Simulator protocol driver foundation

Добавлен:

- `SimulatorProtocolDriver`.

### Шаг 38 — Protocol driver registry

Добавлен:

- `ProtocolDriverRegistry`;
- `create_default_protocol_driver_registry()`.

### Шаг 39 — Polling task and group model

Добавлен модуль:

- `scada_polling`.

Реализованы:

- `PollingTaskId`;
- `PollingGroupId`;
- `PollingTaskState`;
- `PollingGroupMode`;
- `PollingTask`;
- `PollingGroup`.

### Шаг 40 — Polling scheduler foundation

Добавлена минимальная синхронная основа scheduler.

Реализованы:

- `PollingExecutionStatus`;
- `PollingTaskExecutionResult`;
- `PollingGroupExecutionResult`;
- `PollingSchedulerOptions`;
- `PollingScheduler`.

### Шаг 41 — Polling DTO / repository / migration draft

Добавлены:

- DTO polling;
- `IPollingTaskRepository`;
- `IPollingGroupRepository`;
- миграция `0004_polling_model.sql`.

### Шаг 42 — Sprint 005 docs update and close

Sprint 005 закрыт.

## Итог Sprint 005

Создан фундамент коммуникаций и опроса Dispatcher.

---

# Sprint 006 — Runtime Values and Data Engine

## Статус

Закрыт.

## Цель спринта

Создать runtime-слой текущих значений тегов.

## Выполнено

### Шаг 43 — scada_runtime module and TagValueStore foundation

Добавлен модуль:

- `scada_runtime`.

Реализованы:

- `TagValueStore`;
- `get_runtime_module_info()`.

### Шаг 44 — Apply ProtocolReadResult to TagCurrentValue

Добавлены:

- `RuntimeValueApplyResult`;
- `RuntimeValueApplier`.

Реализована цепочка:

    ProtocolReadResult -> TagCurrentValue -> TagValueStore

### Шаг 45 — Runtime quality and last good value handling

Добавлены runtime quality helpers.

Реализовано обновление:

- `last_good_value`;
- `last_good_timestamp`.

### Шаг 46 — Value conversion and engineering transform

Добавлены:

- runtime value conversion;
- engineering transform.

Реализована формула:

    engineering_value = raw_value * scale + offset

### Шаг 47 — Runtime change detection and events foundation

Добавлены:

- `RuntimeValueChangeKind`;
- `RuntimeValueChangeResult`;
- `RuntimeEventType`;
- `RuntimeValueEvent`.

### Шаг 48 — Runtime DTO / repository / migration draft

Добавлены:

- DTO runtime;
- `IRuntimeValueSnapshotRepository`;
- `IRuntimeEventRepository`;
- миграция `0005_runtime_values.sql`.

### Шаг 49 — Sprint 006 docs update and close

Sprint 006 закрыт.

## Итог Sprint 006

Создан foundation runtime-слоя текущих значений Dispatcher.

---

# Sprint 007 — Historian Foundation

## Статус

Закрыт.

## Цель спринта

Создать фундамент истории значений тегов.

Sprint 007 подготовил цепочку:

    RuntimeValueEvent / TagCurrentValue -> archive decision -> HistorianBuffer -> batch write -> future PostgreSQL / TimescaleDB

## Выполнено

### Шаг 50 — scada_historian module and HistorySample model

Добавлен модуль:

- `scada_historian`.

Добавлены:

- `HistorySampleId`;
- `HistorySample`;
- `make_history_sample_from_current_value()`;
- `get_historian_module_info()`.

### Шаг 51 — Archive decision foundation

Добавлена основа принятия решения об архивировании.

Добавлены:

- `ArchiveDecisionReason`;
- `ArchiveDecisionOptions`;
- `ArchiveDecision`;
- `decide_archive()`.

### Шаг 52 — Historian buffer and batch write contract

Добавлен in-memory buffer и контракт будущей batch-записи.

Добавлены:

- `HistorianBuffer`;
- `HistoryBatch`;
- `HistoryBatchWriteResult`;
- `IHistoryBatchWriter`.

### Шаг 53 — History query model

Добавлена доменная модель запроса истории.

Добавлены:

- `HistoryQueryTimeRange`;
- `HistoryQueryFilter`;
- `HistoryQueryOptions`;
- `HistoryQuery`;
- `HistoryQueryResult`;
- `validate_history_query()`.

### Шаг 54 — Historian DTO / repository / migration draft

Добавлены:

- DTO historian;
- `IHistorySampleRepository`;
- миграция `0006_tag_history.sql`.

### Шаг 55 — Sprint 007 docs update and close

Sprint 007 закрыт.

## Итог Sprint 007

Создан foundation Historian.

---

# Sprint 008 — Events and Alarms Foundation

## Статус

Закрыт.

## Цель спринта

Создать фундамент событий и аварий Dispatcher.

Sprint 008 подготовил цепочку:

    Runtime / Historian / Polling / Devices -> Events -> Alarms -> future Event Manager / Alarm Manager / UI

## Выполнено

### Шаг 56 — scada_events module and EventRecord foundation

Добавлен модуль:

- `scada_events`.

Добавлены:

- `EventId`;
- `EventTimestamp`;
- `EventCategory`;
- `EventSeverity`;
- `EventSourceType`;
- `EventRecord`;
- `make_event_record()`;
- `get_event_module_info()`.

### Шаг 57 — scada_alarms module and Alarm model foundation

Добавлен модуль:

- `scada_alarms`.

Добавлены:

- `AlarmId`;
- `AlarmTimestamp`;
- `AlarmSourceType`;
- `AlarmSeverity`;
- `AlarmPriority`;
- `AlarmState`;
- `AlarmRecord`;
- `make_alarm_record()`;
- `to_event_severity()`;
- `get_alarm_module_info()`.

### Шаг 58 — Alarm lifecycle and transitions

Добавлен lifecycle foundation аварий.

Добавлены:

- `AlarmTransitionType`;
- `AlarmTransitionValidationCode`;
- `AlarmTransitionRequest`;
- `AlarmTransitionRecord`;
- `AlarmTransitionResult`;
- `can_apply_alarm_transition()`;
- `apply_alarm_transition()`.

### Шаг 59 — Alarm rules foundation

Добавлен foundation alarm rules.

Добавлены:

- `AlarmRuleId`;
- `AlarmRuleType`;
- `AlarmRuleState`;
- `AlarmRuleComparison`;
- `AlarmRuleEvaluationStatus`;
- `AlarmRule`;
- `AlarmRuleEvaluationResult`;
- `evaluate_alarm_rule()`.

### Шаг 60 — Events/Alarms DTO / repository / migration draft

В `scada_contracts` добавлены DTO events и alarms.

В `scada_events` добавлен:

- `IEventRecordRepository`.

В `scada_alarms` добавлены:

- `IAlarmRepository`;
- `IAlarmTransitionRepository`;
- `IAlarmRuleRepository`.

Добавлена миграция:

- `database/migrations/0007_events_alarms.sql`.

### Шаг 61 — Sprint 008 docs update and close

Sprint 008 закрыт.

## Итог Sprint 008

Создан foundation событий и аварий Dispatcher.

---

# Sprint 009 — API and Realtime Gateway

## Статус

Закрывается на шаге 67.

## Цель спринта

Создать foundation backend API и realtime gateway.

Sprint 009 должен был подготовить цепочку:

    Domain modules / DTO
        -> API foundation
        -> Read endpoint model
        -> Realtime foundation
        -> Application composition
        -> future HTTP/WebSocket transport

## Выполнено

### Шаг 62 — scada_api module and API route/response foundation

Добавлен модуль:

- `scada_api`.

Добавлены:

- `ApiHttpMethod`;
- `ApiEndpointArea`;
- `ApiEndpoint`;
- `ApiRouteRegistry`;
- `ApiResponseStatus`;
- `ApiResponse`;
- `create_default_api_route_registry()`;
- `get_api_module_info()`.

Начальный route registry содержит будущие endpoint definitions:

- `/api/system/health`;
- `/api/system/modules`;
- `/api/runtime/values`;
- `/api/history/query`;
- `/api/events`;
- `/api/alarms/active`;
- `/api/realtime`.

На этом шаге реальный HTTP-сервер не добавлялся.

### Шаг 63 — API mapper foundation

В `scada_api` добавлен mapper foundation.

Добавлены:

- `ApiMappingStatus`;
- `ApiTimestampFormat`;
- `ApiMapperOptions`;
- `ApiMappingIssue`;
- `ApiMappingResult`;
- `make_mapping_success()`;
- `make_mapping_error()`;
- `format_timestamp()`;
- `format_bool()`;
- `format_uint64()`.

Конкретные domain-to-DTO мапперы не добавлялись, чтобы не превращать `scada_api` в God-module.

### Шаг 64 — API read endpoints model foundation

В `scada_api` добавлен read endpoint model foundation.

Добавлены:

- `ApiReadResourceType`;
- `ApiReadEndpointKind`;
- `ApiReadQueryOptions`;
- `ApiReadFilter`;
- `ApiReadEndpointDefinition`;
- `ApiReadEndpointCatalog`;
- `create_default_api_read_endpoint_catalog()`.

Начальный read endpoint catalog содержит будущие read definitions:

- `/api/system/health`;
- `/api/system/modules`;
- `/api/objects/tree`;
- `/api/objects`;
- `/api/devices`;
- `/api/tags`;
- `/api/runtime/values`;
- `/api/history/query`;
- `/api/events`;
- `/api/alarms/active`;
- `/api/alarms/rules`.

На этом шаге реальные controllers, handlers и repository calls не добавлялись.

### Шаг 65 — scada_realtime module and realtime message foundation

Добавлен модуль:

- `scada_realtime`.

Добавлены:

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
- `make_realtime_message()`;
- `make_realtime_subscription()`;
- `get_realtime_module_info()`.

На этом шаге реальный WebSocket/SSE transport не добавлялся.

### Шаг 66 — Application composition and gateway startup foundation

Добавлен модуль:

- `scada_app`.

Добавлены:

- `GatewayMode`;
- `GatewayStartupStatus`;
- `GatewayStartupOptions`;
- `GatewayStartupPlan`;
- `ApplicationComposition`;
- `create_gateway_startup_plan()`;
- `create_default_application_composition()`;
- `get_app_module_info()`.

`scada_app` связывает:

- API route registry;
- API read endpoint catalog;
- realtime foundation;
- gateway startup plan.

На этом шаге реальный запуск HTTP/WebSocket сервера не добавлялся.

### Шаг 67 — Sprint 009 docs update and close

Обновлена документация:

- `docs/development-log.md`;
- `docs/known-limitations.md`;
- `docs/SPRINT_009_SUMMARY.md`.

Sprint 009 закрывается.

## Итог Sprint 009

В Sprint 009 создан foundation API и realtime gateway.

Сейчас в проекте есть:

- модуль `scada_api`;
- API route model;
- API response model;
- API mapper foundation;
- API read endpoint model;
- модуль `scada_realtime`;
- realtime message model;
- realtime subscription model;
- модуль `scada_app`;
- gateway startup options;
- gateway startup plan;
- application composition snapshot.

## Что сознательно не делали в Sprint 009

В Sprint 009 не добавлялись:

- настоящий HTTP server;
- настоящий WebSocket server;
- SSE server;
- JSON serialization;
- REST controllers;
- request handlers;
- repository implementations;
- authorization;
- OpenAPI;
- frontend.

Эти задачи будут выполняться позже.

---

# Текущее состояние после Sprint 009

Проект находится в состоянии:

    Dispatcher backend foundation
        +
    Object Model foundation
        +
    Device Model foundation
        +
    Tag Model foundation
        +
    Communication and Polling foundation
        +
    Runtime Values foundation
        +
    Historian foundation
        +
    Events and Alarms foundation
        +
    API and Realtime Gateway foundation

Следующий логический фокус:

    Minimal Operator UI and MVP stabilization

---

# Следующий спринт

## Sprint 010 — Minimal Operator UI and MVP stabilization

Цель Sprint 010:

Создать минимальный frontend/operator UI foundation и стабилизировать MVP-контур.

Предварительные направления:

- Blazor WebAssembly project foundation;
- MudBlazor foundation;
- minimal layout;
- system/modules page;
- object/device/tag placeholder pages;
- runtime values placeholder page;
- events/alarms placeholder pages;
- frontend API client placeholder;
- MVP stabilization docs;
- final MVP limitations.

---

# Общий статус проекта

## Завершено

- Sprint 001 — Project Foundation.
- Sprint 002 — Object Model Foundation.
- Sprint 003 — Device Model Foundation.
- Sprint 004 — Tag Model Foundation.
- Sprint 005 — Communication and Polling Foundation.
- Sprint 006 — Runtime Values and Data Engine.
- Sprint 007 — Historian Foundation.
- Sprint 008 — Events and Alarms Foundation.
- Sprint 009 — API and Realtime Gateway.

## Следующий

- Sprint 010 — Minimal Operator UI and MVP stabilization.
---

# Sprint 010 — Minimal Operator UI and MVP stabilization

## Статус

Закрыт.

## Цель спринта

Создать минимальный frontend/operator UI foundation и стабилизировать MVP-контур Dispatcher.

Sprint 010 подготовил цепочку:

    backend foundation
        +
    frontend operator UI foundation
        +
    MVP stabilization docs

## Выполнено

### Шаг 68 — Blazor WebAssembly frontend project foundation

Создан frontend-проект:

- `frontend/Dispatcher.Frontend`.

Добавлены:

- Blazor WebAssembly project foundation;
- frontend solution `Dispatcher.Frontend.slnx`;
- базовый frontend shell;
- navigation;
- placeholder pages;
- `frontend/README.md`;
- `frontend/Dispatcher.Frontend/SERVICE.md`.

Особенность:

- проект создан в классической структуре Blazor WebAssembly;
- маршрутизация находится в `App.razor`;
- HTML shell находится в `wwwroot/index.html`.

Исправление:

- страница `System.razor` переименована в `SystemPage.razor`, чтобы не конфликтовать с namespace `System`.

### Шаг 69 — Minimal operator layout and navigation

Подключен MudBlazor.

Добавлены:

- MudBlazor package;
- MudBlazor services;
- MudBlazor CSS/JS;
- `MudThemeProvider`;
- `MudLayout`;
- `MudAppBar`;
- `MudDrawer`;
- sidebar navigation;
- dark operator layout.

Исправления:

- заменены несовместимые enum/value names текущей версии MudBlazor;
- включен dark mode;
- усилены CSS-правила drawer/navigation.

### Шаг 70 — System/modules and API client placeholder

Добавлен frontend API client placeholder.

Добавлены models:

- `BackendModuleViewModel`;
- `GatewaySummaryViewModel`;
- `ApiRouteSummaryViewModel`.

Добавлены services:

- `DispatcherApiClientOptions`;
- `IDispatcherApiClient`;
- `DispatcherApiClient`.

System page теперь показывает:

- backend modules;
- gateway summary;
- API route summary;
- realtime foundation summary.

Данные пока mock.

### Шаг 71 — Runtime/events/alarms placeholder pages

Добавлены models:

- `RuntimeValueViewModel`;
- `EventRecordViewModel`;
- `ActiveAlarmViewModel`.

Расширен API client placeholder.

Обновлены страницы:

- Runtime;
- Events;
- Alarms.

Runtime page показывает mock current values.

Events page показывает mock event journal.

Alarms page показывает mock active alarms.

### Шаг 72 — MVP stabilization docs

Созданы документы:

- `docs/MVP_STATUS.md`;
- `docs/MVP_STABILIZATION_CHECKLIST.md`;
- `docs/MVP_SCOPE.md`.

Документы фиксируют:

- текущий MVP status;
- MVP scope;
- mock scope;
- foundation scope;
- stabilization checklist.

### Шаг 73 — Sprint 010 docs update and MVP closure

Создан:

- `docs/SPRINT_010_SUMMARY.md`.

Обновлены:

- `docs/development-log.md`;
- `docs/known-limitations.md`.

Sprint 010 закрыт.

MVP foundation stage закрыт.

## Языковая политика frontend

После Sprint 010 frontend-интерфейс должен разрабатываться на русском языке по умолчанию.

Английский язык будет рассматриваться как будущая локализация.

Текущие английские подписи в UI foundation являются временными.

## Итог Sprint 010

Создан минимальный operator UI foundation.

MVP foundation достигнут.

Текущий MVP не является production-ready SCADA, но показывает:

- backend modular foundation;
- frontend operator UI direction;
- API/realtime foundation;
- runtime/events/alarms UI placeholders;
- documented MVP boundaries.

---

# MVP Foundation Closure

## Статус

Закрыт.

## Завершенные спринты MVP foundation

- Sprint 001 — Project Foundation.
- Sprint 002 — Object Model Foundation.
- Sprint 003 — Device Model Foundation.
- Sprint 004 — Tag Model Foundation.
- Sprint 005 — Communication and Polling Foundation.
- Sprint 006 — Runtime Values and Data Engine.
- Sprint 007 — Historian Foundation.
- Sprint 008 — Events and Alarms Foundation.
- Sprint 009 — API and Realtime Gateway.
- Sprint 010 — Minimal Operator UI and MVP stabilization.

## Итог

MVP foundation stage завершен.

Следующий этап:

    Post-MVP development

Основной фокус следующего этапа:

- русский UI cleanup;
- real HTTP API transport;
- JSON serialization;
- system/modules real API;
- transition from mock frontend data to backend data;
- frontend real API client;
- real integration foundations.

---

# Sprint 011 — Russian UI Cleanup and Frontend Terminology

## Статус

Закрыт.

## Этап

Post-MVP development.

## Цель спринта

Привести frontend-интерфейс Dispatcher к русскому языку по умолчанию и зафиксировать единую терминологию пользовательского интерфейса.

## Выполнено

### Шаг 74 — Sprint 011 plan and UI terminology foundation

Созданы:

- `docs/SPRINT_011_PLAN.md`;
- `docs/UI_TERMINOLOGY.md`.

Зафиксирована языковая политика:

    новый пользовательский текст во frontend пишем на русском языке

Английский язык оставлен как будущая локализация.

### Шаг 75 — Translate layout and navigation

Переведены:

- AppBar;
- Drawer;
- sidebar navigation;
- operator status panel.

Пункты меню теперь отображаются на русском:

- Обзор;
- Система;
- Объекты;
- Устройства;
- Теги;
- Текущие значения;
- События;
- Аварии.

### Шаг 76 — Translate overview, system, objects, devices and tags pages

Переведены:

- overview page;
- system page;
- objects page;
- devices page;
- tags page.

System page теперь показывает русские названия и описания demo backend modules.

### Шаг 77 — Translate runtime, events and alarms pages

Переведены:

- runtime page;
- events page;
- alarms page.

На русском отображаются:

- заголовки;
- summary cards;
- table headers;
- quality labels;
- severity labels;
- priority labels;
- alarm states;
- acknowledgement labels;
- demo messages.

### Шаг 78 — Add UI state text foundation

Добавлены:

- `UiStateKind`;
- `UiText`;
- `UiStatePanel`.

UI state foundation покрывает:

- demo data;
- loading;
- empty;
- warning;
- error;
- not implemented;
- no connection.

Страницы System/Runtime/Events/Alarms используют общий блок «Демо-данные».

Страницы Objects/Devices/Tags используют общий блок «Функция пока не реализована».

### Шаг 79 — Update frontend docs and improve loading screen

Обновлены:

- `frontend/README.md`;
- `frontend/Dispatcher.Frontend/SERVICE.md`.

Улучшен стартовый экран загрузки Blazor WebAssembly.

Теперь при первом открытии frontend пользователь видит:

- брендовый loading panel;
- логотип `D`;
- текст «Загрузка интерфейса»;
- spinner;
- анимированную полосу загрузки.

### Шаг 80 — Sprint 011 docs update and closure

Создан:

- `docs/SPRINT_011_SUMMARY.md`.

Обновлены:

- `docs/development-log.md`;
- `docs/known-limitations.md`.

Sprint 011 закрыт.

## Итог

Frontend стал русскоязычным по умолчанию.

Принята политика:

    русский UI сейчас
    английский позже как локализация

## Ограничения после Sprint 011

Остаются:

- demo-data;
- mock API client;
- отсутствие real HTTP API;
- отсутствие realtime client;
- отсутствие localization engine;
- отсутствие real backend integration.

## Следующий рекомендуемый спринт

    Sprint 012 — Backend HTTP API Transport Foundation

Фокус:

- backend HTTP transport;
- `/api/system/health`;
- `/api/system/modules`;
- подготовка real frontend API integration.

---

## Sprint 012 — Backend HTTP API Transport Foundation

### Статус

Closed.

### Цель

Добавить foundation реального backend HTTP transport для Dispatcher.

### Выполнено

В Sprint 012 выполнены шаги 81–90.

Создан backend-модуль:

    backend/libs/scada_http

Добавлены:

- HTTP transport models;
- HTTP route dispatcher foundation;
- `/api/system/health` endpoint foundation;
- `/api/system/modules` endpoint foundation;
- vcpkg manifest;
- Drogon dependency;
- Drogon HTTP server wrapper;
- system endpoint registry;
- запуск Drogon HTTP server из `dispatcher_server`.

### Transport decision

Первоначально рассматривался вариант:

    Boost.Asio + Boost.Beast

После анализа решение было пересмотрено.

Итоговое решение:

    vcpkg + Drogon

Причины:

- Drogon является более подходящим постоянным HTTP/WebSocket gateway foundation;
- меньше низкоуровневого infrastructure-кода;
- быстрее переход к реальному API;
- framework можно изолировать внутри `scada_http`;
- vcpkg дает воспроизводимое dependency management.

### vcpkg

Добавлен:

    vcpkg.json

CMake presets обновлены для использования vcpkg toolchain.

Важно:

- используется `VCPKG_ROOT`;
- рекомендованный локальный путь: `C:\vcpkg`;
- `vcpkg.json` должен содержать `builtin-baseline`.

### Реализованные endpoints

Доступны:

    GET /api/system/health
    GET /api/system/modules

Проверка:

    Invoke-RestMethod http://127.0.0.1:8080/api/system/health
    Invoke-RestMethod http://127.0.0.1:8080/api/system/modules

### dispatcher_server

`dispatcher_server` теперь:

- регистрирует `scada_http`;
- создает system route dispatcher;
- запускает `DrogonHttpServer`;
- выводит HTTP URLs в console output;
- остается запущенным и ожидает Enter для остановки.

### Проверки

Проверено:

- backend configure;
- backend build;
- backend run;
- `/api/system/health`;
- `/api/system/modules`;
- frontend project build;
- frontend solution build;
- git commit and push.

### Результат

Sprint 012 закрыл первый реальный backend HTTP API foundation.

После Sprint 012 backend имеет рабочую цепочку:

    dispatcher_server
        -> scada_http
        -> DrogonHttpServer
        -> HttpRouteDispatcher
        -> system endpoints

### Следующий этап

Следующий рекомендуемый спринт:

    Sprint 013 — Frontend Real API Client Integration

Цель следующего спринта:

- подключить frontend к реальному backend API;
- заменить demo-data на System page;
- добавить loading/error/no connection states.

---

## Sprint 013 — Frontend Real API Client Integration

### Статус

Started.

### Цель

Подключить frontend Dispatcher к реальному backend HTTP API, созданному в Sprint 012.

### Начальный scope

Первый real integration scope:

    GET /api/system/health
    GET /api/system/modules

### Ожидаемый результат

Страница frontend:

    /system

должна перейти от demo-only data к реальным backend данным.

### Шаг 91

Создан план Sprint 013:

    docs/SPRINT_013_PLAN.md

Зафиксированы правила:

- frontend вызывает backend только через `IDispatcherApiClient`;
- System page первой переходит на real API;
- mock mode остается как fallback/development option;
- нужен backend development CORS foundation;
- production auth/CORS/WebSocket не входят в Sprint 013.
