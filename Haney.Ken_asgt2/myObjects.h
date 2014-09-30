// Ken Haney
// Kenbob.21@csu.fullerton.edu 
// CS486 picking project
// 09/29/2014
// this file: myObjects.h,   contains Object classes & skybox function prototypes

#define GLFW_INCLUDE_GLU            //
#include <GLFW/glfw3.h>             // using GLFW

#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cmath>

#ifndef MYOBJECTS_H
#define MYOBJECTS_H
 #include "FaceList.h"
 extern    FaceList*   pL;           // defined in main.cpp  ??

 #define   max3(a,b,c)     (a > b ? (a>c ? a : c) : (b>c ? b : c))
 #define   max(a,b)        ((a) > (b) ? (a) : (b))           // useful macro
 #define   min(a,b)        ((a) < (b) ? (a) : (b))           // useful macros (done in-line)
 #define   sqr(a)          ((a) * (a))
 #define   vcopy(a, b)     a[0]=b[0];        a[1]=b[1];        a[2]=b[2]            // vec copy
 #define   vadd(a, b, c)   a[0]=b[0]+c[0];   a[1]=b[1]+c[1];   a[2]=b[2]+c[2]       // vec add
 #define   vsub(a, b, c)   a[0]=b[0]-c[0];   a[1]=b[1]-c[1];   a[2]=b[2]-c[2]       // vec sub
 #define   mag3(a)         sqrt(a[0]*a[0] +a[1]*a[1] +a[2]*a[2])                    // vec magnitude
 #define   unit3(a)        m=mag3(a);  if(m>0.0) { a[0]/=m;  a[1]/=m;  a[2]/=m; }   // requires defining "m" :{
 #define   vcross(a,b,c)   a[0]=b[1]*c[2]-b[2]*c[1];  a[1]=b[2]*c[0]-b[0]*c[2];  a[2]=b[0]*c[1]-b[1]*c[0]
 #define   vdot(a, b)      a[0]*b[0] +a[1]*b[1] +a[2]*b[2]                          // vec dot-prod

 #define  UINT   unsigned int
                                                             // prototypes
 void initSkybox(char*, char*, char*, char*, char*, char*);  // load 6 Skybox faces & 4 texture files
 UINT loadTexture (const char* filename);
 void killSkybox();                                          // delete the Skybox & textures
 void drawSkybox(float size);                                // draw the Skybox & ground-plane
 FaceList* myCube(float sx,float sy,float szi,UINT texID);        // draw a rect or cube w/ texID on 2 faces?
 
 bool fileExists (const char *filename);                     // prototype: checks if "filename" exists  /.. main.cpp
 FaceList* readPlyModel (const char* filename);              // prototype: reads a PLYfile model

#ifndef M_PI
 #define  M_PI   3.1415926535
#endif

struct Gf3{    
   GLfloat x;  GLfloat y;  GLfloat z;   
   Gf3(float a,float b,float c);       // constructor
};

class Object {                // class Object: attributes of any Object
public:
	char*    sName;               // string "FileName" or "objType" to Load/generate
	GLint    LID;                 // glCallList IDentifier; set during "build" of each Object
	bool     visible;             // objID visible ? yes : culled 
	bool     select;              // objID highlighted ? 0 : 1  (when picked)
	double   ctr[3],   RR;        // objID center and radius^2, for picking w/ RAY_SPHERE alg
	FaceList *fl;                 // object (saved) FaceList struct
	double   velx,  vely;         //                                     object velocity
	GLfloat  pos[3];              // glTranslatef(px,py,pz)              object position
	GLfloat  rotA[4];             // glRotatatef(Angle, rx,ry,rz)        object rotation= orient
	GLfloat  sx,sy,sz;            // glScalef(sx,sy,sz)                  object scale factor
	GLfloat  ambient[3];          // glMaterialfw( , , ambient)          object
	GLfloat  diffuse[3];          // glMaterialfw( , , diffuse)          modified by (select ? 0.5 : 1.0)
	GLfloat  specular[3];         // glMaterialfw( , , specular)
	GLfloat  shine[1];            // glMaterialfw( , , shine)
	UINT     textureID;           // (0=non, 1, 2, ... max) loaded texture IDs
	UINT     shaderID;            // glShadeModel()   ie.  GL_FLAT, GL_SMOOTH... any loaded shader

	void build(const char* fnm);  // build object FaceList: set object sName & LID w/ vector sequences
	void draw();                  // draw object: uses current pos,rotA,materials,texture,shader & CallList
	void drawBOUND();             // draw object: uses current pos & bounding radius -> CallList?
	void printObj();              // print Object parameters (debug)
	Object();                     // class constructor: w/ build() & all <init> params
	Object(const char* fnm,float*,float*,float*,float*, float*,float*,float*,UINT,UINT);
	Object(const char* fname,float px,float py,float pz, float rA,float rx,float ry,float rz,
	       float fx,float fy,float fz, float aR,float aG,float aB, 
	       float dR,float dG,float dB, float cR,float cG,float cB,	float sh0,UINT tID,UINT sID);
	~Object() {   };              // class destructor
};

class scene {
public: 
   std::vector<Object>    Objects;               // scene objects (all)
   std::vector<GLint>     visObjID;              // list of "visible" objects (objID identifier)
   UINT                   ObjCnt;        // ??   // count of "all" objects (in scene)
   UINT                   visObjCnt;     // ??   // count of "visible" objects
   void add (Object obj);                        // add an Object to the scene
   void kill ();                                 // delete an Object from the scene
   scene() {  ObjCnt=0;  visObjCnt=0;  };        // class constructor
   ~scene() {    };                              // class destructor
};

#endif    // end FUNCTION_H