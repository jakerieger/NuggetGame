namespace AssetTool
{
    partial class MainWindow
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            menuStrip1 = new MenuStrip();
            fileToolStripMenuItem = new ToolStripMenuItem();
            openADFToolStripMenuItem = new ToolStripMenuItem();
            saveADFToolStripMenuItem = new ToolStripMenuItem();
            packAssetToolStripMenuItem = new ToolStripMenuItem();
            toolStripSeparator1 = new ToolStripSeparator();
            exitToolStripMenuItem = new ToolStripMenuItem();
            editToolStripMenuItem = new ToolStripMenuItem();
            validateADFToolStripMenuItem = new ToolStripMenuItem();
            viewToolStripMenuItem = new ToolStripMenuItem();
            togglePreviewToolStripMenuItem = new ToolStripMenuItem();
            toolStripMenuItem1 = new ToolStripMenuItem();
            splitContainer1 = new SplitContainer();
            groupBox2 = new GroupBox();
            label4 = new Label();
            TxtData = new TextBox();
            label3 = new Label();
            DgProperties = new DataGridView();
            NameColumn = new DataGridViewTextBoxColumn();
            ValueColumn = new DataGridViewTextBoxColumn();
            BtnEncodeFile = new Button();
            groupBox1 = new GroupBox();
            ComType = new ComboBox();
            TxtName = new TextBox();
            label2 = new Label();
            label1 = new Label();
            menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)splitContainer1).BeginInit();
            splitContainer1.Panel1.SuspendLayout();
            splitContainer1.SuspendLayout();
            groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)DgProperties).BeginInit();
            groupBox1.SuspendLayout();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem, editToolStripMenuItem, viewToolStripMenuItem, toolStripMenuItem1 });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(1054, 24);
            menuStrip1.TabIndex = 0;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { openADFToolStripMenuItem, saveADFToolStripMenuItem, packAssetToolStripMenuItem, toolStripSeparator1, exitToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new Size(37, 20);
            fileToolStripMenuItem.Text = "File";
            // 
            // openADFToolStripMenuItem
            // 
            openADFToolStripMenuItem.Name = "openADFToolStripMenuItem";
            openADFToolStripMenuItem.Size = new Size(180, 22);
            openADFToolStripMenuItem.Text = "Open ADF";
            // 
            // saveADFToolStripMenuItem
            // 
            saveADFToolStripMenuItem.Name = "saveADFToolStripMenuItem";
            saveADFToolStripMenuItem.Size = new Size(180, 22);
            saveADFToolStripMenuItem.Text = "Save ADF";
            saveADFToolStripMenuItem.Click += saveADFToolStripMenuItem_Click;
            // 
            // packAssetToolStripMenuItem
            // 
            packAssetToolStripMenuItem.Name = "packAssetToolStripMenuItem";
            packAssetToolStripMenuItem.Size = new Size(180, 22);
            packAssetToolStripMenuItem.Text = "Pack Asset...";
            packAssetToolStripMenuItem.Click += packAssetToolStripMenuItem_Click;
            // 
            // toolStripSeparator1
            // 
            toolStripSeparator1.Name = "toolStripSeparator1";
            toolStripSeparator1.Size = new Size(177, 6);
            // 
            // exitToolStripMenuItem
            // 
            exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            exitToolStripMenuItem.Size = new Size(180, 22);
            exitToolStripMenuItem.Text = "Exit";
            exitToolStripMenuItem.Click += exitToolStripMenuItem_Click;
            // 
            // editToolStripMenuItem
            // 
            editToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { validateADFToolStripMenuItem });
            editToolStripMenuItem.Name = "editToolStripMenuItem";
            editToolStripMenuItem.Size = new Size(46, 20);
            editToolStripMenuItem.Text = "Tools";
            // 
            // validateADFToolStripMenuItem
            // 
            validateADFToolStripMenuItem.Name = "validateADFToolStripMenuItem";
            validateADFToolStripMenuItem.Size = new Size(140, 22);
            validateADFToolStripMenuItem.Text = "Validate ADF";
            // 
            // viewToolStripMenuItem
            // 
            viewToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { togglePreviewToolStripMenuItem });
            viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            viewToolStripMenuItem.Size = new Size(44, 20);
            viewToolStripMenuItem.Text = "View";
            // 
            // togglePreviewToolStripMenuItem
            // 
            togglePreviewToolStripMenuItem.Name = "togglePreviewToolStripMenuItem";
            togglePreviewToolStripMenuItem.Size = new Size(153, 22);
            togglePreviewToolStripMenuItem.Text = "Toggle Preview";
            // 
            // toolStripMenuItem1
            // 
            toolStripMenuItem1.Name = "toolStripMenuItem1";
            toolStripMenuItem1.Size = new Size(12, 20);
            // 
            // splitContainer1
            // 
            splitContainer1.Dock = DockStyle.Fill;
            splitContainer1.Location = new Point(0, 24);
            splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            splitContainer1.Panel1.Controls.Add(groupBox2);
            splitContainer1.Panel1.Controls.Add(groupBox1);
            // 
            // splitContainer1.Panel2
            // 
            splitContainer1.Panel2.BackColor = SystemColors.AppWorkspace;
            splitContainer1.Size = new Size(1054, 537);
            splitContainer1.SplitterDistance = 509;
            splitContainer1.TabIndex = 1;
            // 
            // groupBox2
            // 
            groupBox2.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            groupBox2.Controls.Add(label4);
            groupBox2.Controls.Add(TxtData);
            groupBox2.Controls.Add(label3);
            groupBox2.Controls.Add(DgProperties);
            groupBox2.Controls.Add(BtnEncodeFile);
            groupBox2.Location = new Point(12, 82);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(485, 443);
            groupBox2.TabIndex = 1;
            groupBox2.TabStop = false;
            groupBox2.Text = "Asset Data";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(6, 236);
            label4.Name = "label4";
            label4.Size = new Size(31, 15);
            label4.TabIndex = 7;
            label4.Text = "Data";
            // 
            // TxtData
            // 
            TxtData.Font = new Font("Courier New", 9F, FontStyle.Bold, GraphicsUnit.Point, 0);
            TxtData.Location = new Point(6, 254);
            TxtData.MaxLength = 999999;
            TxtData.Multiline = true;
            TxtData.Name = "TxtData";
            TxtData.ReadOnly = true;
            TxtData.ScrollBars = ScrollBars.Vertical;
            TxtData.Size = new Size(473, 154);
            TxtData.TabIndex = 6;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(6, 19);
            label3.Name = "label3";
            label3.Size = new Size(60, 15);
            label3.TabIndex = 5;
            label3.Text = "Properties";
            // 
            // DgProperties
            // 
            DgProperties.AllowUserToAddRows = false;
            DgProperties.AllowUserToDeleteRows = false;
            DgProperties.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            DgProperties.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            DgProperties.Columns.AddRange(new DataGridViewColumn[] { NameColumn, ValueColumn });
            DgProperties.Location = new Point(6, 37);
            DgProperties.Name = "DgProperties";
            DgProperties.RowHeadersWidthSizeMode = DataGridViewRowHeadersWidthSizeMode.AutoSizeToFirstHeader;
            DgProperties.Size = new Size(473, 186);
            DgProperties.TabIndex = 1;
            // 
            // NameColumn
            // 
            NameColumn.HeaderText = "Name";
            NameColumn.Name = "NameColumn";
            // 
            // ValueColumn
            // 
            ValueColumn.HeaderText = "Value";
            ValueColumn.Name = "ValueColumn";
            // 
            // BtnEncodeFile
            // 
            BtnEncodeFile.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            BtnEncodeFile.Location = new Point(6, 414);
            BtnEncodeFile.Name = "BtnEncodeFile";
            BtnEncodeFile.Size = new Size(473, 23);
            BtnEncodeFile.TabIndex = 0;
            BtnEncodeFile.Text = "Select source file...";
            BtnEncodeFile.UseVisualStyleBackColor = true;
            BtnEncodeFile.Click += BtnEncodeFile_Click;
            // 
            // groupBox1
            // 
            groupBox1.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            groupBox1.Controls.Add(ComType);
            groupBox1.Controls.Add(TxtName);
            groupBox1.Controls.Add(label2);
            groupBox1.Controls.Add(label1);
            groupBox1.Location = new Point(12, 3);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(485, 73);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "Metadata";
            // 
            // ComType
            // 
            ComType.DropDownStyle = ComboBoxStyle.DropDownList;
            ComType.FormattingEnabled = true;
            ComType.Items.AddRange(new object[] { "Sprite", "Font", "Audio", "Level", "Movie" });
            ComType.Location = new Point(243, 37);
            ComType.Name = "ComType";
            ComType.Size = new Size(236, 23);
            ComType.TabIndex = 4;
            // 
            // TxtName
            // 
            TxtName.Location = new Point(6, 37);
            TxtName.Name = "TxtName";
            TxtName.Size = new Size(231, 23);
            TxtName.TabIndex = 3;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(243, 19);
            label2.Name = "label2";
            label2.Size = new Size(31, 15);
            label2.TabIndex = 1;
            label2.Text = "Type";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(6, 19);
            label1.Name = "label1";
            label1.Size = new Size(39, 15);
            label1.TabIndex = 0;
            label1.Text = "Name";
            // 
            // MainWindow
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1054, 561);
            Controls.Add(splitContainer1);
            Controls.Add(menuStrip1);
            Icon = (Icon)resources.GetObject("$this.Icon");
            MainMenuStrip = menuStrip1;
            Name = "MainWindow";
            Text = "Asset Tool";
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            splitContainer1.Panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)splitContainer1).EndInit();
            splitContainer1.ResumeLayout(false);
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)DgProperties).EndInit();
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private MenuStrip menuStrip1;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem openADFToolStripMenuItem;
        private ToolStripMenuItem saveADFToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripMenuItem exitToolStripMenuItem;
        private ToolStripMenuItem viewToolStripMenuItem;
        private ToolStripMenuItem togglePreviewToolStripMenuItem;
        private ToolStripMenuItem packAssetToolStripMenuItem;
        private ToolStripMenuItem toolStripMenuItem1;
        private ToolStripMenuItem editToolStripMenuItem;
        private ToolStripMenuItem validateADFToolStripMenuItem;
        private SplitContainer splitContainer1;
        private GroupBox groupBox1;
        private GroupBox groupBox2;
        private Label label2;
        private Label label1;
        private Button BtnEncodeFile;
        private TextBox TxtName;
        private ComboBox ComType;
        private DataGridView DgProperties;
        private DataGridViewTextBoxColumn NameColumn;
        private DataGridViewTextBoxColumn ValueColumn;
        private Label label3;
        private Label label4;
        private TextBox TxtData;
    }
}
