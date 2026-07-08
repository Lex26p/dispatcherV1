# dispatcher_server

## Назначение

`dispatcher_server` — основное backend-приложение Dispatcher.

На первом этапе оно запускает все backend-модули внутри одного процесса. В будущем часть модулей может быть вынесена в отдельные сервисы, но бизнес-логика должна оставаться в соответствующих библиотеках из `backend/libs`.

## Границы ответственности

`dispatcher_server` отвечает за:

- запуск backend-приложения;
- инициализацию core-модулей;
- загрузку bootstrap-конфигурации;
- подключение модулей;
- запуск REST API;
- запуск WebSocket;
- запуск фоновых сервисов;
- graceful shutdown;
- health endpoints.

## Что модуль не должен делать

`dispatcher_server` не должен содержать бизнес-логику SCADA.

Бизнес-логика должна находиться в библиотеках:

- `scada_core`;
- `scada_objects`;
- `scada_devices`;
- `scada_tags`;
- `scada_protocols`;
- `scada_polling`;
- `scada_historian`;
- `scada_alarms`;
- `scada_events`;
- `scada_commands`;
- и других backend-модулях.

## Текущая реализация

Сейчас приложение является минимальной точкой входа и выводит стартовую информацию:

- название продукта;
- имя executable;
- версию;
- режим;
- статус запуска.

## Статус реализации

### Реализовано сейчас

- Создано backend-приложение `dispatcher_server`.
- Создан минимальный `main.cpp`.
- Приложение выводит стартовую информацию.
- Приложение подключено к корневому CMake-проекту.

### Делаем сейчас

- Формируем базовый каркас backend-приложения.

### Нужно доделать

- Добавить `ApplicationContext`.
- Добавить регистрацию модулей.
- Добавить bootstrap-конфигурацию.
- Добавить логирование.
- Добавить graceful shutdown.
- Добавить health endpoints.
- Добавить REST API.
- Добавить WebSocket.

### Расширить позже

- Поддержка запуска модулей в external service mode.
- Поддержка конфигурации через production config.
- Интеграция с systemd.
- Метрики и диагностика процесса.
- OpenTelemetry.

### Не входит в MVP

- Кластеризация.
- Hot standby.
- Kubernetes deployment.
- Docker deployment.