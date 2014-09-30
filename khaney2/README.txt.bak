/*
README.txt file

Ken Haney
Kenbob.21@csu.fullerton.edu
CS 486
9/29/2014

Assignment 2  -  Picking Objects Project

Description:     This program contains the graphics rendering code and user interface code 
		 as created by Ken Haney. This program builds *.ply file objects and elongated Cubes from 
		 triangles and displays them using openGL and GLFW. Some objects are shaded with different 
		 textures on some faces allowing the remaining faces to change color.  Objects reverse spin 
		 direction (animation) when the object is picked.  The Camera direction is fixed forwards. 
		 Objects will change to bounding spheres when "b" is enabled and the spheres will still 
		 change color when selected but do not move. 

Files Included: 
		 picking.vcxproj
		  
		 picking.cpp
		 myObjects.cpp
		 shaders.cpp
		 plyLoader.cpp

		 myObjects.h
		 shaders.h
		 common_header.h
		 plyLoader.h
		 FaceList.h
		 Vec3.h
		 Mat3.h
		 
		 README.txt (this)

		 supporting files stored in "data/" subdirectory include:
		 the Texture/Image file for the SkyBox (required):
		    land_top.bmp,   land_bottom.bmp, 
		    land_front.bmp, land_back.bmp,
                    land_left.bmp,  land_right.bmp
                 other object Textures are:
                    grass24.bmp, mplate24.bmp, crate24.bmp, checker24.bmp
                 *.ply files read in are:
                    boxr.ply, pyrm2.ply, tetra2.ply
		 
To run from the command line:   
		 prompt > picking
		  

Application User Interface:  
		 Once the program is running, the user can use the keyboard and mouse to interact with
		 the program.  The keyboard controls are case sensitive.
                 
		 The user Help Menu can be invoked at any time be typing "h" key which displays
		 the full list of available user commands. I have added "v" for DEBUG output. 
		 The "v" key toggles the verbose mode on and off, and is set initially to "ON".
		 The "b" key toggles the rendering of objects or bounding spheres.
		 The "g" key is intended to toggle the fixed pipeline and shader (which is in progress).
		 The "h" key displays the Help menu, and the "q" or <esc> key quits the program.

Note:    This code does not complete all the texturing shader rendering implementation, although many sample
         FRAG and VERT shaders are included in "shaders" subfolder.  Camera Control is not implimented at 
	 this time. Other *.ply objects in the data/ subfolder could be used, but some computed centers 
	 did not seem to be right. Implimentation of reading and including *.obj type files is not complete. 
	 Object animation is limitted to spinning objects and spin direction. 