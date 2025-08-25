using NUnit.Framework;
using Lab9;
using Moq;

namespace CatTest
{

    [TestFixture]
    public class CatTests
    {
        private Cat cat;
        private Mock<IReactions> mockReactions;

        [SetUp]
        public void Setup()
        {
            mockReactions = new Mock<IReactions>();
            cat = new Cat("������", mockReactions.Object);
        }

        [Test]
        public void Play_WhenAngryGreaterThan50_IncreasesAngryLevel()
        {
            cat.Food = 50;
            mockReactions.Setup(r => r.PlayReaction(It.Is<int>(f => f <= 60)))
                         .Returns("��� �������� � �����.");
            cat.Angry = 60;

            cat.Play();
            mockReactions.Verify(r => r.PlayReaction(50), Times.Once, "PlayReaction ������ ���� ������ � ������� ��� ������ 60.");
            Assert.AreEqual(65, cat.Angry, "������� ������ ������ ����������� �� 65.");
        }

        [Test]
        public void Play_WhenFoodIsSufficient_IncreasesHappyAndDecreasesFoodAndWater()
        {
            cat.Food = 80;
            cat.Angry = 0;
            mockReactions.Setup(r => r.PlayReaction(It.Is<int>(f => f > 60)))
                         .Returns("��� �������� �� �������!");
            short initialHappy = cat.Happy;
            short initialFood = cat.Food;
            short initialWater = cat.Water;

            // Act
            cat.Play();

            // Assert
            mockReactions.Verify(r => r.PlayReaction(80), Times.Once, "PlayReaction ������ ���� ������ � ������� ��� ������ 60.");
            Assert.AreEqual(initialHappy + 10, cat.Happy, "������� ������� ������ ����������� �� 10.");
            Assert.AreEqual(initialFood - 10, cat.Food, "������� ��� ������ ����������� �� 10.");
            Assert.AreEqual(initialWater - 5, cat.Water, "������� ���� ������ ����������� �� 5.");
        }
    }

}