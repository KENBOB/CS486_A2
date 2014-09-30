//   $ID: haney.ken_asgt2
//    File Name: psurfviewer_main.cpp
//       Author: Ken Haney, kenbob.21@csu.fullerton.edu
// Assignment 2: Transformations and Simple Shading
//         Date: 04/25/2013
// Description: contains the psurfviewer_main routines
// 

// $Id: Vec3.h 4095 2013-02-25 07:39:08Z mshafae $
#ifndef _MSGFX_VEC3_H_
#define _MSGFX_VEC3_H_

#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>

#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288
#endif

#ifndef DEG2RAD
#define DEG2RAD( theta ) ((theta) * (M_PI/180.0))
#endif

#ifndef RAD2DEG
#define RAD2DEG( theta ) ((theta) * (180.0/M_PI))
#endif

#ifndef ABS
#define ABS( x ) ((x) < (0) ? (-x) : (x))
#endif

#ifndef SQR
#define SQR( x ) ((x) * (x))
#endif

#ifndef MAX
#define MAX( a, b ) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN( a, b ) ((a) < (b) ? (a) : (b))
#endif

#ifndef MSGFX_FP
#define MSGFX_FP

#define FP_EPSILON 1e-9
#define ALMOST_ZERO FP_EPSILON

#define FP_EQUAL(a, b, delta) ( ((a) == (b)) || \
                              ( (a)-(delta) < (b) && \
                                (a)+(delta) > (b)) )

#define FP_LT(a, b, delta)  (fabs((a) - (b)) < (delta))

#define FP_GT(a, b, delta)  (fabs((a) - (b)) > (delta))

#define IOS_FP_PRECISION 5
#endif

#ifndef XYZW
//  enum{X = 0, Y, Z, W};
#endif

class Vec3{
private:
  double components[3];
public:
  Vec3( );
  Vec3( const Vec3& v);
  explicit Vec3( const Vec3* v );
  explicit Vec3( const double* ptr );
  Vec3( const double& a, const double& b, const double& c );
  Vec3& operator =( const Vec3 &src );
  bool operator ==( const Vec3& src ) const;
  bool operator !=( const Vec3& src ) const;
  const Vec3& operator +=( const Vec3 &v );
  const Vec3& operator +=( const double &scalar );
  const Vec3& operator -=( const Vec3 &v );
  const Vec3& operator -=( const double &scalar );
  const Vec3& operator *=( const double &scalar );
  const Vec3& operator /=( const double &scalar );
  Vec3 operator +( const Vec3 &v ) const;
  Vec3 operator -( const Vec3 &v ) const;
  Vec3 operator -( ) const;
  Vec3 operator *( const double &scalar) const;
  Vec3 operator /( const double &scalar ) const;
  const double operator []( const size_t index ) const;
  double& operator []( const size_t index );
    // Returning a pointer is terrible but handy
  const double* constPtr( ) const;
  const double& x( ) const;
  const double& y( ) const;
  const double& z( ) const;
  void x( const double& a );
  void y( const double& b );
  void z( const double& c );
    // Math
  void normalize( );
  double length( ) const;
  double squaredLength( ) const;
  double angle( const Vec3& v ) const;
  double cosDoubleTheta( const Vec3 &v ) const;
  double distanceFrom( const Vec3& v ) const;
  double squaredDistanceFrom( const Vec3& v ) const;
  double dot( const Vec3 &v ) const;
    // I/O
  std::ostream& write( std::ostream &out ) const;
  std::ostream& write_row( std::ostream &out) const;
  std::ostream& description( std::ostream &out = std::cerr ) const;
};

Vec3 operator *( const double &scalar, const Vec3 &v);


Vec3 cross( const Vec3 &u, const Vec3 &v );

Vec3 normal( Vec3 &a, Vec3 &b, Vec3 &c );

// Basic I/O
std::ostream& operator <<( std::ostream &out, const Vec3 &v );

#endif
