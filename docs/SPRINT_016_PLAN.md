# Sprint 016 Plan

## Название

Configuration Persistence Foundation

## Статус

Approved — In Progress

## Этап

Post-MVP persistence foundation.

## Диапазон шагов

Шаги 117–125.

## Цель

Заменить временный in-process configuration snapshot явно выбираемым PostgreSQL-backed read provider, сохранив существующие HTTP и frontend contracts:

```
PostgreSQL
    ->
scada_persistence
    ->
IConfigurationReadService
    ->
GET /api/objects
GET /api/objects/tree
GET /api/devices
GET /api/tags
    ->
ConfigurationHttpApiClient
    ->
Objects, Devices and Tags pages
```

## Главный результат

После Sprint 016 Dispatcher поддерживает два явно выбираемых режима configuration provider:

```
development
postgres
```

Development mode использует:

```
DevelopmentConfigurationReadService
```

PostgreSQL mode использует:

```
PostgresConfigurationReadService
```

Автоматический или скрытый fallback между providers запрещен.

## Архитектурные решения

### Новый infrastructure-модуль

Создается:

```
backend/libs/scada_persistence
```

Модуль отвечает за:

* PostgreSQL connection foundation;
* database options;
* safe connection diagnostics;
* migration discovery;
* migration validation;
* migration execution;
* migration history;
* PostgreSQL row mapping;
* PostgreSQL implementation `IConfigurationReadService`;
* database error sanitization.

### Dependency direction

Допустимо:

```
scada_persistence
    ->
scada_common

scada_persistence
    ->
scada_api

scada_persistence
    ->
scada_objects

scada_persistence
    ->
scada_devices

scada_persistence
    ->
scada_tags

scada_persistence
    ->
libpqxx
```

Недопустимо:

```
domain modules
    ->
scada_persistence

scada_api
    ->
scada_persistence

scada_http
    ->
libpqxx

scada_persistence
    ->
scada_http

scada_persistence
    ->
Drogon HTTP types

frontend
    ->
database types
```

### Configuration read boundary

PostgreSQL implementation напрямую реализует существующий interface:

```
IConfigurationReadService
```

Операции остаются неизменными:

```
read_objects
read_object_tree
read_devices
read_tags
```

Существующие HTTP endpoints и frontend client не узнают, какой provider выбран.

### Domain repositories

В Sprint 016 не изменяются:

```
IObjectRepository
IDeviceRepository
ITagRepository
```

Эти interfaces ориентированы на будущие CRUD и command workflows.

Они не используются как промежуточный слой для HTTP read queries, потому что такой подход потребовал бы загружать всю конфигурацию и выполнять filtering и pagination в памяти.

### PostgreSQL dependency

Используется:

```
libpqxx
```

Dependency является private implementation detail `scada_persistence`.

Другие модули Dispatcher не включают headers `pqxx`.

Drogon ORM не используется.

### Provider selection

Provider выбирается явно через:

```
DISPATCHER_CONFIGURATION_PROVIDER
```

Допустимые значения:

```
development
postgres
```

Пустое или неизвестное значение является configuration error.

Для local development может быть документирован явный default:

```
development
```

Production-like запуск должен явно задавать:

```
postgres
```

### Database connection configuration

Основной connection input:

```
DISPATCHER_DATABASE_URL
```

Также допускаются стандартные libpq mechanisms:

```
PGHOST
PGPORT
PGDATABASE
PGUSER
PGPASSWORD
PGSERVICE
PGSERVICEFILE
```

Connection string, password и secret environment values не выводятся:

* в startup log;
* в HTTP responses;
* в exceptions, передаваемые frontend;
* в test output;
* в committed configuration files.

### Startup behavior

Для provider:

```
development
```

PostgreSQL не требуется.

Для provider:

```
postgres
```

startup выполняет:

1. database options validation;
2. PostgreSQL connection check;
3. migration catalog validation;
4. migration state validation;
5. migrations в разрешенном режиме;
6. создание `PostgresConfigurationReadService`;
7. регистрацию существующих HTTP routes.

Если PostgreSQL недоступен или schema invalid:

```
dispatcher_server startup завершается с ошибкой
```

Запрещено:

```
postgres failure
    ->
silent development fallback
```

### HTTP compatibility

Не изменяются endpoints:

```
GET /api/objects
GET /api/objects/tree
GET /api/devices
GET /api/tags
```

Не изменяются:

* query parameter names;
* JSON property names;
* nullable relationship semantics;
* pagination metadata;
* deterministic ordering;
* error envelope;
* correlation ID behavior;
* frontend DTO;
* frontend view models;
* Razor pages.

