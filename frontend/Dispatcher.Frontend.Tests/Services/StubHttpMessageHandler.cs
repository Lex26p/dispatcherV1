namespace Dispatcher.Frontend.Tests.Services;

internal sealed class StubHttpMessageHandler : HttpMessageHandler
{
    private readonly Func<
        HttpRequestMessage,
        CancellationToken,
        Task<HttpResponseMessage>
    > _sendAsync;

    public StubHttpMessageHandler(
        Func<
            HttpRequestMessage,
            CancellationToken,
            Task<HttpResponseMessage>
        > sendAsync
    )
    {
        ArgumentNullException.ThrowIfNull(sendAsync);

        _sendAsync = sendAsync;
    }

    public HttpMethod? LastMethod { get; private set; }

    public Uri? LastRequestUri { get; private set; }

    protected override Task<HttpResponseMessage> SendAsync(
        HttpRequestMessage request,
        CancellationToken cancellationToken
    )
    {
        LastMethod = request.Method;
        LastRequestUri = request.RequestUri;

        return _sendAsync(
            request,
            cancellationToken
        );
    }
}
