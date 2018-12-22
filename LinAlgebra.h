#ifndef LINALG_H
#define LINALG_H

#include "Vec3.h"
#include <cassert>
#include <vector>
#include <array>

class LinAlgebra {
  public:
    static Vec3 changeBasis(Vec3 v, Vec3 p, std::array<Vec3, 3> basis);
    static bool pointInPolygon(std::vector<Vec3> polygon, Vec3 point);
};

// changes basis in a 3 dimension coord system 
Vec3 LinAlgebra::changeBasis(Vec3 v, Vec3 p, std::array<Vec3, 3> basis) {
  // basis vectors all need to orthogonal
  for(int i = 0; i < 3; ++i) {
    float threshold = 0.000001;
    float dotbasis = dot(basis[i], basis[(i+1) % 3]);
    assert(dotbasis > threshold || -threshold < dotbasis); 
  }
  
  Vec3 result;
  for(int i = 0; i < 3; ++i) {
    float num = dot(basis[i],v-p);
    float denom = dot(basis[i],basis[i]);
    result[i] = num / denom;
  }
  return result; 
}

bool LinAlgebra::pointInPolygon(std::vector<Vec3> polygon, Vec3 point) {
  int i,j = polygon.size()-1;
  bool oddNodes=false;
  std::vector<float> polyX, polyY;
  
  for(Vec3 v : polygon) {
    polyX.push_back(v[0]);
    polyY.push_back(v[2]);
    //std::cout << "(" <<  v[0] << "," << v[1] << "," << v[2] << ")" << std::endl;
  }
  //std::cout << "______________________________________" << std::endl;

  for (i=0; i<(int)polygon.size(); i++) {
    if ((polyY[i]<point.z() && polyY[j]>=point.z()) || (polyY[j]<point.z() && polyY[i]>=point.z())) {
      if (polyX[i]+(point.z()-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<point.x()) {
        oddNodes=!oddNodes; 
      }   
    }   
    j=i; 
  }
  return oddNodes; 
}


#endif
