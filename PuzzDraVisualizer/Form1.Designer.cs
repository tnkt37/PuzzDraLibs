namespace PuzzDraVisualizer
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.mainPictureBox = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.logTextBox = new System.Windows.Forms.TextBox();
            this.beamWeightText = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.operatedTimeText = new System.Windows.Forms.TextBox();
            this.waitedTimeText = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.fallDoropCheckBox = new System.Windows.Forms.CheckBox();
            this.moveFinishButton = new System.Windows.Forms.Button();
            this.randomizeButton = new System.Windows.Forms.Button();
            this.autoMovementButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.mainPictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // mainPictureBox
            // 
            this.mainPictureBox.Location = new System.Drawing.Point(12, 12);
            this.mainPictureBox.Name = "mainPictureBox";
            this.mainPictureBox.Size = new System.Drawing.Size(310, 260);
            this.mainPictureBox.TabIndex = 0;
            this.mainPictureBox.TabStop = false;
            this.mainPictureBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.mainPictureBox_MouseUp);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(371, 347);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(236, 60);
            this.label1.TabIndex = 1;
            this.label1.Text = "操作説明：\r\n右クリックでドロップ切り替え\r\n左クリックでドロップ選択\r\n矢印キーで選択したドロップを移動\r\n移動終了ボタン/別のドロップを選択でコンボ評価";
            // 
            // logTextBox
            // 
            this.logTextBox.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.logTextBox.Location = new System.Drawing.Point(373, 12);
            this.logTextBox.Multiline = true;
            this.logTextBox.Name = "logTextBox";
            this.logTextBox.ReadOnly = true;
            this.logTextBox.Size = new System.Drawing.Size(297, 322);
            this.logTextBox.TabIndex = 6;
            this.logTextBox.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.logTextBox_PreviewKeyDown);
            // 
            // beamWeightText
            // 
            this.beamWeightText.Location = new System.Drawing.Point(12, 297);
            this.beamWeightText.Name = "beamWeightText";
            this.beamWeightText.Size = new System.Drawing.Size(100, 19);
            this.beamWeightText.TabIndex = 7;
            this.beamWeightText.Text = "300";
            this.beamWeightText.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.beamWeightText_PreviewKeyDown);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 279);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(45, 12);
            this.label2.TabIndex = 8;
            this.label2.Text = "ビーム幅";
            // 
            // operatedTimeText
            // 
            this.operatedTimeText.Location = new System.Drawing.Point(10, 340);
            this.operatedTimeText.Name = "operatedTimeText";
            this.operatedTimeText.Size = new System.Drawing.Size(100, 19);
            this.operatedTimeText.TabIndex = 9;
            this.operatedTimeText.Text = "100";
            this.operatedTimeText.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.beamWeightText_PreviewKeyDown);
            // 
            // waitedTimeText
            // 
            this.waitedTimeText.Location = new System.Drawing.Point(10, 391);
            this.waitedTimeText.Name = "waitedTimeText";
            this.waitedTimeText.Size = new System.Drawing.Size(100, 19);
            this.waitedTimeText.TabIndex = 10;
            this.waitedTimeText.Text = "800";
            this.waitedTimeText.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.beamWeightText_PreviewKeyDown);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 322);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(108, 12);
            this.label3.TabIndex = 11;
            this.label3.Text = "操作待ち時間(ミリ秒)";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 373);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(145, 12);
            this.label4.TabIndex = 12;
            this.label4.Text = "アニメーション待ち時間(ミリ秒)";
            // 
            // fallDoropCheckBox
            // 
            this.fallDoropCheckBox.AutoSize = true;
            this.fallDoropCheckBox.Location = new System.Drawing.Point(177, 391);
            this.fallDoropCheckBox.Name = "fallDoropCheckBox";
            this.fallDoropCheckBox.Size = new System.Drawing.Size(145, 16);
            this.fallDoropCheckBox.TabIndex = 13;
            this.fallDoropCheckBox.Text = "コンボ中にドロップを落とす";
            this.fallDoropCheckBox.UseVisualStyleBackColor = true;
            this.fallDoropCheckBox.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.logTextBox_PreviewKeyDown);
            // 
            // moveFinishButton
            // 
            this.moveFinishButton.Location = new System.Drawing.Point(177, 279);
            this.moveFinishButton.Name = "moveFinishButton";
            this.moveFinishButton.Size = new System.Drawing.Size(143, 23);
            this.moveFinishButton.TabIndex = 14;
            this.moveFinishButton.Text = "移動完了";
            this.moveFinishButton.UseVisualStyleBackColor = true;
            this.moveFinishButton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.moveFinishButton_MouseClick);
            this.moveFinishButton.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.beamWeightText_PreviewKeyDown);
            // 
            // randomizeButton
            // 
            this.randomizeButton.Location = new System.Drawing.Point(177, 317);
            this.randomizeButton.Name = "randomizeButton";
            this.randomizeButton.Size = new System.Drawing.Size(143, 23);
            this.randomizeButton.TabIndex = 15;
            this.randomizeButton.Text = "盤面のランダム生成";
            this.randomizeButton.UseVisualStyleBackColor = true;
            this.randomizeButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.randomizeButton_MouseClick);
            this.randomizeButton.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.beamWeightText_PreviewKeyDown);
            // 
            // autoMovementButton
            // 
            this.autoMovementButton.Location = new System.Drawing.Point(177, 354);
            this.autoMovementButton.Name = "autoMovementButton";
            this.autoMovementButton.Size = new System.Drawing.Size(143, 23);
            this.autoMovementButton.TabIndex = 16;
            this.autoMovementButton.Text = "コンボ解析＆自動操作";
            this.autoMovementButton.UseVisualStyleBackColor = true;
            this.autoMovementButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.autoMovementButton_MouseClick);
            this.autoMovementButton.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.beamWeightText_PreviewKeyDown);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(682, 422);
            this.Controls.Add(this.autoMovementButton);
            this.Controls.Add(this.randomizeButton);
            this.Controls.Add(this.moveFinishButton);
            this.Controls.Add(this.fallDoropCheckBox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.waitedTimeText);
            this.Controls.Add(this.operatedTimeText);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.beamWeightText);
            this.Controls.Add(this.logTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.mainPictureBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "PuzzDraVisualizer";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.Form1_PreviewKeyDown);
            ((System.ComponentModel.ISupportInitialize)(this.mainPictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox mainPictureBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox logTextBox;
        private System.Windows.Forms.TextBox beamWeightText;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox operatedTimeText;
        private System.Windows.Forms.TextBox waitedTimeText;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox fallDoropCheckBox;
        private System.Windows.Forms.Button moveFinishButton;
        private System.Windows.Forms.Button randomizeButton;
        private System.Windows.Forms.Button autoMovementButton;
    }
}