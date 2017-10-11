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

#include "q3dmath.h"
#include "quickcg.h"
#include <cmath>

using namespace QuickCG;

Vector3::Vector3(double x, double y, double z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3::Vector3()
{
  x = 0.0;
  y = 0.0;
  z = 0.0;
} 

double Vector3::length() const
{
  return sqrt(x * x + y * y + z * z);
}

double length(const Vector3& v)
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void Vector3::normalize()
{
  double l = length();
  if(l != 0)
  {
    x /= l;
    y /= l;
    z /= l;
  }  
}

Vector3 normalize(const Vector3& v)
{
  Vector3 result = v;
  result.normalize();
  return result;
}

double Vector3::distance(const Vector3& v) const
{
  return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
}  

double distance(const Vector3& v, const Vector3& w)
{
  return sqrt((v.x - w.x) * (v.x - w.x) + (v.y - w.y) * (v.y - w.y) + (v.z - w.z) * (v.z - w.z));
}

double Vector3::dot(const Vector3& v) const
{
  return v.x * x + v.y * y + v.z * z;
}

double dot(const Vector3& v, const Vector3& w)
{
  return v.x * w.x + v.y * w.y + v.z * w.z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
  Vector3 u;

  u.x = ((v.y * z) - (v.z * y));
  u.y = ((v.z * x) - (v.x * z));
  u.z = ((v.x * y) - (v.y * x));
  return u;
} 

Vector3 cross(const Vector3& v, const Vector3& w)
{
  Vector3 u;

  u.x = ((v.y * w.z) - (v.z * w.y));
  u.y = ((v.z * w.x) - (v.x * w.z));
  u.z = ((v.x * w.y) - (v.y * w.x));
  return u;
}

//Subtract two vectors
Vector3 operator-(const Vector3& v, const Vector3& w)
{
  Vector3 u;
  u.x = v.x - w.x;
  u.y = v.y - w.y;
  u.z = v.z - w.z;
  return u;
}

//Return the negative of the vector
Vector3 operator-(const Vector3& v)
{
  Vector3 u;
  u.x = -v.x;
  u.y = -v.y;
  u.z = -v.z;
  return u;
}

//Add two vectors
Vector3 operator+(const Vector3& v, const Vector3& w)
{
  Vector3 u;
  u.x = v.x + w.x;
  u.y = v.y + w.y;
  u.z = v.z + w.z;
  return u;
}

//Multiplies a vector by a scalar
Vector3 operator*(const Vector3& v, double a)
{
  Vector3 w;
  w.x = v.x * a;
  w.y = v.y * a;
  w.z = v.z * a;
  return w;
}

//Multiplies a vector by a scalar
Vector3 operator*(double a, const Vector3& v)
{
  Vector3 w;
  w.x = v.x * a;
  w.y = v.y * a;
  w.z = v.z * a;
  return w;
}

//Divides a vector through a scalar
Vector3 operator/(const Vector3& v, double a)
{
  Vector3 w;
  w.x = v.x / a;
  w.y = v.y / a;
  w.z = v.z / a;
  return w;
}

//Gives the angle between two 3D vectors (in radians)
double vectorAngle(const Vector3& v, const Vector3& w)
{
  //dot product(v,w) = length(v)*length(w)*cos(angle) ==> angle = acos(dot/(length(v)*length(w))) = acos(dot(norm(v)*norm(w)));
  double cosineOfAngle = dot(normalize(v), normalize(w));
  //for acos, the value has to be between -1.0 and 1.0, but due to numerical imprecisions it sometimes comes outside this range
  if(cosineOfAngle > 1.0) cosineOfAngle = 1.0;
  if(cosineOfAngle < -1.0) cosineOfAngle = -1.0;
  return -acos(cosineOfAngle);
}

//Rotate vector v around arbitrary axis axis for angle radians
//It can only rotate around an axis through our object, to rotate around another axis:
//first translate the object to the axis, then use this function, then translate back in the new direction.
Vector3 rotateAroundArbitrary(const Vector3& v, const Vector3& axis, double angle)
{
  if((v.x == 0) && (v.y == 0) && (v.z == 0)) return Vector3();

  Vector3 w;
  double c, s, t;

  Vector3 naxis = normalize(axis);

  //calculate parameters of the rotation matrix
  c = cos(angle);
  s = sin(angle);
  t = 1 - c;

  //multiply v with rotation matrix
  w.x = (t * naxis.x * naxis.x +      c) * v.x 
      + (t * naxis.x * naxis.y + s * naxis.z) * v.y 
      + (t * naxis.x * naxis.z - s * naxis.y) * v.z;
    
  w.y = (t * naxis.x * naxis.y - s * naxis.z) * v.x 
      + (t * naxis.y * naxis.y +      c) * v.y 
      + (t * naxis.y * naxis.z + s * naxis.x) * v.z;
    
  w.z = (t * naxis.x * naxis.z + s * naxis.y) * v.x 
      + (t * naxis.y * naxis.z - s * naxis.x) * v.y 
      + (t * naxis.z * naxis.z +      c) * v.z;

  w.normalize();
  w = w * v.length();
  
  return w;
}



Matrix3::Matrix3(double a, double b, double c, double d, double e, double f, double g, double h, double i)
{
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
  this->e = e;
  this->f = f;
  this->g = g;
  this->h = h;
  this->i = i;
}
  
Matrix3::Matrix3()
{
  a = 0.0;
  b = 0.0;
  c = 0.0;
  d = 0.0;
  e = 0.0;
  f = 0.0;
  g = 0.0;
  h = 0.0;
  i = 0.0;
}

////////////////////////////////////////////////////////////////////////////////
//Transpose:                                                                  //
//                                                                            //
// [ a b c ]T    [ a d g ]                                                    //
// [ d e f ]  =  [ b e h ]                                                    //
// [ g h i ]     [ c f i ]                                                    //
////////////////////////////////////////////////////////////////////////////////
void Matrix3::transpose()
{
  b += d; d = b - d; b -= d; //swap b and d
  c += g; g = c - g; c -= g; //swap c and g
  f += h; h = f - h; f -= h; //swap f and h
}  

Matrix3 transpose(const Matrix3& A)
{
  Matrix3 result = A;
  result.transpose();
  return result;
} 

double Matrix3::determinant() const
{
  double det = a * e * i 
             + b * f * g 
             + d * h * c 
             - g * e * c 
             - d * b * i 
             - h * f * a;
  return det;
}

double determinant(const Matrix3& A)
{
  return A.determinant();
}

//Inverse of a 3x3 matrix
void Matrix3::invert()
{
  /*
  the inverse is the adjoint divided through the determinant
  find the matrix of minors (minor = determinant of 2x2 matrix of the 2 rows/colums current element is NOT in)
  turn them in cofactors (= change some of the signs)
  find the adjoint by transposing the matrix of cofactors
  divide this through the determinant to get the inverse
  */
  
  double det = determinant();
  Matrix3 B;

  //included in these calculations: minor, cofactor (changed signs), transpose (by the order of "="), division through determinant
  B.a = ( e * i - h * f) / det;
  B.b = (-b * i + h * c) / det;
  B.c = ( b * f - e * c) / det;
  B.d = (-d * i + g * f) / det;
  B.e = ( a * i - g * c) / det;
  B.f = (-a * f + d * c) / det;
  B.g = ( d * h - g * e) / det;
  B.h = (-a * h + g * b) / det;
  B.i = ( a * e - d * b) / det;
  
  *this = B;
}

Matrix3 inverse(const Matrix3& A)
{
  Matrix3 result = A;
  result.invert();
  return result;
} 

//Add two matrices
Matrix3 operator+(const Matrix3& A, const Matrix3& B)
{
  Matrix3 result = A;
  result.a += B.a;
  result.b += B.b;
  result.c += B.c;
  result.d += B.d;
  result.e += B.e;
  result.f += B.f;
  result.g += B.g;
  result.h += B.h;
  result.i += B.i;
  return result;
}

//Subtract two matrices
Matrix3 operator-(const Matrix3& A, const Matrix3& B)
{
  Matrix3 result = A;
  result.a -= B.a;
  result.b -= B.b;
  result.c -= B.c;
  result.d -= B.d;
  result.e -= B.e;
  result.f -= B.f;
  result.g -= B.g;
  result.h -= B.h;
  result.i -= B.i;
  return result;
}

//Multiply a matrix with a scalar
Matrix3 operator*(const Matrix3& A, double a)
{
  Matrix3 result = A;
  result.a *= a;
  result.b *= a;
  result.c *= a;
  result.d *= a;
  result.e *= a;
  result.f *= a;
  result.g *= a;
  result.h *= a;
  result.i *= a;
  return result;
}

//Multiply a matrix with a scalar
Matrix3 operator*(double a, const Matrix3& A)
{
  Matrix3 result = A;
  result.a *= a;
  result.b *= a;
  result.c *= a;
  result.d *= a;
  result.e *= a;
  result.f *= a;
  result.g *= a;
  result.h *= a;
  result.i *= a;
  return result;
}

//Divide a matrix through a scalar
Matrix3 operator/(const Matrix3& A, double a)
{
  Matrix3 result = A;
  result.a /= a;
  result.b /= a;
  result.c /= a;
  result.d /= a;
  result.e /= a;
  result.f /= a;
  result.g /= a;
  result.h /= a;
  result.i /= a;
  return result;
}

//Multiply a matrix with a column vector, resulting in a column vector
Vector3 operator*(const Matrix3& A, const Vector3& v)
{
  Vector3 w;
  w.x = A.a * v.x + A.b * v.y + A.c * v.z;
  w.y = A.d * v.x + A.e * v.y + A.f * v.z;
  w.z = A.g * v.x + A.h * v.y + A.i * v.z;
  return w;
}  

//Multiply a vector with a row matrix, resulting in a row vector
Vector3 operator*(const Vector3& v, const Matrix3& A)
{
  Vector3 w;
  w.x = A.a * v.x + A.d * v.y + A.g * v.z;
  w.y = A.b * v.x + A.e * v.y + A.h * v.z;
  w.z = A.c * v.x + A.f * v.y + A.i * v.z;
  return w;
}

//Multiply a 3x3 matrix with a 3x3 matrix
Matrix3 operator*(const Matrix3& A, const Matrix3& B)
{
  Matrix3 C;
  C.a = A.a * B.a + A.b * B.d + A.c * B.g;
  C.b = A.a * B.b + A.b * B.e + A.c * B.h;
  C.c = A.a * B.c + A.b * B.h + A.c * B.i;
  C.d = A.d * B.a + A.e * B.d + A.f * B.g;
  C.e = A.d * B.b + A.e * B.e + A.f * B.h;
  C.f = A.d * B.c + A.e * B.h + A.f * B.i;
  C.g = A.g * B.a + A.h * B.d + A.i * B.g;
  C.h = A.g * B.b + A.h * B.e + A.i * B.h;
  C.i = A.g * B.c + A.h * B.h + A.i * B.i;
  return C;
}



//Camera functions

/*
Note: 
Every function that changes u, v or dir should use "generateMatrix();" at the end,
unless that function already calls another one that generates it, at the end.
*/
Camera::Camera(double posx, double posy, double posz, double ux, double uy, double uz, double vx, double vy, double vz, double dirx, double diry, double dirz, double nearClip, double farClip)
{
  pos.x = posx;
  pos.y = posy;
  pos.z = posz;
  u.x = ux;
  u.y = uy;
  u.z = uz;
  v.x = vx;
  v.y = vy;
  v.z = vz;
  dir.x = dirx;
  dir.y = diry;
  dir.z = dirz;
  this->nearClip = nearClip;
  this->farClip = farClip;
  generateMatrix();
}  

//Construct a camera with default parameters
Camera::Camera()
{
  pos.x = 0.0;
  pos.y = 0.0;
  pos.z = 0.0;
  u.x = 1.0;
  u.y = 0.0;
  u.z = 0.0;
  v.x = 0.0;
  v.y = 1.0;
  v.z = 0.0;
  dir.x = 0.0;
  dir.y = 0.0;
  dir.z = 1.0;
  nearClip = 0.1;
  farClip = 1000000.0;
  generateMatrix();    
}

Vector3 Camera::getU()
{
  return u;
}  
Vector3 Camera::getV()
{
  return v;
}  
Vector3 Camera::getDir()
{
  return dir;
}  
Vector3 Camera::getPos()
{
  return pos;
}

void Camera::setU(const Vector3& newU)
{
  u = newU;
  generateMatrix();
}  
void Camera::setV(const Vector3& newV)  
{
  v = newV;
  generateMatrix();
}

void Camera::setDir(const Vector3& newDir)  
{
  dir = newDir;
  generateMatrix();
}  

void Camera::setPos(const Vector3& newPos)
{
  pos = newPos;
} 

void Camera::move(const Vector3& offset)
{
  pos = pos + offset;
}  

void Camera::rotate(const Vector3& axis, double angle)
{
  u = rotateAroundArbitrary(u, axis, angle);
  v = rotateAroundArbitrary(v, axis, angle);
  dir = rotateAroundArbitrary(dir, axis, angle);
  generateMatrix();
}   

//Look in a certain direction.
void Camera::setLookDir(const Vector3& newDir)
{
  Vector3 axis = cross(dir, newDir);
  if(axis.length() == 0) return;
  
  double angle = vectorAngle(dir, newDir);
  if(angle != 0) rotate(axis, angle);   
}

//Look at a certain point (the point will come in the center of the screen, if nothing's in front of it that is)
void Camera::lookAt(const Vector3& lookAtMe)
{
  setLookDir(lookAtMe - pos);
}

double Camera::getDist(const Vector3& point)
{
  return distance(pos, point);
}

//Set the distance to a point
void Camera::setDist(const Vector3& point, double dist)
{
  move(dist * normalize(point - pos));
}  

void Camera::zoom(double a)
{
  //increasing length of dir or decreasing length of the plane (u and v) = zoom IN
  u = u / a;
  v = v / a;
  generateMatrix();
}

void Camera::zoomU(double a)
{
  u = u / a;
  generateMatrix();
}

void Camera::zoomV(double a)
{
  v = v / a;
  generateMatrix();
}

double Camera::getZoomU()
{
   return(dir.length() / u.length());
}

double Camera::getZoomV()
{
   return(dir.length() / v.length());
}

void Camera::setZoomU(double a)
{
  u = u / (a / getZoomU());
  generateMatrix();
}

void Camera::setZoomV(double a)
{
  v = v / (a / getZoomV());
  generateMatrix();
}  

double Camera::getFOVU()
{
  return(2.0 * atan2(u.length(), dir.length()));
}

double Camera::getFOVV()
{
  return(2.0 * atan2(v.length(), dir.length()));
}

void Camera::setFOVU(double angle)
{
  setZoomU(1.0 / tan(angle / 2.0));
}

void Camera::setFOVV(double angle)
{
  setZoomV(1.0 / tan(angle / 2.0));
}   

////////////////////////////////////////////////////////////////////////////////
//Functions to get Yaw, Pitch and Roll. Normally to do things with the camera, 
//you never need these angles, everything works with vectors!
//However, these can be useful if you want a compass in your game, or a weapon 
//that allows you to accurately change it's pitch
////////////////////////////////////////////////////////////////////////////////

/*
Yaw is the wind direction the camera is looking at if you'd be standing on planet earth, and the ground has vectors X and Z, while Y points to the sky
     N
     ^ 
     |z 
     |   x    Looking in the direction Z = North (0°, 0 rad)
W -----+-----> E  Looking in the direction X = East (+90°, +1.57 rad)
     |      Looking in negative Z = South (180°, 3.1415 rad, sign jump)
     |      Looking in negative X = West (-90°, -1.57 rad)
     |
     S
*/
double Camera::getYaw()
{  
  //the atan2 function returns the angle of a 2D point (like from polar coordinates), so here it gives angle of dir projected on XZ plane, which is what we want for the yaw
  return(atan2(dir.x, dir.z));
}

//setYaw can be used to make you look at a certain wind direction (where the ground is the XZ plane)
void Camera::setYaw(double angle)
{  
  double currentAngle = getYaw();
  //to change yaw, you have to rotate around the "up" axis of the WORLD = the y axis
  rotate(Vector3(0,1,0), - angle + currentAngle);
}

//rotates camera around world Y-axis with given angle (in space this would make no sense since there isn't really an "up" or "down" of the world, instead you rotate around the "up" and "down" of your own spaceship there
//When rolled, yawPlanet will give very annoying rotation, so for a spacegame or flight simulator you'll want to use yawSpace instead.
void Camera::yawPlanet(double angle)
{  
  rotate(Vector3(0,1,0), angle);
}

//rotates camera around camera v axis with given angle (this one makes sense in space, but not on a planet, on a planet your camera would start getting a "roll")
void Camera::yawSpace(double angle)
{  
  rotate(v, angle);
}

/*
Pitch is only useful to define if you're standing on a plane or a planet, and there's a sky
The plane you're standing on is the XZ plane, and Y points to the sky
Pitch is 0° if you look forward (direction vector parallel to the XZ plane)
Pitch is +90° if you look to the sky (maximum "up")
Pitch is -90° if you look at the ground (maximum "down")
Pitches of more than 90° or less than -90° aren't defined, since these can also be made by having  yaw 180° rotated and having a pitch between -90° and +90°
*/
double Camera::getPitch()
{
  //Project dir on the XZ plane
  //Then find angle between dir and projected dir   
  //With atan2: angle of the point (lengthof2Dvector(dir.x, dir.z), dir.y)
  return atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z));
}

