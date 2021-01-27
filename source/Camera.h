#pragma once

#include "rt.h"

class Camera {
    public:

    Camera(Point3 lookfrom, Point3 lookat, Vector3 vup, double vfov, double aspectRatio,
            double aperture, double focusDistance        
    ) {
        auto theta = DegreesToRadians(vfov);
        auto h = tan(theta / 2);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = viewportHeight * aspectRatio;
        //auto focalLength = 1.0;

        w = UnitVector(lookfrom - lookat);
        u = UnitVector(Cross(vup, w));
        v = Cross(w, u);

        origin = lookfrom;
        horizontal = focusDistance * viewportWidth * u;
        vertical = focusDistance * viewportHeight * v;
        lowerLeftCorner = origin - horizontal/2 - vertical/2 - focusDistance*w;

        lensRadius = aperture / 2;
    }

    Ray GetRay(double s, double t) const {
        Vector3 rd = lensRadius * RandomInUnitDisk();
        Vector3 offset = u * rd.X() + v * rd.Y();

        return Ray(origin + offset, lowerLeftCorner + s*horizontal + t*vertical - origin - offset);
    }

    private:

    Point3 origin;
    Point3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u, v, w;
    double lensRadius;
};