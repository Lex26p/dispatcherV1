using System.Net.Http.Json;
using System.Text.Json;
using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public sealed class ConfigurationHttpApiClient
    : IConfigurationApiClient
{
    private const string CorrelationIdHeaderName =
        "X-Correlation-Id";

    private readonly HttpClient _httpClient;
    private readonly DispatcherApiClientOptions _options;

    public ConfigurationHttpApiClient(
        HttpClient httpClient,
        DispatcherApiClientOptions options
    )
    {
        ArgumentNullException.ThrowIfNull(httpClient);
        ArgumentNullException.ThrowIfNull(options);

        _httpClient = httpClient;
        _options = options;
    }

    public async Task<ConfigurationPage<ObjectViewModel>> GetObjectsAsync(
        ObjectReadRequest? request = null,
        CancellationToken cancellationToken = default
    )
    {
        var endpointPath =
            ConfigurationApiQueryBuilder.BuildObjectsPath(
                request
            );

        var response =
            await GetAsync<ObjectCollectionApiResponse>(
                endpointPath,
                cancellationToken
            );

        return response.ToViewModel();
    }

    public async Task<ObjectTreeViewModel> GetObjectTreeAsync(
        ObjectTreeReadRequest? request = null,
        CancellationToken cancellationToken = default
    )
    {
        var endpointPath =
            ConfigurationApiQueryBuilder.BuildObjectTreePath(
                request
            );

        var response =
            await GetAsync<ObjectTreeApiResponse>(
                endpointPath,
                cancellationToken
            );

        return response.ToViewModel();
    }

    public async Task<ConfigurationPage<DeviceViewModel>> GetDevicesAsync(
        DeviceReadRequest? request = null,
        CancellationToken cancellationToken = default
    )
    {
        var endpointPath =
            ConfigurationApiQueryBuilder.BuildDevicesPath(
                request
            );

        var response =
            await GetAsync<DeviceCollectionApiResponse>(
                endpointPath,
                cancellationToken
            );

        return response.ToViewModel();
    }

    public async Task<ConfigurationPage<TagViewModel>> GetTagsAsync(
        TagReadRequest? request = null,
        CancellationToken cancellationToken = default
    )
    {
        var endpointPath =
            ConfigurationApiQueryBuilder.BuildTagsPath(
                request
            );

        var response =
            await GetAsync<TagCollectionApiResponse>(
                endpointPath,
                cancellationToken
            );

        return response.ToViewModel();
    }

    private async Task<TResponse> GetAsync<TResponse>(
        string endpointPath,
        CancellationToken cancellationToken
    )
        where TResponse : class
    {
        var apiPath =
            _options.BuildApiPath(endpointPath);

        string? responseCorrelationId = null;

        try
        {
            using var response =
                await _httpClient.GetAsync(
                    apiPath,
                    HttpCompletionOption.ResponseHeadersRead,
                    cancellationToken
                );

            responseCorrelationId =
                GetCorrelationId(response);

            if (!response.IsSuccessStatusCode)
            {
                var apiError =
                    await TryReadApiErrorAsync(
                        response,
                        cancellationToken
                    );

                var errorMessage =
                    apiError?.Error?.Message;

                if (string.IsNullOrWhiteSpace(errorMessage))
                {
                    errorMessage =
                        $"Backend вернул HTTP " +
                        $"{(int)response.StatusCode} " +
                        $"({response.ReasonPhrase}).";
                }

                var apiErrorCode =
                    NormalizeOptionalValue(
                        apiError?.Error?.Code
                    );

                var bodyCorrelationId =
                    NormalizeOptionalValue(
                        apiError?.Error?.CorrelationId
                    );

                throw new DispatcherApiException(
                    DispatcherApiErrorKind.HttpStatus,
                    errorMessage,
                    apiPath,
                    response.StatusCode,
                    apiErrorCode: apiErrorCode,
                    correlationId:
                        bodyCorrelationId
                        ?? responseCorrelationId
                );
            }

            var payload =
                await response.Content
                    .ReadFromJsonAsync<TResponse>(
                        cancellationToken:
                            cancellationToken
                    );

            if (payload is null)
            {
                throw new DispatcherApiException(
                    DispatcherApiErrorKind.InvalidResponse,
                    "Backend вернул пустой ответ.",
                    apiPath,
                    response.StatusCode,
                    correlationId:
                        responseCorrelationId
                );
            }

            return payload;
        }
        catch (DispatcherApiException)
        {
            throw;
        }
        catch (OperationCanceledException exception)
            when (!cancellationToken.IsCancellationRequested)
        {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.Timeout,
                $"Backend не ответил за " +
                $"{_options.RequestTimeout.TotalSeconds:0} секунд.",
                apiPath,
                innerException: exception
            );
        }
        catch (HttpRequestException exception)
        {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.Connection,
                "Не удалось установить соединение " +
                "с backend Dispatcher.",
                apiPath,
                exception.StatusCode,
                exception
            );
        }
        catch (JsonException exception)
        {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.InvalidResponse,
                "Backend вернул некорректный JSON.",
                apiPath,
                innerException: exception,
                correlationId:
                    responseCorrelationId
            );
        }
        catch (NotSupportedException exception)
        {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.InvalidResponse,
                "Формат ответа backend " +
                "не поддерживается frontend.",
                apiPath,
                innerException: exception,
                correlationId:
                    responseCorrelationId
            );
        }
        catch (InvalidOperationException exception)
        {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.Configuration,
                "Не удалось сформировать адрес backend API.",
                apiPath,
                innerException: exception
            );
        }
    }

    private static async Task<ApiErrorResponse?>
        TryReadApiErrorAsync(
            HttpResponseMessage response,
            CancellationToken cancellationToken
        )
    {
        try
        {
            return await response.Content
                .ReadFromJsonAsync<ApiErrorResponse>(
                    cancellationToken:
                        cancellationToken
                );
        }
        catch (JsonException)
        {
            return null;
        }
        catch (NotSupportedException)
        {
            return null;
        }
    }

    private static string? GetCorrelationId(
        HttpResponseMessage response
    )
    {
        if (
            !response.Headers.TryGetValues(
                CorrelationIdHeaderName,
                out var values
            )
        )
        {
            return null;
        }

        return NormalizeOptionalValue(
            values.FirstOrDefault()
        );
    }

    private static string? NormalizeOptionalValue(
        string? value
    )
    {
        return string.IsNullOrWhiteSpace(value)
            ? null
            : value;
    }
}
