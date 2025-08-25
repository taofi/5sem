using System;

class OS07_04_X
{
    static void Main(string[] args)
    {
        int duration = int.Parse(args[0]); // Длительность выполнения в секундах
        DateTime startTime = DateTime.Now;
        int count = 0;
        int number = 2;

        Console.WriteLine($"Дочерний процесс {Environment.ProcessId} начал выполнение на {duration} секунд");

        while ((DateTime.Now - startTime).TotalSeconds < duration)
        {
            if (IsPrime(number))
            {
                count++;
                Console.WriteLine($"Простое число {count}: {number}");
            }
            number++;
        }

        Console.WriteLine($"Дочерний процесс {Environment.ProcessId} завершил выполнение.");
    }

    static bool IsPrime(int num)
    {
        if (num < 2) return false;
        for (int i = 2; i <= Math.Sqrt(num); i++)
        {
            if (num % i == 0) return false;
        }
        return true;
    }
}
