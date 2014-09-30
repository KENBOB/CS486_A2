// Ken Haney -
// Kenbob.21@csu.fullerton.edu 
// CS486 picking project
// 09/29/2014
// this file picking.cpp - contains main.cpp and picking routine

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdio>
#include <string>
using namespace std;                             // ?? need ??

#include "plyLoader.h"
//#include "objLoader.h"
#include "myObjects.h"

void HelpMenu (void);                            // prototype: diplay HELP_MENU & exit?
void usage (std::string msg);                    // prototype: prints Usage & HELP_MENU message
UINT pickOBS (int x, int y);                     // prototype: pick Objects w/ Bounding SPHERE intersection
bool fileExists (const char *filename);          // prototype: checks if "filename" exists
void initScene (int argc,char **argv);           // prototype: initialize the scene - Parse cmd-line & read object files
void display(void);                              // prototype: displays the scene

GLFWwindow  *window;
int          width=800,  height=600;

static scene      sg;                            // create a scene_obj  (global)
FaceList*         pL;                            // 

const double  D2R=acos(0.0)/90.0;                // const pi/180
const double  R2D=1.0/D2R;                       // const 180/pi

double     camPitch=0.0;                         // CAMERA x-axis rotation, view elevation (up/down)             
double     camYaw=0.0;                           // CAMERA y-axis rotation, view azimuth (left/right)
double     camDIR[3] = {0.0, 0.0, -1.0};         // CAMERA view DIRection (initial)
double     camX=0.0,  camY=0.0,  camZ=15.0;      // CAMERA (x,y,z) position (initial)

double     Theta=45.0;                           // CAMERA Field-Of-View
double     Aspect=double(width)/double(height);  // CAMERA Aspect (initial?)
double     zNear=1.8,  zFar=100.0;               // CAMERA view: FRUSTUM definition
double     hNear_2 = zNear * tan(0.5*Theta);     // CAMERA view: hNear/2  (const)
double     wNear_2 = hNear_2 *Aspect;            // CAMERA view: wNear/2  (*** do not change window Aspect ratio ***)

bool       renderOBJ = true;                     // default: render the Object(s)
bool       GLSL_ON = false;                      // default: use the fixed pipeline
bool       vbON = true;   //  false;                         // verbose flag: used for DEBUG output 
bool       CAPS_LOCK = false;                    // default: assumed OFF at begin
bool       clickON=false;                        // Mouse button: clickON flag
int        xMouse=0,   yMouse=0;                 // current Mouse cursor (x,y)

void HelpMenu( ) {
   char*  sh[13];
   sh[ 0] = "\n\n ------------------ PICKING HELP MENU ---------------------------- ";
   sh[ 1] = "\n Program Displays shaded objects, that change color/spin when picked ";
   sh[ 2] = "\n Pick an Object with mouse click & release (no Drag)                 ";
   sh[ 3] = "\n Objects that are picked (by mouse click) will reverse spin          ";
   sh[ 4] = "\n Note: keyboard is case sensitive                                    ";
   sh[ 5] = "\n -------------------- KEYBOARD OPTIONS ----------------------------- ";
   sh[ 6] = "\n b: toggle rendering the bounding sphere and object                  ";
   sh[ 7] = "\n g: toggle between my GLSL program(s) and fixed function pipline     ";
   sh[ 8] = "\n h: print this HELP message                                          ";
   sh[ 9] = "\n v: toggle verbose on/off (debug output)                             ";
   sh[10] = "\n q:  quit program                                                    ";
   sh[11] = "\n esc: quit program                                                   ";
   sh[12] = "\n ------------------------------------------------------------------- \n";
   for (int i=0; i<13; i++) printf("%s",sh[i]);                  //
}

void usage (std::string msg) {                                   // prints Usage & HELP_MENU message
   std::cerr << msg << std::endl;
   std::cerr << "usage: picking \n" << std::endl;
   HelpMenu();                                                   // also display HelpMenu()
}

static void error_callback (int error, const char* description) {
   if (vbON) fputs(description, stderr);
}

