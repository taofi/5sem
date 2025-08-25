using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab9
{
    
    public class Cat
    {
        private string name;
        private short happy;
        IReactions reactions;
        public short Happy
        {
            get => happy;
            set => happy = (short)Math.Clamp((int)value, 0, 100);
        }

        private short food;
        public short Food
        {
            get => food;
            set => food = (short)Math.Clamp((int)value, 0, 100);
        }

        private short water;
        public short Water
        {
            get => water;
            set => water = (short)Math.Clamp((int)value, 0, 100);
        }

        private short angry;
        public short Angry
        {
            get => angry;
            set => angry = (short)Math.Clamp((int)value, 0, 100);
        }
        public Cat(string  Name, IReactions obj)
        {
            reactions = obj;
            name = Name;
            Happy = 0;
            Food = 100;
            Water = 100;
            Angry = 0;
        }

        public override string ToString()
        {
            return $"{name}: Счастье = {Happy}, Сытость = {Food}, Жажда = {Water}, Злость = {Angry}";
        }
        public string Play()
        {
            Random random = new Random();
            string result;
            if (angry > 50)
            {
                Angry += 5;
                Food -= 10;
                Water -= 5;
            }
            else if (food > 0 && water > 0)
            {
                Happy += 10;
                Food -= 10;
                Water -= 5;

            }
            return reactions.PlayReaction(food + 10);
        }

        public string Eat()
        {
            Random random = new Random();
            int reactionIndex = random.Next(CatStrings.FoodReactions.Length);
            Happy += 3;
            Food += 10;
            Angry -= 10;
            return CatStrings.FoodReactions[reactionIndex];

        }
        public string Drink()
        {
            Random random = new Random();
            int reactionIndex = random.Next(CatStrings.WaterReactions.Length);
            Happy += 2;
            Water += 10;
            Angry -= 15;
            return CatStrings.WaterReactions[reactionIndex];
        }
        public string PetTheCat()
        {
            Random random = new Random();
            string result;
            if (food > 60)
            {
                int reactionIndex = random.Next(CatStrings.PositiveReactions.Length);
                result = CatStrings.PositiveReactions[reactionIndex];
                Happy += 10;
                Food -= 10;
                Water -= 5;
                Angry -= 10;

            }
            else
            {
                int reactionIndex = random.Next(CatStrings.NegativeReactions.Length);
                result = CatStrings.NegativeReactions[reactionIndex];
                Angry += 10;
                Food -= 5;
                Happy -= 10;
            }
            return result;
        }
        public string Wait()
        {
            Random random = new Random();

            Happy -= 10;
            Food -= 10;
            Water -= 10;
            Angry -= 10;
            int reactionIndex = random.Next(CatStrings.CatIdleActions.Length);
            return CatStrings.CatIdleActions[reactionIndex];
        }
    }
}
