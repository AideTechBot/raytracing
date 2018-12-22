#include <iostream>
#include "Camera.h"
#include "Sphere.h"
#include "Face.h"
#include "Plane.h"
#include "HitableList.h"
#include "Material.h"
#include <vector>
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

Hitable *randomscene() {
  int n = 500;
  Hitable **list = new Hitable*[n+1];
  list[0] = new Plane(Vec3(0,0,0), Vec3(0,1,0), new Metal(Vec3(0.7,0.6,0.5),0.1));
  int i = 1;
  //for(int a = -11; a < 11; a++) {
  //  for(int b = -11; b < 11; b++) {
  //    float choose_mat = drand48();
  //    Vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
  //    if ((center-Vec3(4,0.2,0)).length() > 0.9) {
  //      if (choose_mat < 0.8) { // diffuse
  //        list[i++] = new Sphere(center, 0.2, new Lambertian(Vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
  //      } else if (choose_mat < 0.95) { // metal
  //        list[i++] = new Sphere(center, 0.2, new Metal(Vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
  //      } else { // glass
  //        list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
  //      }
  //    }
  //  }
  //}

  list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
  list[i++] = new Sphere(Vec3(-2, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
  list[i++] = new Sphere(Vec3(2, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
  std::vector<Vec3> poly;
  poly.push_back(Vec3(-4,1,-3));
  poly.push_back(Vec3(-4,4,0));
  poly.push_back(Vec3(-4,1,3));
  list[i++] = new Face(poly,new Lambertian(Vec3(0.1, 0.1, 0.1)));
  return new HitableList(list, i);
}

void loadingBar(double progress) {
  int barWidth = 70;

  std::cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos) std::cout << "=";
    else if (i == pos) std::cout << ">";
    else std::cout << " ";
  }
  std::cout << "] " << progress * 100.0 << " %\r";
  std::cout.flush();
}

int main() {
    system("setterm -cursor off");
    
    // configuration
    int nx = 600;
    int ny = 300;
    bool antialiasing = false;
    int ns = 50;
    
    std::cerr << "P3\n" << nx << " " << ny << "\n255\n";

    //objects in scene
    
    //Hitable *list[4];
    //list[0] = new Sphere(Vec3(0,0,-1), 0.5, new Dielectric(1.5));
    //list[1] = new Sphere(Vec3(1.0,-0.25,-1.0),0.25, new Metal(Vec3(0.8, 0.8, 0.8), 0.3));
    //list[2] = new Sphere(Vec3(-0.6,-0.25,-1.50),0.25, new Lambertian(Vec3(0.5, 1.0, 0.5)));
    //list[3] = new Sphere(Vec3(0,-100.5,-1), 100, new Lambertian(Vec3(0.7, 0.3, 0.3)));
    Hitable *world = randomscene();

    //Hitable *list[2];
    //float R = cos(M_PI/4);
    //list[0] = new Sphere(Vec3(-R, 0, -1), R,  new Lambertian(Vec3(0,0,1)));
    //list[1] = new Sphere(Vec3(R, 0, -1), R,  new Lambertian(Vec3(1,0,0)));
    //Hitable *world = new HitableList(list, 2);
    
    Vec3 lookfrom(6,1.75,4);
    Vec3 lookat(0, 1, 0);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 0.5;

    Camera cam(lookfrom, lookat, Vec3(0,1,0), 40, float(nx)/float(ny), aperture, dist_to_focus);
    long total = nx * ny * (antialiasing ? ns : 1);
    long current = 0;
    for (int j = ny-1; j >= 0; j--) {
      for (int i = 0; i < nx; i++) {
        Vec3 col(0, 0, 0);
        if(antialiasing) {
          for(int s = 0; s <  ns; s++) {
            float u = float(i + drand48()) / float(nx);
            float v = float(j + drand48()) / float(ny);
            Ray r = cam.get_ray(u, v);
            col += color(r, world, 0);

            loadingBar(double(current++)/double(total));
            //std::cout << current++ << "/" << total << "\r";
            //std::cout.flush();
          }
          col /= float(ns);
        } else {
          float u = float(i) / float(nx);
          float v = float(j) / float(ny);
          Ray r = cam.get_ray(u, v);
          col += color(r, world, 0);

          loadingBar(double(current++)/double(total));
        }
        col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); 
        int ir = int(255.99*col[0]);
        int ig = int(255.99*col[1]);
        int ib = int(255.99*col[2]);
        
        std::cerr << ir << " " << ig << " " << ib << "\n";

      }
    }
    std::cout << std::endl;
    system("setterm -cursor on");
}
