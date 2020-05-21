@echo off
del Core
md Core
mkdir Core/Resources
mkdir Core/Shaders
xcopy "../GLGame/Core/Shaders" "Core/Shaders" /s/h/e/k/f/c/q/y
xcopy "../GLGame/Core/Resources" "Core/Resources" /s/h/e/k/f/c/q/y
cd "../GLGame/Debug"
copy "Tiny OpenGL Game Engine.exe" "../../Profiling" 
