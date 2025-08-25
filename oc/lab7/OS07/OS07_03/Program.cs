using System;
using System.Threading;

class OS07_03
{
    static void Main()
    {
        int counter = 0;
        bool running = true;
        
        // Таймер на 3 секунды
        Timer periodicTimer = new Timer(_ =>
        {
            Console.WriteLine($"Итерации: {counter}");
        }, null, 3000, 3000); // Периодическое срабатывание каждые 3 сек.
        DateTime startTime = DateTime.Now;

        while (running)
        {
            counter++;
            if ((DateTime.Now - startTime).TotalSeconds >= 15)
            {
                running = false;
            }
        }
        periodicTimer.Dispose();
        Console.WriteLine($"Завершено через 15 секунд. Итерации: {counter}");
    }
}
