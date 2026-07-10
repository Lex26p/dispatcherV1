using System.Text.Json;
using System.Text.Json.Serialization;

namespace Dispatcher.Frontend.Models;

public sealed class ApiErrorResponse
{
    [JsonPropertyName("error")]
    public ApiErrorDto? Error { get; set; }
}

public sealed class ApiErrorDto
{
    [JsonPropertyName("code")]
    public string Code { get; set; } = string.Empty;

    [JsonPropertyName("message")]
    public string Message { get; set; } = string.Empty;

    [JsonPropertyName("correlationId")]
    public string CorrelationId { get; set; } = string.Empty;

    [JsonPropertyName("details")]
    public JsonElement Details { get; set; }
}
