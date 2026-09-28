/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef PLAYERBOTS_EXTERNALHOOKS_H
#define PLAYERBOTS_EXTERNALHOOKS_H

#include "Define.h"

#include <functional>

class Engine;
class Player;

// [mod-custom citizens] Lets another module pin random bots to a place (docs\city-life-spec.md, section 2).
// Register while scripts load; the functions are only read afterwards (from the world and map threads),
// so they must be thread-safe themselves. With nothing registered every wrapper is a no-op / false and
// playerbots behaves exactly as before.
//   isPinned(bot)              - true: RandomPlayerbotMgr leaves the bot alone (no randomize / random
//                                teleport), the AI keeps it active and does not clear its AFK flag.
//   decorate(bot, engine, st)  - called at the end of AiFactory::AddDefault*Strategies (before
//                                Engine::Init), so the changes survive ResetStrategies; st is a BotState.
namespace PlayerbotExternalHooks
{
    using IsPinnedFn = std::function<bool(Player*)>;
    using DecorateFn = std::function<void(Player*, Engine*, uint8)>;

    inline IsPinnedFn& IsPinnedHook()
    {
        static IsPinnedFn fn;
        return fn;
    }

    inline DecorateFn& DecorateHook()
    {
        static DecorateFn fn;
        return fn;
    }

    inline void Register(IsPinnedFn isPinned, DecorateFn decorate)
    {
        IsPinnedHook() = std::move(isPinned);
        DecorateHook() = std::move(decorate);
    }

    inline bool IsPinned(Player* bot)
    {
        IsPinnedFn const& fn = IsPinnedHook();
        return bot && fn && fn(bot);
    }

    inline void Decorate(Player* bot, Engine* engine, uint8 botState)
    {
        DecorateFn const& fn = DecorateHook();
        if (bot && engine && fn)
            fn(bot, engine, botState);
    }
}

#endif
