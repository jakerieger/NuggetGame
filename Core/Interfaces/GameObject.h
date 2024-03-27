//
// Created by conta on 2/16/2024.
//

#pragma once
#include "Interfaces/Component.h"
#include "Input.h"
#include "Interfaces/InputListener.h"
#include "Interfaces/Lifetime.h"
#include "STL.h"
#include "Transform.h"

struct FSceneContext;

class IGameObject : public ILifetime,
                    public IInputListener {
public:
    explicit IGameObject(const std::string& name) : m_Name(name) {}

    template<typename T>
    static std::unique_ptr<T> Create(const std::string& name) {
        return make_unique<T>(name);
    }

    std::string GetName() const { return m_Name; }

    virtual void Awake(FSceneContext& sceneContext) { m_Transform.Awake(sceneContext); }

    virtual void Start(FSceneContext& sceneContext) { m_Transform.Start(sceneContext); }

    virtual void Update(const float deltaTime, FSceneContext& sceneContext) {
        m_Transform.Update(deltaTime, sceneContext);
    }

    virtual void LateUpdate(FSceneContext& sceneContext) { m_Transform.LateUpdate(sceneContext); }

    virtual void FixedUpdated(FSceneContext& sceneContext) {
        m_Transform.FixedUpdated(sceneContext);
    }

    virtual void Destroyed(FSceneContext& sceneContext) { m_Transform.Destroyed(sceneContext); }

    ATransform* GetTransform() { return &m_Transform; }

    template<typename T>
    T* Cast() {
        return dynamic_cast<T*>(this);
    }

    template<typename T>
    void RegisterComponent(T* component) {
        static_assert(std::is_base_of_v<IComponent, T>, "T must be a subclass of IComponent");

        m_Components.push_back(component);
    }

    template<typename T>
    T* GetComponent() {
        static_assert(std::is_base_of_v<IComponent, T>, "T must be a subclass of IComponent");

        for (auto& component : m_Components) {
            auto casted = dynamic_cast<T*>(component);
            if (casted) {
                return casted;
            }
        }

        return nullptr;
    }

protected:
    std::string m_Name;
    ATransform m_Transform;
    std::vector<IComponent*> m_Components;
};