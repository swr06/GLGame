# GLGame (A Tiny OpenGL based C++ Game Engine)

![GLGame Logo](https://github.com/swr06/GLGame/blob/master/Branding/logo1.png)
<p align=center>
Created by Samuel Rasquinha using C++17 and OpenGL 3.3 :game_die:
</p>

# Note

This game has long since been ABANDONED. I do not recommend using it as I was new to graphics/gamedev/game engine dev when I wrote it.

# What is GLGame ? 

GLGame is a Tiny OpenGL Game Engine that is simple, lightweight and easy to use.

# Objective 

GLGame was created to be as simple and straightforward as possible. Each part of the engine requires little to no understanding of OpenGL and other computer graphic concepts.

It uses simple type names and has simple arguments such as ``Object``, ``Entity`` or ``Sprite``.

# Features

A Tiny 2D OpenGL based C++ Game Engine that is fast, lightweight and comes with a level editor and the below features :
- Abstract OpenGL (To write custom code) ✓
- Camera Culling ✓
- Sprite Batching ✓
- Lighting  ✓
  - Custom Light colors  ✓
  - Light Batching ✓
- Animations ✓
  - Custom animation speed ✓
- Scene Editor ✓
  - Parser ✓
  - Slight encryption ✓
  - Grid ✓
- Transformations ✓
- Logger ✓
- Audio 
- Lighting ✓
- Particles ✓
  - Particle Batcher ✓
- Physics 
- Helper functions (such as windowing functions etc..)  ✓
- Texture atlas / Sprite Sheet api ✓
  - Custom Sampling ✓
  

This engine is still in development.

# Features to come

I would like to implement the following features in the coming years :
- Multithreading
- Networking
- Save game
- Post Processing
- More helper functions
- Custom GUI Library

# Compiling

To compile this project, you will need Microsoft Visual Studio 2019 and the VSC++ compiler (which supports C++17).
Open the ``.sln`` file using Visual Studio and select the x86 (or Win32) configuration. 
You can compile it in both modes (Debug or Release) with the x86 (Win32) configuration.

# About the Scene Editor 

The scene editor has to be run during runtime.
Just set the fifth argument of the ``Init()`` function to true and it will launch the Scene Editor with all the objects you have or will create. 

# Dependencies 

GLGame has very humble dependencies.

- ``C++17`` (for ``std::filesystem``)
- ``stb_image`` (for reading texture files)
- ``ImGui`` (Bloat free immediate mode GUI interface by Omar Cornut)
- ``GLEW`` (OpenGL wrangler library)
- ``GLFW`` (Windowing environment)

# Development Specs 

- Processor : AMD Ryzen 3 2200g
- Graphics  : Integrated Vega 8 graphics
- Ram       : 8.0 GB
- MB        : AsRock A320M MB

This engine managed to get 500+ FPS in the game with the scene editor running. (With the above specs) and about 300 FPS on an intel HD 4000 GPU. 

It got around 1000+ FPS in game without the scene editor running and 400+ FPS on an Intel HD 4000.

# Using it

A wiki has been written for GLGame. Please refer to the demos and the ``Wiki`` tab in the repo page.

# License

This project is licensed under the highly permissive MIT License. Read the LICENSE file for more info.

# Supporting this project

If you like this project and would like to support it, please do star it on GitHub.

# Screenshots and Demos

![GLGame Demo-1](https://github.com/swr06/GLGame/blob/master/Docs/Readme/Lighting%20demo.png)
<p align="center">
<i>A Basic lighting demo created with the built-in 2D Light classes and batchers</i>
</p>

![GLGame Demo-2](https://github.com/swr06/GLGame/blob/master/Docs/Readme/Lighting%20demo%20-%201.PNG)
<p align="center">
<i>A Basic lighting demo emulated over a 2D sprite</i>
</p>


![GLGame Demo-3](https://github.com/swr06/GLGame/blob/master/Docs/Readme/Scene%20Editor.png)
<p align="center">
<i>The GLGame Scene Editor created and rendered at runtime</i>
</p>

![GLGame Demo-4](https://github.com/swr06/GLGame/blob/master/Docs/Readme/Particle%20Demo.png)
<p align="center">
<i>The particle emitter demo that has a particle batcher and can render upto 10,000 particles in one draw call</i>
</p>


![GLGame Demo-5](https://github.com/swr06/GLGame/blob/master/Docs/Readme/Batch%20Rendering.png)
<p align="center">
<i>The batch renderer that comes with GLGame can render upto 10,000 sprites in one draw call</i>
</p>

![GLGame Platformer Demo](https://github.com/swr06/GLGame/blob/master/Docs/Readme/Platformer%20Demo.png)
<p align="center">
<i>A platformer demo that comes included with GLGame</i>
</p>


Thankyou for visiting the GLGame github page.
