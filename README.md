# billy-the-game
Billy the game is a personal project to create an open world 2D game, by using C++, OpenGL and other similar libraries to ease development. 
The main goal is to learn rendering techniques, physics, memory 
management, data structures, engine development and anything else that might be used 
in the development of a game. While the main goal is in 2D, the core features are in 3D, and I spend a considerable amount of time developing for 3D. Featuring:   


In Graphics with OpenGL:  
- Reading models through assimp
- Rendering with Phong shading (Point/Directional/Spot lights), one diffuse texture, and one specular map
- Deferred and forward rendering queues
- Instancing
- Shadows with cascaded shadow maps
- View frustum culling and occlusion queries
- Screen Space Ambient Occlusion
- Water rendering with a grid algorithm  
- Terrain rendering with displacement with tessellation shaders  
- Water rendering with displacement and bump mapping with tessellation shaders, and skybox reflections

In world building:  
- Importing maps generated from Tiled
- Draw call batching  

In physics and interaction:  
- Custom collision detection
- QuadTree accelerated ray casting  

Other:  
- Several data structures created: Uniform grids, Circular buffers, HashTables, and several QuadTree variants
- Sequential memory allocation with custom allocators  


## Current progress in main game - billy the game:  
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/M_0gH1M3FH0/0.jpg)](https://www.youtube.com/watch?v=M_0gH1M3FH0&list=PLocuszpm1snUGVn5kgk-LlLE86c4E2u7H&index=12)

## Current progress in 3D:   
Ambient Occlusion    
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/jtVoM0I_rxw/0.jpg)](https://www.youtube.com/watch?v=jtVoM0I_rxw&list=PLocuszpm1snUGVn5kgk-LlLE86c4E2u7H&index=10)

Tessellation, terrain:    
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/aVAk7tuQ9Bw/0.jpg)](https://www.youtube.com/watch?v=aVAk7tuQ9Bw&list=PLocuszpm1snWZPt-0sGtqyRBMn3De0Rnq&index=2)

Tessellation, water:    
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/AP2Zud0mouY/0.jpg)](https://www.youtube.com/watch?v=AP2Zud0mouY&list=PLocuszpm1snWZPt-0sGtqyRBMn3De0Rnq&index=4)

Instancing and cascaded shadow maps:    
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/tQ1OW0HHD0k/0.jpg)](https://www.youtube.com/watch?v=tQ1OW0HHD0k&list=PLocuszpm1snXQ3j0rftTlSW7g4SMdAK3z&index=2)



## Particles branch:  
Fluid simulation  
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/GZs0-xvkecA/0.jpg)](https://www.youtube.com/watch?v=GZs0-xvkecA)

Rope and cloth  
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/JzwKVZXjkoE/0.jpg)](https://www.youtube.com/watch?v=JzwKVZXjkoE)

3D jelly object  
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/LectfDOBszc/0.jpg)](https://www.youtube.com/watch?v=LectfDOBszc)



## Roadmap  
In Graphics:   
- Create a 3D scene that has a little bit of everything
- Cloud rendering through volumetric rendering
- Replace Phong shading with Physically based rendering
- Create some post processing effects, fog, bloom, etc. 
- Other things like, particle systems...  

In world building:  
- NPC dialogs, and inventory panels




## Build and run instructions
- Create a build folder  
  - In the top level directory create folder:  {top-levell-directory}/build/  
- Run cmake with destination the build folder, and source folder the top level directory  
  - Navigate into the build folder, and type: cmake ../
- Compile the project  
  - Open the .sln project file with visual studio and build the project (For animation branch, build in Release because of cally3d libs or change the .lib file to the debug one)
- Locate the executable and copy on the same folder the dependencies  
  - The files: config.txt files, map files for main binary. Configure config.txt to point to your assets and shaders folders 
  - The dependencies: freetype271.dll, assimp-vc140-mt.dll
