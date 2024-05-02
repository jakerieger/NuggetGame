// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"

namespace AssetTool {
    std::vector<u8> IAssetDescriptor::Serialize() {
        printf("Processing Asset Descriptor '%s'. ", m_Name.c_str());

        std::vector<u8> bytes = {};

        const auto type            = (u8)m_Type;
        const auto version         = m_Version;
        const auto nameLen         = (u32)m_Name.size();
        const auto propertiesLen   = (u32)m_Properties->GetSize();
        const auto propertiesBytes = m_Properties->Serialize();
        const auto dataLen         = (u32)m_SrcData.size();
        const auto data            = m_SrcData.data();

        printf("Size: %llu bytes.\n", m_SrcData.size());

        const auto reserveSize = GetSize();
        bytes.resize(reserveSize);

        memcpy(bytes.data(), &type, sizeof(u8));
        memcpy(bytes.data() + sizeof(type), &version, sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version), &nameLen, sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen),
               m_Name.c_str(),
               nameLen);
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen,
               &propertiesLen,
               sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen +
                 sizeof(propertiesLen),
               propertiesBytes.data(),
               propertiesBytes.size());
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen +
                 sizeof(propertiesLen) + propertiesLen,
               &dataLen,
               sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen +
                 sizeof(propertiesLen) + propertiesLen + sizeof(dataLen),
               data,
               m_SrcData.size());

        printf("Descriptor serialized.\n");

        return bytes;
    }

}  // namespace AssetTool