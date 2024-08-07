/*
QuickCG

Copyright (c) 2004-2007, Lode Vandevenne

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __3DMATH_H__
#define __3DMATH_H__

#include "quickcg.h"

////////////////////////////////////////////////////////////////////////////////
//               [ x ]                                                        //
// Vector Class  [ y ]                                                        //
//               [ z ]                                                        //
////////////////////////////////////////////////////////////////////////////////
class Vector3
{
  public:
         
  double x;
  double y;
  double z;
  
  Vector3(double x, double y, double z);
  Vector3();
  
  double length() const;
  void normalize();
  double distance(const Vector3& v) const;
  double dot(const Vector3& v) const;
  Vector3 cross(const Vector3& v) const;
};

double length(const Vector3& v);
Vector3 normalize(const Vector3& v);
double distance(const Vector3& v, const Vector3& w);     
double dot(const Vector3& v, const Vector3& w);
Vector3 cross(const Vector3& v, const Vector3& w);
Vector3 operator-(const Vector3& v, const Vector3& w);
Vector3 operator-(const Vector3& v);
Vector3 operator+(const Vector3& v, const Vector3& w);
Vector3 operator*(const Vector3& v, double a);
Vector3 operator*(double a, const Vector3& v);
Vector3 operator/(const Vector3& v, double a);
double vectorAngle(const Vector3& v, const Vector3& w);
Vector3 rotateAroundArbitrary(const Vector3& v, const Vector3& axis, double angle);

////////////////////////////////////////////////////////////////////////////////
//               [ a b c ]                                                    //
// Matrix Class  [ d e f ]                                                    //
//               [ g h i ]                                                    //
////////////////////////////////////////////////////////////////////////////////
class Matrix3
{
  public:
    
  double a; double b; double c;
  double d; double e; double f;
  double g; double h; double i;
  
  Matrix3(double a, double b, double c, double d, double e, double f, double g, double h, double i);
  Matrix3();
  
  void transpose();
  double determinant() const;
  void invert();
};

Matrix3 transpose(const Matrix3& A);
double determinant(const Matrix3& A);
Matrix3 inverse(const Matrix3& A);
Matrix3 operator+(const Matrix3& A, const Matrix3& B);
Matrix3 operator-(const Matrix3& A, const Matrix3& B);
Matrix3 operator*(const Matrix3& A, double a);
Matrix3 operator*(double a, const Matrix3& A);
Matrix3 operator/(const Matrix3& A, double a);
Vector3 operator*(const Matrix3& A, const Vector3& v);
Vector3 operator*(const Vector3& v, const Matrix3& A);
Matrix3 operator*(const Matrix3& A, const Matrix3& B);


////////////////////////////////////////////////////////////////////////////////
//               [ u.x v.x dir.x ]   [ pos.x ]                                //
// Camera Class  [ u.y v.y dir.y ] + [ pos.y ]                                //
//               [ u.z v.z dir.z ]   [ pos.z ]                                //
////////////////////////////////////////////////////////////////////////////////
class Camera
{

  public:
  double nearClip; //near clipping plane
  double farClip; //far clipping plane

  Camera();
  Camera(double posx, double posy, double posz,
       double ux,   double uy,   double uz,
       double vx,   double vy,   double vz,
       double dirx, double diry, double dirz,
       double nearClip, double farClip);
           
  //Get and Set the 4 important vectors 
  Vector3 getU(); //the "right" vector of the camera, x coordinate of screen
  Vector3 getV(); //the "up" vector of the camera, y coordinate of screen
  Vector3 getDir(); //the direction of the camera, vector that points into the screen (z direction)
  Vector3 getPos(); //the position of the camera in world coordinates
  void setDir(const Vector3& newDir);
  void setU(const Vector3& newY);
  void setV(const Vector3& newV);  
  void setPos(const Vector3& newPos);
  
  //move and rotate with vectors
  void move(const Vector3& offset);
  void rotate(const Vector3& axis, double angle);
  void setLookDir(const Vector3& newDir);
  void lookAt(const Vector3& lookAtMe);
  
  //get and set distance to a certain point
  double getDist(const Vector3& point);
  void setDist(const Vector3& point, double dist);
  
  //get and set zoom
  double getZoomU();
  double getZoomV();
  void setZoomU(double a);
  void setZoomV(double a); 
  void zoom(double a);
  void zoomU(double a);
  void zoomV(double a);
    
  //get and set FOV
  double getFOVU();
  double getFOVV();
  void setFOVU(double angle);
  void setFOVV(double angle);  
  
  //get and set pitch, yaw and roll (these are NOT native parameters of the camera and should normally never be needed!)   
  double getYaw();
  double getPitch();
  double getRoll();    
  void setYaw(double angle);
  void setPitch(double angle);  
  void setRoll(double angle);
  void yawPlanet(double angle);
  void yawSpace(double angle);  
  void pitch(double angle);
  void roll(double angle);
   
  //make camera orthogonal (reset skewing)
  void resetSkewU();
  void resetSkewV();
  
  //set screen ratio of the camera (ratio of length of u and v, e.g. 4:3, 16:9, 640:480, ...)
  double getRatioUV();
  double getRatioVU();
  void setRatioUV(double ratio);
  void setRatioVU(double ratio);
  
  //scale U, V and Dir without changing what you see
  double getScale();
  void setScale(double dirLength);
  void scale(double factor);
  
  //generate, get and use the camera matrix to transform points
  void generateMatrix();
  Matrix3 getMatrix();
  Matrix3 getInvMatrix();
  void setMatrix(const Matrix3& matrix);   //sets the u, v and dir vector to given matrix (and generates the actual matrix too of course)
  Vector3 transform(const Vector3& v);
  bool projectOnScreen(const Vector3& point, int & x, int & y, double & z);
  bool projectOnScreen(const Vector3& point, int & x, int & y);
  bool camSpaceToScreen(const Vector3& point, int & x, int & y);
  
  private:
  //the camera plane, described by the vectors u and v, is described by "z = 0" in camera space
  Vector3 pos; //the location of the camera
  Vector3 u; //horizontal vector, horizontal side of computer screen
  Vector3 v; //vertical "up" vector, vertical side of computer screen
  Vector3 dir; //direction of the camera, direction of the projection
  Matrix3 camMatrix; //the camera matrix, is nothing more than the column vectors u, v and dir in one matrix
  Matrix3 invCamMatrix; //the inverse of the camera matrix
};


//Auxiliary functions
//swap between radians and degrees
double radToDeg(double rad);
double degToRad(double deg);

#endif
