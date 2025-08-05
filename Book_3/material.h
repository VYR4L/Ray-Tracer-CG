
#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include "hittable.h"
#include "ray.h"
#include "color.h"
#include "pdf.h"
#include "camera.h"
#include "texture.h"

class scatter_record
{
public:
    color attenuation;
    std::shared_ptr<pdf> pdf_ptr;
    bool skip_pdf;
    ray skip_pdf_ray;
};

class material
{
public:
    virtual ~material() = default;

    virtual color emitted(
        const ray &r_in, const hit_record &rec, double u, double v, const point3 &p) const
    {
        return color(0, 0, 0);
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const {
        return false;
    }

    virtual double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered)
        const
    {
        return 0;
    }
};

class lambertian : public material
{
public:
    lambertian(const color &albedo) : tex(std::make_shared<solid_color>(albedo)) {}
    lambertian(std::shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        // Gradiente de cor baseado na posição Y
        double gradient = std::clamp(rec.p.y() / 555.0, 0.0, 1.0);
        color grad_color = lerp(tex->value(rec.u, rec.v, rec.p), color(1,1,1), gradient);
        srec.attenuation = grad_color;
        srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal);
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override {
        auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
        return cos_theta < 0 ? 0 : cos_theta/pi;
    }

private:
    color lerp(const color& a, const color& b, double t) const {
        return a*(1-t) + b*t;
    }


private:
    std::shared_ptr<texture> tex;
};

class metal : public material
{
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        // Tintura procedural e rugosidade variável conforme posição
        double tint = std::sin(rec.p.x() * 0.05) * 0.5 + 0.5;
        color tint_color = lerp(albedo, color(0.8,0.8,1.0), tint);
        double variable_fuzz = std::clamp(fuzz + rec.p.z()/555.0, 0.0, 1.0);
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.attenuation = tint_color;
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        srec.skip_pdf_ray = ray(rec.p, reflected + variable_fuzz*random_unit_vector(), r_in.time());
        return (dot(srec.skip_pdf_ray.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
    color lerp(const color& a, const color& b, double t) const {
        return a*(1-t) + b*t;
    }
};

// Novo material: Cerâmica
class ceramic : public material {
public:
    ceramic(const color& base, double roughness = 0.3, double shine = 0.2)
        : base_color(base), roughness(roughness), shine(shine) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        // Cerâmica: cor base, leve brilho, rugosidade média
        double highlight = std::pow(std::max(dot(unit_vector(r_in.direction()), rec.normal), 0.0), 20.0);
        color final_color = lerp(base_color, color(1,1,1), shine * highlight);
        srec.attenuation = final_color;
        srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal);
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override {
        auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine/pi;
    }

private:
    color base_color;
    double roughness;
    double shine;
    color lerp(const color& a, const color& b, double t) const {
        return a*(1-t) + b*t;
    }
};

class dielectric : public material
{
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        srec.attenuation = color(1.0, 1.0, 1.0);
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        srec.skip_pdf_ray = ray(rec.p, direction, r_in.time());
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index;

    static double reflectance(double cosine, double refraction_index)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class diffuse_light : public material
{
public:
    diffuse_light(std::shared_ptr<texture> tex) : tex(tex) {}
    diffuse_light(const color &emit) : tex(std::make_shared<solid_color>(emit)) {}

    color emitted(const ray &r_in, const hit_record &rec, double u, double v, const point3 &p)
        const override
    {
        if (!rec.front_face)
            return color(0, 0, 0);
        return tex->value(u, v, p);
    }

private:
    std::shared_ptr<texture> tex;
};

class isotropic : public material
{
public:
    isotropic(const color &albedo) : tex(std::make_shared<solid_color>(albedo)) {}
    isotropic(std::shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        srec.attenuation = tex->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<sphere_pdf>();
        srec.skip_pdf = false;
        return true;
    }


private:
    std::shared_ptr<texture> tex;
};

#endif