using System;
using System.Threading;

class OS07_02
{
    static void Main()
    {
        int counter = 0;
        bool running = true;

        Timer timer5Sec = new Timer(_ =>
        {
            Console.WriteLine($"Прошло 5 секунд. Итерации: {counter}");
        }, null, 5000, Timeout.Infinite);

        Timer timer10Sec = new Timer(_ =>
        {
            Console.WriteLine($"Прошло 10 секунд. Итерации: {counter}");
        }, null, 10000, Timeout.Infinite);

        DateTime startTime = DateTime.Now;

        while (running)
        {
            counter++;
            if ((DateTime.Now - startTime).TotalSeconds >= 15)
            {
                running = false;
            }
        }
        Console.WriteLine($"Завершено через 15 секунд. Итерации: {counter}");
    }
}
