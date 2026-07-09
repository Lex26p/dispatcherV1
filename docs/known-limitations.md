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

SQL-миграции уже начали появляться как черновики.

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

Но пока нет реализаций:

- in-memory;
- PostgreSQL;
- mock;
- file-based.

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

## Протоколы пока частично являются контрактами

В `DeviceProtocol` заложены:

- `ModbusTcp`;
- `Snmp`;
- `Simulator`;
- `Custom`.

В `scada_protocols` уже есть общий контракт `IProtocolDriver`.

Но пока реализован только локальный simulator driver.

Нет реализации:

- Modbus TCP;
- SNMP v2c;
- SNMP v3;
- real TCP exchange;
- real UDP exchange.

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

- runtime TagValueStore;
- записи значений из polling;
- Historian;
- tag history;
- alarm rules;
- command execution;
- API;
- UI;
- PostgreSQL-реализации;
- unit-тестов.

## Текущие значения тегов пока являются только моделью

`TagCurrentValue` уже содержит:

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

Но пока нет сервиса, который:

- хранит текущие значения в памяти;
- обновляет значения из polling;
- обновляет `last_good_value`;
- отправляет события изменения;
- пишет историю;
- инициирует аварии.

## Инженерное преобразование пока не выполняется

В `Tag` есть:

- `scale`;
- `offset`;
- `engineering_unit`.

Но пока нет сервиса, который фактически выполняет:

    engineering_value = raw_value * scale + offset

Это будет реализовано позже в runtime values слое.

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
- device diagnostics update;
- runtime value updates.

## PollingScheduler пока синхронный

`PollingScheduler` умеет выполнить одну polling group один раз.

Он пока не:

- запускается сам;
- работает в фоне;
- планирует следующие опросы;
- обновляет состояние задач в repository;
- пишет значения в `TagValueStore`;
- отправляет события в EventBus.

## Polling не пишет TagCurrentValue

После Sprint 005 polling возвращает результат выполнения.

Но он не пишет:

- `TagCurrentValue`;
- `last_good_value`;
- `change_counter`;
- историю;
- аварии.

Это будет задачей Sprint 006 и следующих спринтов.

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
- polling.

Но пока нет DTO для:

- аварий;
- событий;
- истории;
- команд;
- пользователей;
- дашбордов;
- protocol diagnostics;
- runtime value events.

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
- `database/migrations/0004_polling_model.sql`.

Они не выполняются автоматически.

## Нет механизма применения миграций

Пока отсутствует:

- migration runner;
- версия схемы БД;
- rollback;
- seed-данные;
- проверка совместимости схемы.

## Нет истории тегов

Таблица `tag_current_values` хранит только snapshot текущего значения.

Она не является историей тегов.

История тегов будет реализована позже в Historian sprint.

## Нет истории polling execution

Миграция polling хранит только configuration:

- `polling_groups`;
- `polling_tasks`.

Она не хранит:

- polling execution history;
- polling statistics;
- latency history;
- error history;
- protocol diagnostics.

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
- карточки polling group.

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

После Sprint 005 проект имеет хороший фундамент объектной модели, модели устройств, модели тегов, протокольного слоя и polling foundation.

Но Dispatcher еще не является рабочей диспетчерской системой.

Следующий важный этап:

    Sprint 006 — Runtime Values and Data Engine