/*
setPitch can be used to make you look to the sky or floor with certain angle without changing wind direction
NOT useful for spacecrafts etc... because there's no up or down in space and X, Y and Z axes have no physical meaning there
only useful for FPS games or other games where you walk on a planet surface
*/
void Camera::setPitch(double angle)
{  
  double currentAngle = getPitch();
  //to change pitch, you have to rotate around the horizontal vector of the camera
  rotate(u, angle - currentAngle);
}

//pitches the camera over a certain amount
void Camera::pitch(double angle)
{  
  rotate(u, angle);
}

/*
Roll is only necessary for spacegames, flightsimulators or rare effects of FPS games
Roll 0° = cam.u parallel with XZ plane, cam.v points upwards
Roll +90° = cam.v parallel with XZ plane, cam.u points downwards
Roll +-180° = cam.u parallel with XZ plane, cam.v points downwards (= you're upside down)
Roll -90° = cam.v parallel with XZ plane, cam.u points upwards
In space games the angles have no physical meaning, but in the coordinate system this angle is an "Euler" angle.
*/
double Camera::getRoll()
{
  //roll is the angle between the plane (world_up, camera_dir) and the plane (camera_up, camera_dir)
  //the angle between two planes is the angle between their normals
  //the normals are gotten with cross products
  //the vectorAngle function uses acos and dot product
  double roll = vectorAngle(cross(Vector3(0, 1, 0), dir), cross(v, dir));
  if(u.y < 0) roll = -roll;
  return roll;
}

