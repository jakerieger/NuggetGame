//
// Created by conta on 2/18/2024.
//

#pragma once

#include "Types.h"

struct FKeyEvent {
    u32 KeyCode;
    u32 Modifiers;
};

struct FMouseEvent {
    u32 Button;
};

struct FMouseMoveEvent {
    f64 X;
    f64 Y;
};

struct FScrollEvent {
    f64 X;
    f64 Y;
};