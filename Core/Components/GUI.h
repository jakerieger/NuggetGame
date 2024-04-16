// Author: Jake Rieger
// Created: 4/16/2024.
//

#pragma once

#include "Types.h"
#include "Interfaces/Component.h"

class AGUI final : public IComponent {
public:
    AGUI() = default;
    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    void ToggleVisibility(bool visible);

protected:
    bool m_Visible = true;
};
