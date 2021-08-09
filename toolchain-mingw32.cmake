# cmake toolchain for mingw-w64(64-bit) cross compilation
set(CMAKE_SYSTEM_NAME Windows)

set(tools /usr/bin)
set(CMAKE_C_COMPILER ${tools}/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${tools}/x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER ${tool}/x86_64-w64-mingw32-windres)

# target environment location
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# adjustment to the find behavior of the FIND_XXX() commands:

# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search header and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_INSTALL_PREFIX ${CMAKE_FIND_ROOT_PATH}/usr/ CACHE FILEPATH "install path prefix")

# initialize required linker flags
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static-libgcc -static-libstdc++")

# end of toolchainfile
