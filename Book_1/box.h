
#ifndef BOX_H
#define BOX_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"
#include <algorithm>
#include <memory>


class box : public hittable {
public:
    box(const point3& min_corner, const point3& max_corner, std::shared_ptr<material> mat)
        : min_corner(min_corner), max_corner(max_corner), mat_ptr(mat) {}

    virtual ~box() = default;

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        double tmin = ray_t.min, tmax = ray_t.max;
        for (int a = 0; a < 3; a++) {
            double invD = 1.0 / r.direction()[a];
            double t0 = (min_corner[a] - r.origin()[a]) * invD;
            double t1 = (max_corner[a] - r.origin()[a]) * invD;
            if (invD < 0.0) std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin) return false;
        }
        rec.t = tmin;
        rec.p = r.at(rec.t);
        // Simple normal calculation (not robust for corners/edges)
        for (int a = 0; a < 3; a++) {
            if (std::abs(rec.p[a] - min_corner[a]) < 1e-4)
                rec.normal = -vec3((a==0), (a==1), (a==2));
            if (std::abs(rec.p[a] - max_corner[a]) < 1e-4)
                rec.normal = vec3((a==0), (a==1), (a==2));
        }
        rec.set_face_normal(r, rec.normal);
        rec.mat = mat_ptr;
        return true;
    }
private:
    point3 min_corner, max_corner;
    std::shared_ptr<material> mat_ptr;
};

#endif
