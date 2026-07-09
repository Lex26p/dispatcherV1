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
- обработку аварий;
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

- alarm rules;
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

## HistorySample не сохраняется автоматически

`HistorySample` является доменной моделью.

Пока нет слоя, который автоматически берет runtime value и сохраняет его как history sample.

## HistoryQuery пока не выполняется

`HistoryQuery` описывает параметры будущего запроса.

Но пока нет реализации:

- SQL SELECT;
- фильтрации в repository;
- чтения из PostgreSQL;
- pagination на уровне БД;
- History API.

## Tag history migration является черновой

Миграция `0006_tag_history.sql` описывает таблицу:

- `tag_history_samples`.

Но приложение пока ее не применяет.

## TimescaleDB пока не подключен

Миграция написана как обычная PostgreSQL-таблица.

Позже таблицу можно будет преобразовать в TimescaleDB hypertable.

## Retention и compression пока отсутствуют

Пока нет:

- retention policies;
- compression policies;
- downsampling;
- continuous aggregates;
- rollups;
- materialized views.

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
- historian.

Но пока нет DTO для:

- аварий;
- Event Manager;
- команд;
- пользователей;
- дашбордов;
- protocol diagnostics.

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
- `database/migrations/0006_tag_history.sql`.

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
- карточки объекта;
- карточки устройства;
- карточки тега;
- runtime value widgets.

---

# Ограничения аварий и событий

Пока нет:

- Event Manager;
- Alarm Manager;
- жизненного цикла аварий;
- квитирования;
- истории событий;
- уведомлений;
- аварий связи;
- аварий по значениям тегов.

---

# Ограничения безопасности

Пока нет:

- пользователей;
- ролей;
- сессий;
- токенов;
- прав доступа;
- аудита действий;
- политик команд.

---

# Ограничения тестирования

Пока нет полноценного набора unit-тестов.

Сборка и запуск проверяются вручную.

Тесты будут добавляться позже по мере стабилизации модулей.

---

# Итог

После Sprint 007 проект имеет хороший фундамент объектной модели, модели устройств, модели тегов, протокольного слоя, polling foundation, runtime values foundation и historian foundation.

Но Dispatcher еще не является рабочей диспетчерской системой.

Следующий важный этап:

    Sprint 008 — Events and Alarms Foundation