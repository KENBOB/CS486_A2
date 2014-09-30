// Kendall Haney
// Kenbob.21@csu.fullerton.edu 
// CS486 picking project
// 09/29/2014
// this file: myObjects.cpp contains the skybox & object functions

#include "myObjects.h"
extern bool  vbON;

Gf3::Gf3(float a, float b,float c) { x=a;  y=b;  z=c; }    // constructor: loads the vector

enum {SKY_LEFT=0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM, GRASStex,CRATEtex,MPLATEtex,CHECKERtex};   //skybox image IDs
unsigned int texIMG[10];                                   //the textureIDs for the skybox cube faces

//loads 6 Skybox Faces & some textures to array
void initSkybox(char* leftFACE, char* backFACE, char* rightFACE, char* frontFACE, char* topFACE, char* bottomFACE)
{
   texIMG[SKY_LEFT]  = loadTexture(leftFACE);
   texIMG[SKY_BACK]  = loadTexture(backFACE);
   texIMG[SKY_RIGHT] = loadTexture(rightFACE);
   texIMG[SKY_FRONT] = loadTexture(frontFACE);
   texIMG[SKY_TOP]   = loadTexture(topFACE);
   texIMG[SKY_BOTTOM]= loadTexture(bottomFACE);
   if (vbON) printf ("skybox Faces:  %d %d %d %d %d %d\n",texIMG[0],texIMG[1],texIMG[2],texIMG[3],texIMG[4],texIMG[5]);

   texIMG[GRASStex]  = loadTexture("grass24.bmp");           // use this for a Ground Plane
   texIMG[CRATEtex]  = loadTexture("crate24.bmp");           // use these for objects
   texIMG[MPLATEtex] = loadTexture("mplate24.bmp");
   texIMG[CHECKERtex]= loadTexture("checker24.bmp");
   if (vbON) printf ("textures: %d %d %d %d\n",texIMG[6],texIMG[7],texIMG[8],texIMG[9]);
}
  
//loads a texture/image from a BMP(24bit) file - format BMP(5_6_5bit) not supported... yet 
unsigned int loadTexture (const char* filename)    // load a "BMP" filename texture/image
{  unsigned char tb;                               // temp byte for swapping RGB -> BGR
   unsigned char header[54];                       // Each BMP file begins with 54-byte header
   unsigned int dataPos;                           // Position in file where actual data begins
   unsigned int wide, high;
   unsigned int imageSize;                         // #bytes= wide*high*3 (typ BMP)
   unsigned char *data;                            // pointer to data pixels
   char fname[200];
   
   sprintf(fname,"data/%s",filename);              // assumed sub-directory: data
   FILE *fh = fopen(fname,"rb");
   if (!fh) {  printf("Error: Image could not be opened\n");   return -1;  }  
   else{      if(vbON) printf("Image %s opened OK\n",filename);  }

   if ( fread(header, 1, 54, fh) != 54) {          // read header
	  printf("Error: %s file Header Not correct BMP format.\n",filename);   
	  return -1;  
   }
   if (header[0]!='B' || header[1]!='M') {         // check header
	  printf("Error: %s file Header Not correct BMP format.\n",filename);   
	  return 0;   
   }
   dataPos  = *(int*)&(header[0x0A]);              // extract data location (1st pixel?)
   imageSize= *(int*)&(header[0x22]);              // extract imageSize [pixels]
   wide     = *(int*)&(header[0x12]);              // extract image width [pixels]
   high     = *(int*)&(header[0x16]);              // extract image height [pixels]
   if(vbON) printf("Image %s param:  w=%d  h=%d dP=%d  imgSZ=%d\n",filename,wide,high,dataPos,imageSize);  
   if (imageSize==0) imageSize=wide*high*3;        // best guess (typical fix)
   if (dataPos==0)   dataPos = 54;                 // best guess (typical fix)
   data = new unsigned char[imageSize];            // allocate mem
   fread (data, 1, imageSize, fh);                 // read pixel data
   fclose(fh);                                     // close file
   for (unsigned int i=0; i<imageSize; i+=3) { tb=data[i]; data[i]=data[i+2]; data[i+2]=tb; }

   unsigned int num;                               // textureID for the openGL
   glGenTextures(1,&num);                          // generate a unique textureID
   glBindTexture(GL_TEXTURE_2D, num);              //register? the texture, we have just generated
   glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, wide,high, 0,GL_RGB,GL_UNSIGNED_BYTE, data);  // give image to OpenGL ?
