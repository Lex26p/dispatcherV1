# Sprint 013 Summary

## Название

Frontend Real API Client Integration

## Статус

Completed

## Шаги

91–98.

## Цель

Подключить Blazor WebAssembly frontend Dispatcher к реальному backend HTTP API и выполнить первый рабочий вертикальный срез:

    frontend page
        -> typed frontend API client
        -> HTTP
        -> Drogon transport
        -> Dispatcher endpoint
        -> JSON response
        -> frontend state

## Реализованный вертикальный срез

Страница:

    /system

Backend endpoints:

    GET /api/system/health
    GET /api/system/modules

Frontend отображает:

- состояние backend;
- версию;
- режим запуска;
- API transport;
- зарегистрированные backend-модули;
- количество работающих модулей;
- время последнего обновления.

## Frontend API architecture

Добавлен интерфейс:

    ISystemApiClient

Реализации:

    SystemHttpApiClient
    MockSystemApiClient

Real и mock transport разделены.

`SystemHttpApiClient` использует настроенный через dependency injection `HttpClient`.

`MockSystemApiClient` включается только через configuration.

Legacy `IDispatcherApiClient` временно остается для страниц, которым еще не предоставлены реальные backend endpoints.

## Frontend configuration

Добавлена секция:

    DispatcherApi

Параметры:

    BaseUrl
    ApiBasePath
    UseMockData
    RequestTimeoutSeconds

Development backend:

    http://127.0.0.1:8080

## DTO foundation

Добавлены:

    SystemHealthApiResponse
    SystemModulesApiResponse
    SystemModuleApiDto
    BackendHealthViewModel

Transport response отделен от UI view model.

## Error handling

Добавлены:

    DispatcherApiException
    DispatcherApiErrorKind

Поддерживаются ошибки:

- connection;
- timeout;
- HTTP status;
- invalid JSON;
- unsupported response;
- invalid configuration;
- cancellation.

Frontend показывает понятные русскоязычные состояния вместо необработанных transport exceptions.

## System page states

Реализованы:

- loading;
- connected;
- backend unhealthy;
- no connection;
- timeout;
- API error;
- invalid response;
- empty modules;
- mock mode.

Кнопка «Обновить» позволяет повторить загрузку.

При новом запросе предыдущий запрос отменяется.

## CORS

Backend development CORS foundation добавлен в:

    scada_http

OPTIONS preflight перенесен в:

    drogon::app().registerPreRoutingAdvice

Это исключило конфликт со встроенной обработкой OPTIONS в Drogon.

Проверены:

- разрешенный frontend origin;
- GET;
- OPTIONS;
- Content-Type;
- неизвестный origin.

## Integration testing

Добавлен:

    scripts/test-local-integration.ps1

Проверяются:

- `/api/system/health`;
- `/api/system/modules`;
- структура модулей;
- CORS GET response;
- OPTIONS preflight;
- неизвестный origin.

Итог проверки:

    SMOKE TEST PASSED

Также вручную проверены:

- работающий backend;
- остановленный backend;
- повторный запуск backend;
- восстановление соединения без перезапуска frontend.

## Документация

Обновлены:

    frontend/README.md
    frontend/Dispatcher.Frontend/SERVICE.md
    docs/SPRINT_013_PLAN.md
    docs/known-limitations.md
    docs/development-log.md

Создан:

    docs/SPRINT_013_SUMMARY.md

## Дополнительные технические решения

В ходе спринта:

- repository text files нормализованы к LF;
- добавлены `.gitattributes` и `.editorconfig`;
- команды нормализации используют абсолютный путь репозитория;
- проверки разработки оформляются повторяемыми скриптами;
- backend framework types не проникают во frontend или domain boundaries.

## Что не входит в Sprint 013

Не реализованы:

- Objects API;
- Devices API;
- Tags API;
- Runtime API;
- Events API;
- Alarms API;
- authentication;
- authorization;
- realtime frontend client;
- production CORS;
- HTTPS;
- production deployment.

## Следующий этап

Следующий спринт:

    Sprint 014 — Engineering Baseline

Цель — защитить сформированный foundation тестами, CI и стабильными transport contracts до активного расширения прикладного API.
