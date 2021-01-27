#pragma once

#include "Ray.h"

class Material;

struct HitRecord {
    Point3 p;
    Vector3 normal;
    std::shared_ptr<Material> matPtr;
    double t;
    bool frontFace;

    inline void SetFaceNormal(const Ray& r, const Vector3& outwardNormal) {
        frontFace = Dot(r.Direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
    public:

    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};