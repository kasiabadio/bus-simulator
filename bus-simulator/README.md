# bus-simulator

## Table of contents

## General info

## Technologies
- C++
- OpenGL
- GLM, GLFW, GLEW
- Assimp

## Setup
1. In SolutionDir of a project, create folder Dependencies.
2. Create folders for GLM, GLFW, GLEW and assimp.
3. Downloading and pasting certain files into our project structure.
a) GLM
Under $(SolutionDir)Dependencies\GLM\ paste the glm folder with detail, ext, tc etc. subfolders

b) GLFW
Under $(SolutionDir)Dependencies\GLFW\include\GLFW\ you should have glfw3.h and glfw3native.h
Under $(SolutionDir)Dependencies\GLFW\lib\ you should have glfw3.lib

c) GLEW
Under $(SolutionDir)Dependencies\GLEW\ you should have bin\, doc\, include\, lib\ subfolders

d) assimp  
- Install cmake and download assimp.  
- Then go to cmd and to assimp\assimp\ subfolder. Then type cmake -A win32.
- Open assimp.sln and rebuild solution in win32 mode (Release mode).
- Under $(SolutionDir)Dependencies\assimp create include\ and lib\ folders.
- To the include\ subfolder copy the content which is inside include\ folder in assimp original folder.
- To the lib\ subfolder copy binaries which are in lib\Release folder in assimp original folder (copy only those which have .lib extension).
  Paste there also .dll files for bin\Release folder which are in the assimp original folder.

3. Go to bus-simulator project properties (All configurations, x86). 
a) Under C/C++, you have to paste:
$(SolutionDir)Dependencies\assimp\include
$(SolutionDir)Dependencies\GLFW\include
$(SolutionDir)Dependencies\GLEW\include
$(SolutionDir)Dependencies\GLM

b) Under Linker -> additional libraries:
$(SolutionDir)Dependencies\assimp\lib;
$(SolutionDir)\Dependencies\GLFW\lib;
$(SolutionDir)\Dependencies\GLEW\lib\Release\Win32

-> input data -> additional dependencies:  
zlib.lib;IrrXML.lib;assimp-vc142-mt.lib;glew32s.lib;glfw3.lib;opengl32.lib;User32.lib;Gdi32.lib;Shell32.lib