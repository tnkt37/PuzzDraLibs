using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PuzzDraAnalyzer
{
    class Program
    {
        static void Main(string[] args)
        {
            new Program().Calc(args);
        }

        void Calc(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("start");
                //for (int i = 1; i <= 200; i++)
                //{
                //    CalcDoubleLimitedSerachOnce(i, 10000);
                //}
                //for (int i = 50; i <= 2000; i+=50)
                //{
                //    CalcDoubleLimitedSerachOnce(i, 10000);
                //}
                CalcDoubleLimitedSerachOnce(600, 10000);
            }
            else
            {
                int w = int.Parse(args[0]);
                int c = int.Parse(args[1]);
                CalcDoubleLimitedSerachOnce(w, c);
            }

            Console.WriteLine("解析を終了しました Enterキーで終了します");
            Console.ReadLine();

        }

        void CalcDoubleLimitedSerachOnce(int beamWeight, int count)
        {
            string dirName = "results";
            if (!Directory.Exists(dirName)) Directory.CreateDirectory(dirName);

            Console.WriteLine("解析開始　ビーム幅{0}　回数{1}", beamWeight, count);

            decimal otisum = 0;
            decimal nfsum = 0;
            double time = 0;
            var watch = new Stopwatch();
            for (int j = 0; j < count; j++)
            {
                var board = GenerateRandomBoard();
                watch.Restart();
                var result = PuzzDraPInvoke.BeamSerach(board, beamWeight).Result;
                watch.Stop();
                time += watch.Elapsed.TotalSeconds;
                otisum += CountScore(result);
                nfsum += result.Item2;
                if ((j + 1) % 100 == 0)
                {
                    Console.WriteLine((j + 1) + ":" + (otisum / (j + 1)) + " / " + (nfsum / (j + 1)) + " / " + (time / (j + 1)));
                }
            }
            var text = string.Format("{0}/{1}/{2}/{3}", beamWeight, otisum / count, nfsum / count, time / count);
            File.WriteAllText(dirName + @"\result" + beamWeight + "w," + count + "c.txt", text);
            Console.WriteLine(text);
        }

        int CountScore(Tuple<int[], int, int[], int, int> tup)
        {
            int score = 0;
            var board = tup.Item1;
            var route = tup.Item3;
            var startRow = tup.Item4;
            var startColumn = tup.Item5;
            PuzzDraPInvoke.MoveByRoute(board, route, startRow, startColumn);

            while (true)
            {
                int combo = PuzzDraPInvoke.EvalCombo(board);
                if (combo == 0) break;
                score += combo * 1000;//リーチより価値を高くするための1000倍
                PuzzDraPInvoke.PushDownDrops(board);
                PuzzDraPInvoke.FillBoardRandom(board);
            }
            int reaches = PuzzDraPInvoke.CountReach(board);
            score += reaches;

            return score;
        }

        void CalcTheoryComboAvarage()
        {
            var count = 1000000000;
            var sum = 0d;
            for (int i = 0; i < count; i++)
            {
                sum += GenerateRandomBoard()
                    .Where(x => x != 0)
                    .GroupBy(x => x)
                    .Select(x => x.Count() / 3)
                    .Sum();
                if (i % 100000 == 0) Console.WriteLine(sum / (i + 1));
            }
        }

        static Random random = new Random();
        int[] GenerateRandomBoard()
        {
            const int Width = 6;
            const int Height = 5;
            var board = new int[30];
            for (int i = 0; i < board.Length; i++)
            {
                int drop = 0;
                int r = i / Width, c = i % Width;
                while (true)
                {
                    drop = random.Next(6) + 1;
                    if (c - 2 >= 0 && drop == board[i - 1] && drop == board[i - 2])
                        continue;
                    if (r - 2 >= 0 && drop == board[i - Width] && drop == board[i - Width * 2])
                        continue;

                    break;
                }
                board[i] = drop;
            }
            return board;
        }
    }
}
