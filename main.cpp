//
//  main.cpp
//  RayTracing
//
//  Created by 岑运池 on 2020/7/15.
//  Copyright © 2020 岑运池. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "light.h"

using namespace std;

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    lightModel *l_m = new lightModel(0.2, 5);
    light *l = new light(vec3(1.0, 1.0, 1.0), 5.0, 0.2);
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            float I_spec = l_m->phong(*l, r, rec);
            vec3 I_spec_v3 = vec3(I_spec, I_spec, I_spec);
            return 0.5*(I_spec_v3 + attenuation*color(scattered, world, depth+1));
        } else{
            return vec3(0, 0, 0);
        }
    }else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}


int main(int argc, const char * argv[]) {
    int nx = 800;
    int ny = 800;
    int ns = 100; // 在每个像素点附近随机采样次数（减少锯齿）

    ofstream out("out.ppm");
    out << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[4];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new metal(vec3(0.8, 0.3, 0.3), 0.9));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.2, 0.2, 0.2)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 0));

    hitable *world = new hitable_list(list, 4);

    camera cam;
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }

            col /= float (ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            out << ir << " " << ig << " " << ib << "\n";
        }
    }
    
    return 0;
}
