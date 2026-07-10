using Dispatcher.Frontend.Models;

namespace Dispatcher.Frontend.Tests.Models;

public sealed class ObjectPagePresentationTests
{
    [Theory]
    [InlineData("Site", "Площадка")]
    [InlineData("Building", "Здание")]
    [InlineData("Floor", "Этаж")]
    [InlineData("Room", "Помещение")]
    [InlineData("Zone", "Зона")]
    [InlineData("Cabinet", "Шкаф")]
    [InlineData("Rack", "Стойка")]
    [InlineData("Line", "Линия")]
    [InlineData("EquipmentGroup", "Группа оборудования")]
    [InlineData("Custom", "Пользовательский объект")]
    [InlineData("FutureType", "FutureType")]
    public void FormatType_ReturnsRussianPresentation(
        string source,
        string expected
    )
    {
        Assert.Equal(
            expected,
            ObjectPagePresentation.FormatType(
                source
            )
        );
    }

    [Fact]
    public void FormatParent_DistinguishesRootAndChild()
    {
        Assert.Equal(
            "Корневой объект",
            ObjectPagePresentation.FormatParent(
                null
            )
        );

        Assert.Equal(
            "Корневой объект",
            ObjectPagePresentation.FormatParent(
                " "
            )
        );

        Assert.Equal(
            "site-main",
            ObjectPagePresentation.FormatParent(
                "site-main"
            )
        );
    }

    [Fact]
    public void CalculateMaxDepth_ReturnsNestedTreeDepth()
    {
        var tree =
            new ObjectTreeViewModel
            {
                Count = 3,
                Roots =
                [
                    new ObjectTreeNodeViewModel
                    {
                        Id = "site-main",
                        Children =
                        [
                            new ObjectTreeNodeViewModel
                            {
                                Id = "building-admin",
                                Children =
                                [
                                    new ObjectTreeNodeViewModel
                                    {
                                        Id = "room-control"
                                    }
                                ]
                            }
                        ]
                    }
                ]
            };

        Assert.Equal(
            3,
            ObjectPagePresentation.CalculateMaxDepth(
                tree.Roots
            )
        );
    }

    [Fact]
    public void CalculateMaxDepth_EmptyTree_ReturnsZero()
    {
        Assert.Equal(
            0,
            ObjectPagePresentation.CalculateMaxDepth(
                []
            )
        );
    }
}