void Camera::setRoll(double angle)
{  
  double currentAngle = getRoll();
  //to change roll, you have to rotate around the direction vector of the camera
  rotate(dir, angle - currentAngle);
}

//rolls the camera by rotating it around the direction vector (only makes sense in space or for "shaking" effects)
void Camera::roll(double angle)
{  
  rotate(dir, angle);
}

//makes u, v and dir perpendicular by using cross product, maintains exact direction and roll if only v was skewed
void Camera::resetSkewU()
{
  double oldZoomU = getZoomU(); 
  double oldZoomV = getZoomV();
  u = cross(dir, v);
  v = cross(dir, -u);
  setZoomU(oldZoomU);
  setZoomV(oldZoomV);
}

//makes u, v and dir perpendicular by using cross product, maintains exact direction and roll if only u was skewed
void Camera::resetSkewV()
{
  double oldZoomU = getZoomU(); 
  double oldZoomV = getZoomV();
  v = cross(dir, u);
  u = cross(dir, -v);
  setZoomU(oldZoomU);
  setZoomV(oldZoomV);
}

//get and set screen ratios of the camera (ratio of length of u and v, e.g. 4:3, 16:9, 640:480, ...)
double Camera::getRatioUV()
{
  return u.length() / v.length();
}  

double Camera::getRatioVU()
{
  return v.length() / u.length();
}

