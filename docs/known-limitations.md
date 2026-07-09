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

Dispatcher пока находится на этапе формирования backend-фундамента.

На текущем этапе система еще не является готовой SCADA-платформой.

## Нет production-ready функциональности

Текущая версия не предназначена для промышленной эксплуатации.

Пока реализуются базовые архитектурные модули и доменные модели.

## Нет полноценного запуска системы

`dispatcher_server` запускается и выводит информацию о модулях.

Но он пока не выполняет:

- полноценный фоновый опрос оборудования;
- подключение к PostgreSQL;
- HTTP API;
- WebSocket;
- авторизацию;
- архивирование в БД;
- обработку аварий через Alarm Manager;
- управление командами.

---

# Ограничения backend

## Нет HTTP API

На текущем этапе отсутствуют:

- REST API;
- WebSocket API;
- OpenAPI;
- контроллеры;
- middleware;
- авторизация запросов.

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
- базовую валидацию.

Но пока нет:

- хранения в БД через приложение;
- API;
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
- repository-интерфейсы.

Но пока нет:

- реального сетевого обмена с устройствами;
- Modbus TCP driver;
- SNMP driver;
- проверки связи через сеть;
- test poll через реальное устройство;
- API;
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
- repository-интерфейсы тегов.

Но пока нет:

- полноценного alarm rule management через API;
- command execution;
- API;
- UI;
- PostgreSQL-реализации;
- unit-тестов.

## Политики архивирования частично используются только доменно

`TagArchivePolicy` уже используется в `scada_historian` для archive decision.

Но пока нет:

- автоматической интеграции runtime -> historian;
- периодического scheduler;
- batch writer в БД;
- TimescaleDB-интеграции;
- deadband diagnostics;
- per-tag archive configuration repository.

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

Эта интеграция будет добавлена позже.

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
- WebSocket integration;
- HTTP API.

## TagValueStore является in-memory хранилищем

`TagValueStore` хранит текущие значения только в памяти процесса.

После остановки приложения runtime-state теряется.

Постоянное snapshot-хранилище пока описано только интерфейсами и SQL-черновиком.

## Runtime events пока не публикуются

`RuntimeValueEvent` формируется и возвращается в `RuntimeValueApplyResult`.

Но пока нет:

- публикации в EventBus;
- подписчиков;
- доставки в WebSocket;
- записи в Historian;
- alarm evaluation.

## Change detection пока базовый

Runtime сравнивает:

- `engineering_value`;
- `quality`;
- `source`.

Пока не учитываются:

- deadband;
- archive policy;
- alarm thresholds;
- hysteresis;
- debounce;
- rate limit;
- stale timeout.

---

# Ограничения historian

## Historian является foundation

Модуль `scada_historian` уже содержит:

- `HistorySampleId`;
- `HistorySample`;
- `ArchiveDecisionReason`;
- `ArchiveDecisionOptions`;
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
- History API;
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

## Archive decision не пишет историю

`decide_archive()` только принимает решение.

Она не выполняет:

- создание sample;
- запись в buffer;
- запись в БД;
- публикацию события.

Будущая цепочка должна быть отдельной:

    RuntimeValueEvent / TagCurrentValue
        -> ArchiveDecision
        -> HistorySample
        -> HistorianBuffer
        -> HistoryBatch
        -> IHistoryBatchWriter
        -> PostgreSQL / TimescaleDB

## HistoryQuery пока не выполняется

`HistoryQuery` описывает параметры будущего запроса.

Но пока нет реализации:

- SQL SELECT;
- фильтрации в repository;
- чтения из PostgreSQL;
- pagination на уровне БД;
- History API.

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
- event API;
- WebSocket delivery;
- UI event journal.

## EventRecord пока не создается автоматически

Модель события есть, но runtime, historian, polling, alarms и другие модули пока не создают события автоматически.

Будущая цепочка:

    Module -> EventRecord -> Event Manager -> Repository / EventBus / WebSocket

## EventBus пока отсутствует

В `scada_core` есть foundation `IEventBus`, но пока нет полноценной интеграции с доменными events.

Нет:

- InMemoryEventBus implementation для domain events;
- подписчиков;
- routing;
- persistence;
- delivery guarantees;
- external event bus.

## События пока не связаны с аудитом

Пока нет audit subsystem.

Не реализованы:

- события входа пользователя;
- события изменения конфигурации;
- события команд;
- события подтверждения аварий;
- события изменения прав.

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
- alarm API;
- WebSocket delivery;
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

## Alarm rules пока базовые

Поддержаны только:

- `NumericThreshold`;
- `QualityEquals`;
- `QualityNotGood`;
- `QualityBad`.

Пока нет:

- hysteresis;
- debounce;
- delay before activation;
- delay before clear;
- rate of change;
- expression rules;
- script rules;
- composite rules;
- schedule-aware rules;
- maintenance-aware rules.

## Severity и priority пока не используются UI

Severity и priority уже есть в модели.

Но пока нет UI и Alarm Manager, которые используют их для:

- сортировки активных аварий;
- цветовой индикации;
- escalation;
- уведомлений;
- фильтрации.

## Shelve и suppress пока только состояния

Состояния `Shelved` и `Suppressed` есть.

Но пока нет:

- политики shelving;
- срока shelving;
- причины suppression;
- автоматического возврата;
- UI для управления этими режимами.

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
- уведомлений.

## Нет mapper-слоя

Преобразование domain <-> DTO пока не реализовано.

Это будет сделано позже, когда появятся API и repository-реализации.

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
- проверки прав на command execution.

---

# Ограничения тестирования

Пока нет полноценного набора unit-тестов.

Сборка и запуск проверяются вручную.

Тесты будут добавляться позже по мере стабилизации модулей.

---

# Итог

После Sprint 008 проект имеет хороший фундамент объектной модели, модели устройств, модели тегов, протокольного слоя, polling foundation, runtime values foundation, historian foundation и events/alarms foundation.

Но Dispatcher еще не является рабочей диспетчерской системой.

Следующий важный этап:

    Sprint 009 — API and Realtime Gateway