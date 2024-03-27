#include "Resources.h"
#include "STL.h"
#include "Utilities.inl"

namespace Resources {
    std::string g_Cwd;
    std::vector<Packer::Schemas::Sprite> g_Sprites = {};

    namespace fs = std::filesystem;

    void SetCwd(const char* exePath) {
        const std::string exePathStr = exePath;
#ifdef _WIN32
        const std::string cwdStr = exePathStr.substr(0, exePathStr.find_last_of('\\'));
#else
        const std::string cwdStr = exePathStr.substr(0, exePathStr.find_last_of('/'));
#endif
        g_Cwd = cwdStr;
    }

    std::filesystem::path GetRoot() { return {g_Cwd}; }

    std::vector<Packer::Schemas::Sprite>& GetSprites() { return g_Sprites; }
}  // namespace Resources