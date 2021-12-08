#pragma once
#include "json.hpp"
#include "pass.hpp"

NLOHMANN_JSON_SERIALIZE_ENUM(PassType, {
    {PassType::INSERTION, "insertion"},
    {PassType::INSERTION_IMPROVED, "insertion_improved"},
    {PassType::BRICK, "brick"},
    {PassType::SHAKE, "shake"},
    {PassType::BUBBLE, "bubble"},
})

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PassBlueprint, passType, gap);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AlgorithmBlueprint, passBlueprints);