// glfwLoadTexture2D(filename, 0);            /////// not working...

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);   //if texture < image, use avg of pixels (smoother)
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);   //same if the image bigger
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);        //hide cube edges w/ repeat pixels
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);        //do same vertically and horizontally
   return num;                                     //return the textureID
}

//delete all textures from skybox array (avoid memory leaks)
void killSkybox()
{
   glDeleteTextures(10,&texIMG[0]);
}

//draws the Skybox & the Ground Plane 
void drawSkybox (float sf)
{
  #define myTex4Face(t,u0,v0,x0,y0,z0,u1,v1,x1,y1,z1,u2,v2,x2,y2,z2,u3,v3,x3,y3,z3) \
   { \
     glBindTexture(GL_TEXTURE_2D, t); \
     glBegin(GL_QUADS); \
     glTexCoord2f(u0,v0);   glVertex3f(x0,y0,z0); \
	 glTexCoord2f(u1,v1);   glVertex3f(x1,y1,z1); \
	 glTexCoord2f(u2,v2);   glVertex3f(x2,y2,z2); \
	 glTexCoord2f(u3,v3);   glVertex3f(x3,y3,z3); \
     glEnd(); \
   }

   GLboolean b1=glIsEnabled(GL_TEXTURE_2D);          // save if textures ON at start
   glDisable(GL_LIGHTING);                           // turn OFF lighting, when making the skybox
   glDisable(GL_DEPTH_TEST);                         // turn OFF depth texting   
   glEnable(GL_TEXTURE_2D);                          // turn ON texturing 
                                                     // Draw 6 Faces of skybox w/ images
   myTex4Face(texIMG[SKY_BACK],  1,1, sf,sf,sf, 0,1,-sf,sf,sf,  0,0,-sf,-sf,sf,  1,0,sf,-sf,sf);
   myTex4Face(texIMG[SKY_LEFT],  1,1,-sf,sf,sf, 0,1,-sf,sf,-sf, 0,0,-sf,-sf,-sf, 1,0,-sf,-sf,sf);
   myTex4Face(texIMG[SKY_FRONT], 0,1,sf,sf,-sf, 1,1,-sf,sf,-sf, 1,0,-sf,-sf,-sf, 0,0,sf,-sf,-sf);
   myTex4Face(texIMG[SKY_RIGHT], 1,1,sf,sf,-sf, 0,1,sf,sf,sf,   0,0,sf,-sf,sf,   1,0,sf,-sf,-sf);
   myTex4Face(texIMG[SKY_TOP],   0,0,sf,sf,sf,  0,1,-sf,sf,sf,  1,1,-sf,sf,-sf,  1,0,sf,sf,-sf);
   myTex4Face(texIMG[SKY_BOTTOM],0,1,sf,-sf,sf, 0,0,-sf,-sf,sf, 1,0,-sf,-sf,-sf, 1,1,sf,-sf,-sf);

// And since we are here:  one more Texture for the Ground Plane - use GRASStex ?
   float   sE =(float)sf/8;    float   sG =-2.0f;
   myTex4Face(texIMG[GRASStex],  0,1,sE,sG,sE,  0,0,-sE,sG,sE,  1,0,-sE,sG,-sE,  1,1,sE,sG,-sE);

   glEnable(GL_LIGHTING);                  //turn lighting back ON, ie.(OFF -> ON -> OFF) and (ON -> OFF -> ON)
   glEnable(GL_DEPTH_TEST);                //turn depth_test back ON
   if(!b1) glDisable(GL_TEXTURE_2D);       //disable (if was OFF)
}