static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) 
{
   if (action != GLFW_PRESS) return;                      // if no key(s) pressed, return 
//   if (GLFW_KEY_CAPS_LOCK) CAPS_LOCK = ! CAPS_LOCK;
//   if (CAPS_LOCK) return;
   if (mods & GLFW_MOD_SHIFT) return;
 
   switch (key) {
   case GLFW_KEY_ESCAPE:                                  // <ESC> to exit Program or
   case GLFW_KEY_Q:                                       // "Q" or "q" key to exit Program
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
   case GLFW_KEY_H:                                       // "H" or "h" key
      HelpMenu();                                         // Display the User Help Menu
	  break;
   case GLFW_KEY_V:                                       // "V" or "v" key
	  vbON = ! vbON;                                      // Toggle verbose DEBUG output
	  printf (" Verbose is %s \n", (vbON ? "ON" : "OFF"));
	  break;
   case GLFW_KEY_B:                                       // Toggle between Object or Bounding Volume
	  renderOBJ = !renderOBJ;                             // Toggle:
	  printf (" renderOBJ is %s \n", (renderOBJ ? "ON" : "OFF"));
	  break;
   case GLFW_KEY_G:                                       // Toggle between GLSL program & fixed pipeline
      GLSL_ON = !GLSL_ON;                                 // Toggle
	  printf (" GLSL program is %s \n", (GLSL_ON ? "ON" : "OFF"));
	  break;              
   default:
	  if (vbON) std::cerr << " Unknown key press \'" << key << "\'" << std::endl;
   }
   return;
}

unsigned int pickOBS (int x, int y)                    // picking scene Objects w/ RAY_SPHERE algorithm
{  Object   O;                   double  m;            // scratch: for mag3
   GLdouble  projMatrix[16];     glGetDoublev( GL_PROJECTION_MATRIX, projMatrix );   // get projection Matrix
   GLdouble  modelMatrix[16];    glGetDoublev( GL_MODELVIEW_MATRIX, modelMatrix );   // get modelMatrix
   GLint  viewPort[4];           glGetIntegerv( GL_VIEWPORT, viewPort );             // get viewPort coords
   GLint   wy = viewPort[3]-1 - (y);                          // set window 'y'
   GLint   wx = (x);                                          // set window 'x'

   GLdouble  xx, yy, zz;                                      // 
   glReadPixels( wx,wy, 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &zz );                   // dont need these?
   gluUnProject( double(wx),double(wy), 0.0, modelMatrix, projMatrix,viewPort, &xx,&yy,&zz );

   gluUnProject( double(wx),double(wy),0.0, modelMatrix, projMatrix, viewPort, &xx,&yy,&zz );
   double E[] = { xx, yy, zz };                               // save near pt, ray EYE
   if(vbON) printf (" PickOBS: Near = %lf,%lf,%lf \n",E[0],E[1],E[2]);
   gluUnProject( double(wx),double(wy),1.0, modelMatrix, projMatrix, viewPort, &xx,&yy,&zz );
   double D[] = { xx-E[0], yy-E[1], zz-E[2] };                // save far-near pt, ray Direction
   unit3(D);                                                  // normalize "Direction" (not needed this ?)
   if(vbON) printf (" PickOBS: Dir  = %lf,%lf,%lf \n",D[0],D[1],D[2]);

   double   C[3],   RR,   EsubC[3],   DdotEsubC,   DdotD,   disc,   t;
   static UINT  oldID=0;  UINT hitID=0;   double tmin=500.0;        // default: no ObjID hit
   if(vbON) printf (" PickOBS:  sg.visObjCnt=%i ?? %i\n", sg.visObjCnt,sg.visObjID.size());
   for (UINT i=0; i<sg.Objects.size(); i++) {  O=sg.Objects[i];  // get current Object
      if (!O.visible) continue;                                     // check if visible
      vadd(C,O.pos,O.ctr);    RR=O.RR;                              // get curObject Center & Radius^2
      vsub(EsubC,E,C);	    DdotEsubC=vdot(D,EsubC);      DdotD=vdot(D,D); 
      disc = sqr(DdotEsubC) -DdotD *(vdot(EsubC,EsubC) -RR);        // RAY_SPHERE discriminant, p77
	  if (disc >= 0.0) {                                            // check if "line" hits ObjID ?
	     t = min( (-DdotEsubC +sqrt(disc))/DdotD, (-DdotEsubC -sqrt(disc))/DdotD );   // eq. p77
		 if (t >= 0.0 && t <= tmin) { tmin=t;  hitID=O.LID;         // save nearest hit ObjID, RAY(t>0)
		    if(vbON) printf(" PickOBS: ** new ** hitID=%d   t=%lf \n",hitID,t);
		 } else {  if(vbON) printf(" PickOBS: missed objID=%d \n",O.LID);   }       
	  } else       if(vbON) printf(" PickOBS: missed objID=%d \n",O.LID);
   }
   if (hitID > 0) {                                          // if hit: change/swap color components
      if (oldID > 0) sg.Objects[oldID-1].select = false;     // un-select old Object (if any)
      oldID=hitID;   sg.Objects[hitID-1].select = true;      // save hit, save latest ObjID
	                  GLfloat temp = sg.Objects[hitID-1].diffuse[0];     // change color components
	  sg.Objects[hitID-1].diffuse[0]=sg.Objects[hitID-1].diffuse[1];
	  sg.Objects[hitID-1].diffuse[1]=sg.Objects[hitID-1].diffuse[2];     
	  sg.Objects[hitID-1].diffuse[2]=temp;      
	  sg.Objects[hitID-1].rotA[0] = -sg.Objects[hitID-1].rotA[0];        // reverse spin direction & incr spin rate ??
   }
   return (hitID);           // needed for DEBUG, Object already "select" herein
}

