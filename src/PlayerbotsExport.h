/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef _PLAYERBOTS_EXPORT_H
#define _PLAYERBOTS_EXPORT_H

// Data that other plugins read from the playerbots plugin library needs dllimport on Windows. Functions and
// classes are exported without it. In the static module build this is empty.
#if defined(_WIN32) && defined(AC_PLUGIN_BUILD)
#  ifdef PLAYERBOTS_EXPORTS
#    define PLAYERBOTS_API __declspec(dllexport)
#  else
#    define PLAYERBOTS_API __declspec(dllimport)
#  endif
#else
#  define PLAYERBOTS_API
#endif

#endif
