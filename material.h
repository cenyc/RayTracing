//
// Created by 岑运池 on 2020/7/17.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "ray.h"
#include "hitable_list.h"
#include "light.h"


vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    }while (p.squared_length() >= 1.0);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v, n)*n;
}

class material {
public:
    virtual bool scatter(const ray& r, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

/**
 * Lambert光照模型
 */
class lambertian: public material {
public:
    lambertian(const vec3& a): albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

/**
 * 金属材质光照模型
 */
class metal: public material {
public:
    metal(const vec3& a, float f): albedo(a) { fuzz = f < 1? f:1;};
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
    float fuzz;
};

class lightModel {
public:
    lightModel(float kIn, float nsIn): k(kIn), ns(nsIn) {};
    float phong(const light &light, const ray &ray_in, const hit_record &record);

    float k; //材质的反光系数
    float ns; //高光系数
};

float lightModel::phong(const light &light, const ray &ray_in,const hit_record &record) {
    vec3 l = light.getOrigin() - record.p;
    l.make_unit_vector();
    vec3 r = 2*dot(record.normal, l)*record.normal - l; //反射光方向
    float I = k*light.getIntensity()*pow(dot(-unit_vector(ray_in.direction()), unit_vector(r)), ns);

    if (I > 1) {
        I = 1.0;
    } else if (I < 0) {
        I = 0.0009;
    }
    return I;
};

#endif //RAYTRACING_MATERIAL_H