//draws a Cube/box w/ Texture on Face 'one'
FaceList* myCube (float sx, float sy, float sz, UINT texID)
{
   FaceList*  mL;                               // make pointer for new FaceList
   int n=0;   int  nv=8;   int  nf=12;          // cube: 8 verts, 12 faces  (default)

  #define myNor4TexF(nx,ny,nz,t,u0,v0,x0,y0,z0,u1,v1,x1,y1,z1,u2,v2,x2,y2,z2,u3,v3,x3,y3,z3) \
   { \
     if (t>0) { glDisable(GL_LIGHTING);  glEnable(GL_TEXTURE_2D);  glBindTexture(GL_TEXTURE_2D, t); } \
     glBegin(GL_TRIANGLES); \
      glNormal3f(nx,ny,nz); \
      if(t>0) glTexCoord2f(u0,v0);   glVertex3f(x0,y0,z0); \
      if(t>0) glTexCoord2f(u1,v1);   glVertex3f(x1,y1,z1); \
	  if(t>0) glTexCoord2f(u2,v2);   glVertex3f(x2,y2,z2); \
      if(t>0) glTexCoord2f(u3,v3);   glVertex3f(x3,y3,z3); \
	  if(t>0) glTexCoord2f(u0,v0);   glVertex3f(x0,y0,z0); \
	  if(t>0) glTexCoord2f(u2,v2);   glVertex3f(x2,y2,z2); \
     glEnd(); \
	 if (t>0) { glEnable(GL_LIGHTING);  glDisable(GL_TEXTURE_2D); } \
	 if (n<nv) {   mL->vertices[n][0]=x0;   mL->vertices[n][1]=y0;  mL->vertices[n++][2]=z0; \
	               mL->vertices[n][0]=x1;   mL->vertices[n][1]=y1;  mL->vertices[n++][2]=z1; \
				   mL->vertices[n][0]=x2;   mL->vertices[n][1]=y2;  mL->vertices[n++][2]=z2; \
				   mL->vertices[n][0]=x3;   mL->vertices[n][1]=y3;  mL->vertices[n++][2]=z3; \
     } \
   }
   
   if (sz == 0.0) {        nv=4;   nf=2;   }             // quad: 4 verts,  2 faces
   if ( !(mL = new FaceList(nv,nf)) ) {    }             // allocate new FaceList
   mL->vc = nv;     mL->fc = nf;                         // set vc & fc  (cube or quad)
   myNor4TexF(0.0,0.0,1.0,texID, 0,0, sx, sy, sz, 1,0,-sx, sy, sz, 1,1,-sx,-sy, sz, 0,1, sx,-sy, sz);     // front: 0,1,2,3,0,2
   mL->faces[0][0]=0;  mL->faces[0][1]=1;  mL->faces[0][2]=2;  mL->faces[1][0]=3;  mL->faces[1][1]=0;  mL->faces[1][2]=2;
   myNor4TexF(0.0,0.0,-1.0,   0, 0,0, sx, sy,-sz, 1,0, sx,-sy,-sz, 1,1,-sx,-sy,-sz, 0,1,-sx, sy,-sz);  // back:  4,7,6,5,4,6
   mL->faces[2][0]=4;  mL->faces[2][1]=7;  mL->faces[2][2]=6;  mL->faces[3][0]=5;  mL->faces[3][1]=4;  mL->faces[3][2]=6;
   myNor4TexF(-1.0,0.0,0.0,   0, 0,0,-sx, sy, sz, 1,0,-sx, sy,-sz, 1,1,-sx,-sy,-sz, 0,1,-sx,-sy, sz);  // left:  1,5,6,2,1,6
   mL->faces[4][0]=1;  mL->faces[4][1]=5;  mL->faces[4][2]=6;  mL->faces[5][0]=2;  mL->faces[5][1]=1;  mL->faces[5][2]=6;
   myNor4TexF(1.0,0.0,0.0,    0, 0,0, sx, sy,-sz, 1,0, sx, sy, sz, 1,1, sx,-sy, sz, 0,1, sx,-sy,-sz);  // rght:  4,0,3,7,4,0
   mL->faces[6][0]=4;  mL->faces[6][1]=0;  mL->faces[6][2]=3;  mL->faces[7][0]=7;  mL->faces[7][1]=4;  mL->faces[7][2]=0;
   myNor4TexF(0.0,1.0,0.0,texID-1,0,0,sx, sy, sz, 1,0, sx, sy,-sz, 1,1,-sx, sy,-sz, 0,1,-sx, sy, sz);  // top:   0,4,5,1,0,5
   mL->faces[8][0]=0;  mL->faces[8][1]=4;  mL->faces[8][2]=5;  mL->faces[9][0]=1;  mL->faces[9][1]=0;  mL->faces[9][2]=5;
   myNor4TexF(0.0,-1.0,0.0,   0, 0,0, sx,-sy, sz, 1,0,-sx,-sy, sz, 1,1,-sx,-sy,-sz, 0,1, sx,-sy,-sz);  // botm:  3,2,6,4,3,6
   mL->faces[10][0]=3;  mL->faces[10][1]=2;  mL->faces[10][2]=6;  mL->faces[11][0]=4;  mL->faces[11][1]=3;  mL->faces[11][2]=6;
   return( mL );
}

enum objTYP { CUBE=0, PLYfile, OBJfile};  

