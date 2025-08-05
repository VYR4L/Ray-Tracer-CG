#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include <memory>
#include "quad.h"
#include "sphere.h"

using namespace std;

int main() {
    hittable_list world;

    // Materiais
    auto grad_lamb = make_shared<lambertian>(color(.65, .05, .05)); // gradiente
    auto proc_metal = make_shared<metal>(color(.8, .8, .9), 0.2); // tintura procedural
    auto ceramic_mat = make_shared<ceramic>(color(.85, .82, .75), 0.3, 0.25); // cerâmica
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto glass = make_shared<dielectric>(1.5);

    // Cornell box sides
    world.add(make_shared<quad>(point3(555,0,0), vec3(0,0,555), vec3(0,555,0), grad_lamb)); // verde
    world.add(make_shared<quad>(point3(0,0,555), vec3(0,0,-555), vec3(0,555,0), proc_metal)); // vermelho
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), ceramic_mat)); // branco cerâmico
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,0,-555), ceramic_mat)); // chão cerâmico
    world.add(make_shared<quad>(point3(555,0,555), vec3(-555,0,0), vec3(0,555,0), ceramic_mat)); // teto cerâmico

    // Luz superior
    world.add(make_shared<quad>(point3(213,554,227), vec3(130,0,0), vec3(0,0,105), light));

    // Objetos
    world.add(make_shared<sphere>(point3(190,90,190), 90, glass)); // esfera de vidro
    world.add(make_shared<sphere>(point3(370,120,370), 60, proc_metal)); // esfera metal
    world.add(make_shared<sphere>(point3(120,60,400), 60, grad_lamb)); // esfera gradiente
    world.add(make_shared<sphere>(point3(300,40,100), 40, ceramic_mat)); // esfera cerâmica
    world.add(make_shared<sphere>(point3(450,40,120), 40, proc_metal)); // esfera metal

    // Luzes para amostragem
    auto empty_material = shared_ptr<material>();
    hittable_list lights;
    lights.add(make_shared<quad>(point3(213,554,227), vec3(130,0,0), vec3(0,0,105), empty_material));
    lights.add(make_shared<sphere>(point3(190,90,190), 90, empty_material));

    camera cam;

    // Paisagem
    cam.aspect_ratio      = 1.5;
    cam.image_width       = 900;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;

    // POV 1
    // cam.lookfrom = point3(278, 278, -800);
    // cam.lookat   = point3(278, 278, 0);
    
    // POV 2
    cam.lookfrom = point3(800, 278, 278);
    cam.lookat   = point3(0, 278, 278);
    
    // POV 3
    // cam.lookfrom = point3(278, 800, 278);
    // cam.lookat   = point3(278, 0, 278);
    
    // POV 4
    // cam.lookfrom = point3(278, 278, 800);
    // cam.lookat   = point3(278, 278, 0);
    
    // POV 5
    // cam.lookfrom = point3(50, 500, -400);
    // cam.lookat   = point3(278, 278, 0);

    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world, lights);
}