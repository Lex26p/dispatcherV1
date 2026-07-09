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

- backend API integration;
- realtime integration;
- MudBlazor;
- authentication;
- real data;
- dashboards;
- mimic diagrams;
- alarm operations;
- command execution.