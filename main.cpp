
#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "box.h"
#include "triangle.h"
#include "camera.h"
#include "material.h"
#include <omp.h>

using namespace std;

int main() {
    hittable_list world;

    // Materials
    auto wood_material = make_shared<wood>(color(0.55, 0.27, 0.07), color(0.35, 0.16, 0.07), 8.0);
    auto gradient_lambertian = make_shared<lambertian>(color(0.2, 0.7, 0.3), color(0.8, 0.3, 0.1));
    auto tinted_metal = make_shared<metal>(color(0.7, 0.7, 0.7), 0.2, color(0.3, 0.5, 0.8));

    // Spheres
    world.add(make_shared<sphere>(point3(0.0, -1.0, -3.0), 1.0, wood_material)); // Large sphere (wood)
    world.add(make_shared<sphere>(point3(2.0, 0.5, -2.0), 0.5, gradient_lambertian)); // Medium sphere (gradient)
    world.add(make_shared<sphere>(point3(-2.0, 0.3, -2.5), 0.3, tinted_metal)); // Small sphere (tinted metal)

    // Box
    world.add(make_shared<box>(point3(-1.5, -1.0, -4.0), point3(-0.5, 0.0, -3.0), gradient_lambertian));

    // Triangle
    world.add(make_shared<triangle>(
        point3(1.5, -0.5, -2.5),
        point3(2.5, -0.5, -2.5),
        point3(2.0, 0.5, -2.0),
        wood_material
    ));

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    // First POV
    cam.vfov     = 30;
    cam.lookfrom = point3(2, 2, 2);
    cam.lookat   = point3(0, 0, -3);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 5.0;
    cam.focus_dist    = 4.0;

    cam.render(world);

}