#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3 {
  private:
    Vec3() {}
  public:
    float e[3];
    
    Vec3(float e0, float e1, float e2);
    
    inline float x() const;
    inline float y() const;
    inline float z() const;

    inline float r() const;
    inline float g() const;
    inline float b() const;

    inline const Vec3 & operator+() const;
    inline Vec3 operator-() const; 
    inline float operator[](int i) const;
    inline float& operator[](int i);

    inline Vec3& operator+=(const Vec3 &v2);
    inline Vec3& operator-=(const Vec3 &v2);
    inline Vec3& operator*=(const Vec3 &v2);
    inline Vec3& operator/=(const Vec3 &v2);
    inline Vec3& operator*=(const float t);
    inline Vec3& operator/=(const float t);

    inline float length() const;
    inline float squaredLength() const;
    inline void normalize();
};

#endif
