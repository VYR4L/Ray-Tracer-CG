 #ifndef MATERIAL_H
 #define MATERIAL_H

 #include <memory>
 #include "hittable.h"
 #include "ray.h"
 #include "color.h"
 #include "texture.h"

class material
{
public:
    virtual ~material() = default;

    virtual color emitted(double u, double v, const point3 &p) const
    {
        return color(0, 0, 0);
    }

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        return false;
    }
};

class lambertian : public material
{
public:
    lambertian(const color &albedo) : tex(std::make_shared<solid_color>(albedo)) {}
    lambertian(std::shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero()) scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        // Gradiente de cor baseado na posição Y
        double t = 0.5 * (rec.p.y() + 1.0) / 100.0;
        color grad = tex->value(rec.u, rec.v, rec.p) * (1.0 - t) + color(0.2, 0.5, 0.8) * t;
        attenuation = grad;
        return true;
    }

private:
    std::shared_ptr<texture> tex;
};

class metal : public material
{
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        // Tintura procedural: cor varia com X
        color tint = albedo * (0.7 + 0.3 * std::sin(rec.p.x() * 0.05));
        double local_fuzz = fuzz * (0.5 + 0.5 * std::abs(std::sin(rec.p.z() * 0.1)));
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (local_fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected, r_in.time());
        attenuation = tint;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material
{
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        attenuation = color(1.0, 1.0, 1.0);
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

        scattered = ray(rec.p, direction, r_in.time());
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

    color emitted(double u, double v, const point3 &p) const override
    {
        return tex->value(u, v, p);
    }

private:
        std::shared_ptr<texture> tex;
};

class isotropic : public material {
  public:
    isotropic(const color& albedo) : tex(std::make_shared<solid_color>(albedo)) {}
    isotropic(std::shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        scattered = ray(rec.p, random_unit_vector(), r_in.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

  private:
    std::shared_ptr<texture> tex;
};

class suede : public material {
public:
    suede(const color& base, double roughness = 0.5)
        : base_color(base), roughness(roughness) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        // Camurça: difuso com ruído procedural e rugosidade
        auto scatter_direction = rec.normal + roughness * random_unit_vector();
        if (scatter_direction.near_zero()) scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction, r_in.time());
        // Ruído simples baseado na posição
        double noise = 0.7 + 0.3 * std::sin(rec.p.x() * 0.2 + rec.p.y() * 0.2 + rec.p.z() * 0.2);
        double facing = std::max(0.0, dot(rec.normal, unit_vector(-r_in.direction())));
        attenuation = base_color * noise * (0.7 + 0.3 * facing);
        return true;
    }
private:
    color base_color;
    double roughness;
};

#endif