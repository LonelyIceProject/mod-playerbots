/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

// Entry point when playerbots is built as a plugin (CMakeLists.txt); not part of the static module build.

#include "PluginApi.h"

void Addmod_playerbotsScripts();

AC_PLUGIN(Addmod_playerbotsScripts)
