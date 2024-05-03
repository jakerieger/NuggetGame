// Author: Jake Rieger
// Created: 5/2/2024.
//

#pragma once

namespace AssetTool::Utilities {
    template<typename... Types>
    size_t SizeOfAll() {
        return (sizeof(Types) + ...);
    }
}  // namespace AssetTool::Utilities
