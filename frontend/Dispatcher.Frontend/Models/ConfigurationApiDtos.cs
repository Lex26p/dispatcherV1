using System.Text.Json.Serialization;

namespace Dispatcher.Frontend.Models;

public sealed class ObjectCollectionApiResponse
{
    [JsonPropertyName("objects")]
    public List<ObjectApiDto>? Objects { get; set; }

    [JsonPropertyName("count")]
    public int Count { get; set; }

    [JsonPropertyName("total")]
    public long Total { get; set; }

    [JsonPropertyName("offset")]
    public int Offset { get; set; }

    [JsonPropertyName("limit")]
    public int Limit { get; set; }

    [JsonPropertyName("hasMore")]
    public bool HasMore { get; set; }
}

public sealed class ObjectApiDto
{
    [JsonPropertyName("id")]
    public string Id { get; set; } = string.Empty;

    [JsonPropertyName("parentId")]
    public string? ParentId { get; set; }

    [JsonPropertyName("type")]
    public string Type { get; set; } = string.Empty;

    [JsonPropertyName("code")]
    public string Code { get; set; } = string.Empty;

    [JsonPropertyName("name")]
    public string Name { get; set; } = string.Empty;

    [JsonPropertyName("description")]
    public string Description { get; set; } = string.Empty;

    [JsonPropertyName("isRoot")]
    public bool IsRoot { get; set; }
}

public sealed class ObjectTreeApiResponse
{
    [JsonPropertyName("roots")]
    public List<ObjectTreeNodeApiDto>? Roots { get; set; }

    [JsonPropertyName("count")]
    public long Count { get; set; }
}

public sealed class ObjectTreeNodeApiDto
{
    [JsonPropertyName("id")]
    public string Id { get; set; } = string.Empty;

    [JsonPropertyName("parentId")]
    public string? ParentId { get; set; }

    [JsonPropertyName("type")]
    public string Type { get; set; } = string.Empty;

    [JsonPropertyName("code")]
    public string Code { get; set; } = string.Empty;

    [JsonPropertyName("name")]
    public string Name { get; set; } = string.Empty;

    [JsonPropertyName("description")]
    public string Description { get; set; } = string.Empty;

    [JsonPropertyName("children")]
    public List<ObjectTreeNodeApiDto>? Children { get; set; }
}

public sealed class DeviceCollectionApiResponse
{
    [JsonPropertyName("devices")]
    public List<DeviceApiDto>? Devices { get; set; }

    [JsonPropertyName("count")]
    public int Count { get; set; }

    [JsonPropertyName("total")]
    public long Total { get; set; }

    [JsonPropertyName("offset")]
    public int Offset { get; set; }

    [JsonPropertyName("limit")]
    public int Limit { get; set; }

    [JsonPropertyName("hasMore")]
    public bool HasMore { get; set; }
}

public sealed class DeviceApiDto
{
    [JsonPropertyName("id")]
    public string Id { get; set; } = string.Empty;

    [JsonPropertyName("objectId")]
    public string? ObjectId { get; set; }

    [JsonPropertyName("code")]
    public string Code { get; set; } = string.Empty;

    [JsonPropertyName("name")]
    public string Name { get; set; } = string.Empty;

    [JsonPropertyName("description")]
    public string Description { get; set; } = string.Empty;

    [JsonPropertyName("protocol")]
    public string Protocol { get; set; } = string.Empty;

    [JsonPropertyName("state")]
    public string State { get; set; } = string.Empty;

    [JsonPropertyName("runtimeEnabled")]
    public bool RuntimeEnabled { get; set; }

    [JsonPropertyName("connection")]
    public DeviceConnectionApiDto? Connection { get; set; }
}

public sealed class DeviceConnectionApiDto
{
    [JsonPropertyName("host")]
    public string Host { get; set; } = string.Empty;

    [JsonPropertyName("port")]
    public int Port { get; set; }

    [JsonPropertyName("ipMode")]
    public string IpMode { get; set; } = string.Empty;

    [JsonPropertyName("networkInterface")]
    public string NetworkInterface { get; set; } = string.Empty;
}

public sealed class TagCollectionApiResponse
{
    [JsonPropertyName("tags")]
    public List<TagApiDto>? Tags { get; set; }

    [JsonPropertyName("count")]
    public int Count { get; set; }

    [JsonPropertyName("total")]
    public long Total { get; set; }

    [JsonPropertyName("offset")]
    public int Offset { get; set; }

    [JsonPropertyName("limit")]
    public int Limit { get; set; }

    [JsonPropertyName("hasMore")]
    public bool HasMore { get; set; }
}

public sealed class TagApiDto
{
    [JsonPropertyName("id")]
    public string Id { get; set; } = string.Empty;

    [JsonPropertyName("objectId")]
    public string? ObjectId { get; set; }

    [JsonPropertyName("deviceId")]
    public string? DeviceId { get; set; }

    [JsonPropertyName("type")]
    public string Type { get; set; } = string.Empty;

    [JsonPropertyName("valueType")]
    public string ValueType { get; set; } = string.Empty;

    [JsonPropertyName("archivePolicy")]
    public string ArchivePolicy { get; set; } = string.Empty;

    [JsonPropertyName("code")]
    public string Code { get; set; } = string.Empty;

