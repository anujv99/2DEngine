# 2DEngine

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/743201b8fff64f09a64aa8383d95b332)](https://app.codacy.com/app/preversewharf45/2DEngine?utm_source=github.com&utm_medium=referral&utm_content=preversewharf45/2DEngine&utm_campaign=Badge_Grade_Dashboard)
[![Build status](https://ci.appveyor.com/api/projects/status/etaolb6d63ofblf1/branch/master?svg=true)](https://ci.appveyor.com/project/preversewharf45/2dengine/branch/master)
![GitHub commit activity](https://img.shields.io/github/commit-activity/w/preversewharf45/2DEngine.svg)
![Cocoapods](https://img.shields.io/cocoapods/l/AFNetworking.svg)

A 2D Game Engine which only support windows and DirectX 11. There is also a pre-built game
that you can directly run.

Some Key Features:
* Windows
* DirectX 11
* Audio using FMOD
* GUI using ImGui
* Scripting using GameMonkey
* and many more to come...

# Build Process

* clone using ``` https://github.com/preversewharf45/2DEngine/ ```
* ``` cd 2DEngine ```
* Now the process can vary from platform to platform. Basically it's a Premake project, so you
  can use your preferred toolset to build it.
* For example, If you are using Visual Studio do:
* premake5 vs2019 (this will generate visual studio 2019 solution)
* Now simply open the sln file and build the program.

# Note
The engine uses GameMonkey scripting language. There are a few example games in the ```Engine/games``` folder.
To add a new game simply create a new folder in the ```Engine/games``` folder, in that create a ```scripts``` sub folder and add a new file ```GameMain.gm``` next you can check how to setup the ```GameMain.gm``` file from the ```Engine/games/blankGame/scripts/GameMan.gm```.
