#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "aarect.h"
#include "camera.h"
#include "float.h"
#include "hittable_list.h"
#include "material.h"
#include "moving_sphere.h"
#include "numgen.h"
#include "sphere.h"


using namespace std;

vec3 color(const ray& r, hittable* world, int depth) //Blends White + Blue, Dependant On Y Axis
{
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) //0.001 Cleans Up Near 0 Hits
    {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * color(scattered, world, depth + 1);
        else
            return emitted;
    }
    else
        return vec3(0, 0, 0);
}


hittable* cornell_box()
{
    hittable** list = new hittable * [6];
    int i = 0;
    material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));

    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

    return new hittable_list(list, i);
}


hittable* simple_light()
{
    texture* checker = new checker_texture(
        new constant_texture(vec3(0.2, 0.3, 0.1)),
        new constant_texture(vec3(0.9, 0.9, 0.9))
    );
    //color* pertext = new noise_texture(4);
    hittable** list = new hittable * [4];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(checker));
    list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
    list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
    return new hittable_list(list, 4);
}

hittable* two_spheres()
{
    texture* checker = new checker_texture(
        new constant_texture(vec3(0.2, 0.3, 0.1)),
        new constant_texture(vec3(0.9, 0.9, 0.9))
        );

    int n = 50;
    hittable** list = new hittable * [n + 1];
    list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
    list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
    
    return new hittable_list(list, 2);
}



hittable *random_scene()
{
    int n = 50000;
    hittable **list = new hittable*[n + 1];

    texture* checker = new checker_texture(
        new constant_texture(vec3(0.2, 0.3, 0.1)),
        new constant_texture(vec3(0.9, 0.9, 0.9))
    );

    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
    int i = 1;
    for (int a = -10; a < 10; a++)
    {
        for (int b = -10; b < 10; b++)
        {
            float choose_mat = rand_float();
                vec3 center(a + 0.9 * rand_float(), 0.2, b + 0.9 * rand_float());
                if ((center - vec3(4, 0.2, 0)).length() > 0.9)
                {
                    if (choose_mat < 0.8) //Diffuse
                    {
                        list[i++] = new moving_sphere(center, center+vec3(0,0.5 *rand_float(), 0), 0.0, 1.0, 0.2,
                            new lambertian((new constant_texture(vec3(rand_float() * rand_float(),
                                rand_float() * rand_float(),
                                rand_float() * rand_float())))));
                    }
                    else if (choose_mat < 0.95) //Metallic
                    {
                        list[i++] = new sphere(center, 0.2,
                            new metal(vec3(0.5 * (1 + rand_float()),
                                0.5 * (1 + rand_float()),
                                0.5 * (1 + rand_float())),
                                0.5 * rand_float())
                        );
                    }
                    else //Glass
                    {
                        list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                    }
                }
        }
    }


    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.5, 0.5, 0.5))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hittable_list(list, i);

}


int main()
{
    //sf::RenderWindow window(sf::VideoMode(640, 480), "Ray Tracer");

    //while (window.isOpen())
    //{

    //    //handle events

    //    //update 

    //    window.clear();

    //    // draw objects 

    //    window.display();


    //}

    int nx = 700;
    int ny = 350;
    int ns = 100;

    ofstream img("picture.ppm");
    img << "P3" << endl;
    img << nx << " " << ny << endl;
    img << "255" << endl;

    //---------------------\\
       Select World To Run
    //---------------------\\
    
    //hittable* world = random_scene();

    //hittable* world = two_spheres();

    //hittable* world = simple_light();

    hittable* world = cornell_box();

    //Adjust Camera Viewpoint
    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278, 278, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;

    camera cam(lookfrom, lookat, vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + rand_float()) / float(nx);
                float v = float(j + rand_float()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.position(2.0);
                col += color(r, world, 0);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            img << ir << " " << ig << " " << ib << endl;

        }
    }
}