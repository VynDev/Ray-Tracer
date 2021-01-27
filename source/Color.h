#pragma once

#include <iostream>
#include "Vector3.h"

void WriteColor(std::ostream& out, Color pixelColor, int samplesPerPixel) {

    auto r = pixelColor.X();
    auto g = pixelColor.Y();
    auto b = pixelColor.Z();

    auto scale = 1.0 / samplesPerPixel;

    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    out << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << " ";
    out << static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << " ";
    out << static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << "\n";
}