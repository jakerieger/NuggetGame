using System.Text;

namespace AssetTool {
    public enum AssetType {
        Sprite = 0,
        Font = 1,
        Audio = 2,
        Level = 3,
        Movie = 4
    }

    public abstract class IProperties {
        public abstract uint GetSize();
        public abstract List<byte> Serialize();
        public abstract IProperties Deserialize(byte[] data);
    }

    public class SpriteProperties : IProperties {
        public uint Width;
        public uint Height;
        public bool IsAlpha;

        public override uint GetSize() => (sizeof(uint) * 2) + sizeof(bool);

        public override List<byte> Serialize() {
            return [
                .. BitConverter.GetBytes(Width),
                .. BitConverter.GetBytes(Height),
                .. BitConverter.GetBytes(IsAlpha),
            ];
        }

        public override IProperties Deserialize(byte[] data) {
            return new SpriteProperties();
        }
    }

    public abstract class IAssetDescriptor {
        public IAssetDescriptor() {
            _name = string.Empty;
            _data = new byte[0];
            _nameLen = 0;
            _dataLen = 0;
        }

        public AssetType AssetType { get; set; }
        public string Name { get => _name; set { _name = value; _nameLen = (uint)(sizeof(char) * value.Length); } }
        public uint Version { get; set; }
        public byte[] Data { get => _data; set { _data = value; _dataLen = (uint)(sizeof(byte) * value.Length); } }

        public IProperties? Properties;

        private string _name;
        private byte[] _data;

        protected uint _nameLen;
        protected uint _dataLen;

        public abstract List<byte> Serialize();
        public abstract IAssetDescriptor Deserialize(byte[] data);
    }

    public class SpriteDescriptor : IAssetDescriptor {
        public SpriteDescriptor() {
            Properties = new SpriteProperties();
            _propertiesLen = Properties.GetSize();
        }

        private readonly uint _propertiesLen;

        public override List<byte> Serialize() {
            List<byte> bytes = [];

            bytes.Add(BitConverter.GetBytes((uint)AssetType)[0]);
            bytes.AddRange(BitConverter.GetBytes(Version));
            bytes.AddRange(BitConverter.GetBytes(_nameLen));
            bytes.AddRange(Encoding.UTF8.GetBytes(Name));
            bytes.AddRange(BitConverter.GetBytes(_propertiesLen));
            if (Properties != null) {
                bytes.AddRange(Properties.Serialize());
            }
            bytes.AddRange(BitConverter.GetBytes(_dataLen));
            bytes.AddRange(Data);

            return bytes;
        }

        public override IAssetDescriptor Deserialize(byte[] data) {
            if (data == null) {
                throw new Exception("Provided data is empty");
            }

            SpriteDescriptor descriptor = new();

            descriptor.AssetType = (AssetType)data[0];

            {
                byte[] versionBytes = new byte[4];
                Array.Copy(data, 1, versionBytes, 0, 4);
                descriptor.Version = BitConverter.ToUInt32(versionBytes, 0);
            }

            {
                byte[] nameLenBytes = new byte[4];
                Array.Copy(data, 5, nameLenBytes, 0, 4);
                descriptor._nameLen = BitConverter.ToUInt32(nameLenBytes, 0);
            }

            return descriptor;
        }
    }
}
