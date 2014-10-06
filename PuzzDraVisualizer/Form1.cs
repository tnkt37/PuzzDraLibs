using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using PuzzDraAnalyzer;

namespace PuzzDraVisualizer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        Color[] backColors = { Color.FromArgb(30, 17, 11), Color.FromArgb(73, 42, 35) };
        Bitmap[] dropImages;

        int[] board;

        private void Form1_Load(object sender, EventArgs e)
        {
            logTextBox.Enabled = false;
            mainPictureBox.Focus();
            PuzzDraPInvoke.Initialize();
            dropImages = new[]
                {
                    new Bitmap(size, size),
                    new Bitmap("火ドロップ.png"),
                    new Bitmap("水ドロップ.png"),
                    new Bitmap("木ドロップ.png"),
                    new Bitmap("光ドロップ.png"),
                    new Bitmap("闇ドロップ.png"),
                    new Bitmap("ハートドロップ.png"),
                };

            //初期はランダム生成
            board = GenerateRandomBoard();

            //描画
            DrawBoard();
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

        void ChangeBoard(int row, int column, int attr)
        {
            board[ToIndex(row, column)] = attr;
        }

        int GetBoardData(int row, int column)
        {
            return board[ToIndex(row, column)];
        }

        int[] dr = { -1, 0, 1, 0 };
        int[] dc = { 0, 1, 0, -1 };
        void SwapDrop(int row, int column, int direction, bool selectedDrop = false)
        {
            int nr = row + dr[direction];
            int nc = column + dc[direction];
            if (OutOfRange(nr, nc)) return;

            int index = ToIndex(row, column);
            int ni = ToIndex(nr, nc);

            int tmp = board[index];
            board[index] = board[ni];
            board[ni] = tmp;

            if (selectedDrop)
            {
                selectedRow = nr;
                selectedColumn = nc;
            }
        }

        int selectedRow = -1, selectedColumn = -1;

        int ToIndex(int row, int column)
        {
            return row * Columns + column;
        }

        const int size = 50;
        const int Columns = 6;
        const int Rows = 5;
        void DrawBoard()
        {
            var image = new Bitmap(mainPictureBox.Width, mainPictureBox.Height);
            using (var gr = Graphics.FromImage(image))
            {
                for (int i = 0; i < Rows; i++)
                {
                    for (int j = 0; j < Columns; j++)
                    {
                        var back = backColors[(ToIndex(i, j) + (i % 2)) % backColors.Length];
                        gr.FillRectangle(new SolidBrush(back), j * size, i * size, size, size);
                        gr.DrawImage(dropImages[GetBoardData(i, j)], j * size, i * size, size, size);
                    }
                }

                if (!OutOfRange(selectedRow, selectedColumn))
                {
                    var pen = new Pen(Color.GhostWhite);
                    pen.Width = 3;
                    gr.DrawEllipse(pen, selectedColumn * size - 1, selectedRow * size - 1, size + 1, size + 1);
                }
            }


            mainPictureBox.Image = image;
        }

        bool IsSelecting()
        {
            return !OutOfRange(selectedRow, selectedColumn);
        }

        void UnSelect()
        {
            selectedRow = -1;
            selectedColumn = -1;
        }

        bool OutOfRange(int row, int column)
        {
            return row < 0 || row >= Rows || column < 0 || column >= Columns;
        }

        void ChangeToNextDrop(int row, int column)
        {
            if (OutOfRange(row, column)) return;
            ChangeBoard(row, column, (GetBoardData(row, column) + 1) % dropImages.Length);
            DrawBoard();
        }

        void SelectDrop(int row, int column)
        {
            selectedRow = row;
            selectedColumn = column;
            DrawBoard();
        }

        //この後矢印の描画をやりたかった
        //async Task CalcRoute()
        //{
        //    var tup = PuzzDraPInvoke.BeamSerach(board);
        //    int score = tup.Item2;
        //    var route = tup.Item3;
        //    int startRow = tup.Item4;
        //    int startColumn = tup.Item5;

        //    await MoveDropByRoute(startRow, startColumn, route);
        //    await ShowCombo();
        //}

        bool isAnimating = false;

        async Task CalcAutoMovemnt()
        {
            if (isAnimating) return;
            
            int beamWeight;
            beamWeight = int.TryParse(beamWeightText.Text, out beamWeight)
                ? Math.Max(beamWeight, 1)
                : 50;

            WriteLineLog();
            WriteLineLog("ルート解析開始");
            WriteLineLog("ビーム幅: " + beamWeight);
            await Task.Delay(10);
            isAnimating = true;
            var startTime = DateTime.Now;
            var tup = await PuzzDraPInvoke.BeamSerach(board, beamWeight);
            var elapsed = DateTime.Now - startTime;
            WriteLineLog("ルート解析完了");
            WriteLineLog("解析時間:" + elapsed.TotalSeconds + "秒");

            int score = tup.Item2;
            var route = tup.Item3;
            int startRow = tup.Item4;
            int startColumn = tup.Item5;
            WriteLineLog("開始地点 (上, 左) = (" + startRow + ", " + startColumn + ")");
            await MoveDropByRoute(startRow, startColumn, route);
            await ShowCombo();
            WriteLineLog();
            isAnimating = false;
        }

        int waitedTime = 800;
        int operatedTime = 100;

        char[] arrows = { '↑', '→', '↓', '←' };
        async Task MoveDropByRoute(int startRow, int startColumn, int[] route)
        {
            operatedTime = int.TryParse(operatedTimeText.Text, out operatedTime)
                ? Math.Max(30, operatedTime)
                : 500;
            SelectDrop(startRow, startColumn);
            WriteLineLog("自動操作開始");
            int size = 0;
            for (int i = 0; i < route.Length && route[i] != -1; i++)
            {
                SwapDrop(selectedRow, selectedColumn, route[i], true);
                DrawBoard();
                logTextBox.Text += arrows[route[i]];
                ScrollToEndLog();
                size++;
                await Task.Delay(operatedTime);
            }
            WriteLineLog();
            UnSelect();
            WriteLineLog("自動操作完了");
            WriteLineLog("ルート長 " + size);
        }

        void WriteLineLog(object text = null, bool returnLine = true)
        {
            text = text ?? "";
            logTextBox.Text += text.ToString() + (returnLine ? "\r\n" : "");
            ScrollToEndLog();
        }

        void ScrollToEndLog()
        {
            logTextBox.SelectionStart = logTextBox.TextLength;
            logTextBox.ScrollToCaret();
        }


        async Task ShowCombo()
        {
            waitedTime = int.TryParse(waitedTimeText.Text, out waitedTime)
                ? Math.Max(30, waitedTime)
                : 800;
            var isFalenDrop = fallDoropCheckBox.Checked;

            UnSelect();
            WriteLineLog();
            WriteLineLog("コンボ処理開始");
            int comboNum = 0;
            while (true)
            {
                var combo = PuzzDraPInvoke.EvalCombo(board);
                DrawBoard();
                comboNum += combo;
                if (combo >= 1)
                {
                    WriteLineLog("+" + combo + "コンボ");
                }
                await Task.Delay(waitedTime);
                if (combo == 0) break;
                var pushed = PuzzDraPInvoke.PushDownDrops(board);
                DrawBoard();
                await Task.Delay(waitedTime);

                if (isFalenDrop)
                {
                    PuzzDraPInvoke.FillBoardRandom(board);
                }

                DrawBoard();
                await Task.Delay(waitedTime);
            }
            WriteLineLog("合計" + comboNum + "コンボ");
            WriteLineLog("コンボ処理完了");
        }

        private async void mainPictureBox_MouseUp(object sender, MouseEventArgs e)
        {
            if (isAnimating) return;
            int column = e.X / size;
            int row = e.Y / size;
            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                ChangeToNextDrop(row, column);

                return;
            }

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                if (!IsSelecting() || (selectedRow == row && selectedColumn == column))
                {
                    SelectDrop(row, column);
                    return;
                }
                else
                {
                    await ShowCombo();
                    WriteLineLog();
                    UnSelect();
                }
            }
        }

        private void Form1_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (isAnimating || !IsSelecting()) return;

            int direction =
                  e.KeyCode == Keys.Up
                    ? 0
                : e.KeyCode == Keys.Right
                    ? 1
                : e.KeyCode == Keys.Down
                    ? 2
                : e.KeyCode == Keys.Left
                    ? 3
                : -1;

            if (direction == -1) return;

            moveFinishButton.Focus();

            SwapDrop(selectedRow, selectedColumn, direction, true);
            var arr = new[] { '上', '右', '下', '左' };
            WriteLineLog(arr[direction], false);
            ScrollToEndLog();
            DrawBoard();
        }

        private void logTextBox_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            Form1_PreviewKeyDown(this, e);
        }

        private void beamWeightText_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            Form1_PreviewKeyDown(this, e);
        }

        private async void moveFinishButton_MouseClick(object sender, MouseEventArgs e)
        {
            if (isAnimating) return;
            isAnimating = true;
            await ShowCombo();
            WriteLineLog();
            UnSelect();
            isAnimating = false;
        }

        private void randomizeButton_MouseClick(object sender, MouseEventArgs e)
        {
            if (isAnimating) return;
            board = GenerateRandomBoard();
            DrawBoard();
        }

        private async void autoMovementButton_MouseClick(object sender, MouseEventArgs e)
        {
            await CalcAutoMovemnt();
        }
    }
}
