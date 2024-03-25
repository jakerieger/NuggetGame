//
// Created by conta on 2/16/2024.
//

#pragma once
#include "Interfaces/Lifetime.h"

class IGameObject;
struct FSceneContext;

class IComponent : public ILifetime {
public:
    virtual void Awake(FSceneContext& sceneContext) {}
    virtual void Start(FSceneContext& sceneContext) {}
    virtual void Update(float deltaTime, FSceneContext& sceneContext) {}
    virtual void LateUpdate(FSceneContext& sceneContext) {}
    virtual void FixedUpdated(FSceneContext& sceneContext) {}
    virtual void Destroyed(FSceneContext& sceneContext) {}

    void SetParent(IGameObject* parent) {
        m_Parent = parent;
    }

    IGameObject* GetParent() const {
        return m_Parent;
    }

private:
    IGameObject* m_Parent = nullptr;
};