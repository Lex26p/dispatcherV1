using System.Net;

namespace Dispatcher.Frontend.Services;

public sealed class DispatcherApiException : Exception
{
    public DispatcherApiException(
        DispatcherApiErrorKind errorKind,
        string message,
        string endpoint,
        HttpStatusCode? statusCode = null,
        Exception? innerException = null,
        string? apiErrorCode = null,
        string? correlationId = null
    )
        : base(
            message,
            innerException
        )
    {
        ErrorKind = errorKind;
        Endpoint = endpoint;
        StatusCode = statusCode;
        ApiErrorCode = apiErrorCode;
        CorrelationId = correlationId;
    }

    public DispatcherApiErrorKind ErrorKind { get; }

    public string Endpoint { get; }

    public HttpStatusCode? StatusCode { get; }

    public string? ApiErrorCode { get; }

    public string? CorrelationId { get; }

    public bool IsConnectionFailure =>
        ErrorKind == DispatcherApiErrorKind.Connection;

    public bool IsTimeout =>
        ErrorKind == DispatcherApiErrorKind.Timeout;

    public bool HasCorrelationId =>
        !string.IsNullOrWhiteSpace(CorrelationId);
}
