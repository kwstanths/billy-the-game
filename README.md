# billy-the-game
Billy the game is a personal project to create an open world 2D game. 
The goal is to learn basic rendering techniques, physics, memory 
management, data structures and anything else that might be used 
in the development of a game

## Current progress
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/0BQiodqsgZk/0.jpg)](https://www.youtube.com/watch?v=0BQiodqsgZk)

## Build and run instructions
- Create a build folder.  
  - In the top level directory create folder:  {top-levell-directory}/build  
- Run cmake with destination the build folder, and source folder the top level directory  
  - Navigate into the build folder, and type: cmake ../  
- Compile the project  
  - If running on windows, open the .sln project file with visual studio and build the project. If running on Linux run make  
- Locate the executable and copy on the same folder the dependencies  
  - The folders: assets, fonts, src/shaders  
  - If running on Windows the freetype271.dll from src/lib/external/freetype/Windows/lib/win32/. On linux, freetype.a is linked statically, if i remember correctly.  
