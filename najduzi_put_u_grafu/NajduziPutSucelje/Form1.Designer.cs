namespace NajduziPutSucelje
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
            this.naslovLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.urlTextBox = new System.Windows.Forms.TextBox();
            this.testFileButton = new System.Windows.Forms.Button();
            this.clearButton = new System.Windows.Forms.Button();
            this.runButton = new System.Windows.Forms.Button();
            this.ispisTextBox = new System.Windows.Forms.TextBox();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.SuspendLayout();
            // 
            // naslovLabel
            // 
            this.naslovLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.naslovLabel.AutoSize = true;
            this.naslovLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.naslovLabel.Location = new System.Drawing.Point(378, 46);
            this.naslovLabel.Name = "naslovLabel";
            this.naslovLabel.Size = new System.Drawing.Size(225, 29);
            this.naslovLabel.TabIndex = 0;
            this.naslovLabel.Text = "Najduži put u grafu";
            this.naslovLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(27, 104);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(160, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Odaberite testni primjer:";
            // 
            // urlTextBox
            // 
            this.urlTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.urlTextBox.Location = new System.Drawing.Point(30, 135);
            this.urlTextBox.Name = "urlTextBox";
            this.urlTextBox.ReadOnly = true;
            this.urlTextBox.Size = new System.Drawing.Size(736, 22);
            this.urlTextBox.TabIndex = 2;
            // 
            // testFileButton
            // 
            this.testFileButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.testFileButton.Location = new System.Drawing.Point(772, 135);
            this.testFileButton.Name = "testFileButton";
            this.testFileButton.Size = new System.Drawing.Size(95, 23);
            this.testFileButton.TabIndex = 3;
            this.testFileButton.Text = "Otvori";
            this.testFileButton.UseVisualStyleBackColor = true;
            this.testFileButton.Click += new System.EventHandler(this.testFileButton_Click);
            // 
            // clearButton
            // 
            this.clearButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.clearButton.Location = new System.Drawing.Point(873, 135);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(92, 23);
            this.clearButton.TabIndex = 4;
            this.clearButton.Text = "Obriši";
            this.clearButton.UseVisualStyleBackColor = true;
            this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
            // 
            // runButton
            // 
            this.runButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.runButton.Location = new System.Drawing.Point(30, 169);
            this.runButton.Name = "runButton";
            this.runButton.Size = new System.Drawing.Size(157, 23);
            this.runButton.TabIndex = 5;
            this.runButton.Text = "Pokreni";
            this.runButton.UseVisualStyleBackColor = true;
            this.runButton.Click += new System.EventHandler(this.runButton_Click);
            // 
            // ispisTextBox
            // 
            this.ispisTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.ispisTextBox.Location = new System.Drawing.Point(30, 218);
            this.ispisTextBox.Multiline = true;
            this.ispisTextBox.Name = "ispisTextBox";
            this.ispisTextBox.ReadOnly = true;
            this.ispisTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.ispisTextBox.Size = new System.Drawing.Size(931, 306);
            this.ispisTextBox.TabIndex = 6;
            // 
            // progressBar
            // 
            this.progressBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar.Location = new System.Drawing.Point(383, 542);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(578, 23);
            this.progressBar.TabIndex = 7;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(977, 589);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.ispisTextBox);
            this.Controls.Add(this.runButton);
            this.Controls.Add(this.clearButton);
            this.Controls.Add(this.testFileButton);
            this.Controls.Add(this.urlTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.naslovLabel);
            this.Name = "Form1";
            this.Text = "Najduži put u grafu";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label naslovLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox urlTextBox;
        private System.Windows.Forms.Button testFileButton;
        private System.Windows.Forms.Button clearButton;
        private System.Windows.Forms.Button runButton;
        private System.Windows.Forms.TextBox ispisTextBox;
        private System.Windows.Forms.ProgressBar progressBar;
    }
}

