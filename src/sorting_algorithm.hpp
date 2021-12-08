#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "pass.hpp"

// describes a single sorting pass with gap and pass type
struct PassBlueprint {
    PassBlueprint() = default;
    PassBlueprint(PassType _passType, int _gap): passType(_passType), gap(_gap) {}

    bool operator > (const PassBlueprint& b) const
    {
        return gap > b.gap;
    }

    PassType passType;
    int gap;
};

// describes an algorithm, contains a list of pass descriptions
struct AlgorithmBlueprint {
    AlgorithmBlueprint() = default;
    AlgorithmBlueprint(std::vector<PassBlueprint> _passBlueprints): passBlueprints(std::move(_passBlueprints)) {}

    std::vector<PassBlueprint> passBlueprints;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PassBlueprint, passType, gap)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AlgorithmBlueprint, passBlueprints)

template <typename T>
class ConcretePass {
public:
    ConcretePass(std::unique_ptr<Pass<T>> _pass, int _gap): pass(std::move(_pass)), gap(_gap) {}

    void performPass(T & container, std::size_t n) const {
        // only perform pass if gap is not too big
        if (gap < n) {
            pass->performPass(container, n, gap);
        }
    }

private:
    std::unique_ptr<Pass<T>> pass;
    int gap;
};

// use this to actually perform the sorting
template <typename T>
class ConcreteAlgorithm {
public:
    explicit ConcreteAlgorithm(const AlgorithmBlueprint & blueprint) {
        auto passBlueprints = blueprint.passBlueprints;

        concretePasses.reserve(passBlueprints.size());

        for (const auto & pb: passBlueprints) {
            concretePasses.push_back(ConcretePass<T>(PassFactory::getPass<T>(pb.passType), pb.gap));
        }
    }

    void sort(T & container) {
        std::size_t n = container.size();

        for (const auto & cp: concretePasses) {
            cp.performPass(container, n);
        }
    }
private:
    std::vector<ConcretePass<T>> concretePasses;
};

class ConcreteAlgorithmFactory {
public:
    template <typename T>
    static std::unique_ptr<ConcreteAlgorithm<T>> getConcreteAlgorithm(const AlgorithmBlueprint & blueprint) {
        return std::make_unique<ConcreteAlgorithm<T>>(blueprint);
    }
};