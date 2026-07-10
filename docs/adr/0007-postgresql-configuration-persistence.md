# ADR 0007 — PostgreSQL Configuration Persistence

## Статус

Accepted

## Дата

2026-07-10

## Контекст

После Sprint 015 Dispatcher имеет стабильный read-only configuration vertical slice:

```
domain
    ->
IConfigurationReadService
    ->
HTTP endpoints
    ->
ConfigurationHttpApiClient
    ->
Objects, Devices and Tags pages
```

Текущая implementation:

```
DevelopmentConfigurationReadService
```

использует deterministic in-process snapshot.

Snapshot подходит для development и contract validation, но не обеспечивает:

* сохранение данных;
* restart persistence;
* database constraints;
* migration history;
* production configuration loading;
* shared configuration между process instances.

В repository уже существуют draft SQL migrations:

```
0001_object_model.sql
0002_device_model.sql
0003_tag_model.sql
0004_polling_model.sql
0005_runtime_values.sql
0006_tag_history.sql
0007_events_alarms.sql
```

Они были созданы до появления application migration runner.

Draft migrations имеют несколько несовместимостей с текущей архитектурой:

* configuration IDs объявлены как PostgreSQL UUID;
* domain `EntityId` хранит string;
* development API использует human-readable stable IDs;
* SQL files содержат собственные `BEGIN` и `COMMIT`;
* отсутствуют migration history и checksum verification;
* отсутствует application-controlled execution;
* migrations еще не считаются immutable applied migrations.

Необходимо подключить PostgreSQL, не меняя существующие HTTP и frontend contracts.

## Решение

### 1. Создать отдельный persistence module

Создается module:

```
scada_persistence
```

Он является infrastructure layer и владеет:

* PostgreSQL client integration;
* database options;
* migrations;
* SQL execution;
* row mapping;
* database error conversion;
* PostgreSQL configuration read provider.

Persistence concerns не размещаются:

* в domain modules;
* в `scada_api`;
* в `scada_http`;
* во frontend;
* непосредственно в `dispatcher_server`.

### 2. Использовать libpqxx

PostgreSQL client implementation использует:

```
libpqxx
```

Dependency является private для `scada_persistence`.

Public headers других Dispatcher modules не содержат:

```
pqxx types
libpq types
```

Drogon ORM не используется.

Причины:

* persistence не должна зависеть от HTTP framework;
* connection и transaction model должны быть изолированы;
* SQL projections нужны напрямую для существующих read queries;
* dependency direction остается явным.

### 3. Реализовать IConfigurationReadService напрямую

Создается:

```
PostgresConfigurationReadService
```

Он реализует существующий:

```
IConfigurationReadService
```

Existing domain repository interfaces не изменяются и не используются как обязательный промежуточный слой.

Причины:

* repositories ориентированы на будущий CRUD;
* read queries имеют filtering и pagination semantics;
* загрузка всех entities для каждого HTTP request неприемлема;
* HTTP и frontend contracts уже стабилизированы на read-service boundary.

### 4. Явно выбирать provider

Используется option:

```
DISPATCHER_CONFIGURATION_PROVIDER
```

Значения:

```
development
postgres
```

Неизвестное значение является startup configuration error.

Запрещен автоматический fallback:

```
postgres
    ->
development
```

При PostgreSQL failure startup завершается с безопасной ошибкой.

### 5. Хранить domain identifiers как TEXT

Configuration tables используют PostgreSQL:

```
TEXT
```

для Object, Device и Tag identifiers.

Причины:

* Dispatcher `EntityId` является string value type;
* текущие identifiers не обязаны быть UUID;
* существующий API возвращает stable string IDs;
* development и PostgreSQL providers должны иметь contract parity;
* скрытое преобразование slug-to-UUID усложнило бы mapping;
* storage type не должен менять domain identity semantics.

Обязательны:

* non-empty checks;
* primary keys;
* matching foreign-key types;
* unique code constraints.

### 6. Выполнить одноразовую baseline-нормализацию drafts

Migrations `0001`–`0007` еще не применяются runner и имеют status draft.

До первого controlled application разрешено:

* исправить identifier types;
* удалить embedded transaction statements;
* устранить conflicts с текущим domain contract;
* исправить indexes и checks;
* обновить comments;
* проверить execution на пустой PostgreSQL database.

После первого успешного controlled application:

```
migrations immutable
```

Любая дальнейшая schema evolution выполняется новой migration.

### 7. Migration runner владеет transaction

Каждая migration выполняется в отдельной transaction.

Migration SQL files не содержат:

```
BEGIN
COMMIT
ROLLBACK
```

Runner выполняет:

```
begin transaction
execute migration
insert history record
commit
```

При exception:

```
rollback
```

Такой подход задает одну однозначную transaction boundary.

### 8. Использовать migration history и checksum

Создается table:

```
dispatcher_schema_migrations
```

Она хранит:

```
version
name
checksum
applied_at
execution_time_ms
```

Checksum algorithm:

```
SHA-256
```

Applied migration сверяется с текущим file content.

Checksum mismatch останавливает startup в PostgreSQL mode.

Runner не обновляет checksum автоматически.

### 9. Защитить migrations advisory lock

Перед validation и apply runner получает PostgreSQL advisory lock.

Это предотвращает одновременное изменение schema несколькими Dispatcher processes.

Lock принадлежит migration database session и освобождается после завершения session или явного unlock.

### 10. Использовать существующую numbering sequence

Сохраняется формат:

```
NNNN_name.sql
```

