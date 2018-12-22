#ifndef FACE_H
#define FACE_H

#include "Hitable.h"
#include "Plane.h"
#include "LinAlgebra.h"
#include <array>
#include <iostream>
#include <vector>
#include <cassert>

class Face : public Hitable {
  private:
    Plane plane;
    std::vector<Vec3> planeBasisVertices;
    std::array<Vec3,3> basis;
  public:
    Face() {}
    Face(std::vector<Vec3> verts, Material *mat): vertices(verts), mat_ptr(mat) {
      //cant be a face if its a line xd
      assert(verts.size() >= 3);

      //setup
      Plane plane(verts[0], verts[1], verts[2], mat); 
      normal = plane.normal;
      point = plane.point;

      //create orthogonal basis vectors
      basis[0] = normalize(point-verts[1]);
      basis[1] = normalize(normal);
      basis[2] = normalize(cross(normal,basis[0]));

      //verify all pts are on the plane then change their basis
      for(Vec3 vert : vertices) {
        assert(("Points are not all on the same plane", plane.onSurface(vert)));
        planeBasisVertices.push_back(LinAlgebra::changeBasis(vert, point, basis));
      }
    }
    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
    std::vector<Vec3> vertices;
    Vec3 normal, point;
    Material *mat_ptr;
};

bool Face::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
  float denom = dot(r.direction(), normal);
  if(denom != 0) {
    float num1 = dot(normal, point);
    float num2 = dot(normal, r.origin());
    float temp = (num1 - num2) / denom;
    if(temp < t_max && temp > t_min) {
      Vec3 pointAtParam = r.point_at_parameter(temp);
      Vec3 pointInBasis = LinAlgebra::changeBasis(pointAtParam,point,basis);
      if(LinAlgebra::pointInPolygon(planeBasisVertices, pointInBasis)) {
        rec.t = temp;
        rec.p = pointAtParam;
        rec.normal = normal;
        rec.mat_ptr = mat_ptr;
        return true;
      }
    }
  }
  return false;
}

#endif
