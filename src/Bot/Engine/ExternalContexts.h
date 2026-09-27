/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef PLAYERBOTS_EXTERNALCONTEXTS_H
#define PLAYERBOTS_EXTERNALCONTEXTS_H

#include "NamedObjectContext.h"

#include <functional>
#include <vector>

// Lets other modules add named strategies/actions/triggers to every bot context. Register factories
// while scripts load (before OnBeforeWorldInitialized builds the shared contexts).
namespace PlayerbotExternalContexts
{
    template <class T>
    using Factory = std::function<NamedObjectContext<T>*()>;

    template <class T>
    inline std::vector<Factory<T>>& Registry()
    {
        static std::vector<Factory<T>> factories;
        return factories;
    }

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
