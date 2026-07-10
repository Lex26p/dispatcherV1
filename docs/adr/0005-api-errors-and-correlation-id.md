# ADR 0005 — API Errors and Correlation ID

## Статус

Accepted

## Контекст

До Sprint 014 HTTP routing errors возвращали частично структурированный JSON без correlation ID.

Framework-level ошибки Drogon могли возвращаться в собственном формате.

Frontend знал только HTTP status и не мог показать идентификатор запроса для диагностики.

## Решение

Каждый Dispatcher API request получает correlation ID.

Header:

    X-Correlation-Id

Правила:

- валидный входящий ID сохраняется;
- отсутствующий или некорректный ID заменяется;
- максимальная длина — 64 символа;
- допустимы латинские буквы, цифры, точка, дефис и подчеркивание;
- сгенерированный ID содержит 32 hexadecimal symbols;
- response возвращает ID в header;
- JSON error возвращает тот же ID в body.

## Error envelope

Формат:

    {
      "error": {
        "code": "not_found",
        "message": "The requested API route was not found.",
        "correlationId": "example-request-id",
        "details": {
          "path": "/api/missing"
        }
      }
    }

`details` всегда присутствует.

Если дополнительных сведений нет:

    "details": null

## Framework errors

Drogon custom error handler преобразует framework-level ошибки в Dispatcher envelope.

Покрываются:

- 400;
- 401;
- 403;
- 404;
- 405;
- 409;
- 413;
- 500;
- 501;
- 503.

## Handler exceptions

Исключения route handler преобразуются в:

    500 internal_server_error

Текст внутреннего исключения клиенту не возвращается.

Structured logging с correlation ID будет добавлено позже.

## Frontend

`SystemHttpApiClient` читает:

- HTTP status;
- backend error code;
- backend error message;
- correlation ID из body;
- correlation ID из response header как fallback.

`DispatcherApiException` хранит:

    ApiErrorCode
    CorrelationId

Malformed error body не скрывает исходный HTTP status.

## Последствия

Положительные:

- единый формат ошибок;
- диагностика request/response;
- отсутствие plain-text framework errors;
- frontend получает стабильный error contract;
- внутренние exception messages не раскрываются.

Ограничения:

- correlation ID пока не включен в structured logs;
- проверяется только синтаксис входящего ID;
- ID не является security token;
- успешный response содержит ID только в header.
