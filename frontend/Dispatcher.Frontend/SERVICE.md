# Dispatcher.Frontend

## Назначение

`Dispatcher.Frontend` — frontend-приложение Dispatcher.

На текущем этапе это Blazor WebAssembly foundation для будущего operator UI.

## Статус

Foundation.

Приложение пока не подключено к backend API.

## Текущие возможности

Реализовано:

- Blazor WebAssembly project foundation;
- базовый shell;
- sidebar navigation;
- topbar;
- overview page;
- placeholder pages.

## Страницы

Текущие страницы:

- `/`;
- `/system`;
- `/objects`;
- `/devices`;
- `/tags`;
- `/runtime`;
- `/events`;
- `/alarms`;
- `/not-found`.

## Что пока не реализовано

Пока нет:

- MudBlazor;
- API client;
- realtime client;
- authentication;
- authorization;
- real data loading;
- charts;
- mimic diagrams;
- dashboards;
- alarm acknowledgement;
- command execution.

## Будущая цель

Frontend должен стать минимальным operator workspace.

Планируемые направления:

- system/modules page;
- object tree page;
- devices page;
- tags page;
- runtime values page;
- event journal page;
- active alarms page;
- API client;
- realtime client;
- MudBlazor layout;
- MVP stabilization.

## Зависимости

На текущем этапе frontend зависит только от стандартного Blazor WebAssembly шаблона.

MudBlazor будет добавлен отдельным шагом.

## Запуск

Из корня репозитория:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

После запуска приложение будет доступно по адресу, который выведет dotnet CLI.

Фактический адрес нужно смотреть в выводе команды `dotnet run`.