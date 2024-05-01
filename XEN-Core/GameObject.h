//
// Created by jr on 3/28/24.
//

#pragma once

#include "Interfaces/GameObject.h"

namespace GameObject {
    template<typename T>
    static std::unique_ptr<T> Create(const std::string& name) {
        return make_unique<T>(name);
    }
}  // namespace GameObject