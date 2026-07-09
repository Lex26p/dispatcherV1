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
- архивирование;
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

Но пока нет реализаций:

- in-memory repository;
- PostgreSQL repository;
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

## Правила иерархии пока статические

Допустимые связи parent/child пока зашиты в C++-коде.

В будущем возможны:

- пользовательские типы объектов;
- конфигурируемые правила иерархии;
- разные шаблоны объектов для разных отраслей.

---

# Ограничения зон ответственности

## ResponsibilityZone пока не является системой прав доступа

`ResponsibilityZone` только хранит связи:

- объекты;
- пользователи;
- роли.

Она пока не выполняет:

- проверку прав;
- проверку доступа к объектам;
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

- Historian;
- tag history;
- alarm rules;
- command execution;
- API;
- UI;
- PostgreSQL-реализации;
- unit-тестов.

## TagCurrentValue уже используется, но не является постоянным хранилищем

`TagCurrentValue` используется runtime-слоем как модель текущего значения.

Но пока нет:

- восстановления snapshots при старте;
- PostgreSQL snapshot repository;
- потокобезопасного доступа;
- API для чтения текущих значений;
- WebSocket рассылки изменений.

## Политики архивирования пока не исполняются

В `TagArchivePolicy` уже заложены значения:

- `Disabled`;
- `OnChange`;
- `Periodic`;
- `OnChangeWithDeadband`;
- `OnQualityChange`;
- `OnAlarm`;
- `AlwaysButThrottled`.

Но пока нет:

- Historian;
- deadband logic;
- periodic archive scheduler;
- archive buffer;
- batch writer;
- TimescaleDB-интеграции.

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

Он:

- не открывает сетевые соединения;
- не проверяет IP/port;
- не читает реальные регистры;
- не читает реальные OID;
- не гарантирует поведение реального оборудования.

## Batch read пока базовый

Batch read уже есть в контракте и simulator driver.

Но пока нет:

- оптимизации Modbus registers;
- группировки contiguous ranges;
- SNMP OID grouping;
- driver-specific batch optimizer;
- ограничения по PDU;
- retry отдельных элементов batch.

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

## PollingScheduler пока синхронный

`PollingScheduler` умеет выполнить одну polling group один раз.

Он пока не:

- запускается сам;
- работает в фоне;
- планирует следующие опросы;
- обновляет состояние задач в repository;
- автоматически пишет значения в `TagValueStore`;
- отправляет события в EventBus.

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

## Runtime snapshots не являются Historian

`runtime_value_snapshots` хранит только последнее известное состояние тега.

Это не временной ряд.

История значений тегов будет реализована в Historian sprint.

## Runtime events не являются полным Event Manager

`runtime_value_events` хранит только базовые события изменения runtime value.

Это не заменяет будущий Event Manager.

Полный Event Manager будет отвечать за:

- lifecycle events;
- alarms;
- acknowledgements;
- operator actions;
- audit trail;
- system events;
- integrations.

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

## Engineering transform базовый

Engineering transform поддерживает формулу:

    engineering_value = raw_value * scale + offset

Пока нет:

- сложных формул;
- enum mapping;
- lookup tables;
- script-based transforms;
- unit conversion catalog;
- calibration curves.

## Runtime value conversion базовый

Поддерживаются простые преобразования между:

- boolean;
- numeric;
- string;
- enum string;
- json string.

Пока нет:

- строгой валидации диапазонов;
- защиты от overflow;
- культуры форматирования;
- сложного JSON parsing;
- enum dictionary.

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
- runtime events.

Но пока нет DTO для:

- аварий;
- событий Event Manager;
- истории;
- команд;
- пользователей;
- дашбордов;
- protocol diagnostics;
- historian queries.

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
- `database/migrations/0005_runtime_values.sql`.

Они не выполняются автоматически.

## Нет механизма применения миграций

Пока отсутствует:

- migration runner;
- версия схемы БД;
- rollback;
- seed-данные;
- проверка совместимости схемы.

## Нет истории тегов

Таблицы runtime values не являются историей тегов.

История тегов будет реализована позже в Historian sprint.

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

# Ограничения истории

Пока нет:

- Historian;
- буфера записи;
- batch writer;
- политик архивирования;
- TimescaleDB-интеграции;
- History API.

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

После Sprint 006 проект имеет хороший фундамент объектной модели, модели устройств, модели тегов, протокольного слоя, polling foundation и runtime values foundation.

Но Dispatcher еще не является рабочей диспетчерской системой.

Следующий важный этап:

    Sprint 007 — Historian Foundation