#pragma once

#include <memory>
#include <vector>

#include "Hittable.h"

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {

    public:

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) {Add(object);}

    void Clear() {objects.clear();}
    void Add(shared_ptr<Hittable> object) {objects.push_back(object);}

    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const;

    public:

    std::vector<shared_ptr<Hittable>> objects;

};

bool HittableList::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    HitRecord tmpRec;
    bool hitAnything = false;
    auto closestSoFar = tMax;

    for (const auto& object: objects) {
        if (object->Hit(r, tMin, closestSoFar, tmpRec)) {
            hitAnything = true;
            closestSoFar = tmpRec.t;
            rec = tmpRec;
        }
    }

    return hitAnything;
}