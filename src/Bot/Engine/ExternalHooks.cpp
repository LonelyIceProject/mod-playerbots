/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "ExternalHooks.h"

namespace
{
    PlayerbotExternalHooks::IsPinnedFn& IsPinnedHook()
    {
        static PlayerbotExternalHooks::IsPinnedFn fn;
        return fn;
    }

    PlayerbotExternalHooks::DecorateFn& DecorateHook()
    {
        static PlayerbotExternalHooks::DecorateFn fn;
        return fn;
    }
}

void PlayerbotExternalHooks::Register(IsPinnedFn isPinned, DecorateFn decorate)
{
    IsPinnedHook() = std::move(isPinned);
    DecorateHook() = std::move(decorate);
}

bool PlayerbotExternalHooks::IsPinned(Player* bot)
{
    IsPinnedFn const& fn = IsPinnedHook();
    return bot && fn && fn(bot);
}

void PlayerbotExternalHooks::Decorate(Player* bot, Engine* engine, uint8 botState)
{
    DecorateFn const& fn = DecorateHook();
    if (bot && engine && fn)
        fn(bot, engine, botState);
}
