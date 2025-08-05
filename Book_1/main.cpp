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
    auto wood_material = make_shared<wood>(color(0.55, 0.27, 0.07), color(0.35, 0.16, 0.07), 8.0);
    auto button_material = make_shared<lambertian>(color(0.2, 0.7, 0.3), color(0.8, 0.3, 0.1));
    auto arrow_material = make_shared<metal>(color(0.7, 0.7, 0.7), 0.2, color(0.3, 0.5, 0.8));
    auto analog_material = make_shared<lambertian>(color(0.8, 0.8, 0.1));

    // Corpo do controle (caixas)
    // Esquerda
    world.add(make_shared<box>(point3(-3.5, -1.0, -1.0), point3(-1.5, 1.0, 1.0), wood_material));
    // Direita
    world.add(make_shared<box>(point3(1.5, -1.0, -1.0), point3(3.5, 1.0, 1.0), wood_material));
    // Centro
    world.add(make_shared<box>(point3(-1.5, -0.5, -1.0), point3(1.5, 0.5, 1.0), wood_material));

    // Analógicos (esferas grandes) - elevando acima do corpo, material amarelo
    world.add(make_shared<sphere>(point3(-1.3, 1.1, 0.7), 0.5, analog_material));
    world.add(make_shared<sphere>(point3(1.3, 1.1, 0.7), 0.5, analog_material));

    // Botões (esferas pequenas) - elevando acima do corpo
    world.add(make_shared<sphere>(point3(-3.0, 1.5, 0.5), 0.2, button_material));
    world.add(make_shared<sphere>(point3(-2.5, 1.5, 0.0), 0.2, button_material));
    world.add(make_shared<sphere>(point3(-3.0, 1.5, -0.5), 0.2, button_material));
    world.add(make_shared<sphere>(point3(-2.5, 1.5, -0.5), 0.2, button_material));

    // Setas (triângulos) - elevando acima do corpo
    // Cima
    world.add(make_shared<triangle>(
        point3(3.0, 1.2, 0.0),
        point3(3.3, 1.0, 0.0),
        point3(2.7, 1.0, 0.0),
        arrow_material));
    // Baixo
    world.add(make_shared<triangle>(
        point3(3.0, 0.8, 0.0),
        point3(3.3, 1.0, 0.0),
        point3(2.7, 1.0, 0.0),
        arrow_material));
    // Esquerda
    world.add(make_shared<triangle>(
        point3(2.5, 1.0, 0.3),
        point3(2.5, 1.2, 0.0),
        point3(2.5, 0.8, 0.0),
        arrow_material));
    // Direita
    world.add(make_shared<triangle>(
        point3(3.5, 1.0, 0.3),
        point3(3.5, 1.2, 0.0),
        point3(3.5, 0.8, 0.0),
        arrow_material));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // POVs
    struct POV
    {
        double vfov;
        point3 lookfrom;
        point3 lookat;
        vec3 vup;
        double defocus_angle;
        double focus_dist;
        std::string name;
    };

    cam.vfov = 70;
    cam.lookfrom = point3(0, 4, 0);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 0, -1);
    cam.defocus_angle = 5.0;
    cam.focus_dist = 4.0;
    cam.render(world);
}