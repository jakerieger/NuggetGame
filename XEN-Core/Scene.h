//
// Created by jr on 2/16/24.
//

#pragma once

#include "SceneContext.h"
#include "STL.h"
#include "Interfaces/Serializable.h"

class AScene final : public ILifetime,
                     public ISerializable {
public:
    explicit AScene(std::string name);

    void Serialize() override {}
    void Deserialize() override {}

    void Awake() override;
    void Start() override;
    void Update(float deltaTime) override;
    void LateUpdate() override;
    void FixedUpdate() override;
    void Destroyed() override;

    void Render();

    void SetActive(const bool active) {
        m_Active = active;
    }

    [[nodiscard]] bool GetActive() const {
        return m_Active;
    }

    std::string& GetName() {
        return m_Name;
    }

    FSceneContext& GetContext() {
        return m_SceneContext;
    }

    [[nodiscard]] bool GetLoading() const {
        return m_Loading;
    }

    void SetLoading(const bool loading) {
        m_Loading = loading;
    }

    template<typename T>
    static std::vector<T*> FindAllGameObjectsOf(FSceneContext& context) {
        static_assert(std::is_base_of_v<IGameObject, T>, "T must be a subclass of IGameObject");
        std::vector<T*> found;
        for (auto& go : context.GameObjects) {
            if (auto casted = dynamic_cast<T*>(go.get()); casted) {
                found.push_back(casted);
            }
        }

        return found;
    }

    template<typename T>
    static T* FindGameObjectOf(FSceneContext& context, const std::string& name) {
        static_assert(std::is_base_of_v<IGameObject, T>, "T must be a subclass of IGameObject");
        for (auto& go : context.GameObjects) {
            if (auto casted = dynamic_cast<T*>(go.get()); casted && casted->GetName() == name) {
                return casted;
            }
        }

        return nullptr;
    }

    static IGameObject* FindGameObject(FSceneContext& context, const std::string& name);

    template<typename T>
    void AddGameObject(std::unique_ptr<T>& gameObject) {
        static_assert(std::is_base_of_v<IGameObject, T>, "T must be a subclass of IGameObject");
        m_SceneContext.GameObjects.push_back(std::move(gameObject));
    }

private:
    FSceneContext m_SceneContext;
    std::string m_Name;
    bool m_Active  = false;
    bool m_Loading = false;

    void DrawMainPass();
};

namespace Scene {
    static std::unique_ptr<AScene> Create(const std::string& name) {
        return make_unique<AScene>(name);
    }
}  // namespace Scene