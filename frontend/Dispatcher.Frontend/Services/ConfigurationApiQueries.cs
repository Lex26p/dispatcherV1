using System.Globalization;

namespace Dispatcher.Frontend.Services;

public sealed class ObjectReadRequest
{
    public string? Id { get; init; }

    public string? ParentId { get; init; }

    public bool? IncludeChildren { get; init; }

    public int? Limit { get; init; }

    public int? Offset { get; init; }
}

public sealed class ObjectTreeReadRequest
{
    public string? Id { get; init; }

    public string? ParentId { get; init; }
}

public sealed class DeviceReadRequest
{
    public string? Id { get; init; }

    public string? ObjectId { get; init; }

    public string? State { get; init; }

    public bool? IncludeDisabled { get; init; }

    public int? Limit { get; init; }

    public int? Offset { get; init; }
}

public sealed class TagReadRequest
{
    public string? Id { get; init; }

    public string? ObjectId { get; init; }

    public string? DeviceId { get; init; }

    public bool? IncludeDisabled { get; init; }

    public int? Limit { get; init; }

    public int? Offset { get; init; }
}

internal static class ConfigurationApiQueryBuilder
{
    public static string BuildObjectsPath(
        ObjectReadRequest? request
    )
    {
        if (request is null)
        {
            return "objects";
        }

        return BuildPath(
            "objects",
            [
                Pair("id", request.Id),
                Pair("parentId", request.ParentId),
                Pair("includeChildren", request.IncludeChildren),
                Pair("limit", request.Limit),
                Pair("offset", request.Offset)
            ]
        );
    }

    public static string BuildObjectTreePath(
        ObjectTreeReadRequest? request
    )
    {
        if (request is null)
        {
            return "objects/tree";
        }

        return BuildPath(
            "objects/tree",
            [
                Pair("id", request.Id),
                Pair("parentId", request.ParentId)
            ]
        );
    }

    public static string BuildDevicesPath(
        DeviceReadRequest? request
    )
    {
        if (request is null)
        {
            return "devices";
        }

        return BuildPath(
            "devices",
            [
                Pair("id", request.Id),
                Pair("objectId", request.ObjectId),
                Pair("state", request.State),
                Pair("includeDisabled", request.IncludeDisabled),
                Pair("limit", request.Limit),
                Pair("offset", request.Offset)
            ]
        );
    }

    public static string BuildTagsPath(
        TagReadRequest? request
    )
    {
        if (request is null)
        {
            return "tags";
        }

        return BuildPath(
            "tags",
            [
                Pair("id", request.Id),
                Pair("objectId", request.ObjectId),
                Pair("deviceId", request.DeviceId),
                Pair("includeDisabled", request.IncludeDisabled),
                Pair("limit", request.Limit),
                Pair("offset", request.Offset)
            ]
        );
    }

    private static string BuildPath(
        string endpoint,
        IEnumerable<KeyValuePair<string, string?>> parameters
    )
    {
        var queryParts = parameters
            .Where(static parameter => parameter.Value is not null)
            .Select(
                static parameter =>
                    $"{Uri.EscapeDataString(parameter.Key)}=" +
                    $"{Uri.EscapeDataString(parameter.Value!)}"
            )
            .ToArray();

        return queryParts.Length == 0
            ? endpoint
            : $"{endpoint}?{string.Join("&", queryParts)}";
    }

    private static KeyValuePair<string, string?> Pair(
        string name,
        string? value
    )
    {
        return new KeyValuePair<string, string?>(
            name,
            value
        );
    }

    private static KeyValuePair<string, string?> Pair(
        string name,
        bool? value
    )
    {
        return Pair(
            name,
            value.HasValue
                ? value.Value.ToString().ToLowerInvariant()
                : null
        );
    }

    private static KeyValuePair<string, string?> Pair(
        string name,
        int? value
    )
    {
        return Pair(
            name,
            value?.ToString(CultureInfo.InvariantCulture)
        );
    }
}
