@echo off

call "GenerateProject.bat"

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"

call MSBuild 2DEngine.sln /p:Configuration=Debug
call MSBuild 2DEngine.sln /p:Configuration=Release
call MSBuild 2DEngine.sln /p:Configuration=Distribute

PAUSE