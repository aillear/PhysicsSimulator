#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SDL3_gfx::SDL3_gfx" for configuration "Release"
set_property(TARGET SDL3_gfx::SDL3_gfx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SDL3_gfx::SDL3_gfx PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libSDL3_gfx.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libSDL3_gfx.dll"
  )

list(APPEND _cmake_import_check_targets SDL3_gfx::SDL3_gfx )
list(APPEND _cmake_import_check_files_for_SDL3_gfx::SDL3_gfx "${_IMPORT_PREFIX}/lib/libSDL3_gfx.dll.a" "${_IMPORT_PREFIX}/bin/libSDL3_gfx.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
