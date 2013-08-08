Spectrum Engine
============

Simple game development engine/framework built on SFML 2.

Currently 'in progress'. 
Linux compilation not yet tested, though intended.

All dependencies have been resolved, and the source should
compile fine now.
For the source to compile, if using the command-line tools, 
you will have to add the /headers directory to GCC's search 
path, as well as the /source directory.
The libraries are linked in this order:
 -libGME-win32.a
 -libglu32
 -libsfml-audio-s
 -libsfml-graphics-s
 -libsfml-window-s
 -libsfml-system-s
 -libsfml-main
 -libgcc_eh

And the following extra commands are passed to the linker:
 -static-libgcc
 -static-libstdc++






