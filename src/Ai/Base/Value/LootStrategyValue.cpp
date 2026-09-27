/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "LootStrategyValue.h"
#include "AiObjectContext.h"
#include "ItemUsageValue.h"
#include "LootObjectStack.h"
#include "Playerbots.h"

class NormalLootStrategy : public LootStrategy
{
public:
    bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) override
    {
        // Identify the source of loot, loot it if the source is an item in the bots inventory
        LootObject lootObject = AI_VALUE(LootObject, "loot target");
        ObjectGuid lootGuid = lootObject.guid;
        if (lootGuid.IsItem())
        {
            return true;
        }

        // Otherwise, continue with the normal loot logic
        std::ostringstream out;
        out << proto->ItemId;
        ItemUsage usage = AI_VALUE2(ItemUsage, "item usage", out.str());
        return usage != ITEM_USAGE_NONE;
    }

    std::string const GetName() override { return "normal"; }
};

// mod-custom (party window default for bots of a real player): what the character can use, and valuables.
// Upgrades, quest items, consumables / ammo / trade goods it needs, disenchant material of its enchanter, and
// anything of uncommon (green) quality or better. No grey or white vendor trash (usage AH / VENDOR / BAD_EQUIP).
class UsefulLootStrategy : public LootStrategy
{
public:
    bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) override
    {
        LootObject lootObject = AI_VALUE(LootObject, "loot target");
        if (lootObject.guid.IsItem())
            return true;   // opening a container / clam of the bot's own bags

        return proto->Quality >= ITEM_QUALITY_UNCOMMON || LootStrategyValue::IsNeeded(context, proto->ItemId);
    }

    std::string const GetName() override { return "useful"; }
};

bool LootStrategyValue::IsNeeded(AiObjectContext* context, uint32 itemId)
{
    std::ostringstream out;
    out << itemId;
    switch (AI_VALUE2(ItemUsage, "item usage", out.str()))
    {
        case ITEM_USAGE_EQUIP:
        case ITEM_USAGE_REPLACE:
        case ITEM_USAGE_QUEST:
        case ITEM_USAGE_SKILL:
        case ITEM_USAGE_USE:
        case ITEM_USAGE_KEEP:
        case ITEM_USAGE_AMMO:
        case ITEM_USAGE_DISENCHANT:
            return true;
        default:
            return false;
    }
}

bool LootStrategyValue::KeepsBagsClean(PlayerbotAI* botAI)
{
    if (!botAI || !IsRealPlayer(botAI->GetMaster()))
        return false;

    LootStrategy* strategy = botAI->GetAiObjectContext()->GetValue<LootStrategy*>("loot strategy")->Get();
    return strategy == useful;
}

class GrayLootStrategy : public NormalLootStrategy
{
public:
    bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) override
    {
        return NormalLootStrategy::CanLoot(proto, context) || proto->Quality == ITEM_QUALITY_POOR;
    }

    std::string const GetName() override { return "gray"; }
};

class DisenchantLootStrategy : public NormalLootStrategy
{
public:
    bool CanLoot(ItemTemplate const* proto, AiObjectContext* context) override
    {
        return NormalLootStrategy::CanLoot(proto, context) ||
               (proto->Quality >= ITEM_QUALITY_UNCOMMON && proto->Bonding != BIND_WHEN_PICKED_UP &&
                (proto->Class == ITEM_CLASS_ARMOR || proto->Class == ITEM_CLASS_WEAPON));
    }

    std::string const GetName() override { return "disenchant"; }
};

class AllLootStrategy : public LootStrategy
{
public:
    bool CanLoot(ItemTemplate const* /*proto*/, AiObjectContext* /*context*/) override { return true; }

    std::string const GetName() override { return "all"; }
};

LootStrategyValue::~LootStrategyValue()
{
    // delete defaultValue;
}

LootStrategy* LootStrategyValue::normal = new NormalLootStrategy();
LootStrategy* LootStrategyValue::gray = new GrayLootStrategy();
LootStrategy* LootStrategyValue::disenchant = new DisenchantLootStrategy();
LootStrategy* LootStrategyValue::all = new AllLootStrategy();
LootStrategy* LootStrategyValue::useful = new UsefulLootStrategy();

LootStrategy* LootStrategyValue::instance(std::string const strategy)
{
    if (strategy == "*" || strategy == "all")
        return all;

    if (strategy == "u" || strategy == "useful")
        return useful;

    if (strategy == "g" || strategy == "gray")
        return gray;

    if (strategy == "d" || strategy == "e" || strategy == "disenchant" || strategy == "enchant")
        return disenchant;

    return normal;
}

std::string const LootStrategyValue::Save() { return value ? value->GetName() : "?"; }

bool LootStrategyValue::Load(std::string const text)
{
    value = LootStrategyValue::instance(text);
    return true;
}
