
class Program
{
    const int TaskCount = 8;
    const int ThreadLifeTime = 30;
    const int ObservationTime = 60;
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
        t[0] = new Task(() => { WorkTask(0); });
        t[0].Start();
        t[1] = t[0].ContinueWith(delegate { WorkTask(1); });
        t[2] = t[1].ContinueWith(delegate { WorkTask(2); });
        t[3] = t[2].ContinueWith(delegate { WorkTask(3); });
        t[4] = t[3].ContinueWith(delegate { WorkTask(4); });
        t[5] = t[4].ContinueWith(delegate { WorkTask(5); });
        t[6] = t[5].ContinueWith(delegate { WorkTask(6); });
        t[7] = t[6].ContinueWith(delegate { WorkTask(7); });

      


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