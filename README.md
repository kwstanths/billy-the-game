# billy-the-game
Billy the game is a personal project to create an open world 2D game. 
The goal is to learn basic rendering techniques, physics, memory 
management, data structures and anything else that might be used 
in the development of a game

## Current progress - billy the game
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/RobalKH8GVY/0.jpg)](https://www.youtube.com/watch?v=RobalKH8GVY&list=PLocuszpm1snUGVn5kgk-LlLE86c4E2u7H&index=9)

## Current progress - Ambient Occlusion
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/jtVoM0I_rxw/0.jpg)](https://www.youtube.com/watch?v=jtVoM0I_rxw&list=PLocuszpm1snUGVn5kgk-LlLE86c4E2u7H&index=10)

## Particles branch
Fluid simulation  
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/GZs0-xvkecA/0.jpg)](https://www.youtube.com/watch?v=GZs0-xvkecA)

3D jelly object  
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/LectfDOBszc/0.jpg)](https://www.youtube.com/watch?v=LectfDOBszc)

## Animation branch
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/_7ZKdgwjL7s/0.jpg)](https://www.youtube.com/watch?v=_7ZKdgwjL7s)

## Build and run instructions
- Create a build folder.  
  - In the top level directory create folder:  {top-levell-directory}/build  
- Run cmake with destination the build folder, and source folder the top level directory  
  - Navigate into the build folder, and type: cmake ../  
- Compile the project  
  - Open the .sln project file with visual studio and build the project (For animation branch, build in Release because of cally3d libs or change the .lib file to the debug one)
- Locate the executable and copy on the same folder the dependencies  
  - The folders: assets, fonts, src/shaders, map files, config files
  - The dependencies: freetype271.dll, assimp-vc140-mt.dll
