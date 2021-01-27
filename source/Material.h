#pragma once

#include "rt.h"
#include "Hittable.h"

class Material {
    public:

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
    public:

    Lambertian(const Color& a) : albedo(a) {}

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        auto scatterDirection = rec.normal + RandomUnitVector();

        if (scatterDirection.NearZero())
            scatterDirection = rec.normal;

        scattered = Ray(rec.p, scatterDirection);
        attenuation = albedo;
        return true;
    }

    public:

    Color albedo;
};

class Metal: public Material {
    public:

    Metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        Vector3 reflected = Reflect(UnitVector(rIn.Direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*RandomInUnitSphere());
        attenuation = albedo;
        return (Dot(scattered.Direction(), rec.normal) > 0);
    }

    public:

    Color albedo;
    double fuzz;
};

class Dieletric : public Material {
    public:

    Dieletric(double indexOfReflection) : ir(indexOfReflection) {}

    virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        /*attenuation = Color(1.0, 1.0, 1.0);
        double refractionRatio = rec.frontFace ? (1.0/ir) : ir;

        Vector3 unitDirection = UnitVector(rIn.Direction());
        Vector3 refracted = Refract(unitDirection, rec.normal, refractionRatio);

        scattered = Ray(rec.p, refracted);
        return true;*/
        attenuation = Color(1.0, 1.0, 1.0);
        double refractionRatio = rec.frontFace ? (1.0 / ir) : ir;

        Vector3 unitDirection = UnitVector(rIn.Direction());
        double cosTheta = fmin(Dot(-unitDirection, rec.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        Vector3 direction;

        if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble()) {
            direction = Reflect(unitDirection, rec.normal);
        } else {
            direction = Refract(unitDirection, rec.normal, refractionRatio);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

    public:

    double ir;

    private:

    static double Reflectance(double cosine, double refIdx) {
        auto r0 = (1 - refIdx) / (1 + refIdx);
        r0 = r0*r0;
        return r0 + (1 - r0)*pow((1 - cosine), 5);
    }
};