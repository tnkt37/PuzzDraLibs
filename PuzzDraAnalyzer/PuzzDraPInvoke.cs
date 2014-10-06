using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PuzzDraAnalyzer
{
    public unsafe class PuzzDraPInvoke
    {
        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void InitializeSearchData(int maxLookAhead);

        public static void Initialize()
        {
            InitializeSearchData(10);
        }

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int BeamSearch(int* boardArr, int aheads, int length, int beamWeight, int startRow, int startColumn, int* outRoute);

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int DoubleLimitedSearch(int* boardArr, int beamWeight, int limitWeight, int startRow, int startColumn, int* outRoute);

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int CountReach(int* boardArr);

        public static int CountReach(int[] boardArr)
        {
            fixed (int* b = boardArr)
            {
                return CountReach(b);
            }
        }

        public static Task<Tuple<int[], int, int[], int, int>> BeamSerach(int[] board, int beamWeight, int limitWeight = 500000)
        {
            return Task.Run(() =>
            {
                var results = new Tuple<int[], int, int[], int, int>[30];//board, score, route, startRow, startColumn;
                Parallel.For(0, 6 * 5, idx =>
                {
                    var route = new int[50];
                    int tmpScore = 0;

                    fixed (int* b = board, r = route)
                    {
                        try
                        {
                            tmpScore = DoubleLimitedSearch(b, beamWeight, limitWeight, idx / 6, idx % 6, r);
                        }
                        catch
                        {
                            System.Diagnostics.Debug.WriteLine(idx);
                            results[idx] = Tuple.Create(new int[50], 0, new int[50], idx / 6, idx % 6);
                            return;
                        }
                        //tmpScore = BeamSearch(b, 1, 42, 400, idx / 6, idx % 6, r);
                    }

                    results[idx] = Tuple.Create(board, tmpScore, route, idx / 6, idx % 6);
                });
                return results.OrderByDescending(x => x.Item2).First();
            });
        }

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int FillBoardRandom(int* boardArr);

        static Random random = new Random(0);
        public static void FillBoardRandom(int[] board)
        {
            //fixed (int* b = board)
            //    FillBoardRandom(b);

            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    if (board[i * 6 + j] == 0)
                    {
                        board[i * 6 + j] = random.Next(1, 7);
                    }
                }
            }
        }

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int EvalProcess(int* boardArr);

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int MoveByRoute(int* boardArr, int* routeArr, int startRow, int startColumn);

        public static void MoveByRoute(int[] board, int[] route, int startRow, int startColumn)
        {
            fixed (int* b = board, r = route)
                MoveByRoute(b, r, startRow, startColumn);
        }

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int PushDownDrops(int* boardArr);

        public static int PushDownDrops(int[] board)
        {
            fixed (int* b = board)
                return PushDownDrops(b);
        }

        [DllImport("PuzzDraSolver.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int EvalCombo(int* boardArr);

        public static int EvalCombo(int[] board)
        {
            fixed (int* b = board)
                return EvalCombo(b);
        }
    }
}