void Object::build (const char* objName)
{  
   int oTYP=0;       int e=strlen(objName);       char    fname[200];
   sName = (char*)objName;                           // set object Name
   if (strcmp(objName,"cube\0")==0) oTYP=CUBE;       // simple CUBE (3D/2D)
   if (objName[e-3]=='p')           oTYP=PLYfile;    // read PLY file
   if (objName[e-3]=='o')           oTYP=OBJfile;    // read OBJ file
   if (vbON) printf ("Object::build: objName=%s %d  TYP=%d\n",objName,e,oTYP);

   switch ( min(oTYP,1) ) {                          // PLYfile or CUBE ? for now
	  case PLYfile:
		  sprintf(fname,"data/%s",objName);          // assume *.OBJ is in sub-directory: data
		  fl =pL =readPlyModel(fname);               // read in the objName file
          LID = glGenLists(1);                       // get an ID for this object
		  glNewList (LID, GL_COMPILE);               // begin an New List
		     for (int i=0; i<pL->fc; i++) {      // Loop thru each Face
				if (textureID>0) { glDisable(GL_LIGHTING);  glEnable(GL_TEXTURE_2D);  glBindTexture(GL_TEXTURE_2D,textureID);  }
				glBegin(GL_TRIANGLES);
		        for (int j=0; j<3; j++) {            // ...Loop thru each Triangle vertex
		     	  if (textureID > 0) {  glTexCoord2f(float(j/2),float(j%2));   }       /////    macro TBD ???
		     	  else {   glColor3dv(pL->colors[pL->faces[i][j]]);
				           glNormal3dv(pL->v_normals[pL->faces[i][j]]);  }
				  glVertex3dv(pL->vertices[pL->faces[i][j]]);
			    }
		        glEnd();                                // end of Triangles
				if (textureID>0) { glEnable(GL_LIGHTING);  glDisable(GL_TEXTURE_2D);  }
			 }
		  glEndList();                               // end of List
		  vcopy(ctr,pL->center);                     // set PlyModel Bounding center(x,y,z)
		  RR = sqr(pL->radius);                      // set PlyModel Bounding radius^2
		  if(vbON) printf(" PLYfile: %s object Loaded OK.\n",sName);
		  if(vbON) printf(" PLYfile: R=%lf, ctr=%lf %lf %lf\n",sqrt(RR),ctr[0],ctr[1],ctr[2]);  //  getchar();
		  break;

//	  case OBJfile:   LID =loadOBJ( objName );   
//	     break;

	  case CUBE:
         LID = glGenLists(1);                        // get/save ListID to use for this Object
         glNewList(LID, GL_COMPILE);                 // begin BUILDing object: ListID of Triangles
		    fl = myCube(sx,sy,sz,textureID);         // gen List of primatives
	     glEndList();
		 sx =sy =sz =1.0;                            // now disable scale: done by drawCube()
		 ctr[0] =ctr[1] =ctr[2] =0.0;                // set Bounding Sphere center: (0,0,0)
		 RR = float(sx*sx +sy*sy +sz*sz);            // set Bounding Sphere radius^2
		 if(vbON) printf(" myCUBE %s object Loaded OK.\n",sName);
		 break;

	  default:                                       // should not get here?                  
         break;    
   }
};

void Object::draw (void)
{ 
   GLfloat  rotAng,  diSEL;
   rotAng = GLfloat(rotA[0] *3*glfwGetTime());                       // default: animation= spinning
   if (select) {  
//	  diSEL=diffuse[0];  diffuse[0]=diffuse[1];  diffuse[1]=diffuse[2];  diffuse[2]=diSEL;  // rotate color bytes (blink?)
   } 
   glPushMatrix();
   	 glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);                    // set ambient material property
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);                    // set diffuse material property
     glMaterialfv(GL_FRONT, GL_SPECULAR, specular);                  // set specular material property
     glMaterialfv(GL_FRONT, GL_SHININESS, shine);                    // set shine material property	 
	 if (shaderID > 0) glShadeModel(shaderID);                       // set object shader 
     glTranslatef(pos[0],pos[1],pos[2]);                             // set object position
     glRotatef(rotAng, rotA[1],rotA[2],rotA[3]);                     // set object orientation (rotAng,rotAxis[])
     glScalef(sx,sy,sz);                                             // set object scale factors
     glCallList( LID );                                              // the ListID of Triangles w/Texture?
   glPopMatrix();
};

