// Author: Jake Rieger
// Created: 4/17/2024.
//

#pragma once

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void Serialize() = 0;
    virtual void Deserialize() = 0;
};