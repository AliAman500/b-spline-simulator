workspace "dev"
    architecture "x86_64"
    configurations { "debug" }
    location "build"

    project "app"
        kind "consoleapp"
        language "c++"
        toolset "gcc"
        location "build"
        targetdir "build/bin"
        objdir "build/bin-int"

        includedirs { "app/include" }
        files { "app/src/**.cpp", "app/src/**.c" }

        symbols "on"
        optimize "off"

        filter "system:windows"
            libdirs { "app/lib/win32" }
            links { "glfw3", "gdi32", "opengl32" }
        
        filter "system:linux"
            links { "glfw", "GL" }
        