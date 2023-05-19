# turbo-c
Experiments with Turbo C++ for DOS, compiled with version 3.0! Complete with its own C++ version of the BGL, the SBGL. (Second Best Graphics Library)

What have I done.

## Notes
* Due to how memory allocation works, the program heap size needs to be set to a value above 64kb in the "Debugger" menu. The heap size decides how much memory the program gets to use.
* When including external libraries, the .cpp source files need to be added to a "project", which confusingly, is created by using "Open Project". If you don't do this, you'll get an "undefined symbol in module" error that left me stumped for nearly an hour.
* These must be compiled using the "large" memory model! This setting can be found in Options -> Compiler -> Code Generation.
