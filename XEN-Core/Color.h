// Author: Jake Rieger
// Created: 3/25/2024.
//

#pragma once

#include "Types.h"

class AColor {
public:
    AColor();
    AColor(f32 r, f32 g, f32 b, f32 a = 1.0);
    explicit AColor(u32 color);

    [[nodiscard]] u32 Value() const;
    [[nodiscard]] f32 Luminance() const;
    [[nodiscard]] AColor WithAlpha(u32 a) const;
    [[nodiscard]] AColor WithAlpha(f32 a) const;
    [[nodiscard]] AColor WithRed(u32 r) const;
    [[nodiscard]] AColor WithRed(f32 r) const;
    [[nodiscard]] AColor WithGreen(u32 g) const;
    [[nodiscard]] AColor WithGreen(f32 g) const;
    [[nodiscard]] AColor WithBlue(u32 b) const;
    [[nodiscard]] AColor WithBlue(f32 b) const;
    [[nodiscard]] AColor Greyscale() const;

    static AColor AlphaBlend(const AColor& foreground, const AColor& background);
    static u32 GetAlphaFromOpacity(f32 opacity);
    static AColor Lerp(const AColor& a, const AColor& b, f32 t);
    static f32 LinearizeComponent(f32 c);

    f32 Red;
    f32 Green;
    f32 Blue;
    f32 Alpha;
};
