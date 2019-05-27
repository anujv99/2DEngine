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

    include "Engine/vendor/spdlog"

    project "Engine"
        location "Engine"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
		staticruntime "on"

        targetdir ("bin/" .. outputDir .. "%{prj.name}")
        objdir ("bin-int/" .. outputDir .. "%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        links {
            "d3d11.lib",
            "dxgi.lib",
            "d3dcompiler.lib"
        }

        includedirs {
            "%{IncludeDirs.spdlog}",
			"%{prj.name}/src",
			"%{prj.name}"
        }
		
		defines {
			"USER_ENGINE"
		}

        pchsource "%{prj.name}/src/pch.cpp"
        pchheader "pch.h"

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