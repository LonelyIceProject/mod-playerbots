/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef PLAYERBOTS_EXTERNALCONTEXTS_H
#define PLAYERBOTS_EXTERNALCONTEXTS_H

#include "NamedObjectContext.h"
#include "PlayerbotsExport.h"

#include <functional>
#include <vector>

class Action;
class Strategy;
class Trigger;

// Lets other modules add named strategies/actions/triggers to every bot context. Register factories
// while scripts load (before OnBeforeWorldInitialized builds the shared contexts).
namespace PlayerbotExternalContexts
{
    template <class T>
    using Factory = std::function<NamedObjectContext<T>*()>;

    // The registries are defined once, in ExternalContexts.cpp, so that modules built as separate shared
    // libraries register into the same lists playerbots reads.
    template <class T>
    std::vector<Factory<T>>& Registry();

    template <>
    PLAYERBOTS_API std::vector<Factory<Strategy>>& Registry<Strategy>();
    template <>
    PLAYERBOTS_API std::vector<Factory<Action>>& Registry<Action>();
    template <>
    PLAYERBOTS_API std::vector<Factory<Trigger>>& Registry<Trigger>();

    template <class T>
    inline void Register(Factory<T> factory) { Registry<T>().push_back(std::move(factory)); }

    template <class T>
    inline void AddAll(SharedNamedObjectContextList<T>& list)
    {
        for (Factory<T> const& factory : Registry<T>())
            list.Add(factory());
    }
}

#endif
