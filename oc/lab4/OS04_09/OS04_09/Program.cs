class Program
{
    const int TaskCount = 20;
    const int ThreadLifeTime = 30;
    const int ObservationTime = 40;
    static int[,] Matrix = new int[TaskCount, ObservationTime];
    static DateTime StartTime = DateTime.Now;
    static void WorkTask(object? o)
    {
        if (o == null)
        {
            return;
        }
        int id = (int)o;
        for (int i = 0; i < ThreadLifeTime * 20; i++)
        {
            DateTime CurrentTime = DateTime.Now;
            int ElapsedSeconds =
            (int)Math.Round(CurrentTime.Subtract(StartTime).TotalSeconds - 0.49);
            if (ElapsedSeconds < 0)
            {
                ElapsedSeconds = 0;
            }
            try
            {
                Matrix[id, ElapsedSeconds] += 50;
            }
            catch { }
            MySleep(50); // из задания 5
        }
    }
    static void Main(string[] args)
    {
        Task[] t = new Task[TaskCount];
        int[] numbers = new int[TaskCount];
        for (int i = 0; i < TaskCount; i++)
            numbers[i] = i;
        Console.WriteLine("A student ... is creating tasks...");
        t[0] = Task.Run(() => { WorkTask(0); });
        t[1] = Task.Run(() => { WorkTask(1); });
        t[2] = Task.Run(() => { WorkTask(2); });
        t[3] = Task.Run(() => { WorkTask(3); });
        t[4] = Task.Run(() => { WorkTask(4); });
        t[5] = Task.Run(() => { WorkTask(5); });
        t[6] = Task.Run(() => { WorkTask(6); });
        t[7] = Task.Run(() => { WorkTask(7); });
        t[8] = Task.Run(() => { WorkTask(8); });
        t[9] = Task.Run(() => { WorkTask(9); });
        t[10] = Task.Run(() => { WorkTask(10); });
        t[11] = Task.Run(() => { WorkTask(11); });
        t[12] = Task.Run(() => { WorkTask(12); });
        t[13] = Task.Run(() => { WorkTask(13); });
        t[14] = Task.Run(() => { WorkTask(14); });
        t[15] = Task.Run(() => { WorkTask(15); });
        t[16] = Task.Run(() => { WorkTask(16); });
        t[17] = Task.Run(() => { WorkTask(17); });
        t[18] = Task.Run(() => { WorkTask(18); });
        t[19] = Task.Run(() => { WorkTask(19); });
        Console.WriteLine("A student ... is waiting for tasks to finish...");
        Task.WaitAll(t);
        for (int s = 0; s < ObservationTime; s++)
        {
            Console.Write("{0,3}: ", s);
            for (int th = 0; th < TaskCount; th++)
                Console.Write(" {0,5}", Matrix[th, s]);
            Console.WriteLine();
        }
    }

    static Double MySleep(int ms)
    {
        Double Sum = 0, Temp;
        for (int t = 0; t < ms; ++t)
        {
            Temp = 0.711 + (Double)t / 10000.0;
            Double a, b, c, d, e, nt;
            for (int k = 0; k < 5500; ++k)
            {
                nt = Temp - k / 27000.0;
                a = Math.Sin(nt);
                b = Math.Cos(nt);
                c = Math.Cos(nt / 2.0);
                d = Math.Sin(nt / 2);
                e = Math.Abs(1.0 - a * a - b * b) + Math.Abs(1.0 - c * c - d * d);
                Sum += e;
            }
        }
        return Sum;
    }
}