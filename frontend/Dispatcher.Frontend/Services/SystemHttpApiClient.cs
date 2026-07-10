using System.Net.Http.Json;
using System.Text.Json;

using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Services;

public sealed class SystemHttpApiClient : ISystemApiClient
{
    private readonly HttpClient _httpClient;
    private readonly DispatcherApiClientOptions _options;

    public SystemHttpApiClient(
        HttpClient httpClient,
        DispatcherApiClientOptions options
    )
    {
        _httpClient = httpClient;
        _options = options;
    }

    public async Task<BackendHealthViewModel> GetHealthAsync(
        CancellationToken cancellationToken = default
    )
    {
        var response = await GetAsync<SystemHealthApiResponse>(
            "system/health",
            cancellationToken
        );

        return response.ToViewModel();
    }

    public async Task<IReadOnlyList<BackendModuleViewModel>> GetModulesAsync(
        CancellationToken cancellationToken = default
    )
    {
        var response = await GetAsync<SystemModulesApiResponse>(
            "system/modules",
            cancellationToken
        );

        return response.ToViewModels();
    }

    private async Task<TResponse> GetAsync<TResponse>(
        string endpointPath,
        CancellationToken cancellationToken
    )
    {
        var apiPath = _options.BuildApiPath(endpointPath);

        try {
            using var response = await _httpClient.GetAsync(
                apiPath,
                HttpCompletionOption.ResponseHeadersRead,
                cancellationToken
            );

            if (!response.IsSuccessStatusCode) {
                throw new DispatcherApiException(
                    DispatcherApiErrorKind.HttpStatus,
                    $"Backend вернул HTTP {(int)response.StatusCode} ({response.ReasonPhrase}).",
                    apiPath,
                    response.StatusCode
                );
            }

            var payload = await response.Content.ReadFromJsonAsync<TResponse>(
                cancellationToken: cancellationToken
            );

            if (payload is null) {
                throw new DispatcherApiException(
                    DispatcherApiErrorKind.InvalidResponse,
                    "Backend вернул пустой ответ.",
                    apiPath,
                    response.StatusCode
                );
            }

            return payload;
        }
        catch (DispatcherApiException) {
            throw;
        }
        catch (OperationCanceledException exception)
            when (!cancellationToken.IsCancellationRequested) {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.Timeout,
                $"Backend не ответил за {_options.RequestTimeout.TotalSeconds:0} секунд.",
                apiPath,
                innerException: exception
            );
        }
        catch (HttpRequestException exception) {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.Connection,
                "Не удалось установить соединение с backend Dispatcher.",
                apiPath,
                exception.StatusCode,
                exception
            );
        }
        catch (JsonException exception) {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.InvalidResponse,
                "Backend вернул некорректный JSON.",
                apiPath,
                innerException: exception
            );
        }
        catch (NotSupportedException exception) {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.InvalidResponse,
                "Формат ответа backend не поддерживается frontend.",
                apiPath,
                innerException: exception
            );
        }
        catch (InvalidOperationException exception) {
            throw new DispatcherApiException(
                DispatcherApiErrorKind.Configuration,
                "Не удалось сформировать адрес backend API.",
                apiPath,
                innerException: exception
            );
        }
    }
}
