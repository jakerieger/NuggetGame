//
// Created by conta on 2/16/2024.
//

#pragma once

class ILifetime {
public:
    virtual ~ILifetime() = default;
    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual void LateUpdate() {}
    virtual void FixedUpdated() {}
    virtual void Destroyed() {}
};