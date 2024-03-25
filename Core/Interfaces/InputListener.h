//
// Created by conta on 2/18/2024.
//

#pragma once
#include "InputEvents.h"

class IInputListener {
public:
    virtual ~IInputListener() = default;

    virtual void OnMouseDown(FMouseEvent& event) {}
    virtual void OnMouseUp(FMouseEvent& event) {}
    virtual void OnMouseMove(FMouseMoveEvent& event) {}
    virtual void OnKeyDown(FKeyEvent& event) {}
    virtual void OnKeyUp(FKeyEvent& event) {}
    virtual void OnKey(FKeyEvent& event) {}
    virtual void OnScroll(FScrollEvent& event) {}
};