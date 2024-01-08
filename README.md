# Comp703-Submission

Recreating Doodle Jump in 3D with nothing but Visual Studio and my blood, sweat and tears. Mostly tears

My name is Tolu Ogbaro and I'm a masters student on the Msc Programming Course

Creative Commons Licence if you really want to

For the Comp703 module we're using digital games technology to recreate modern classics using low level graphics and game engine components from scratch 
without using a game engine. 

My API of choice is OpenGL.

For this remake, my plan has changed to having just a single slime character platforming.

To play around with the project in its current form requires downloading the solution file and setting up the include directories as follows:

In the solution properties general configurations first set the C++ Language Standard to C++17.

In C/C++ General Additional Include Directories:

$(SolutionDir)Dependencies\glfw\include
$(SolutionDir)Dependencies\glew\include
$(SolutionDir)src/vendor
$(SolutionDir)src

In C/C++ Preprocessor. Set the Preprocessor Definitions to GLEW_STATIC

In Linker General set the Addition Include Libraries:

$(SolutionDir)Dependencies\glfw\lib-vc2022
$(SolutionDir)Dependencies\glew\lib\Release\Win32

In Linker Input set the Additional Dependencies:

glfw3.lib
glew32s.lib
opengl32.lib
User32.lib
Gdi32.lib
Shell32.lib

Then to wrap it up, add the bin and include files from glew into the glew folder to link glew32s.lib

and finally make sure the project is configured for ALL CONFIGURATIONS and set the platform to Windows32

My steps to achieve this project has been:

1 -> Learning how to render using OpenGL

2 -> Creating a basic physics engine using an Entity Component System with entt library that simulates real world physics

3 -> Using the engine to simulate softbody physics and creating a slime shader with refraction

From mid November 2023 - Beginning of December 2023, Im just getting used to OpenGL and going through some basic tutorials

From December 2023 onward, I will be attempting to do step 2

From Beginning of January 2024, I'll focus on Step 3 and the finishing touches until its submission time

For my stretch goals, I want to add SDL audio and make a couple custom game tunes with Bosca Ceoil if I have the time

Thanks for stopping by and witnessing my suffering

Update: Unfortunately I ran into complications with my laptop being broken over the christmas holiday.
From December 22nd - January 3rd I haven't been able to do much but the project is still underway