static void cursorPos_callback (GLFWwindow* window, double xm, double ym)
{
   xMouse=int(xm);     yMouse=int(ym);                     // save latest (x,y) Mouse pos
}

static void mouseBtn_callback (GLFWwindow* window, int button, int action, int mods)
{  
   static int  xpos=-1,  ypos=-1;
   if (button==GLFW_MOUSE_BUTTON_LEFT || button==GLFW_MOUSE_BUTTON_RIGHT) {
	  if (action == GLFW_PRESS) {                                               // button_press_down
         clickON = true;             xpos=xMouse;   ypos=yMouse;                // save current Mouse (x,y)
	//   glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	     glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);            // ?? this one ??
      } else {
	     clickON = false;                                                       // button_release_up
	     glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		 if (abs(xpos-xMouse)<2 && abs(ypos-yMouse)<2) pickOBS( xpos, ypos );   // if NOdrag ? pickOBS(,)
      }                                      // note: tolerate 1 pixel motion
   }
   if (vbON) printf (" mousePos = %d, %d  clickON=%d\n",xMouse,yMouse,clickON);
}

bool fileExists (const char *filename) 
{
   FILE *fh;
   bool rv = true;
   if ( (fh=fopen(filename, "r")) != NULL ) {
	  if(vbON) fprintf(stderr, "%s was Opened OK.\n", filename);
      fclose( fh );
   } else {
      if(vbON) fprintf(stderr, "Error: %s could Not be found.\n", filename);
      rv = false;
   }
   return(rv);
}

void initScene (int argc, char **argv)
{
   char* texFile[6];
   texFile[0] = "land_left.bmp";   texFile[1]="land_back.bmp";  texFile[2]="land_right.bmp";
   texFile[3] = "land_front.bmp";  texFile[4]="land_top.bmp";   texFile[5]="land_bottom.bmp";

   glClearColor(0.3f,0.0f,0.0f, 1.0f);         // Set a background color:  DRK_RED  (else BLACK?)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(Theta, Aspect, zNear, zFar);                  // setup CAMERA Frustum_box (fixed?)

   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_DEPTH_TEST);
   glFrontFace(GL_CCW);
   glEnable(GL_NORMALIZE);
                          // get 6 Texture/images for SkyBox & some Texture/images for GroundPlane
   initSkybox(texFile[0],texFile[1],texFile[2],texFile[3],texFile[4],texFile[5]);    
                          // fileIN==  "cube"  "pyrm.ply", "tetra.ply"       // add object to the scene
   sg.add(Object("cube",     -3.f,-3.f,0.f,  60.f,-0.7f,1.0f,0.4f, .7f,.6f,.9f, .3f,.3f,.3f, .6f,.2f,.8f, .70f,.70f,.70f,100.f, 9,2));
   sg.add(Object("cube",     -2.f,-3.f,-9.f, 80.f,-0.7f,1.0f,0.4f, .8f,.8f,.8f, .3f,.3f,.3f, .0f,.8f,.7f, .50f,.50f,.50f, 90.f,10,2));
   sg.add(Object("boxr.ply",  3.f,-1.f,0.f,  50.f,-0.7f,1.0f,0.4f, .5f,.7f,.4f, .6f,.6f,.6f, .6f,.2f,.8f, .70f,.70f,.70f,120.f, 5,2)); 
   sg.add(Object("pyrm2.ply", 4.f, 1.f,2.f, 80.f, 0.0f,1.0f,0.0f, .7f,.7f,.7f, .3f,.3f,.3f, .9f,.1f,.6f, .63f,.63f,.63f, 75.f, 9,2));  
   sg.add(Object("pyrm2.ply", 4.f, 2.f,-8.f, 80.f, 0.0f,1.0f,0.0f, .7f,.7f,.7f, .3f,.3f,.3f, .9f,.1f,.6f, .63f,.63f,.63f, 75.f,10,2));    
   sg.add(Object("tetra.ply",-3.f, 0.f,0.f,  50.f,-0.7f,1.0f,0.4f, .7f,.7f,.7f, .5f,.5f,.5f, .7f,.2f,.4f, .60f,.60f,.60f, 80.f, 0,2));
