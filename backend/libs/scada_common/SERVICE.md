# scada_common

## Назначение

`scada_common` — библиотека действительно общих типов и утилит Dispatcher.

Этот модуль должен быть маленьким и стабильным. Его задача — хранить только то, что нужно почти всем backend-модулям.

## Границы ответственности

Модуль отвечает за:

- базовые типы;
- информацию о версии;
- общие ошибки;
- общие идентификаторы;
- timestamp-типы;
- correlation id;
- результат выполнения операций;
- небольшие утилиты без бизнес-логики.

## Что модуль не должен делать

`scada_common` не должен содержать:

- бизнес-логику SCADA;
- модели устройств;
- модели тегов;
- модели аварий;
- работу с БД;
- HTTP/WebSocket;
- логику опроса;
- настройки приложения.

## Основные файлы

- `include/scada_common/version.h`
- `include/scada_common/error.h`
- `include/scada_common/result.h`
- `include/scada_common/id_types.h`
- `include/scada_common/timestamp.h`
- `src/version.cpp`
- `src/error.cpp`
- `src/id_types.cpp`
- `src/timestamp.cpp`

## Зависимости

На текущем шаге внешних зависимостей нет.

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_common`.
- Добавлен `VersionInfo`.
- Добавлена функция `get_version_info()`.
- Добавлен `ErrorCode`.
- Добавлен `Error`.
- Добавлен `Result<T>`.
- Добавлен `Result<void>`.
- Добавлен `Uuid`.
- Добавлен `CorrelationId`.
- Добавлен `Timestamp`.
- Добавлена функция `now_utc()`.
- Добавлена функция `to_iso8601_utc()`.

### Делаем сейчас

- Используем базовые common-типы в `dispatcher_server`.

### Нужно доделать

- Добавить генерацию настоящих UUID.
- Добавить генерацию correlation_id для каждого запроса.
- Добавить более строгую валидацию UUID.
- Добавить clock abstraction для тестов.
- Добавить unit-тесты для `Result`, `Error`, `Uuid`, `CorrelationId`, `Timestamp`.

### Расширить позже

- Добавить безопасные string/id helpers.
- Добавить common validation helpers.
- Добавить common serialization helpers, если они действительно будут общими.
- Добавить compile-time helpers для enum/string mapping, если понадобится.

### Не входит в MVP

- Большая utility-библиотека общего назначения.
- Бизнес-логика любых модулей.
