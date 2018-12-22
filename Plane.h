#ifndef PLANE_H
#define PLANE_H

#include "Hitable.h"
#include <array>

class Plane : public Hitable {
  public:
    Plane() {}
    Plane(Vec3 p, Vec3 n, Material *mat): point(p), normal(normalize(n)), mat_ptr(mat) {}
    Plane(Vec3 a, Vec3 b, Vec3 c, Material *mat): point(a), normal(cross(a-b,a-c)), mat_ptr(mat) {}
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    bool onSurface(Vec3 v) const;
    Vec3 point, normal;
    Material *mat_ptr;
};

bool Plane::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
  float denom = dot(r.direction(), normal);
  if(denom != 0) {
    float num1 = dot(normal, point);
    float num2 = dot(normal, r.origin());
    float temp = (num1 - num2) / denom;
    if(temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = normal;
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}

bool Plane::onSurface(Vec3 v) const {
  if(dot(normal, v-point) == 0) {
    return true;
  }
  return false;
}

#endif
