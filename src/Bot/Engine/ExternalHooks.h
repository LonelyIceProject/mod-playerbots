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

// Lets other modules pin random bots to a place. Register while scripts load; the functions are only read
// afterwards (from the world and map threads), so they must be thread-safe themselves. With nothing
// registered every wrapper is a no-op / false and playerbots behaves exactly as before.
// Every module may register its own pair (either function may be empty); all registered pairs are used:
//   isPinned(bot)              - true from any of them: RandomPlayerbotMgr leaves the bot alone (no randomize /
//                                random teleport), the AI keeps it active and does not clear its AFK flag.
//   decorate(bot, engine, st)  - all of them are called, in registration order, at the end of
//                                AiFactory::AddDefault*Strategies (before Engine::Init), so the changes survive
//                                ResetStrategies; st is a BotState.
// The registered functions live in one place (ExternalHooks.cpp), so this also works when playerbots and the
// registering module are separate shared libraries.
namespace PlayerbotExternalHooks
{
    using IsPinnedFn = std::function<bool(Player*)>;
    using DecorateFn = std::function<void(Player*, Engine*, uint8)>;

    void Register(IsPinnedFn isPinned, DecorateFn decorate);
    bool IsPinned(Player* bot);
    void Decorate(Player* bot, Engine* engine, uint8 botState);
}

#endif
