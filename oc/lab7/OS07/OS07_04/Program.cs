using System;
using System.Diagnostics;

class OS07_04
{
    static void Main()
    {
        Process child1 = StartChildProcess(30);
        Process child2 = StartChildProcess(45);

        // Ожидание завершения дочерних процессов
        child1.WaitForExit();
        child2.WaitForExit();

        Console.WriteLine("Все дочерние процессы завершены. Программа завершена.");
    }
    static Process StartChildProcess(int duration)
    {
        Process process = new Process();
        process.StartInfo.FileName = @"..\..\..\..\OS07_04_X\bin\Debug\net8.0\OS07_04_X.exe"; // Укажите полный путь к OS07_04_X.exe
        process.StartInfo.Arguments = $"{duration}";
        process.StartInfo.UseShellExecute = true; 
        process.StartInfo.CreateNoWindow = false;

      
        process.Start();

        return process;
    }

}
