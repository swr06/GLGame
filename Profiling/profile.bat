@echo off
del Core
md Core
mkdir Core/Resources
mkdir Core/Shaders
xcopy "../Source/Core/Shaders" "Core/Shaders" /s/h/e/k/f/c/q/y
xcopy "../Source/Core/Resources" "Core/Resources" /s/h/e/k/f/c/q/y
cd "../Source/Debug"
copy "Tiny OpenGL Game Engine.exe" "../../Profiling" 
