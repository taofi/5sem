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
            cat = new Cat("Мурзик", mockReactions.Object);
        }

        [Test]
        public void Play_WhenAngryGreaterThan50_IncreasesAngryLevel()
        {
            cat.Food = 50;
            mockReactions.Setup(r => r.PlayReaction(It.Is<int>(f => f <= 60)))
                         .Returns("Кот раздражён и шипит.");
            cat.Angry = 60;

            cat.Play();
            mockReactions.Verify(r => r.PlayReaction(50), Times.Once, "PlayReaction должен быть вызван с уровнем еды меньше 60.");
            Assert.AreEqual(65, cat.Angry, "Уровень злости должен увеличиться до 65.");
        }

        [Test]
        public void Play_WhenFoodIsSufficient_IncreasesHappyAndDecreasesFoodAndWater()
        {
            cat.Food = 80;
            cat.Angry = 0;
            mockReactions.Setup(r => r.PlayReaction(It.Is<int>(f => f > 60)))
                         .Returns("Кот мурлычет от радости!");
            short initialHappy = cat.Happy;
            short initialFood = cat.Food;
            short initialWater = cat.Water;

            // Act
            cat.Play();

            // Assert
            mockReactions.Verify(r => r.PlayReaction(80), Times.Once, "PlayReaction должен быть вызван с уровнем еды больге 60.");
            Assert.AreEqual(initialHappy + 10, cat.Happy, "Уровень счастья должен увеличиться на 10.");
            Assert.AreEqual(initialFood - 10, cat.Food, "Уровень еды должен уменьшиться на 10.");
            Assert.AreEqual(initialWater - 5, cat.Water, "Уровень воды должен уменьшиться на 5.");
        }
    }

}