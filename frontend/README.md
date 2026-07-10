# Dispatcher Frontend

## Назначение

Каталог `frontend` содержит frontend-часть Dispatcher.

Основной проект:

- `Dispatcher.Frontend`.

## Текущий статус

Frontend находится на этапе post-MVP foundation.

Создано Blazor WebAssembly приложение с минимальным операторским UI shell.

## Язык интерфейса

Основной язык интерфейса:

    русский

Английский язык будет добавлен позже как локализация.

На текущем этапе полноценный localization engine не используется.

## Структура

- `Dispatcher.Frontend` — Blazor WebAssembly приложение.
- `Dispatcher.Frontend.slnx` — отдельная solution для frontend.

## UI foundation

Frontend использует:

- Blazor WebAssembly;
- MudBlazor.

Реализовано:

- AppBar;
- Drawer/sidebar;
- navigation;
- improved loading screen;
- русскоязычные страницы;
- UI state panel;
- demo-data предупреждения;
- not implemented состояния.

## API client foundation

Добавлен placeholder API client:

- `IDispatcherApiClient`;
- `DispatcherApiClient`;
- `DispatcherApiClientOptions`.

На текущем этапе API client возвращает demo-data.

Реальные HTTP-запросы будут добавлены после реализации backend HTTP transport и API handlers.

## Placeholder screens

Сейчас frontend содержит operator placeholders:

- System/modules;
- Runtime values;
- Event journal;
- Active alarms;
- Objects;
- Devices;
- Tags.

Пользовательские тексты интерфейса отображаются на русском языке.

## Запуск

Из корня репозитория:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

Фактический адрес нужно смотреть в выводе команды `dotnet run`.

Пример:

    http://localhost:5030

## Сборка

Из корня репозитория:

    dotnet build frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

## Проверка solution

Из корня репозитория:

    dotnet build frontend/Dispatcher.Frontend.slnx

## Ограничения

Пока нет:

- real backend API integration;
- realtime integration;
- authentication;
- real data;
- dashboards;
- mimic diagrams;
- alarm operations;
- command execution;
- localization engine.
