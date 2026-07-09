# Dispatcher.Frontend

## Назначение

`Dispatcher.Frontend` — frontend-приложение Dispatcher.

На текущем этапе это Blazor WebAssembly foundation для будущего operator UI.

## Статус

Foundation.

Приложение пока не подключено к реальному backend HTTP API.

## Текущие возможности

Реализовано:

- Blazor WebAssembly project foundation;
- MudBlazor foundation;
- базовый operator shell;
- AppBar;
- sidebar navigation;
- Drawer;
- overview page;
- System page;
- Runtime page;
- Events page;
- Alarms page;
- backend modules placeholder;
- gateway summary placeholder;
- API client placeholder;
- runtime values placeholder;
- event journal placeholder;
- active alarms placeholder;
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

## API client placeholder

Добавлен frontend service layer:

- `IDispatcherApiClient`;
- `DispatcherApiClient`;
- `DispatcherApiClientOptions`.

На текущем этапе `DispatcherApiClient` возвращает mock-data.

Mock methods:

- `GetBackendModulesAsync()`;
- `GetGatewaySummaryAsync()`;
- `GetApiRouteSummaryAsync()`;
- `GetRuntimeValuesAsync()`;
- `GetEventsAsync()`;
- `GetActiveAlarmsAsync()`.

Причина:

- backend API route/read endpoint foundation уже есть;
- реального HTTP transport пока нет;
- frontend должен получить стабильную точку расширения до появления настоящего API.

Будущая замена:

    mock DispatcherApiClient
        -> real HTTP DispatcherApiClient
        -> /api/system/health
        -> /api/system/modules
        -> /api/runtime/values
        -> /api/events
        -> /api/alarms/active

## Models

Добавлены frontend models:

- `BackendModuleViewModel`;
- `GatewaySummaryViewModel`;
- `ApiRouteSummaryViewModel`;
- `RuntimeValueViewModel`;
- `EventRecordViewModel`;
- `ActiveAlarmViewModel`.

## Что пока не реализовано

Пока нет:

- real HTTP API calls;
- realtime client;
- authentication;
- authorization;
- real data loading;
- charts;
- mimic diagrams;
- dashboards;
- alarm acknowledgement action;
- command execution.

## Будущая цель

Frontend должен стать минимальным operator workspace.

Планируемые направления:

- real system/modules page;
- object tree page;
- devices page;
- tags page;
- runtime values page;
- event journal page;
- active alarms page;
- real API client;
- realtime client;
- MVP stabilization.

## Зависимости

Frontend использует:

- Blazor WebAssembly;
- MudBlazor.

## Запуск

Из корня репозитория:

    dotnet run --project frontend/Dispatcher.Frontend/Dispatcher.Frontend.csproj

После запуска приложение будет доступно по адресу, который выведет dotnet CLI.

Фактический адрес нужно смотреть в выводе команды `dotnet run`.