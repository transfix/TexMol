#ifndef _VEC_H_
#define _VEC_H_

#include <cmath>
#include <cstring>

#include <algorithm>
#include <iostream>

namespace CVC {

// typedef unsigned short ushort;
typedef unsigned char myushort;

template <class NumType, myushort NumDims>
struct cvcvec {
  static const myushort Dim = NumDims;
  static const myushort D = NumDims;
  NumType x[NumDims];

  cvcvec(NumType a = 0, NumType b = 0, NumType c = 0, NumType d = 0) {
    NumType v[] = { a, b, c, d };
    memcpy(x, v, std::min(NumDims, static_cast<myushort>(4)) * sizeof(a));
  }

  cvcvec(const cvcvec<NumType, NumDims-1>& v, NumType a = 0) {
    for (int i = 0; i < NumDims-1; ++i) {
      x[i] = v[i];
    }
    x[NumDims-1] = a;
  }

  explicit cvcvec(NumType* values) {
    memcpy(x, values, NumDims * sizeof(values[0]));
  }

  static cvcvec<NumType, NumDims> zero() {
    cvcvec<NumType, NumDims> z;
    memset(z.x, 0, sizeof(NumType)*NumDims);
    return z;
  }
  void copy(const NumType* y) {
    memcpy(x, y, sizeof(NumType)*NumDims);
  }
  void copy(const cvcvec<NumType, NumDims>& v) {
    memcpy(x, v.x, sizeof(NumType)*NumDims);
  }
  NumType norm2() const {
    return (*this)*(*this);
  }
  NumType norm() const {
    return sqrt(norm2());
  }
  cvcvec<NumType, NumDims> unit() const {
    return (*this)/norm();
  }
  bool operator==(NumType a) const {
    bool eq = true;
    for (int i = 0; i < NumDims && (eq &= (x[i] == a)); ++i);
    return eq;
  }
  bool operator==(const cvcvec<NumType, NumDims>& rhs) const {
    bool eq = true;
    for (int i = 0; i < NumDims && (eq &= (x[i] == rhs.x[i])); i++);
    return eq;
  }
  bool operator!=(const cvcvec<NumType, NumDims>& rhs) const {
    return !(*this == rhs);
  }

