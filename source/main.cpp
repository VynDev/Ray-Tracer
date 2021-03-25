#include <iostream>
#include "rt.h"
#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Face.h"
#include "Camera.h"
#include "Material.h"
#include "Window.h"

using namespace std;

HittableList RandomScene() {
    HittableList world;

    auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0,-1000,0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = RandomDouble();
            Point3 center(a + 0.9*RandomDouble(), 0.2, b + 0.9*RandomDouble());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dieletric>(1.5);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dieletric>(1.5);
    world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

Color RayColor(const Ray& r, const Hittable& world, int depth) {
    HitRecord rec;

    if (depth <= 0)
        return Color(0, 0, 0);

    if (world.Hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.matPtr->Scatter(r, rec, attenuation, scattered)) {
            return attenuation * RayColor(scattered, world, depth - 1);
        }
    }
    Vector3 direction = UnitVector(r.Direction());
    auto t = 0.5 * (direction.Y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    /*const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;*/

    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 50;
    const int maxDepth = 50;

    // World

    //HittableList world = RandomScene();
    HittableList world;

    auto materialGround = make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
    auto materialCenter = make_shared<Lambertian>(Color(0.5, 0.2, 0.4));
    auto materialLeft   = make_shared<Dieletric>(1.5);
    auto materialRight  = make_shared<Metal>(Color(0.8, 0.8, 0.9), 0.1);

    world.Add(make_shared<Sphere>(Point3(0.0, -101, -1.0), 100.0, materialGround));
    //world.Add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, materialCenter));
    //world.Add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, materialLeft));
    //world.Add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, materialRight));

    world.Add(make_shared<Sphere>(Point3(1, 0, 2), 1, materialRight));
    world.Add(make_shared<Face>(Point3(-2, 2, 2), Vector3(1, 0, 0), Vector3(0, 100, 0), Vector3(1, 1, 1), materialCenter));

    // Camera
    Point3 lookfrom(0, 1, 5);
    Point3 lookat(0, 1, 0);
    Vector3 vup(0, 1, 0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 90, aspectRatio, aperture, distToFocus);

    cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    Window window(imageWidth, imageHeight);
    for (int j = 0; j < imageHeight; ++j) {
        cerr << "Scanlines remaining: " << imageHeight - j << endl << flush;
        for (int i = 0; i < imageWidth; ++i) {

            Color pixelColor(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + RandomDouble()) / (imageWidth - 1);
                auto v = (j + RandomDouble()) / (imageHeight - 1);
                Ray r = camera.GetRay(u, v);
                pixelColor += RayColor(r, world, maxDepth);
            }
            window.WriteColor(Vector3(i, imageHeight - j - 1, 0), pixelColor, samplesPerPixel);
        }
    }
    window.Render();
    window.Wait();
    cerr << "Render completed." << endl;
}