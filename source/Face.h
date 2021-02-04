#pragma once

#include <memory>
#include "Hittable.h"
#include "Vector3.h"

class Face : public Hittable {
    public:

    Face() {};
    Face(Point3 origin, Vector3 direction, Vector3 rotation, Vector3 size, std::shared_ptr<Material> m)
        : origin(origin), direction(direction), rotation(rotation), size(size), matPtr(m) {

    };

    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

    public:

    Point3 origin;
    Vector3 direction;
    Vector3 rotation;
    Vector3 size;
    std::shared_ptr<Material> matPtr;
};

bool Face::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {

    Vector3 outwardNormal = Vector3(0, 0, 1);

    Vector3 direction(2, 0, 0);
    Vector3 directionDown(0, -2, 0);

    double upper = -(Dot(outwardNormal, r.Origin()));
    double lower = Dot(outwardNormal, r.Direction());

    double t = upper / lower;

    double x = r.Origin().X() + t*r.Direction().X();
    double y = r.Origin().Y() + t*r.Direction().Y();
    double z = r.Origin().Z() + t*r.Direction().Z();

    Vector3 rayIntersection(x, y, z);
    Vector3 v = rayIntersection - origin;
    Vector3 v2 = rayIntersection - (origin + direction + directionDown);
    Vector3 distanceFromFacePosition = rayIntersection - origin;

    // This condition create circular surface :)
    /*if ((rayIntersection - origin).LengthSquared() > 2)
        return false;*/

    if (Dot(direction, v) < 0 || Dot(directionDown, v) < 0)
        return false;

    if (Dot(-direction, v2) < 0 || Dot(-directionDown, v2) < 0)
        return false;

    if (t < tMin || t > tMax)
        return false;

    rec.t = t;
    rec.p = r.At(rec.t);
    //Vector3 outwardNormal = Vector3(0, 0, 1);
    rec.SetFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;


    return true;

    /*double z = 0;

    double t = (z - r.Origin().Z()) / r.Direction().Z();
    double x = r.Origin().X() + t*r.Direction().X();
    double y = r.Origin().Y() + t*r.Direction().Y();

    if (t < tMin || t > tMax)
        return false;
    if (x < startPoint.X() || x > startPoint.X() + direction.X())
        return false;
    if (y < startPoint.Y() || y > startPoint.Y() + direction.Y())
        return false;

    rec.t = t;
    rec.p = r.At(rec.t);
    Vector3 outwardNormal = Vector3(0, 0, 1);
    rec.SetFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    
    return true;*/
}