    [JsonPropertyName("name")]
    public string Name { get; set; } = string.Empty;

    [JsonPropertyName("description")]
    public string Description { get; set; } = string.Empty;

    [JsonPropertyName("engineeringUnit")]
    public string EngineeringUnit { get; set; } = string.Empty;

    [JsonPropertyName("scale")]
    public double Scale { get; set; } = 1.0;

    [JsonPropertyName("offset")]
    public double Offset { get; set; }

    [JsonPropertyName("address")]
    public string Address { get; set; } = string.Empty;

    [JsonPropertyName("enabled")]
    public bool Enabled { get; set; }
}

public static class ConfigurationApiDtoMapping
{
    public static ConfigurationPage<ObjectViewModel> ToViewModel(
        this ObjectCollectionApiResponse response
    )
    {
        ArgumentNullException.ThrowIfNull(response);

        var items = (response.Objects ?? [])
            .Select(static item => item.ToViewModel())
            .ToArray();

        return new ConfigurationPage<ObjectViewModel>
        {
            Items = items,
            Count = response.Count,
            Total = response.Total,
            Offset = response.Offset,
            Limit = response.Limit,
            HasMore = response.HasMore
        };
    }

    public static ObjectViewModel ToViewModel(
        this ObjectApiDto dto
    )
    {
        ArgumentNullException.ThrowIfNull(dto);

        return new ObjectViewModel
        {
            Id = dto.Id,
            ParentId = NormalizeOptionalValue(dto.ParentId),
            Type = dto.Type,
            Code = dto.Code,
            Name = dto.Name,
            Description = dto.Description,
            IsRoot = dto.IsRoot
        };
    }

    public static ObjectTreeViewModel ToViewModel(
        this ObjectTreeApiResponse response
    )
    {
        ArgumentNullException.ThrowIfNull(response);

        var roots = (response.Roots ?? [])
            .Select(static root => root.ToViewModel())
            .ToArray();

        return new ObjectTreeViewModel
        {
            Roots = roots,
            Count = response.Count
        };
    }

    public static ObjectTreeNodeViewModel ToViewModel(
        this ObjectTreeNodeApiDto dto
    )
    {
        ArgumentNullException.ThrowIfNull(dto);

        var children = (dto.Children ?? [])
            .Select(static child => child.ToViewModel())
            .ToArray();

        return new ObjectTreeNodeViewModel
        {
            Id = dto.Id,
            ParentId = NormalizeOptionalValue(dto.ParentId),
            Type = dto.Type,
            Code = dto.Code,
            Name = dto.Name,
            Description = dto.Description,
            Children = children
        };
    }

    public static ConfigurationPage<DeviceViewModel> ToViewModel(
        this DeviceCollectionApiResponse response
    )
    {
        ArgumentNullException.ThrowIfNull(response);

        var items = (response.Devices ?? [])
            .Select(static item => item.ToViewModel())
            .ToArray();

        return new ConfigurationPage<DeviceViewModel>
        {
            Items = items,
            Count = response.Count,
            Total = response.Total,
            Offset = response.Offset,
            Limit = response.Limit,
            HasMore = response.HasMore
        };
    }

    public static DeviceViewModel ToViewModel(
        this DeviceApiDto dto
    )
    {
        ArgumentNullException.ThrowIfNull(dto);

        var connection = dto.Connection ?? new DeviceConnectionApiDto();

        return new DeviceViewModel
        {
            Id = dto.Id,
            ObjectId = NormalizeOptionalValue(dto.ObjectId),
            Code = dto.Code,
            Name = dto.Name,
            Description = dto.Description,
            Protocol = dto.Protocol,
            State = dto.State,
            RuntimeEnabled = dto.RuntimeEnabled,
            Connection = new DeviceConnectionViewModel
            {
                Host = connection.Host,
                Port = connection.Port,
                IpMode = connection.IpMode,
                NetworkInterface = connection.NetworkInterface
            }
        };
    }

    public static ConfigurationPage<TagViewModel> ToViewModel(
        this TagCollectionApiResponse response
    )
    {
        ArgumentNullException.ThrowIfNull(response);

        var items = (response.Tags ?? [])
            .Select(static item => item.ToViewModel())
            .ToArray();

        return new ConfigurationPage<TagViewModel>
        {
            Items = items,
            Count = response.Count,
            Total = response.Total,
            Offset = response.Offset,
            Limit = response.Limit,
            HasMore = response.HasMore
        };
    }

    public static TagViewModel ToViewModel(
        this TagApiDto dto
    )
    {
        ArgumentNullException.ThrowIfNull(dto);

        return new TagViewModel
        {
            Id = dto.Id,
            ObjectId = NormalizeOptionalValue(dto.ObjectId),
            DeviceId = NormalizeOptionalValue(dto.DeviceId),
            Type = dto.Type,
            ValueType = dto.ValueType,
            ArchivePolicy = dto.ArchivePolicy,
            Code = dto.Code,
            Name = dto.Name,
            Description = dto.Description,
            EngineeringUnit = dto.EngineeringUnit,
            Scale = dto.Scale,
            Offset = dto.Offset,
            Address = dto.Address,
            Enabled = dto.Enabled
        };
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
