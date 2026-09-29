# Plugin build (the core's AC_PLUGIN_SOURCE_DIRS, with -DWITH_PLAYERBOTS_HOOKS=ON). The classic static module
# build in modules/mod-playerbots does not use this file.
CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR}/src PLAYERBOTS_SOURCES)
CollectIncludeDirectories(${CMAKE_CURRENT_SOURCE_DIR}/src PLAYERBOTS_INCLUDES)
AddPlugin(mod-playerbots
  SOURCES ${PLAYERBOTS_SOURCES} plugin/plugin.cpp
  INCLUDES ${PLAYERBOTS_INCLUDES}
  EXPORT_ALL)
target_compile_definitions(mod-playerbots PRIVATE PLAYERBOTS_EXPORTS)
