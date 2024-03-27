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

    /**
     * \brief Gets the full path of a resource stored in the Resources directory
     * \param type Resource type, i.e. RES_FONT, RES_AUDIO, etc
     * \param name File name
     * \return Full path to resource
     */
    std::string GetResource(const char* type, const char* name) {
        const auto root     = fs::path(g_Cwd.c_str());
        const auto res      = fs::path("Assets");
        const auto subdir   = Utilities::BitCompare(type, RES_ROOT) ? fs::path() : fs::path(type);
        const auto filename = fs::path(name);
        const auto path     = Utilities::BitCompare(type, RES_ROOT) ? (root / res / filename)
                                                                    : (root / res / subdir / filename);
        if (!exists(path)) {
            fprintf(stderr, "Resource not found: '%s'\n", path.string().c_str());
            throw std::exception();
        }

        return path.string();
    }

    std::vector<Packer::Schemas::Sprite>& GetSprites() { return g_Sprites; }
}  // namespace Resources