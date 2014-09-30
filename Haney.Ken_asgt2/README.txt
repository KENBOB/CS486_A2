/*
README.txt file

Ken Haney
Kenbob.21@csu.fullerton.edu
CS 486
December 3, 2013

Assignment  -    Separating Axes Project

Description:     This program contains the graphics rendering code and user interface code 
		 as created by Ken Haney. This program builds 5 Quad models from triangles 
		 and displays them using openGL and GLFW.   The "cube" model was modified so that the
		 first 4 vertices are the front face of a Quad by setting the cube sz (z-size) to zero.  
		 The Each object is shaded with different textures.  The Frustum Culling code has been 
		 revised to "bounce" the Quad objects off the window walls using the SAT method (x,y,z=0) 
		 when the Object reaches the edge.  The algorithm should be easily expandable to 3D.  The 
		 SAT method is also used to detect collisions, where upon both objects will reverse 
		 direction.  The Camera can be moved forwards and backwards with arrow keys (see Help Menu)
		 and dragging the mouse in a direction directs the viewer towards that orientation. 
		 Objects can still be "picked" by clicking on & off (without moving) and the objects will 
		 reverse spin and may blink/rotate colors. However, picking is rather dificult when the 
		 objects are colliding and moving so fast. 

Files Included: 
		 sepaxes.vcxproj
		  
		 sepaxes.cpp
		 functions.cpp
		 shaders.cpp
		 plyLoader.cpp

		 functions.h
		 shaders.h
		 common_header.h
		 FaceList.h
		 Vec3.h
		 Mat3.h
		 
		 README.txt (this)

		 other supporting files (included) are:
		 the Texture/Image file for the SkyBox (required):
		    land_top.bmp, land_bottom.bmp, 
		    land_front.bmp, land_back.bmp,
                    land_left.bmp,  land_right.bmp
                 for other object Textures:
                    grass24.bmp, mplate24.bmp, crate24.bmp, checker24.bmp
		 
		  
Application User Interface:   
                 
		 The user Help Menu can be invoked at any time be typing "h" key which displays
		 the full list of available user commands - I have added a few.  For example
		 using the mouse scroll to zoom in and out instead of the up/down arrows.
		 The "v" key toggles the verbose mode on and off, even though set (initially)
		 by the command line.   Other *ply objects can be added by the command line
		 but their placement is fixed (so more than one will simply overlap, for now).
		 The "q" or <esc> key quits the program as expected.

This code does not complete all the texturing shader rendering implementation, although many sample
         FRAG and VERT shaders are included in "shaders" folder.  Camera Control does not (at 
	 this time) include any Quaternion operations. Implimentation of reading and including 
	 *.OBJ files is not complete.  In the Debug mode, Status information has been reduced to 
	 observing Window bounce and collisions as output.  The object animation is limitted 
	 to what is programmed - namely the moving quads.  
	 
	 Also noted in testing, the it may be possible to "lose" objects (out of view) if the 
	 zoom or cammera orientation is moved too fast or far. Since the Quads are in the x-y plane,
	 they can travel out of the skybox, but they will eventually come back into view.
	 Also, with high speeds (per frame), the quads may get "stuck" on each other as overlapping
	 quads will continue to collide and taggle their velocity going nowhere.  Simply move the
	 camera viewer so that the Quads "untangle" themselves off the screen.
	 
	 Also, the camera algorithms and controls are still embedded in "sepaxis.cpp" and should
	 be made into a camera class (tbd).