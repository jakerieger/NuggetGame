using K4os.Compression.LZ4;

namespace AssetTool {
    public partial class MainWindow : Form {
        public IAssetDescriptor AssetDescriptor { get; set; }

        public MainWindow() {
            InitializeComponent();

            ComType.SelectedIndex = 0;
            AssetDescriptor = new SpriteDescriptor {
                Name = "Nugget",
                AssetType = AssetType.Sprite,
                Version = 1
            };

            TxtName.Text = AssetDescriptor.Name;
            ComType.SelectedIndex = (int)AssetDescriptor.AssetType;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
            Close();
        }

        private void BtnEncodeFile_Click(object sender, EventArgs e) {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Select source file to encode...";
            var result = ofd.ShowDialog();
            if (result == DialogResult.OK) {
                var filename = ofd.FileName;

                byte[] bytes = File.ReadAllBytes(filename);
                string bytesAsText;
                if (bytes.Length > 512) {
                    bytesAsText = BitConverter.ToString(bytes).Replace("-", " ").Substring(0, 512) + "... (" + (bytes.Length - 512).ToString() + " more)";
                } else {
                    bytesAsText = BitConverter.ToString(bytes).Replace("-", " ");
                }
                TxtData.Text = bytesAsText;

                AssetDescriptor.Data = bytes;
            }
        }

        private void saveADFToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Title = "Save ADF";
            saveFileDialog.Filter = "Asset Descriptor File (*.adf)|*.adf";
            var result = saveFileDialog.ShowDialog();
            if (result == DialogResult.OK) {
                var adf = AssetDescriptor.Serialize();
                File.WriteAllBytes(saveFileDialog.FileName, [.. adf]);
            }
        }

        private void packAssetToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Title = "Save Asset Pack";
            saveFileDialog.Filter = "NugPak File (*.nugpak)|*.nugpak";
            var result = saveFileDialog.ShowDialog();
            if (result == DialogResult.OK) {
                var adf = AssetDescriptor.Serialize();
                var target = new byte[LZ4Codec.MaximumOutputSize(adf.Count)];
                var encodeLength = LZ4Codec.Encode([.. adf], target);
                File.WriteAllBytes(saveFileDialog.FileName, target);
            }
        }
    }
}
