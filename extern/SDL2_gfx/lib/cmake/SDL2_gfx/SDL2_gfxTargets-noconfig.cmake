#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SDL2_gfx::SDL2_gfx" for configuration ""
set_property(TARGET SDL2_gfx::SDL2_gfx APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(SDL2_gfx::SDL2_gfx PROPERTIES
  IMPORTED_IMPLIB_NOCONFIG "${_IMPORT_PREFIX}/lib/libSDL2_gfx.dll.a"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/libSDL2_gfx.dll"
  )

list(APPEND _cmake_import_check_targets SDL2_gfx::SDL2_gfx )
list(APPEND _cmake_import_check_files_for_SDL2_gfx::SDL2_gfx "${_IMPORT_PREFIX}/lib/libSDL2_gfx.dll.a" "${_IMPORT_PREFIX}/bin/libSDL2_gfx.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
