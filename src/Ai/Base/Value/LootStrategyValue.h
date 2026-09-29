/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef PLAYERBOTS_LOOTSTRATEGYVALUE_H
#define PLAYERBOTS_LOOTSTRATEGYVALUE_H

#include "Value.h"

class AiObjectContext;
class LootStrategy;
class PlayerbotAI;

class LootStrategyValue : public ManualSetValue<LootStrategy*>
{
public:
    LootStrategyValue(PlayerbotAI* botAI, std::string const name = "loot strategy")
        : ManualSetValue<LootStrategy*>(botAI, normal, name)
    {
    }
    virtual ~LootStrategyValue();

    std::string const Save() override;
    bool Load(std::string const value) override;

    static LootStrategy* normal;
    static LootStrategy* gray;
    static LootStrategy* all;
    static LootStrategy* disenchant;
    static LootStrategy* useful;

    // What "useful" counts as needed by the character (upgrade, quest, consumable / ammo / trade goods
    // it uses, disenchant material) - valuables (green+) are not part of it.
    static bool IsNeeded(AiObjectContext* context, uint32 itemId);
    // A bot of a real player on the "useful" loot mode throws away gear its auto-equip replaced and
    // quest rewards / leftovers it does not need (EquipAction, or a module's bag cleanup).
    static bool KeepsBagsClean(PlayerbotAI* botAI);
    static LootStrategy* instance(std::string const name);
};

#endif
