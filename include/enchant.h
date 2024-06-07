#pragma once

#include <map>
#include <string>
enum class Enchant{
    Sharpness = 0,
};

static std::map<Enchant, std::string> EnchantToStringMap = {
    { Enchant::Sharpness, "Sharpness" }
};

std::string EnchantToString(Enchant name);
//std::string StringToEnchant(std::string name);
