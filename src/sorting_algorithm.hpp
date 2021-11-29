#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "pass.hpp"

// pair that describes a single sorting pass
struct PassBlueprint {
    PassBlueprint(PassType _passType, int _gap): passType(_passType), gap(_gap) {}

    PassType passType;
    int gap;
};

// use this simple structure for the GA process to make mutations/crossover easy
struct AlgorithmBlueprint {
    explicit AlgorithmBlueprint(std::vector<PassBlueprint> _passBlueprints): passBlueprints(std::move(_passBlueprints)) {}

    std::vector<PassBlueprint> passBlueprints;
};

template <typename T>
class PassWrapper {
public:
    PassWrapper(std::unique_ptr<Pass<T>> _pass, int _gap): pass(std::move(_pass)), gap(_gap) {}

    void performPass(T & container, std::size_t n) const {
        pass->performPass(container, n, gap);
    }

private:
    std::unique_ptr<Pass<T>> pass;
    int gap;
};

// use this to actually perform the sorting
template <typename T>
class SortingAlgorithm {
public:
    explicit SortingAlgorithm(const AlgorithmBlueprint & blueprint) {
        auto passBlueprints = blueprint.passBlueprints;

        passWrappers.reserve(passBlueprints.size());

        for (const auto & pd: passBlueprints) {
            passWrappers.push_back(PassWrapper<T>(PassFactory::getPass<T>(pd.passType), pd.gap));
        }
    }

    void sort(T & container) {
        std::size_t n = container.size();

        for (const auto & pw: passWrappers) {
            pw.performPass(container, n);
        }
    }
private:
    std::vector<PassWrapper<T>> passWrappers;
};

class SortingAlgorithmFactory {
public:
    template <typename T>
    static std::unique_ptr<SortingAlgorithm<T>> getSortingAlgorithm(const AlgorithmBlueprint & blueprint) {
        return std::make_unique<SortingAlgorithm<T>>(blueprint);
    }
};