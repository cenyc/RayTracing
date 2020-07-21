//
// Created by 岑运池 on 2020/7/20.
//

#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include "vec3.h"
#include "ray.h"
class light {
public:

    light(const vec3& origin, const float intensity, const float ambi):
    origin(origin), intensity(intensity), ambient(ambi) {}
    void setColor(const vec3& c) {color = c;}
    vec3 getOrigin() const {return origin;}
    float getIntensity() const {return intensity;}

    vec3 origin;    //光源位置
    float intensity;//光强度
    float ambient;  //环境光强度
    vec3 color;   //光颜色
};


#endif //RAYTRACING_LIGHT_H
