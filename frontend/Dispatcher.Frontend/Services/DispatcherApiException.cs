using System.Net;

namespace Dispatcher.Frontend.Services;

public sealed class DispatcherApiException : Exception
{
    public DispatcherApiException(
        DispatcherApiErrorKind errorKind,
        string message,
        string endpoint,
        HttpStatusCode? statusCode = null,
        Exception? innerException = null
    )
        : base(message, innerException)
    {
        ErrorKind = errorKind;
        Endpoint = endpoint;
        StatusCode = statusCode;
    }

    public DispatcherApiErrorKind ErrorKind { get; }

    public string Endpoint { get; }

    public HttpStatusCode? StatusCode { get; }

    public bool IsConnectionFailure =>
        ErrorKind == DispatcherApiErrorKind.Connection;

    public bool IsTimeout =>
        ErrorKind == DispatcherApiErrorKind.Timeout;
}
