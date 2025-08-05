#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"
#include <memory>

class triangle : public hittable {
public:
    triangle(const point3& a, const point3& b, const point3& c, std::shared_ptr<material> mat)
        : v0(a), v1(b), v2(c), mat_ptr(mat) {}

    virtual ~triangle() = default;

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Möller–Trumbore intersection algorithm
        const double EPSILON = 1e-8;
        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;
        vec3 h = cross(r.direction(), edge2);
        double a = dot(edge1, h);
        if (std::abs(a) < EPSILON) return false;
        double f = 1.0 / a;
        vec3 s = r.origin() - v0;
        double u = f * dot(s, h);
        if (u < 0.0 || u > 1.0) return false;
        vec3 q = cross(s, edge1);
        double v = f * dot(r.direction(), q);
        if (v < 0.0 || u + v > 1.0) return false;
        double t = f * dot(edge2, q);
        if (t < ray_t.min || t > ray_t.max) return false;
        rec.t = t;
        rec.p = r.at(t);
        rec.normal = unit_vector(cross(edge1, edge2));
        rec.set_face_normal(r, rec.normal);
        rec.mat = mat_ptr;
        return true;
    }
private:
    point3 v0, v1, v2;
    std::shared_ptr<material> mat_ptr;
};

#endif
