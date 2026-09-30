/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "ExternalHooks.h"

#include <utility>
#include <vector>

namespace
{
    struct Hooks
    {
        PlayerbotExternalHooks::IsPinnedFn isPinned;
        PlayerbotExternalHooks::DecorateFn decorate;
    };

    std::vector<Hooks>& RegisteredHooks()
    {
        static std::vector<Hooks> hooks;
        return hooks;
    }
}

void PlayerbotExternalHooks::Register(IsPinnedFn isPinned, DecorateFn decorate)
{
    if (!isPinned && !decorate)
        return;

    RegisteredHooks().push_back({ std::move(isPinned), std::move(decorate) });
}

bool PlayerbotExternalHooks::IsPinned(Player* bot)
{
    if (!bot)
        return false;

    for (Hooks const& hooks : RegisteredHooks())
        if (hooks.isPinned && hooks.isPinned(bot))
            return true;

    return false;
}

void PlayerbotExternalHooks::Decorate(Player* bot, Engine* engine, uint8 botState)
{
    if (!bot || !engine)
        return;

    for (Hooks const& hooks : RegisteredHooks())
        if (hooks.decorate)
            hooks.decorate(bot, engine, botState);
}
