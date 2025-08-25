class Program
{
    // Поток Z работает 10 секунд
    static void ThreadZed()
    {
        for (int i = 0; i < 10; i++)
        {
            Console.Write(" (Z-{0}) \n", Thread.CurrentThread.ManagedThreadId);
            
        Thread.Sleep(1000);
        }
        Console.WriteLine(" Поток Z завершается \n");
    }
    // Поток работает 20 секунд, параметр - строка-идентификатор
    static void ThreadWithParam(object o)
    {
        for (int i = 0; i < 20; i++)
        {
            Console.Write(" ({0}-{1}) \n", o.ToString(),
            Thread.CurrentThread.ManagedThreadId);
            Thread.Sleep(1000);
        }
    }
    static void Main(string[] args)
    {
        var t1 = new Thread(ThreadZed);
        var t1a = new Thread(ThreadWithParam);
        var t1b = new Thread(ThreadWithParam);
        t1.IsBackground = true; // false для п.11
        t1a.IsBackground = true; // false для п.12
        t1b.IsBackground = true;
        t1.Start();
        t1a.Start("Кирилл"); // Имя
        t1b.Start("Ворович"); // Фамилия
                           // Главный поток работает 5 секунд
        for (int i = 0; i < 5; i++)
        {
            Console.Write(" (*-{0}) \n", Thread.CurrentThread.ManagedThreadId);
            Thread.Sleep(1000);
        }
        Console.WriteLine("Главный поток завершается");
    }
}