Существующие versions `0001`–`0007` не перенумеровываются.

Development seed получает следующую version:

```
0008_development_configuration_seed.sql
```

Migration history table создается runner bootstrap logic, а не migration, потому что она требуется до записи первой applied migration.

### 11. Разделить migration validation и apply

Поддерживаются operations:

```
validate
apply
```

`validate` не изменяет database.

`apply` сначала выполняет полную validation.

Production auto-apply policy не включается неявно.

Она должна задаваться explicit application option.

### 12. Использовать safe database configuration

Основной Dispatcher option:

```
DISPATCHER_DATABASE_URL
```

Также допускаются стандартные libpq environment и service configuration mechanisms.

Password и connection secrets не должны попадать:

* в repository;
* в committed settings;
* в logs;
* в API errors;
* в frontend;
* в test snapshots.

Создается centralized connection-description redaction.

### 13. Сохранить HTTP contracts

Не изменяются:

```
GET /api/objects
GET /api/objects/tree
GET /api/devices
GET /api/tags
```

Не изменяются:

* query names;
* JSON fields;
* enum strings;
* null semantics;
* pagination semantics;
* ordering;
* error envelope;
* frontend DTO;
* frontend pages.

### 14. Ввести provider contract parity tests

Одинаковые behavior expectations применяются к:

```
DevelopmentConfigurationReadService
PostgresConfigurationReadService
```

Проверяются:

* filters;
* pagination;
* ordering;
* tree;
* nullable relationships;
* disabled configuration;
* enum mappings;
* empty results.

## Dependency direction

Допустимо:

```
dispatcher_server
    ->
scada_persistence

dispatcher_server
    ->
scada_app

scada_persistence
    ->
scada_api

scada_persistence
    ->
domain modules

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
scada_persistence concrete types

scada_persistence
    ->
scada_http

scada_persistence
    ->
frontend
```

## Отклоненные варианты

### Изменить существующие domain repository interfaces

Отклонено для Sprint 016.

Причины:

* scope read persistence смешался бы с CRUD;
* потребовалось бы согласовать write semantics;
* interfaces могли бы стать зависимыми от pagination;
* HTTP read requirements начали бы управлять domain repository design.

### Использовать repositories и фильтровать в памяти

Отклонено.

Причины:

* загружается вся configuration;
* pagination выполняется слишком поздно;
* database indexes не используются эффективно;
* поведение плохо масштабируется.

### Использовать Drogon ORM

Отклонено.

Причины:

* persistence стала бы зависеть от HTTP framework;
* framework boundary Sprint 012 был бы нарушен;
* замена transport усложнила бы database layer.

### Использовать UUID database identifiers

Отклонено для текущей configuration schema.

Причины:

* domain IDs являются strings;
* API уже использует non-UUID IDs;
* потребовался бы дополнительный mapping layer;
* provider parity стала бы сложнее;
* существующие stable development IDs были бы потеряны.

UUID может использоваться будущими сущностями, если их domain contract явно требует UUID.

### Оставить BEGIN и COMMIT внутри SQL files

Отклонено.

Причины:

* transaction ownership становилась неоднозначной;
* runner не мог бы гарантировать единый rollback behavior;
* migration history insert мог бы оказаться вне той же transaction;
* nested transaction behavior усложнил бы implementation.

### Silent fallback на development provider

Отклонено.

Причины:

* database outage мог бы остаться незаметным;
* production показывал бы тестовые данные;
* monitoring получал бы ложный healthy state;
* operator мог бы работать с неверной configuration.

### Автоматически обновлять migration checksum

Отклонено.

Причины:

* скрывает изменение applied migration;
* разрушает reproducibility;
* не позволяет обнаружить drift.

### Перенумеровать существующие migrations

Отклонено.

Причины:

* номера уже документированы;
* dependencies уже зафиксированы;
* достаточно одноразовой normalization до baseline.

## Последствия

### Положительные

* storage меняется без изменения HTTP;
* frontend продолжает работать без contract changes;
* database layer изолирован;
* development mode остается легким;
* production-like mode не скрывает failures;
* migrations становятся воспроизводимыми;
* schema drift обнаруживается;
* transaction policy однозначна;
* IDs совпадают с domain;
* будущий CRUD может проектироваться отдельно.

### Отрицательные

* появляется новый infrastructure module;
* добавляется external dependency;
* CI потребуется PostgreSQL service;
* migration runner требует integration tests;
* draft migrations придется нормализовать;
* provider contract parity tests увеличат test suite;
* startup PostgreSQL mode станет строже.

### Риски

* Windows и Linux могут различаться в PostgreSQL client linking;
* legacy migrations могут содержать дополнительные domain mismatches;
* raw database errors могут раскрыть connection details;
* Object tree reconstruction может выявить invalid stored hierarchy;
* incorrect migration lock может блокировать startup;
* checksum normalization должна быть детерминированной.

## Меры контроля

* dependency добавляется отдельным шагом;
* Windows и Linux builds проверяются сразу;
* redaction покрывается unit tests;
* migration runner покрывается real PostgreSQL tests;
* migrations применяются к empty database;
* repeated apply проверяется;
* rollback проверяется intentional failing migration;
* provider parity проверяется общими fixtures;
* HTTP smoke-tests выполняются в development и postgres modes.

## Следствия для следующих спринтов

После Sprint 016 возможно отдельно планировать:

* configuration CRUD;
* engineering drafts;
* audit;
* optimistic concurrency;
* import/export;
* protocol credentials;
* production deployment configuration.

Эти функции не входят в текущее решение автоматически.
