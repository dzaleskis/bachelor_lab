#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "pass.hpp"

// describes a single sorting pass with gap and pass type
struct PassBlueprint {
    PassBlueprint() = default;
    PassBlueprint(PassType _passType, int _gap): passType(_passType), gap(_gap) {}

    bool operator < (const PassBlueprint& b) const
    {
        return gap < b.gap;
    }

    PassType passType;
    int gap;
};

// describes an algorithm, contains a list of pass descriptions (gaps are in increasing order)
struct AlgorithmBlueprint {
    AlgorithmBlueprint() = default;

    std::vector<PassBlueprint> passBlueprints;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PassBlueprint, passType, gap)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AlgorithmBlueprint, passBlueprints)

template <typename T>
void run_pass(T& data, const PassBlueprint& blueprint) {
    switch (blueprint.passType) {
        case PassType::INSERTION:
            return insertion_pass(data, blueprint.gap);
        case PassType::BRICK:
            return brick_pass(data, blueprint.gap);
        case PassType::BUBBLE:
            return bubble_pass(data, blueprint.gap);
        case PassType::SHAKE:
            return shake_pass(data, blueprint.gap);
        default:
            throw std::runtime_error("unsupported pass type");
    }
}

template <typename T>
void run_algorithm(T& data, const AlgorithmBlueprint& algorithmBlueprint) {
    // make a copy and reverse the passes, since blueprint stores passes in increasing order
    std::vector<PassBlueprint> passes(algorithmBlueprint.passBlueprints);
    std::reverse(passes.begin(), passes.end());

    for (const auto& pass: passes) {
        run_pass(data, pass);
    }
}