workspace("workshop-gui-uploader")
configurations({"Release"})

platforms({"Win32"})

project("workshop-gui-uploader")
kind("WindowedApp")
language("C++")
targetdir("bin/")
files({"*.h", "*.cpp"})
sysincludedirs({"include/steam", "include/msvc", "include"})
includedirs({"."})
cppdialect("C++14")

filter("configurations:Release")
	defines("NDEBUG")
	optimize("On")

filter {"platforms:Win32"}
	system("Windows")
	architecture("x32")
	libdirs("libs/windows")
    links({"wxmsw30u_core", "wxbase30u", "wxtiff", "wxjpeg", "wxpng", "wxzlib", "wxregexu", "steam_api"})
    defines("__WXMSW__")
    defines("_UNICODE")
    defines("WIN32")
    defines("_WINDOWS")