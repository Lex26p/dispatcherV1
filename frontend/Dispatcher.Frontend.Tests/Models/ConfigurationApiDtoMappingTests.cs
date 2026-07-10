using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Tests.Models;

public sealed class ConfigurationApiDtoMappingTests
{
    [Fact]
    public void ObjectsResponse_ToViewModel_MapsPageAndRelationships()
    {
        var response =
            new ObjectCollectionApiResponse
            {
                Count = 2,
                Total = 5,
                Offset = 1,
                Limit = 2,
                HasMore = true,
                Objects =
                [
                    new ObjectApiDto
                    {
                        Id = "site-main",
                        ParentId = null,
                        Type = "Site",
                        Code = "SITE_MAIN",
                        Name = "Main site",
                        Description = "Development site",
                        IsRoot = true
                    },
                    new ObjectApiDto
                    {
                        Id = "building-admin",
                        ParentId = "site-main",
                        Type = "Building",
                        Code = "BUILDING_ADMIN",
                        Name = "Administration building",
                        Description = string.Empty,
                        IsRoot = false
                    }
                ]
            };

        var page = response.ToViewModel();

        Assert.Equal(2, page.Count);
        Assert.Equal(5, page.Total);
        Assert.Equal(1, page.Offset);
        Assert.Equal(2, page.Limit);
        Assert.True(page.HasMore);
        Assert.False(page.IsEmpty);

        Assert.Equal("site-main", page.Items[0].Id);
        Assert.Null(page.Items[0].ParentId);
        Assert.True(page.Items[0].IsRoot);
        Assert.False(page.Items[0].HasParent);

        Assert.Equal(
            "site-main",
            page.Items[1].ParentId
        );

        Assert.True(page.Items[1].HasParent);
    }

    [Fact]
    public void ObjectTreeResponse_ToViewModel_MapsNestedChildren()
    {
        var response =
            new ObjectTreeApiResponse
            {
                Count = 3,
                Roots =
                [
                    new ObjectTreeNodeApiDto
                    {
                        Id = "site-main",
                        Type = "Site",
                        Code = "SITE_MAIN",
                        Name = "Main site",
                        Children =
                        [
                            new ObjectTreeNodeApiDto
                            {
                                Id = "building-admin",
                                ParentId = "site-main",
                                Type = "Building",
                                Code = "BUILDING_ADMIN",
                                Name = "Administration building",
                                Children =
                                [
                                    new ObjectTreeNodeApiDto
                                    {
                                        Id = "room-control",
                                        ParentId = "building-admin",
                                        Type = "Room",
                                        Code = "CONTROL_ROOM",
                                        Name = "Control room"
                                    }
                                ]
                            }
                        ]
                    }
                ]
            };

        var tree = response.ToViewModel();

        Assert.Equal(3, tree.Count);
        Assert.Single(tree.Roots);
        Assert.True(tree.Roots[0].HasChildren);

        Assert.Equal(
            "building-admin",
            tree.Roots[0].Children[0].Id
        );

        Assert.Equal(
            "room-control",
            tree.Roots[0]
                .Children[0]
                .Children[0]
                .Id
        );
    }

    [Fact]
    public void DeviceResponse_ToViewModel_MapsConnection()
    {
        var response =
            new DeviceCollectionApiResponse
            {
                Count = 1,
                Total = 1,
                Limit = 1000,
                Devices =
                [
                    new DeviceApiDto
                    {
                        Id = "device-snmp-1",
                        ObjectId = "building-admin",
                        Code = "UPS_MAIN",
                        Name = "Main UPS",
                        Protocol = "Snmp",
                        State = "Maintenance",
                        RuntimeEnabled = false,
                        Connection =
                            new DeviceConnectionApiDto
                            {
                                Host = "2001:db8::10",
                                Port = 161,
                                IpMode = "IPv6",
                                NetworkInterface = "eth0"
                            }
                    }
                ]
            };

        var page = response.ToViewModel();
        var device = Assert.Single(page.Items);

        Assert.Equal("device-snmp-1", device.Id);
        Assert.True(device.HasObject);
        Assert.Equal("Snmp", device.Protocol);
        Assert.Equal("Maintenance", device.State);
        Assert.False(device.RuntimeEnabled);

        Assert.Equal(
            "2001:db8::10:161",
            device.Connection.Summary
        );

        Assert.Equal(
            "IPv6",
            device.Connection.IpMode
        );

        Assert.Equal(
            "eth0",
            device.Connection.NetworkInterface
        );
    }

    [Fact]
    public void TagResponse_ToViewModel_MapsEngineeringConfiguration()
    {
        var response =
            new TagCollectionApiResponse
            {
                Count = 1,
                Total = 1,
                Limit = 1000,
                Tags =
                [
                    new TagApiDto
                    {
                        Id = "tag-temperature",
                        ObjectId = "room-control",
                        DeviceId = "device-modbus-1",
                        Type = "DeviceTag",
                        ValueType = "Double",
                        ArchivePolicy = "OnChange",
                        Code = "TEMPERATURE",
                        Name = "Room temperature",
                        EngineeringUnit = "degC",
                        Scale = 0.1,
                        Offset = 0.0,
                        Address = "40001",
                        Enabled = true
                    }
                ]
            };

        var page = response.ToViewModel();
        var tag = Assert.Single(page.Items);

        Assert.Equal("tag-temperature", tag.Id);
        Assert.True(tag.HasObject);
        Assert.True(tag.HasDevice);
        Assert.Equal("DeviceTag", tag.Type);
        Assert.Equal("Double", tag.ValueType);
        Assert.Equal("OnChange", tag.ArchivePolicy);
        Assert.Equal("degC", tag.EngineeringUnit);
        Assert.Equal(0.1, tag.Scale);
        Assert.Equal(0.0, tag.Offset);
        Assert.Equal("40001", tag.Address);
        Assert.True(tag.Enabled);
    }
}