//changes V  
void Camera::setRatioUV(double ratio)
{
  v.normalize();
  v = v * u.length() / ratio;
  generateMatrix();  
}  

//changes U
void Camera::setRatioVU(double ratio)
{
  u.normalize();
  u = u * v.length() / ratio;
  generateMatrix();
} 

//scale U, V and Dir without changing what you see
double Camera::getScale()
{
  return dir.length();
}  
void Camera::setScale(double dirLength)
{
  scale(dir.length() / dirLength);
}
  
void Camera::scale(double factor)  
{
  dir = dir * factor;
  u = u * factor;
  v = v * factor;
  generateMatrix();
}  

void Camera::generateMatrix()
{
  //the cameraMatrix is generated, this is the matrix with 3 column vectors: u, v and dir
  //it has to be generated everytime u, v or dir are changed  
  camMatrix.a = u.x;
  camMatrix.d = u.y;
  camMatrix.g = u.z;
  camMatrix.b = v.x;
  camMatrix.e = v.y;
  camMatrix.h = v.z;
  camMatrix.c = dir.x;
  camMatrix.f = dir.y;
  camMatrix.i = dir.z;

  //this is the inverse of the camMatrix, to use for transformations
  invCamMatrix = inverse(camMatrix);
}

Matrix3 Camera::getMatrix()
{
  return camMatrix;
}

