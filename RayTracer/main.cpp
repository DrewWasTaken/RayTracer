#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "numgen.h"
#include "material.h"


using namespace std;

vec3 color(const ray &r, hitable *world, int depth) //Blends White + Blue, Dependant On Y Axis
{
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) //0.001 Cleans Up Near 0 Hits
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable* random_scene()
{
    int n = 500;
    hitable **list = new hitable* [n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = rand_float();
                vec3 center(a + 0.9 * rand_float(), 0.2, b + 0.9 * rand_float());
                if ((center - vec3(4, 0.2, 0)).length() > 0.9)
                {
                    if (choose_mat < 0.8) //Diffuse
                    {
                        list[i++] = new sphere(center, 0.2,
                            new lambertian(vec3(rand_float() * rand_float(),
                                rand_float() * rand_float(),
                                rand_float() * rand_float()))
                        );
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
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i);

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

    //std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable* world = random_scene();

    //Adjust Camera Viewpoint
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

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