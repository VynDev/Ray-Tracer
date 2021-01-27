#pragma once

#include <iostream>
#include <cmath>

using std::sqrt;

class Vector3 {

    public:

    Vector3() : e{0, 0, 0} {}
    Vector3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double X() const {return e[0];};
    double Y() const {return e[1];};
    double Z() const {return e[2];};

    Vector3 operator-() const {return Vector3(-e[0], -e[1], -e[2]);}
    double operator[](int i) const {return e[i];}
    double& operator[](int i) {return e[i];}

    Vector3& operator+=(const Vector3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double LengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double Length() const {
        return sqrt(LengthSquared());
    }

    bool NearZero() const {
        const auto s = 1e-8;
        return (fabs(e[0] < s) && fabs(e[1] < s) && fabs(e[2] < s));
    }

    inline static Vector3 Random() {
        return Vector3(RandomDouble(), RandomDouble(), RandomDouble());
    }

    inline static Vector3 Random(double min, double max) {
        return Vector3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
    }

    public:

    double e[3];
};

using Point3 = Vector3;
using Color = Vector3;

inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v) {
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v) {
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v) {
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3& v) {
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3& v, double t) {
    return t * v;
}

inline Vector3 operator/(const Vector3& v, double t) {
    return (1/t) * v;
}

inline double Dot(const Vector3& u, const Vector3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline Vector3 Cross(const Vector3& u, const Vector3& v) {
    return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                   u.e[2] * v.e[0] - u.e[0] * v.e[2],
                   u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 UnitVector(const Vector3& v) {
    return v / v.Length();
}

inline Vector3 RandomInUnitSphere() {
    while (true) {
        auto p = Vector3::Random(-1, 1);
        if (p.LengthSquared() >= 1) continue;
        return p;
    }
}

inline Vector3 RandomUnitVector() {
    return UnitVector(RandomInUnitSphere());
}

inline Vector3 RandomInHemisphere(const Vector3& normal) {
    Vector3 inUnitSphere = RandomInUnitSphere();
    if (Dot(inUnitSphere, normal) > 0.0)
        return inUnitSphere;
    else
        return -inUnitSphere;
}

inline Vector3 RandomInUnitDisk() {
    while (true) {
        auto p = Vector3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
        if (p.LengthSquared() >= 1) continue;
        return p;
    }
}

Vector3 Reflect(const Vector3& v, const Vector3& n) {
    return v - 2 * Dot(v, n) * n;
}

// Note: the book doesn't really demonstrate this formula
Vector3 Refract(const Vector3& uv, const Vector3& n, double etaiOverEtat) {
    auto cosTheta = fmin(Dot(-uv, n), 1.0);
    Vector3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vector3 rOutParallel = -sqrt(fabs(1.0 - rOutPerp.LengthSquared())) * n;
    return rOutPerp + rOutParallel;
}