namespace SessionManager
{
    partial class SessionManager
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SessionManager));
            this.CamImageBox = new Emgu.CV.UI.ImageBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBaurd = new System.Windows.Forms.ComboBox();
            this.ledConnect = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupCOM = new System.Windows.Forms.ComboBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.ledHome = new System.Windows.Forms.Button();
            this.ledLayhang = new System.Windows.Forms.Button();
            this.ledGuihang = new System.Windows.Forms.Button();
            this.btnHome = new System.Windows.Forms.Button();
            this.btnLayhang = new System.Windows.Forms.Button();
            this.btnGuihang = new System.Windows.Forms.Button();
            this.index1 = new System.Windows.Forms.Panel();
            this.index2 = new System.Windows.Forms.Panel();
            this.index3 = new System.Windows.Forms.Panel();
            this.index4 = new System.Windows.Forms.Panel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupMonitor = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.ledMotoZ = new System.Windows.Forms.Button();
            this.ledMotoY = new System.Windows.Forms.Button();
            this.ledMotoX = new System.Windows.Forms.Button();
            this.ledBtnZ = new System.Windows.Forms.Button();
            this.ledBtnY = new System.Windows.Forms.Button();
            this.ledBtnX = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.CamImageBox)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupMonitor.SuspendLayout();
            this.SuspendLayout();
            // 
            // CamImageBox
            // 
            this.CamImageBox.BackColor = System.Drawing.SystemColors.Window;
            this.CamImageBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.CamImageBox.Location = new System.Drawing.Point(16, 47);
            this.CamImageBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.CamImageBox.Name = "CamImageBox";
            this.CamImageBox.Size = new System.Drawing.Size(554, 314);
            this.CamImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.CamImageBox.TabIndex = 2;
            this.CamImageBox.TabStop = false;
            this.CamImageBox.Click += new System.EventHandler(this.CamImageBox_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(295, 63);
            this.btnConnect.Margin = new System.Windows.Forms.Padding(4);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(123, 28);
            this.btnConnect.TabIndex = 6;
            this.btnConnect.Text = "Kết nối ";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.groupBox1.Controls.Add(this.CamImageBox);
            this.groupBox1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox1.Location = new System.Drawing.Point(30, 282);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(585, 381);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Camera";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(470, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(574, 76);
            this.label1.TabIndex = 9;
            this.label1.Text = "Mô hình quản lý và phân loại hàng hóa\r\n\r\n";
            // 
            // groupBox2
            // 
            this.groupBox2.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.groupBox2.Controls.Add(this.groupBaurd);
            this.groupBox2.Controls.Add(this.ledConnect);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.groupCOM);
            this.groupBox2.Controls.Add(this.btnConnect);
            this.groupBox2.Cursor = System.Windows.Forms.Cursors.Default;
            this.groupBox2.Location = new System.Drawing.Point(30, 64);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(435, 100);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Trạng thái kết nối";
            // 
            // groupBaurd
            // 
            this.groupBaurd.FormattingEnabled = true;
            this.groupBaurd.Location = new System.Drawing.Point(295, 25);
            this.groupBaurd.Name = "groupBaurd";
            this.groupBaurd.Size = new System.Drawing.Size(123, 24);
            this.groupBaurd.TabIndex = 18;
            // 
            // ledConnect
            // 
            this.ledConnect.BackColor = System.Drawing.Color.Red;
            this.ledConnect.Location = new System.Drawing.Point(98, 24);
            this.ledConnect.Name = "ledConnect";
            this.ledConnect.Size = new System.Drawing.Size(23, 18);
            this.ledConnect.TabIndex = 17;
            this.ledConnect.UseVisualStyleBackColor = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(39, 71);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(186, 16);
            this.label3.TabIndex = 14;
            this.label3.Text = "Vui lòng chọn COM và kết nối !";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(39, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 16);
            this.label2.TabIndex = 13;
            this.label2.Text = "Kết nối :";
            // 
            // groupCOM
            // 
            this.groupCOM.FormattingEnabled = true;
            this.groupCOM.Location = new System.Drawing.Point(165, 25);
            this.groupCOM.Name = "groupCOM";
            this.groupCOM.Size = new System.Drawing.Size(124, 24);
            this.groupCOM.TabIndex = 12;
            // 
            // groupBox4
            // 
            this.groupBox4.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.groupBox4.Controls.Add(this.ledHome);
            this.groupBox4.Controls.Add(this.ledLayhang);
            this.groupBox4.Controls.Add(this.ledGuihang);
            this.groupBox4.Controls.Add(this.btnHome);
            this.groupBox4.Controls.Add(this.btnLayhang);
            this.groupBox4.Controls.Add(this.btnGuihang);
            this.groupBox4.Location = new System.Drawing.Point(30, 170);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(585, 106);
            this.groupBox4.TabIndex = 12;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Chế độ";
            // 
            // ledHome
            // 
            this.ledHome.BackColor = System.Drawing.Color.Red;
            this.ledHome.Location = new System.Drawing.Point(466, 21);
            this.ledHome.Name = "ledHome";
            this.ledHome.Size = new System.Drawing.Size(23, 18);
            this.ledHome.TabIndex = 18;
            this.ledHome.UseVisualStyleBackColor = false;
            // 
            // ledLayhang
            // 
            this.ledLayhang.BackColor = System.Drawing.Color.Red;
            this.ledLayhang.Location = new System.Drawing.Point(268, 21);
            this.ledLayhang.Name = "ledLayhang";
            this.ledLayhang.Size = new System.Drawing.Size(23, 18);
            this.ledLayhang.TabIndex = 17;
            this.ledLayhang.UseVisualStyleBackColor = false;
            // 
            // ledGuihang
            // 
            this.ledGuihang.BackColor = System.Drawing.Color.Red;
            this.ledGuihang.Location = new System.Drawing.Point(80, 21);
            this.ledGuihang.Name = "ledGuihang";
            this.ledGuihang.Size = new System.Drawing.Size(23, 18);
            this.ledGuihang.TabIndex = 16;
            this.ledGuihang.UseVisualStyleBackColor = false;
            // 
            // btnHome
            // 
            this.btnHome.Location = new System.Drawing.Point(438, 53);
            this.btnHome.Name = "btnHome";
            this.btnHome.Size = new System.Drawing.Size(73, 33);
            this.btnHome.TabIndex = 15;
            this.btnHome.Text = "Home";
            this.btnHome.UseVisualStyleBackColor = true;
            this.btnHome.Click += new System.EventHandler(this.btnHome_Click);
            // 
            // btnLayhang
            // 
            this.btnLayhang.Location = new System.Drawing.Point(240, 53);
            this.btnLayhang.Name = "btnLayhang";
            this.btnLayhang.Size = new System.Drawing.Size(73, 33);
            this.btnLayhang.TabIndex = 14;
            this.btnLayhang.Text = "Lấy hàng";
            this.btnLayhang.UseVisualStyleBackColor = true;
            this.btnLayhang.Click += new System.EventHandler(this.btnLayhang_Click);
            // 
            // btnGuihang
            // 
            this.btnGuihang.Location = new System.Drawing.Point(54, 53);
            this.btnGuihang.Name = "btnGuihang";
            this.btnGuihang.Size = new System.Drawing.Size(73, 33);
            this.btnGuihang.TabIndex = 13;
            this.btnGuihang.Text = "Gửi hàng";
            this.btnGuihang.UseVisualStyleBackColor = true;
            this.btnGuihang.Click += new System.EventHandler(this.btnGuihang_Click);
            // 
            // index1
            // 
            this.index1.BackColor = System.Drawing.SystemColors.MenuBar;
            this.index1.Location = new System.Drawing.Point(25, 41);
            this.index1.Name = "index1";
            this.index1.Size = new System.Drawing.Size(287, 259);
            this.index1.TabIndex = 0;
            // 
            // index2
            // 
            this.index2.BackColor = System.Drawing.SystemColors.MenuBar;
            this.index2.Location = new System.Drawing.Point(332, 41);
            this.index2.Name = "index2";
            this.index2.Size = new System.Drawing.Size(287, 259);
            this.index2.TabIndex = 1;
            // 
            // index3
            // 
            this.index3.BackColor = System.Drawing.SystemColors.MenuBar;
            this.index3.Location = new System.Drawing.Point(25, 320);
            this.index3.Name = "index3";
            this.index3.Size = new System.Drawing.Size(287, 259);
            this.index3.TabIndex = 2;
            // 
            // index4
            // 
            this.index4.BackColor = System.Drawing.SystemColors.MenuBar;
            this.index4.Location = new System.Drawing.Point(332, 320);
            this.index4.Name = "index4";
            this.index4.Size = new System.Drawing.Size(287, 259);
            this.index4.TabIndex = 3;
            // 
            // groupBox3
            // 
            this.groupBox3.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.groupBox3.Controls.Add(this.index4);
            this.groupBox3.Controls.Add(this.index3);
            this.groupBox3.Controls.Add(this.index2);
            this.groupBox3.Controls.Add(this.index1);
            this.groupBox3.Location = new System.Drawing.Point(639, 64);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(642, 599);
            this.groupBox3.TabIndex = 11;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Kho hàng";
            // 
            // groupMonitor
            // 
            this.groupMonitor.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.groupMonitor.Controls.Add(this.label9);
            this.groupMonitor.Controls.Add(this.label8);
            this.groupMonitor.Controls.Add(this.label7);
            this.groupMonitor.Controls.Add(this.label6);
            this.groupMonitor.Controls.Add(this.label5);
            this.groupMonitor.Controls.Add(this.label4);
            this.groupMonitor.Controls.Add(this.ledMotoZ);
            this.groupMonitor.Controls.Add(this.ledMotoY);
            this.groupMonitor.Controls.Add(this.ledMotoX);
            this.groupMonitor.Controls.Add(this.ledBtnZ);
            this.groupMonitor.Controls.Add(this.ledBtnY);
            this.groupMonitor.Controls.Add(this.ledBtnX);
            this.groupMonitor.Location = new System.Drawing.Point(32, 682);
            this.groupMonitor.Name = "groupMonitor";
            this.groupMonitor.Size = new System.Drawing.Size(433, 70);
            this.groupMonitor.TabIndex = 13;
            this.groupMonitor.TabStop = false;
            this.groupMonitor.Text = "Monitor";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(375, 23);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(49, 16);
            this.label9.TabIndex = 28;
            this.label9.Text = "MotorZ";
            this.label9.Click += new System.EventHandler(this.label9_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(222, 23);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(49, 16);
            this.label8.TabIndex = 27;
            this.label8.Text = "MotorX";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(302, 23);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(50, 16);
            this.label7.TabIndex = 26;
            this.label7.Text = "MotorY";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(101, 23);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(34, 16);
            this.label6.TabIndex = 25;
            this.label6.Text = "btnY";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(162, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 16);
            this.label5.TabIndex = 24;
            this.label5.Text = "btnZ";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(45, 23);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(36, 16);
            this.label4.TabIndex = 23;
            this.label4.Text = "btnX ";
            // 
            // ledMotoZ
            // 
            this.ledMotoZ.BackColor = System.Drawing.Color.Red;
            this.ledMotoZ.Location = new System.Drawing.Point(378, 46);
            this.ledMotoZ.Name = "ledMotoZ";
            this.ledMotoZ.Size = new System.Drawing.Size(23, 18);
            this.ledMotoZ.TabIndex = 22;
            this.ledMotoZ.UseVisualStyleBackColor = false;
            // 
            // ledMotoY
            // 
            this.ledMotoY.BackColor = System.Drawing.Color.Red;
            this.ledMotoY.Location = new System.Drawing.Point(305, 46);
            this.ledMotoY.Name = "ledMotoY";
            this.ledMotoY.Size = new System.Drawing.Size(23, 18);
            this.ledMotoY.TabIndex = 21;
            this.ledMotoY.UseVisualStyleBackColor = false;
            // 
            // ledMotoX
            // 
            this.ledMotoX.BackColor = System.Drawing.Color.Red;
            this.ledMotoX.Location = new System.Drawing.Point(225, 46);
            this.ledMotoX.Name = "ledMotoX";
            this.ledMotoX.Size = new System.Drawing.Size(23, 18);
            this.ledMotoX.TabIndex = 20;
            this.ledMotoX.UseVisualStyleBackColor = false;
            // 
            // ledBtnZ
            // 
            this.ledBtnZ.BackColor = System.Drawing.Color.Red;
            this.ledBtnZ.Location = new System.Drawing.Point(165, 46);
            this.ledBtnZ.Name = "ledBtnZ";
            this.ledBtnZ.Size = new System.Drawing.Size(23, 18);
            this.ledBtnZ.TabIndex = 19;
            this.ledBtnZ.UseVisualStyleBackColor = false;
            // 
            // ledBtnY
            // 
            this.ledBtnY.BackColor = System.Drawing.Color.Red;
            this.ledBtnY.Location = new System.Drawing.Point(104, 46);
            this.ledBtnY.Name = "ledBtnY";
            this.ledBtnY.Size = new System.Drawing.Size(23, 18);
            this.ledBtnY.TabIndex = 18;
            this.ledBtnY.UseVisualStyleBackColor = false;
            // 
            // ledBtnX
            // 
            this.ledBtnX.BackColor = System.Drawing.Color.Red;
            this.ledBtnX.Location = new System.Drawing.Point(50, 46);
            this.ledBtnX.Name = "ledBtnX";
            this.ledBtnX.Size = new System.Drawing.Size(23, 18);
            this.ledBtnX.TabIndex = 17;
            this.ledBtnX.UseVisualStyleBackColor = false;
            // 
            // SessionManager
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.AutoSize = true;
            this.BackColor = System.Drawing.SystemColors.MenuBar;
            this.ClientSize = new System.Drawing.Size(1319, 764);
            this.Controls.Add(this.groupMonitor);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.label1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.MaximizeBox = false;
            this.Name = "SessionManager";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Camera Session";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SessionManager_FormClosing);
            this.Load += new System.EventHandler(this.CameraCapture_Load);
            ((System.ComponentModel.ISupportInitialize)(this.CamImageBox)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupMonitor.ResumeLayout(false);
            this.groupMonitor.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Emgu.CV.UI.ImageBox CamImageBox;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ComboBox groupCOM;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button btnGuihang;
        private System.Windows.Forms.Button btnHome;
        private System.Windows.Forms.Button btnLayhang;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button ledGuihang;
        private System.Windows.Forms.Button ledConnect;
        private System.Windows.Forms.Button ledHome;
        private System.Windows.Forms.Button ledLayhang;
        private System.Windows.Forms.Panel index1;
        private System.Windows.Forms.Panel index2;
        private System.Windows.Forms.Panel index3;
        private System.Windows.Forms.Panel index4;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ComboBox groupBaurd;
        private System.Windows.Forms.GroupBox groupMonitor;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button ledMotoZ;
        private System.Windows.Forms.Button ledMotoY;
        private System.Windows.Forms.Button ledMotoX;
        private System.Windows.Forms.Button ledBtnZ;
        private System.Windows.Forms.Button ledBtnY;
        private System.Windows.Forms.Button ledBtnX;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
    }
}

