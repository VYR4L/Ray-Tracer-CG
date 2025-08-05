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

int main()
{
    hittable_list world;

    // Materiais
    auto mat1_red = make_shared<lambertian>(color(0.8, 0.3, 0.3));
    auto mat1_blue = make_shared<lambertian>(color(0.3, 0.3, 0.8));

    auto mat2 = make_shared<metal>(color(0.7, 0.7, 0.9), 0.1);
    auto mat3 = make_shared<wood>(color(0.6, 0.4, 0.2), color(0.3, 0.2, 0.1), 12.0);

    // Chão de madeira
    world.add(make_shared<box>(point3(-4.0, -1.1, -4.0), point3(4.0, -1.0, 4.0), mat3));

    // Objetos
    world.add(make_shared<sphere>(point3(-2.0, 0.5, -1.0), 0.5, mat1_red)); // Esfera pequena
    world.add(make_shared<sphere>(point3(1.5, 1.0, 0.5), 1.0, mat2));   // Esfera grande
    world.add(make_shared<box>(point3(-1.0, -0.5, -2.0), point3(0.0, 0.5, -1.0), mat3)); // Caixa média
    world.add(make_shared<box>(point3(2.0, -1.0, -0.5), point3(3.0, 0.0, 0.5), mat1_blue));   // Caixa grande
    // Pirâmide de base triangular
    point3 A(0.0, 0.0, 2.0);
    point3 B(1.0, 0.0, 2.5);
    point3 C(0.5, 1.0, 2.2);
    point3 D(0.5, 0.5, 3.2);

    // Base
    world.add(make_shared<triangle>(A, B, C, mat2));
    // Face 1
    world.add(make_shared<triangle>(A, B, D, mat2));
    // Face 2
    world.add(make_shared<triangle>(B, C, D, mat2));
    // Face 3
    world.add(make_shared<triangle>(C, A, D, mat2));

    // POV
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;

    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 50;

    // Vista frontal
    // cam.lookfrom = point3(2, 3, 6);
    // cam.lookat = point3(0, 0, 0);
    // cam.vup = vec3(0, 1, 0);

    // Lateral esquerda
    // cam.lookfrom = point3(-6, 2, 0);
    // cam.lookat = point3(0, 0, 0);
    // cam.vup = vec3(0, 1, 0);
    

    // // Lateral direita
    // cam.lookfrom = point3(6, 2, 0);
    // cam.lookat = point3(0, 0, 0);
    // cam.vup = vec3(0, 1, 0);
    

    // // Lateral traseira
    // cam.lookfrom = point3(0, 2, -6);
    // cam.lookat = point3(0, 0, 0);
    // cam.vup = vec3(0, 1, 0);
    

    // Vista superior
    cam.lookfrom = point3(0, 8, 0);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 0, -1);

    cam.defocus_angle = 2.0;
    cam.focus_dist = 6.0;

    cam.render(world);
}