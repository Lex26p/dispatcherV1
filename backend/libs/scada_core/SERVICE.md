# scada_core

## Назначение

`scada_core` — базовое ядро backend-приложения Dispatcher.

Модуль содержит каркас приложения, lifecycle модулей, базовые интерфейсы ядра и общие механизмы, которые связывают backend-модули между собой.

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

- `include/scada_core/application_context.h`
- `include/scada_core/module.h`
- `include/scada_core/module_info.h`
- `include/scada_core/health_check.h`
- `include/scada_core/settings_provider.h`
- `include/scada_core/event_bus.h`
- `src/application_context.cpp`
- `src/module_info.cpp`
- `src/event_bus.cpp`

## Зависимости

- `scada_common`

## Статус реализации

### Реализовано сейчас

- Создан модуль `scada_core`.
- Добавлен `ModuleStatus`.
- Добавлен `ModuleInfo`.
- Добавлена функция `get_initial_module_list()`.
- Добавлен `ApplicationContext`.
- Добавлена регистрация модулей в `ApplicationContext`.
- Добавлен запуск `ApplicationContext`.
- Добавлен интерфейс `IModule`.
- Добавлен интерфейс `IHealthCheck`.
- Добавлен интерфейс `ISettingsProvider`.
- Добавлен интерфейс `IEventBus`.
- Добавлен базовый `CoreEvent`.

### Делаем сейчас

- Используем `ApplicationContext` в `dispatcher_server`.

### Нужно доделать

- Добавить полноценный lifecycle запуска и остановки модулей.
- Добавить реальные реализации модулей.
- Добавить `ModuleRegistry`.
- Добавить startup checks.
- Добавить graceful shutdown.
- Добавить базовую диагностику.
- Добавить реальные health checks.
- Добавить runtime settings provider.
- Добавить InMemoryEventBus.

### Расширить позже

- Добавить поддержку embedded/external mode.
- Добавить module dependency validation.
- Добавить runtime diagnostics.
- Добавить OpenTelemetry hooks.
- Добавить интеграцию с внешним event bus.

### Не входит в MVP

- Полноценный distributed service registry.
- Кластеризация.
- Dynamic module loading.
