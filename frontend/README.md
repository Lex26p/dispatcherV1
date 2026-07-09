# Dispatcher Frontend

## Назначение

Каталог `frontend` содержит frontend-часть Dispatcher.

Основной проект:

- `Dispatcher.Frontend`.

## Текущий статус

Frontend находится на этапе foundation.

Создано Blazor WebAssembly приложение с минимальным operator UI shell.

## Структура

- `Dispatcher.Frontend` — Blazor WebAssembly приложение.
- `Dispatcher.Frontend.slnx` — отдельная solution для frontend.

## UI foundation

Frontend использует:

- Blazor WebAssembly;
- MudBlazor.

## API client foundation

Добавлен placeholder API client:

- `IDispatcherApiClient`;
- `DispatcherApiClient`;
- `DispatcherApiClientOptions`.

На текущем этапе API client возвращает mock-data.

Реальные HTTP-запросы будут добавлены после реализации backend HTTP transport и API handlers.

## Запуск

Из корня репозитория:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

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
- command execution.