## Identifier policy

Dispatcher domain identifiers являются строковыми identifiers.

PostgreSQL configuration tables используют:

```
TEXT
```

для:

```
object id
device id
tag id
relationship ids
```

Причины:

* сохранить существующие domain semantics;
* сохранить development IDs;
* сохранить HTTP contract parity;
* не вводить скрытое преобразование между slug и UUID;
* не связывать domain identity с PostgreSQL UUID type.

Минимальные ограничения:

```
NOT NULL
PRIMARY KEY
length(trim(id)) > 0
```

Foreign keys используют тот же `TEXT` type.

## Existing migration baseline

В repository уже существуют draft migrations:

```
0001_object_model.sql
0002_device_model.sql
0003_tag_model.sql
0004_polling_model.sql
0005_runtime_values.sql
0006_tag_history.sql
0007_events_alarms.sql
```

Они еще не управляются application migration runner и отмечены как draft.

До их первого применения допускается одноразовая baseline-нормализация.

Нормализация должна:

* сохранить существующие номера;
* сохранить последовательность dependencies;
* привести identifiers к текущей domain policy;
* убрать embedded `BEGIN` и `COMMIT`;
* привести formatting к LF;
* проверить PostgreSQL syntax;
* устранить schema/domain conflicts;
* обновить comments о migration status.

После первого успешного применения:

```
applied migrations immutable
```

Любое дальнейшее изменение schema выполняется новой migration.

## Migration naming

Используется существующий формат:

```
NNNN_name.sql
```

Примеры:

```
0001_object_model.sql
0007_events_alarms.sql
0008_development_configuration_seed.sql
```

Version извлекается из первых четырех цифр.

Требования:

* version положительная;
* version уникальна;
* filename уникален;
* gaps разрешены только явным архитектурным решением;
* migrations сортируются по version, а не filesystem order.

## Migration history

Migration runner создает служебную таблицу:

```
dispatcher_schema_migrations
```

Минимальные поля:

```
version
name
checksum
applied_at
execution_time_ms
```

Требования:

* `version` — primary key;
* `name` — уникальное имя migration;
* `checksum` — SHA-256 нормализованного содержимого;
* `applied_at` — PostgreSQL timestamp;
* `execution_time_ms` — длительность успешного применения.

## Transaction policy

Migration runner владеет transaction boundary.

Каждая migration выполняется:

```
one migration
    ->
one PostgreSQL transaction
```

При failure:

```
rollback entire migration
```

Migration SQL files после baseline normalization не содержат:

```
BEGIN
COMMIT
ROLLBACK
```

Это предотвращает конфликт между script-owned и runner-owned transactions.

## Migration locking

Перед migration validation и execution runner получает PostgreSQL advisory lock.

Lock предотвращает одновременное применение migrations несколькими процессами Dispatcher.

Lock освобождается:

* после успешного завершения;
* после failure;
* при закрытии database session.

## Checksum policy

Для непримененной migration checksum вычисляется до execution.

Для примененной migration runner сравнивает текущий checksum с сохраненным.

Несовпадение означает:

```
migration_checksum_mismatch
```

Startup в PostgreSQL mode завершается с ошибкой.

Runner не изменяет сохраненный checksum автоматически.

## Migration modes

Планируются режимы:

```
validate
apply
```

`validate`:

* проверяет catalog;
* проверяет sequence;
* проверяет duplicate versions;
* проверяет checksums;
* не изменяет schema.

`apply`:

* выполняет validation;
* применяет pending migrations;
* записывает history.

Production default auto-apply не утверждается этим спринтом.

Конкретная startup option фиксируется на шаге 118.

## Development seed

Добавляется migration:

```
0008_development_configuration_seed.sql
```

Seed воспроизводит contract dataset Sprint 015:

```
5 Objects
4 Devices
6 Tags
```

Сохраняются IDs:

```
site-main
building-admin
room-control
line-water
group-pumps

device-modbus-1
device-snmp-1
device-simulator-1
device-disabled-1

tag-temperature
tag-fan-running
tag-pump-pressure
tag-operator-setpoint
tag-system-status
tag-valve-command-disabled
```

Seed должен быть:

* deterministic;
* idempotent в рамках migration execution policy;
* без passwords;
* без tokens;
* без SNMP community strings;
* без production credentials.

## PostgreSQL Object reads

`PostgresConfigurationReadService` реализует:

```
read_objects
read_object_tree
```

Поддерживаются:

