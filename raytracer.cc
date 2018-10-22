#include <iostream>
#include "Camera.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Material.h"
#include <cfloat>

Vec3 color(const Ray& r, Hitable *world, int depth) {
  HitRecord rec;
  if(world->hit(r, 0.001, FLT_MAX, rec)) {
    Ray scattered;
    Vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation*color(scattered, world, depth+1);
    } else {
      return Vec3(0,0,0);
    }
  } else {
    // background color
    Vec3 unit_direction = normalize(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
  }
}

int main() {
    int nx = 800;
    int ny = 400;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    //objects in scene
    
    //Hitable *list[4];
    //list[0] = new Sphere(Vec3(0,0,-1), 0.5, new Dielectric(1.5));
    //list[1] = new Sphere(Vec3(0.75,-0.25,-1.25),0.25, new Metal(Vec3(0.8, 0.8, 0.8), 0.3));
    //list[2] = new Sphere(Vec3(-0.6,-0.25,-2.0),0.25, new Lambertian(Vec3(0.5, 1.0, 0.5)));
    //list[3] = new Sphere(Vec3(0,-100.5,-1), 100, new Lambertian(Vec3(0.7, 0.3, 0.3)));
    //Hitable *world = new HitableList(list, 4);

    Hitable *list[2];
    float R = cos(M_PI/4);
    list[0] = new Sphere(Vec3(-R, 0, -1), R,  new Lambertian(Vec3(0,0,1)));
    list[1] = new Sphere(Vec3(R, 0, -1), R,  new Lambertian(Vec3(1,0,0)));
    Hitable *world = new HitableList(list, 2);

    Camera cam(90, 2);
    for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
        Vec3 col(0, 0, 0);
        for(int s = 0; s <  ns; s++) {
          float u = float(i + drand48()) / float(nx);
          float v = float(j + drand48()) / float(ny);
          Ray r = cam.get_ray(u, v);
          col += color(r, world, 0);
        }
        col /= float(ns);
        col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); 
        int ir = int(255.99*col[0]);
        int ig = int(255.99*col[1]);
        int ib = int(255.99*col[2]);
        
        std::cout << ir << " " << ig << " " << ib << "\n";
      }
    }
}