Matrix3 Camera::getInvMatrix()
{
  return invCamMatrix;
}

void Camera::setMatrix(const Matrix3& matrix)
{
  //to make sure the vectors and the matrix are according to each other, set the vectors to the given matrix 
  //and then use generateMatrix with these new vectors
  u.x = matrix.a;
  u.y = matrix.d;
  u.z = matrix.g;
  v.x = matrix.b;
  v.y = matrix.e;
  v.z = matrix.h;
  dir.x = matrix.c;
  dir.y = matrix.f;
  dir.z = matrix.i;
  generateMatrix();
} 

Vector3 Camera::transform(const Vector3& v)
{
  /*
  Transformation from coordinate system 1 (base 1) to coordinate system 2 (base 2):
    
  vector v = coordinates of point p in base 1 (column vector)
  matrix A = the 3 vectors representing coordinates of unit vectors of base 2 in base 1 (3 column vectors in matrix)
  vector w = coordinates of point p in base 2(column vector)
  
  Then: v = A * w
      w = A^(-1) * v
  
  Now: base 1 = worldspace, base 2 = cameraspace
  v = coordinates of point in worldspace (the given v )
  A = the matrix from the camera: the coordinates of the camera vectors in worldspace
  w = coordinates of point in cameraspace
  
  So our transformation is w = A^(-1) * v, where v is the parameter given to the function, and A = perspective matrix of camera
  
  All this text to describe only one line of code...
  */
  
  return invCamMatrix * v;  
}

