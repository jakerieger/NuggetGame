// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include <iostream>

#include "IO.h"

namespace PlatformTools::Utilities {
    inline void PrintByteArray(const ByteArray& bytes) {
        std::stringstream ss;
        for (const unsigned char byte : bytes) {
            ss << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte)
                << ", ";
        }
        std::cout << ss.str() << std::endl;
    }
}