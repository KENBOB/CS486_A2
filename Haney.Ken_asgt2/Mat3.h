//   $ID: haney.ken_asgt2
//    File Name: psurfviewer_main.cpp
//       Author: Ken Haney, kenbob.21@csu.fullerton.edu
// Assignment 2: Transformations and Simple Shading
//         Date: 04/25/2013
// Description: contains the psurfviewer_main routines
// 

// $Id: Mat3.h 4097 2013-02-25 08:28:03Z mshafae $
#ifndef _MSGFX_MAT3_H_
#define _MSGFX_MAT3_H_

#include "Vec3.h"
#include <iostream>
#include <iomanip>

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


/*
* OPENGL IS COLUMN MAJOR
* C IS ROW MAJOR
*
* This class is column major
*/

class Mat3{
private:
  double data[9];
public:
  Mat3( );
  Mat3( const Mat3& m );
  Mat3( double a, double b, double c,
        double d, double e, double f,
        double g, double h, double i );
  Mat3( const Vec3 &col_a,
        const Vec3 &col_b,
        const Vec3 &col_c );
  explicit Mat3( double array[9] );
  ~Mat3( );
  Mat3& operator =( const Mat3& m );
  Mat3& operator =( const double array[9] );
  bool operator ==( const Mat3& m ) const;
  bool operator !=( const Mat3& m ) const;
  Mat3& operator +=( const Mat3& m );
  Mat3& operator +=( const double &scalar );
  Mat3& operator -=( const Mat3& m );
  Mat3& operator *=( const double &scalar );
  Mat3& operator /=( const double &scalar );
  const double operator ()( const size_t &col_i, const size_t &row_j ) const;
  double& operator ()( const size_t &col_i, const size_t &row_j );
  //const Vec3 operator []( const size_t col_i ) const;
  Vec3 operator []( const size_t &col_i );
  Vec3 col( const size_t &col_i ) const;
  Vec3 row( const size_t &row_j ) const;
  // Miscellaneous Methods
  double determinant( );
  Mat3 transpose( );
  Mat3 adjoint( );
  Mat3 inverse( double& determinant );
  // Arithmetic Operators
  Mat3 operator +( const Mat3 &n );
  Mat3 operator -( const Mat3 &n );
  Mat3 operator -( );
  Mat3 operator *( const double &scalar ) const;
  Mat3 operator *( const Mat3 &n );
  Vec3 operator *( const Vec3 &v );
  Mat3 operator /( const double &scalar );
  std::ostream& write( std::ostream& out ) const;
  std::ostream& description( std::ostream &out = std::cerr ) const;
};

Mat3 operator *( const double &scalar, const Mat3 &m);

void copyMatrixToArray( Mat3& m, double a[9] );

std::ostream& operator <<( std::ostream &out, const Mat3 &m );

Mat3 rotate(const double angleInDegrees, const double x, const double y, const double z);

Mat3 uniformScale(double s);

Mat3 scale(double a, double b, double c);


#endif
