# Plugin build (the core's AC_PLUGIN_SOURCE_DIRS, with -DWITH_PLAYERBOTS_HOOKS=ON). The classic static module
# build in modules/mod-playerbots does not use this file.
CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR}/src PLAYERBOTS_SOURCES)
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR}/src PLAYERBOTS_INCLUDES)
# Dynamic plugins are absent from the core's classic module list, which otherwise enables Boost.Thread.
# Declare this module's own dependency so ELF loading does not rely on symbols from the executable.
if(WIN32)
  # Match the core and MSVC's Boost auto-linking instead of mixing a DLL import lib with a static one.
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()
find_package(Boost REQUIRED COMPONENTS thread)
AddPlugin(mod-playerbots
  SOURCES ${PLAYERBOTS_SOURCES} plugin/plugin.cpp
  INCLUDES ${PLAYERBOTS_INCLUDES}
  LINK Boost::thread
  EXPORT_ALL)
target_compile_definitions(mod-playerbots PRIVATE PLAYERBOTS_EXPORTS)
