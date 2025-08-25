using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab9
{
    public enum CatActions
    {
        Play = 0,
        Feed,
        Water,
        Pet
    }

    public class Program
    {
        public static void Main(string[] args)
        {
            Console.Write("Введите имя: ");
            string input = Console.ReadLine();

            Cat cat = new Cat(input, new Reactions());
            Actions();
            Console.WriteLine(cat.ToString());
            CatStrings.PrintCatArt(1);
            Random random = new Random();
            int ans;
            string action = "";
            while (true)
            {
                Console.Clear();

                Actions();
                Console.WriteLine(cat.ToString());
                int ArtIndex = random.Next(CatStrings.catAsciiArt.Length);

                CatStrings.PrintCatArt(ArtIndex);
                Console.WriteLine(action);
                ans = ReadIntFromConsole("Введите действие: ");
                action = PerformCatAction((CatActions)ans, cat);



            }

        }
        private static string PerformCatAction(CatActions action, Cat cat)
        {
            string result = " ";
            switch (action)
            {
                case CatActions.Play:
                    result = cat.Play();
                    break;
                case CatActions.Feed:
                    result = cat.Eat();
                    break;
                case CatActions.Water:
                    result = cat.Drink();
                    break;
                case CatActions.Pet:
                    result = cat.PetTheCat();
                    break;
                default:
                    result = cat.Wait();
                    break;
            }
            return result;
        }
        private static int ReadIntFromConsole(string prompt)
        {
            int result;
            Console.Write(prompt);
            string input = Console.ReadLine();

            if (int.TryParse(input, out result))
            {
                return result; 
            }
            return -1;
        }

        private static void Actions()
        {
            string actionsString = "";

            foreach (CatActions action in Enum.GetValues(typeof(CatActions)))
            {
                actionsString += $"{action} - {(int)action} ";
            }

            Console.WriteLine(actionsString);
        }

    }
}