  NumType operator*(const cvcvec<NumType, NumDims>& rhs) const {
    NumType result = NumType(0);
    for (int i = 0; i < NumDims; i++) result += x[i]*rhs.x[i];
    return result;
  }
  // This implementation fails on integer types
  // cvcvec<NumType, NumDims> operator/(NumType a) const {
  //   cvcvec<NumType, NumDims> result;
  //   const NumType inv = 1.0 / a;
  //   for (int i = 0; i < NumDims; i++) result.x[i] = x[i] * inv;
  //   return result;
  // }
  cvcvec<NumType, NumDims> operator/(NumType a) const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) result.x[i] = x[i] / a;
    return result;
  }
  cvcvec<NumType, NumDims> operator*(NumType a) const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) result.x[i] = x[i]*a;
    return result;
  }
  cvcvec<NumType, NumDims> operator+(const cvcvec<NumType, NumDims>& rhs) const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) result.x[i] = x[i]+rhs.x[i];
    return result;
  }
  cvcvec<NumType, NumDims> operator+(NumType a) const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) result.x[i] = x[i]+a;
    return result;
  }
  cvcvec<NumType, NumDims> operator-(const cvcvec<NumType, NumDims>& rhs) const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) result.x[i] = x[i]-rhs.x[i];
    return result;
  }
  cvcvec<NumType, NumDims> operator-(NumType a) const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) result.x[i] = x[i]-a;
    return result;
  }
  cvcvec<NumType, NumDims> operator-() const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) result.x[i] = -x[i];
    return result;
  }
  cvcvec<NumType, NumDims>& operator+=(const cvcvec<NumType, NumDims>& rhs) {
    return *this=(*this)+rhs;
  }
  cvcvec<NumType, NumDims>& operator+=(const NumType rhs) {
    return *this=(*this)+rhs;
  }
  cvcvec<NumType, NumDims>& operator-=(const cvcvec<NumType, NumDims>& rhs) {
    return *this=(*this)-rhs;
  }
  cvcvec<NumType, NumDims>& operator-=(const NumType rhs) {
    return *this=(*this)-rhs;
  }
  cvcvec<NumType, NumDims>& operator*=(NumType a) {
    return *this=(*this)*a;
  }
  cvcvec<NumType, NumDims>& operator/=(NumType a) {
    return *this=(*this)/a;
  }
  cvcvec<NumType, NumDims> operator/(const cvcvec<NumType, NumDims> rhs) const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; ++i)
      result[i] = x[i] / rhs.x[i];
    return result;
  }

  cvcvec<NumType, NumDims> operator^(const cvcvec<NumType, NumDims>& rhs) const {
    cvcvec<NumType, NumDims> result;
    if (NumDims >= 3) {
      result.x[0] = x[1]*rhs.x[2]-x[2]*rhs.x[1];
      result.x[1] = x[2]*rhs.x[0]-x[0]*rhs.x[2];
      result.x[2] = x[0]*rhs.x[1]-x[1]*rhs.x[0];
    }
    return result;
  }

  cvcvec<NumType, NumDims>& operator^=(const cvcvec<NumType, NumDims>& rhs) {
    return *this=(*this)^rhs;
  }

  bool operator<(NumType rhs) const {
    bool lt = true;
    for (int i = 0; i < NumDims && (lt = (x[i] < rhs)); i++);
    return lt;
  }

  bool operator<(const cvcvec<NumType, NumDims>& rhs) const {
    // bool lt = true;
    // for (int i = 0; i < NumDims && (lt = (x[i] < rhs[i])); i++);
    for (int i = 0; i < NumDims; ++i) {
      if (x[i] < rhs[i]) return true;
      if (x[i] > rhs[i]) return false;
    }
    // return lt;
    return false;
  }

  inline NumType &operator[](int _n) {
    return x[_n];
  }

  inline NumType operator[](int _n) const {
    return x[_n];
  }

  operator const NumType*() const {
    return &x[0];
  }

  operator const cvcvec<float, NumDims>() const {
    cvcvec<float, NumDims> v;
    for (int i = 0; i < NumDims; ++i) v[i] = x[i];
    return v;
  }

  operator const cvcvec<double, NumDims>() const {
    cvcvec<double, NumDims> v;
    for (int i = 0; i < NumDims; ++i) v[i] = x[i];
    return v;
  }

  operator const cvcvec<int, NumDims>() const {
    cvcvec<int, NumDims> v;
    for (int i = 0; i < NumDims; ++i) v[i] = static_cast<int>(x[i]);
    return v;
  }

  static cvcvec<NumType, NumDims> min(
      const cvcvec<NumType, NumDims>& a, const cvcvec<NumType, NumDims>& b) {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) {
      result.x[i] = (a.x[i] < b.x[i] ? a.x[i] : b.x[i]);
    }
    return result;
  }

  static cvcvec<NumType, NumDims> max(
      const cvcvec<NumType, NumDims>& a, const cvcvec<NumType, NumDims>& b) {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; ++i) {
      result.x[i]=(a.x[i]>b.x[i]?a.x[i]:b.x[i]);
    }
    return result;
  }

  cvcvec<NumType, NumDims> abs() const {
    cvcvec<NumType, NumDims> result;
    for (int i = 0; i < NumDims; i++) {
      result.x[i]=(x[i]>0?x[i]:-x[i]);
    }
    return result;
  }

  NumType min() const {
    NumType result = x[0];
    for (int i = 0; i < NumDims; i++) {
      result= (result < x[i] ? result : x[i]);
    }
    return result;
  }


  NumType max() const {
    NumType result = x[0];
    for (int i = 0; i < NumDims; i++) {
      result = (result>x[i]?result:x[i]);
    }
    return result;
  }

  template <class NType, myushort NDims>
  friend std::ostream& operator<<(
      std::ostream& out, const cvcvec<NType, NDims>& v);
  template <class NType, myushort NDims>
  friend cvcvec<NType, NDims> operator*(NType a, const cvcvec<NType, NDims>& rhs);
};

template<class NType, myushort NDims>
cvcvec<NType, NDims> operator*(NType a, const cvcvec<NType, NDims>& rhs) {
  return rhs*a;
}

template<class NType, myushort NDims>
std::ostream& operator<<(std::ostream& out, const cvcvec<NType, NDims>& v) {
  out << "(";
  for (int i = 0; i < NDims; i++) {
    out << v.x[i];
    if (i < NDims-1) out << ",";
  }
  out << ")";
  return out;
}

typedef cvcvec<int, 2> Vec2i;
typedef cvcvec<float, 2> Vec2f;
typedef cvcvec<double, 2> Vec2d;
typedef cvcvec<unsigned char, 2> Vec2b;

typedef cvcvec<int, 3> Vec3i;
typedef cvcvec<float, 3> Vec3f;
typedef cvcvec<double, 3> Vec3d;
typedef cvcvec<unsigned char, 3> Vec3b;

typedef cvcvec<int, 4> Vec4i;
typedef cvcvec<float, 4> Vec4f;
typedef cvcvec<double, 4> Vec4d;
typedef cvcvec<unsigned char, 4> Vec4b;

}

#endif
