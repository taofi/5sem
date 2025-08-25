﻿class Program
{
    const int ThreadCount = 20;
    const int ThreadLifeTime = 30;
    const int ObservationTime = 60;
    static int[,] Matrix = new int[ThreadCount, ObservationTime];
    static DateTime StartTime = DateTime.Now;
    static void WorkThread(object? o)
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
        Thread[] t = new Thread[ThreadCount];
        for (int i = 0; i < ThreadCount; ++i)
        {
            object o = i;
            t[i] = new Thread(WorkThread);
            if (i < 10) 
                t[i].Priority = ThreadPriority.Lowest(2);
            else
                t[i].Priority = ThreadPriority.Highest;
            t[i].Start(o);
        }
        Console.WriteLine("Kirill Vorovich");
        Console.WriteLine("A student ... is waiting for the threads to finish");
        for (int i = 0; i < ThreadCount; ++i)
            t[i].Join();
        for (int s = 0; s < ObservationTime; s++)
        {
            Console.Write("{0,3}: ", s);
            for (int th = 0; th < ThreadCount; th++)
            {
                Console.Write(" {0,5}", Matrix[th, s]);
            }
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