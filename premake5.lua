workspace "2DEngine"
    startproject "Engine"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Distribute"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

    IncludeDirs = {}
    IncludeDirs["spdlog"] = "Engine/vendor/spdlog/include"
	IncludeDirs["imgui"] = "Engine/vendor/imgui"
	IncludeDirs["freetype"] = "Engine/vendor/freetype/include"
	IncludeDirs["freetypegl"] = "Engine/vendor/freetype-gl"
	IncludeDirs["Chipmunk2D"] = "Engine/vendor/Chipmunk2D/include"

    include "Engine/vendor/spdlog"
	include "Engine/vendor/imgui"
	include "Engine/vendor/freetype"
	include "Engine/vendor/freetype-gl"
	include "Engine/vendor/Chipmunk2D"

    project "Engine"
        location "Engine"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
		staticruntime "on"

        targetdir ("bin/" .. outputDir .. "%{prj.name}")
        objdir ("bin-int/" .. outputDir .. "%{prj.name}")

        files {
            "%{prj.name}/src/**.*"
        }
		
        links {
			"ImGui",
			"freetype",
			"freetype-gl",
            "d3d11.lib",
            "dxgi.lib",
            "d3dcompiler.lib",
			"Chipmunk2D",
        }

        includedirs {
            "%{IncludeDirs.spdlog}",
            "%{IncludeDirs.imgui}",
            "%{IncludeDirs.freetype}",
            "%{IncludeDirs.freetypegl}",
            "%{IncludeDirs.Chipmunk2D}",
			"%{prj.name}/src",
			"%{prj.name}"
        }
		
		defines {
			"USER_ENGINE",
			"LOG_DETAILED",
			--"LOG_DISABLED",
			"PREVMATH",
		}
		
		undefines {
			"UNICODE"
		}
		
        pchsource "%{prj.name}/src/pch.cpp"
        pchheader "pch.h"
		
		filter "files:Engine/src/gm/**.cpp"
			flags { "NoPCH" }
		
        filter "configurations:Debug"
            defines {"ENGINE_DEBUG"}
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines {"ENGINE_RELEASE"}
            runtime "Release"
	        optimize "On"

	    filter "configurations:Distribute"
            defines {"ENGINE_DIST"}
            runtime "Release"
		    optimize "On"