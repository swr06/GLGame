@echo off
del "Core"
del "Tiny OpenGL Game Engine.exe"
del "log.txt"
del "imgui.ini"
md Core
mkdir Core/Resources
mkdir Core/Shaders
xcopy "../GLGame/Core/Shaders" "Core/Shaders" /s/h/e/k/f/c/q/y
xcopy "../GLGame/Core/Resources" "Core/Resources" /s/h/e/k/f/c/q/y
cd "../GLGame/Debug"
copy "Tiny OpenGL Game Engine.exe" "../../Profiling" 
