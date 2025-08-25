using System;
using System.Diagnostics;
using System.Threading;

class Program
{
    static void Main()
    {
        long objectSize = 128 * 1024 * 1024; // 128 MB in bytes
        var random = new Random();

        while (true)
        {
            byte[] memoryObject = new byte[objectSize];
            Process currentProcess = Process.GetCurrentProcess();
            long memoryUsage = currentProcess.WorkingSet64;

            Console.WriteLine($"Текущий объем используемой памяти: {memoryUsage / 1024 / 1024} МБ");

            Thread.Sleep(5000);
        }
    }
}