/*
uses the transform function of the camera, to project a point on screen, stores result in x and y (screen coordinates)
returns 1 if point is legal, 0 if point is behind camera, outside screen, ...
also stores z value of the point in camera coordinates in z
*/
bool Camera::projectOnScreen(const Vector3& point, int & x, int & y, double & z)
{
  //First transformation: position
  Vector3 a = point - pos;
  
  //Second transformation: rotation
  Vector3 b = transform(a);
  
  //Third transformation: Projection on screen
  //be warned: if b.z is too small, you get integer imprecisions, so don't make near clipping plane too small  
  int px = int(w / 2.0 + w * b.x / b.z);
  int py = int(h / 2.0 - h * b.y / b.z); //inversed: y should be "up", while in screen coordinates it's down
  if(onScreen(px, py) && b.z >= nearClip)
  {
    x = px;
    y = py;
    z = b.z;
    return 1;
  }
  else
  {
    x = px;
    y = py;
    z = b.z;
    return 0;
  }    
}

bool Camera::projectOnScreen(const Vector3& point, int & x, int & y)
{
  double z;
  return projectOnScreen(point, x, y, z);     
} 

bool Camera::camSpaceToScreen(const Vector3& point, int & x, int & y)
{
  //be warned: if point.z is too small, you get integer imprecisions, so don't make near clipping plane too small
  int px = int(w / 2.0 + w * point.x / point.z);
  int py = int(h / 2.0 - h * point.y / point.z); //inversed: y should be "up", while in screen coordinates it's down
  if(onScreen(px, py) && point.z >= nearClip)
  {
    x = px;
    y = py;
    return 1;
  }
  else
  {
    x = px;
    y = py;
    return 0;
  }     
}  


//Auxiliary Functions


//Swap between radians and degrees
double radToDeg(double rad)
{
  return 360.0 * rad / (3.14159 * 2.0);
} 

double degToRad(double deg)
{
  return (3.14159 * 2.0) * deg / 360.0;
}  

//TO DO:
//Z-buffer
//drawLine and disk functions that use the zbuffer