void Object::drawBOUND (void)                                        // draw Bounding Sphere at pos[]
{ 
   glPushMatrix();
	 if (shaderID > 0) glShadeModel(shaderID);                       // set object shader 
   	 glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);                    // set ambient material property
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);                    // set diffuse material property
     glMaterialfv(GL_FRONT, GL_SPECULAR, specular);                  // set specular material property
     glMaterialfv(GL_FRONT, GL_SHININESS, shine);                    // set shine material property	 
     glTranslatef(pos[0],pos[1],pos[2]);                             // set object position
	 GLUquadric* m_sphere_quad = gluNewQuadric();
     gluSphere(m_sphere_quad, 1.0, 30, 30);                          // draw sphere
     gluDeleteQuadric(m_sphere_quad);
   glPopMatrix();
};

void Object::printObj( ) { 
    printf (" Object: pos     = %f %f %f  LID=%i\n",pos[0],pos[1],pos[2],LID);
    printf (" Object: rotA    = %f %f %f %f\n",rotA[0],rotA[1],rotA[2],rotA[3]);
    printf (" Object: sx,sy,sz= %f %f %f \n",sx,sy,sz);
    printf (" Object: ambient = %f %f %f \n",ambient[0],ambient[1],ambient[2]);
    printf (" Object: diffuse = %f %f %f \n",diffuse[0],diffuse[1],diffuse[2]);
    printf (" Object: specular= %f %f %f \n",specular[0],specular[1],specular[2]);
    printf (" Object:shine,texture,shader= %f %i %i \n",shine[0],textureID,shaderID);  // getchar();
}

Object::Object( ) {   sName=NULL;   fl=NULL;  LID=0;   visible=false;  select=false;   }     // blank constructor

// Object constructor - set all parms  (initial)
Object::Object(const char* fname,float *p,float *r,float *f,float *a, float *d,float *s,float *h,UINT tID,UINT sID) 
{
   visible=true;     select=false;
   pos[0]=p[0];      pos[1]=p[1];      pos[2]=p[2];                   // set object position
   rotA[0]=r[0];     rotA[1]=r[1];     rotA[2]=r[2];   rotA[3]=r[3];  // set object orientation/rotation
   sx=f[0];          sy=f[1];          sz=f[2];                       // set object size factor
   ambient[0]=a[0];  ambient[1]=a[1];  ambient[2]=a[2];               // set material: ambient
   diffuse[0]=d[0];  diffuse[1]=d[1];  diffuse[2]=d[2];               // set material: diffuse
   specular[0]=s[0]; specular[1]=s[1]; specular[2]=s[2];              // set material: specular
   shine[0]=h[0];                                                     // set material: shine
   textureID=tID;                                                     // set object textureID
   shaderID=sID;                                                      // set object shaderID
   build (fname);             // Load fname & create FaceList, set sName & LID (FaceList sequence)
   if(vbON) printObj();
};

// Object constructor - set all parms (individually)
Object::Object(const char* fname, float px,float py,float pz, float rA,float rx,float ry,float rz,
	float fx,float fy,float fz, float aR,float aG,float aB, 
	float dR,float dG,float dB, float sR,float sG,float sB,	float sh, UINT tID, UINT sID) 
{
   visible=true;     select=false;
   pos[0]=px;        pos[1]=py;        pos[2]=pz;                     // set object position
   rotA[0]=rA;       rotA[1]=rx;       rotA[2]=ry;     rotA[3]=rz;    // set object orientation/rotation
   sx=fx;            sy=fy;            sz=fz;                         // set object size factor
   ambient[0]=aR;    ambient[1]=aG;    ambient[2]=aB;                 // set material: ambient
   diffuse[0]=dR;    diffuse[1]=dG;    diffuse[2]=dB;                 // set material: diffuse
   specular[0]=sR;   specular[1]=sR;   specular[2]=sB;                // set material: specular
   shine[0]=sh;                                                       // set material: shine
   textureID=tID;                                                     // set object textureID
   shaderID=sID;                                                      // set object shaderID
   build (fname);             // Load fname & create FaceList, set sName & LID (FaceList sequence)
   if(vbON) printObj();
};

void scene::add (Object obj) {         
	Objects.push_back( obj );               // add Object to scene vector
	ObjCnt++;                               // inc scene Object count ?? not needed -> use vector.size ?
	if(vbON) printf (" new Object: sName=%s  LID=%d Cnt=%i visCnt%i\n",Objects[ObjCnt-1].sName,Objects[ObjCnt-1].LID,ObjCnt,visObjCnt);
};

void scene::kill (void) {

};
