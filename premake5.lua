workspace "2DEngine"
    startproject "App"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Distribute"
    }

    outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

    IncludeDirs = {}
    IncludeDirs["spdlog"] = "Engine/vendor/spdlog/include"
	IncludeDirs["gmscript"] = "Engine/vendor/gmscript/gmsrc/src"

    include "Engine/vendor/spdlog"
    include "Engine/vendor/gmscript"

    project "Engine"
        location "Engine"
        kind "StaticLib"
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
            "d3dcompiler.lib",
            "gmscript"
        }

        includedirs {
            "%{IncludeDirs.spdlog}",
            "%{IncludeDirs.gmscript}",
			"%{prj.name}/src",
			"%{prj.name}"
        }
		
		defines {
			"USER_ENGINE",
			"LOG_DETAILED",
			--"LOG_DISABLED"
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
			
	project "App"
		location "App"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
		staticruntime "on"

        targetdir ("bin/" .. outputDir .. "%{prj.name}")
        objdir ("bin-int/" .. outputDir .. "%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        links {
            "Engine",
        }

        includedirs {
			"Engine/src/",
			"%{prj.name}/src",
			"%{prj.name}",
			"%{IncludeDirs.spdlog}",
        }
		
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