#pragma once

#include "Hittable.h"
#include "Vector3.h"

class Sphere : public Hittable {
    public:

    Sphere() {};
    Sphere(Point3 center, double radius, std::shared_ptr<Material> m) : center(center), radius(radius), matPtr(m) {};

    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

    public:

    Point3 center;
    double radius;
    std::shared_ptr<Material> matPtr;
};

bool Sphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    Vector3 oc = r.Origin() - center;
    auto a = r.Direction().LengthSquared();
    auto halfB = Dot(oc, r.Direction());
    auto c = oc.LengthSquared() - radius * radius;
    auto discriminant = halfB*halfB - a*c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-halfB - sqrtd) / a;
    if (root < tMin || root > tMax) {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || root > tMax)
            return false;
    }

    rec.t = root;
    rec.p = r.At(rec.t);
    Vector3 outwardNormal = (rec.p - center) / radius;
    rec.SetFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    
    return true;
}