# scada_core

## Назначение

`scada_core` — базовое ядро backend-приложения Dispatcher.

Модуль должен содержать каркас приложения, lifecycle модулей, базовые интерфейсы ядра и общие механизмы, которые связывают backend-модули между собой.

## Границы ответственности

Модуль отвечает за:

- `ApplicationContext`;
- lifecycle модулей;
- регистрацию модулей;
- health check интерфейсы;
- settings provider интерфейсы;
- event bus интерфейсы;
- базовые service abstractions;
- системный статус приложения.

## Что модуль не должен делать

`scada_core` не должен содержать:

- модели тегов;
- модели устройств;
- опрос оборудования;
- работу протоколов;
- бизнес-логику аварий;
- UI-логику;
- конкретную работу с PostgreSQL.

## Основные файлы

- `include/scada_core/module_info.h`
- `src/module_info.cpp`

## Зависимости

- `scada_common`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_core`.
- Добавлен `ModuleStatus`.
- Добавлен `ModuleInfo`.
- Добавлена функция `get_initial_module_list()`.

### Делаем сейчас

- Подключаем модуль к `dispatcher_server`.

### Нужно доделать

- Добавить `ApplicationContext`.
- Добавить `IModule`.
- Добавить `IHealthCheck`.
- Добавить `ISettingsProvider`.
- Добавить `IEventBus`.
- Добавить lifecycle запуска и остановки модулей.

### Расширить позже

- Добавить поддержку embedded/external mode.
- Добавить module dependency validation.
- Добавить graceful shutdown.
- Добавить startup checks.
- Добавить runtime diagnostics.

### Не входит в MVP

- Полноценный distributed service registry.
- Кластеризация.
- Dynamic module loading.