//
// Created by conta on 2/18/2024.
//

#pragma once

struct FSceneContext;

class IDrawable {
public:
    virtual ~IDrawable()                           = default;
    virtual void Draw(FSceneContext& sceneContext) = 0;

protected:
};