```
id
parentId
includeChildren
limit
offset
```

Требования:

* filters выполняются предсказуемо;
* ordering выполняется до pagination;
* ordering: `code`, затем `id`;
* nullable parent сохраняется;
* empty result успешен;
* tree строится из валидных rows;
* cycles и broken relationships считаются database integrity error;
* subtree semantics совпадают с development provider.

## PostgreSQL Device reads

Реализуется:

```
read_devices
```

Поддерживаются:

```
id
objectId
state
includeDisabled
limit
offset
```

Требования:

* protocol и state используют стабильные domain strings;
* nullable Object relationship сохраняется;
* disabled filtering совпадает с development provider;
* ordering выполняется до pagination;
* `runtimeEnabled` вычисляется по существующим domain rules;
* connection response не содержит secrets;
* IPv4, IPv6 и Simulator connection поддерживаются.

## PostgreSQL Tag reads

Реализуется:

```
read_tags
```

Поддерживаются:

```
id
objectId
deviceId
includeDisabled
limit
offset
```

Требования:

* nullable relationships сохраняются;
* enabled filtering совпадает с development provider;
* type, value type и archive policy используют domain strings;
* engineering metadata сохраняется;
* ordering выполняется до pagination;
* current value, quality, history и alarm state не входят в configuration read.

## Error policy

Database implementation не передает наружу raw PostgreSQL error text.

Внутренне ошибки классифицируются минимум как:

```
configuration_error
connection_error
migration_error
migration_checksum_mismatch
schema_error
query_error
mapping_error
```

HTTP contract на Sprint 016 не расширяется database-specific fields.

Startup errors выводят:

* безопасный error category;
* краткое сообщение;
* operation name;
* correlation or startup context, если применимо.

Не выводятся:

* password;
* полный connection string;
* SQL с secret values;
* raw environment values.

## Contract parity

Одинаковые provider contract tests применяются к:

```
DevelopmentConfigurationReadService
PostgresConfigurationReadService
```

Оба providers обязаны возвращать одинаковые:

* filters;
* ordering;
* pagination;
* nullable relationships;
* tree semantics;
* enum strings;
* disabled filtering;
* empty-result semantics.

## Шаги Sprint 016

### Шаг 117 — Sprint plan and persistence ADR

Создать:

```
docs/SPRINT_016_PLAN.md
docs/adr/0007-postgresql-configuration-persistence.md
```

Зафиксировать:

* boundaries;
* identifier policy;
* libpqxx decision;
* provider selection;
* no-fallback policy;
* migration baseline;
* transaction ownership;
* checksum policy;
* secret policy;
* testing strategy.

Код и dependencies не менять.

### Шаг 118 — Persistence module and database options

Создать:

```
scada_persistence
```

Добавить:

* libpqxx dependency;
* provider mode;
* database options;
* environment parsing;
* options validation;
* safe connection description;
* secret redaction;
* module metadata;
* unit tests.

Сразу подтвердить Windows и Linux build.

### Шаг 119 — Migration catalog and runner

Реализовать:

* catalog discovery;
* filename parsing;
* version ordering;
* duplicate detection;
* SHA-256 checksum;
* migration history table;
* validation mode;
* apply mode;
* transaction per migration;
* advisory lock;
* sanitized migration errors;
* unit and PostgreSQL integration tests.

### Шаг 120 — Baseline schema normalization and seed

Одноразово нормализовать draft migrations `0001`–`0007`.

Добавить:

```
0008_development_configuration_seed.sql
```

Подтвердить:

* TEXT identifiers;
* foreign keys;
* enum checks;
* indexes;
* no embedded transaction statements;
* schema applies to empty database;
* repeated runner execution is safe;
* seed reproduces 5/4/6 dataset.

### Шаг 121 — PostgreSQL Object reads

Реализовать:

```
read_objects
read_object_tree
```

Добавить provider parity tests и PostgreSQL integration tests.

### Шаг 122 — PostgreSQL Device reads

Реализовать:

```
read_devices
```

Добавить filters, ordering, pagination, connection mapping и secret-contract tests.

### Шаг 123 — PostgreSQL Tag reads

Реализовать:

```
read_tags
```

Добавить filters, ordering, pagination, relationship и engineering mapping tests.

### Шаг 124 — Composition root and explicit provider selection

Подключить provider selection в:

```
dispatcher_server
```

Реализовать:

* development provider selection;
* postgres provider selection;
* startup validation;
* migration mode handling;
* safe diagnostics;
* no silent fallback.

Существующий System health JSON contract не менять.

