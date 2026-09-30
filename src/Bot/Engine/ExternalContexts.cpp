/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "ExternalContexts.h"

namespace PlayerbotExternalContexts
{
    template <>
    std::vector<Factory<Strategy>>& Registry<Strategy>()
    {
        static std::vector<Factory<Strategy>> factories;
        return factories;
    }

    template <>
    std::vector<Factory<Action>>& Registry<Action>()
    {
        static std::vector<Factory<Action>> factories;
        return factories;
    }

    template <>
    std::vector<Factory<Trigger>>& Registry<Trigger>()
    {
        static std::vector<Factory<Trigger>> factories;
        return factories;
    }
}
