// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Color.h"

#include "Utilities.inl"

#include <cassert>
#include <cmath>

AColor::AColor() : Red(0.f), Green(0.f), Blue(0.f), Alpha(1.f) {}

AColor::AColor(const f32 r, const f32 g, const f32 b, const f32 a)
    : Red(r), Green(g), Blue(b), Alpha(a) {}

AColor::AColor(const u32 color) : Red(0.f), Green(0.f), Blue(0.f), Alpha(1.f) {
    Utilities::HexToRGBA(color, Red, Green, Blue, Alpha);
}

u32 AColor::Value() const { return Utilities::RGBAToHex(Red, Green, Blue, Alpha); }

f32 AColor::Luminance() const {
    // See <https://www.w3.org/TR/WCAG20/#relativeluminancedef>
    const auto uRed   = static_cast<u32>(Red * 255);
    const auto uGreen = static_cast<u32>(Green * 255);
    const auto uBlue  = static_cast<u32>(Blue * 255);

    const auto R = LinearizeComponent(static_cast<f32>(uRed) / 0xFF);
    const auto G = LinearizeComponent(static_cast<f32>(uGreen) / 0xFF);
    const auto B = LinearizeComponent(static_cast<f32>(uBlue) / 0xFF);

    return 0.2126f * R + 0.7152f * G + 0.0722f * B;
}

AColor AColor::WithAlpha(const u32 a) const {
    return {Red, Green, Blue, static_cast<f32>(a) / 255.f};
}

AColor AColor::WithAlpha(f32 a) const { return {Red, Green, Blue, a}; }

AColor AColor::WithRed(const u32 r) const {
    return {static_cast<f32>(r) / 255.f, Green, Blue, Alpha};
}

AColor AColor::WithRed(f32 r) const { return {r, Green, Blue, Alpha}; }

AColor AColor::WithGreen(const u32 g) const {
    return {Red, static_cast<f32>(g) / 255.f, Blue, Alpha};
}

AColor AColor::WithGreen(f32 g) const { return {Red, g, Blue, Alpha}; }

AColor AColor::WithBlue(const u32 b) const {
    return {Red, Green, static_cast<f32>(b) / 255.f, Alpha};
}

AColor AColor::WithBlue(f32 b) const { return {Red, Green, b, Alpha}; }

AColor AColor::Greyscale() const {
    auto lum = Luminance();
    return {lum, lum, lum, Alpha};
}

AColor AColor::AlphaBlend(const AColor& foreground, const AColor& background) {
    const f32 alpha = foreground.Alpha;
    if (alpha == 0.f) {
        return background;
    }
    const f32 invAlpha = 1.f - alpha;

    f32 backAlpha = background.Alpha;
    if (backAlpha == 1.f) {
        return {alpha * foreground.Red + invAlpha * background.Red,
                alpha * foreground.Green + invAlpha * background.Green,
                alpha * foreground.Blue + invAlpha * background.Blue,
                1.f};
    }

    backAlpha          = (backAlpha * invAlpha) / 1.f;
    const f32 outAlpha = alpha + backAlpha;
    assert(outAlpha != 0.f);
    return {alpha * foreground.Red + invAlpha * background.Red / outAlpha,
            alpha * foreground.Green + invAlpha * background.Green / outAlpha,
            alpha * foreground.Blue + invAlpha * background.Blue / outAlpha,
            1.f};
}

u32 AColor::GetAlphaFromOpacity(const f32 opacity) { return static_cast<u32>(opacity * 255); }

AColor AColor::Lerp(const AColor& a, const AColor& b, const f32 t) {
    return {Utilities::Lerp(a.Red, b.Red, t),
            Utilities::Lerp(a.Green, b.Green, t),
            Utilities::Lerp(a.Blue, b.Blue, t),
            Utilities::Lerp(a.Alpha, b.Alpha, t)};
}

f32 AColor::LinearizeComponent(f32 c) {
    // https://stackoverflow.com/questions/61138110/what-is-the-correct-gamma-correction-function
    if (c <= 0.04045f) {
        return c / 12.92f;
    }

    return std::pow((c + 0.055f) / 1.055f, 2.4f);
}