### Шаг 125 — Integration, CI and Sprint closure

Добавить:

* PostgreSQL integration test environment;
* migration tests;
* postgres-mode smoke-test;
* development-mode regression;
* CI PostgreSQL service;
* Debug и Release checks;
* status documentation;
* Sprint summary.

Закрыть Sprint только после прохождения всех acceptance criteria.

## Testing strategy

### Unit tests без PostgreSQL

Покрывают:

* provider mode parsing;
* environment option parsing;
* option validation;
* redaction;
* safe connection description;
* migration filename parsing;
* migration ordering;
* duplicate versions;
* checksum calculation;
* checksum mismatch;
* row mapping;
* enum mapping;
* database error sanitization.

### PostgreSQL integration tests

Покрывают:

* connection;
* empty database migration;
* migration history creation;
* repeated migration execution;
* pending migrations;
* checksum mismatch;
* transaction rollback;
* advisory lock;
* Object reads;
* Object tree;
* Device reads;
* Tag reads;
* filters;
* pagination;
* disabled configuration;
* nullable relationships;
* deterministic ordering;
* restart persistence;
* foreign-key constraints;
* unique-code constraints.

### Existing regression

Обязательны:

* 78 existing CTest tests;
* новые persistence tests;
* frontend xUnit tests;
* development provider smoke-test;
* browser Objects checks;
* browser Devices checks;
* browser Tags checks;
* Windows CI;
* Linux Debug CI;
* Linux Release CI;
* LF validation.

## Acceptance criteria

Sprint 016 завершен, когда:

* `scada_persistence` существует как отдельный module;
* libpqxx изолирован внутри persistence layer;
* Windows build проходит;
* Linux Debug и Release builds проходят;
* provider выбирается явно;
* неизвестный provider отклоняется;
* PostgreSQL failure не вызывает silent fallback;
* secrets не выводятся;
* migrations применяются к пустой database;
* повторный migration run безопасен;
* checksum mismatch обнаруживается;
* transaction rollback подтвержден;
* concurrent migration execution защищено lock;
* draft migrations нормализованы до baseline;
* database identifiers совместимы с string `EntityId`;
* seed воспроизводит development snapshot;
* PostgreSQL Object contracts совпадают;
* PostgreSQL Object tree совпадает;
* PostgreSQL Device contracts совпадают;
* PostgreSQL Tag contracts совпадают;
* существующие HTTP contracts не изменились;
* frontend не потребовал contract changes;
* development mode продолжает работать без PostgreSQL;
* postgres-mode integration smoke-test проходит;
* все tests проходят;
* CI проходит;
* warnings отсутствуют;
* status documentation актуализирована.

## Out of scope

В Sprint 016 не входят:

* Object CRUD;
* Device CRUD;
* Tag CRUD;
* write HTTP endpoints;
* draft editing workflows;
* optimistic concurrency;
* audit trail;
* production secret vault;
* protocol credentials;
* current Tag values;
* polling persistence;
* historian persistence;
* TimescaleDB;
* events persistence activation;
* alarms persistence activation;
* WebSocket;
* SSE;
* authentication;
* authorization;
* connection-pool production tuning;
* high availability;
* backup;
* restore;
* deployment packaging.

## Риски

### Legacy migration mismatch

Draft migrations были созданы до появления application migration runner и текущих read contracts.

Мера:

```
one-time baseline normalization before first apply
```

### Identifier mismatch

Domain использует string identifiers, а draft schema использовала UUID.

Мера:

```
TEXT identifiers in configuration schema
```

### Secret leakage

Raw connection и PostgreSQL errors могут содержать sensitive information.

Мера:

```
centralized redaction and sanitized diagnostics
```

### Cross-platform dependency

PostgreSQL client dependency должна одинаково собираться на Windows и Linux.

Мера:

```
dependency integration выполняется на шаге 118 отдельно от остальной реализации
```

### Hidden fallback

Silent fallback способен скрыть production database failure.

Мера:

```
explicit provider selection and startup failure
```

### Migration mutability

Редактирование applied migrations разрушает воспроизводимость schema.

Мера:

```
checksum enforcement and immutable applied files
```

## Sprint accounting

Текущий утвержденный Sprint:

```
Sprint 016
```

После выполнения шага 117 остается:

```
8 шагов
```

После закрытия Sprint 016 останется утвержденных Sprint plans:

```
0
```

Дальнейшие этапы `VERSION_1_ROADMAP.txt` не считаются утвержденными спринтами до создания отдельных Sprint plans.