// sg.add(Object("pyrm.ply",  0.f, 2.0f,7.f,120.f, 0.0f,1.0f,0.0f, .5f,-.5f,.5f,.5f,.5f,.5f, .1f,.1f,.8f, .63f,.73f,.63f,100.f, 7,2));
// sg.add(Object("pyrm.ply",  0.f,1.5f,7.f,-120.f, 0.0f,1.0f,0.0f, .5f,.8f,.5f, .5f,.5f,.5f, .1f,.1f,.8f, .63f,.73f,.63f,100.f, 5,2));

   glfwSetTime( 0.0 );             // reset clock timer
}

void display ()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();                   
   glRotatef((GLfloat)camPitch, 1.0f,0.0f,0.0f);        // camera Pitch rotation about x-axis (up/down)
   glRotatef((GLfloat)camYaw, 0.0f,1.0f,0.0f);          // camera Yaw rotation about y-axis (left/right)
   drawSkybox(50.0);                                    // draw Skybox {+/-50 cube}
   glTranslatef((GLfloat)(-camX), (GLfloat)(-camY), (GLfloat)(-camZ));       // camera to position (camX,camY,camZ)

   glEnable(GL_LIGHTING);                               // enable lighting
   glEnable(GL_LIGHT0);                                 // enable light0
   glShadeModel(GL_SMOOTH);                             // default
     GLfloat l_ambient[] = {0.5, 0.5, 0.5, 1.0};
     GLfloat l_diffuse[] = {1.0, 1.0, 1.0, 0.0};
     GLfloat l_position[]= {2.0, 3.0,10.0, 0.0};
   glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, l_position);  
                                                        // draw all "visible" objects (in scene)
   for (UINT k=0; k<sg.Objects.size(); k++) {
	  if ( renderOBJ ) sg.Objects[k].draw();            //  draw OBJECTs
      else             sg.Objects[k].drawBOUND();       //  draw Bounding Volume of OBJECTs
   }
   const GLdouble clr[] = { 1.0,1.0,1.0 };                        // reset color white
   glColor3dv(clr);
}

void reshape (GLFWwindow* window, int w, int h)
{
   glViewport( 0, 0, w, h );         // Aspect = float(w)/float(h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective( Theta, Aspect, zNear, zFar );
   glMatrixMode ( GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt( 0.0,0.0,-camZ,  0.0,0.0,0.0,  0.0,-1.0,0.0);
}

int main (int argc, char** argv)                                  // main Program Entry
{
   glfwSetErrorCallback(error_callback);                          // set Error Callback method

   if( !glfwInit( ) ){                                            // init GLFW
	  fprintf (stderr, "Error: Failed to initialize GLFW\n");  
      exit(EXIT_FAILURE);                                         // kill Program
   }
   glfwWindowHint(GLFW_DEPTH_BITS, 16);                           // set DEPTH_BITS
   window = glfwCreateWindow(width,height,"myPICKING",NULL,NULL);    // create initial window
   if( !window ){                                                    // error
	  fprintf (stderr, "Error: Failed to open GLFW window\n");  
      glfwTerminate( );                                           // kill Program
	  exit(1);  
   }
   glfwSetKeyCallback(window, key_callback);                      // Set KeyPress Callback: key controls
   glfwSetCursorPosCallback(window, cursorPos_callback);          // Set Mouse Cursor: picking & camView direction
   glfwSetMouseButtonCallback(window, mouseBtn_callback);         // Set Mouse Button: picking & camView direction
   glfwMakeContextCurrent(window);                                // make context the current window
// glfwSwapInterval(1);
   glfwGetFramebufferSize(window, &width, &height);               // check for size of window?
// reshape (window, width, height);    // >>> re-set  hFar_2 = wFar_2 *(width/height); 
   glViewport(0, 0, width, height);                               // set viewport
  
   initScene(argc, argv);      // Parse command-line options ?    // initialize scene -> insert Objs

   while (!glfwWindowShouldClose(window)) {                       // check for exit/done/close
	   display();                                                 // draw visible scene object(s)
       glfwSwapBuffers(window);                                   // show new scene
       glfwPollEvents();                                          // check for input events.
   }
   glfwDestroyWindow(window);                                     // delete "window"
   killSkybox();
   glfwTerminate( );                                              // terminate program
   return(0);
}
