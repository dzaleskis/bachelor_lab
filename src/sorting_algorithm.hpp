#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "pass.hpp"

// pair that describes a single sorting pass
struct PassDescription {
    PassDescription(PassType _passType, int _gap): passType(_passType), gap(_gap) {}

    PassType passType;
    int gap;
};

// use this simple structure for the GA process to make mutations/crossover easy
struct SortingAlgorithmBlueprint {
    SortingAlgorithmBlueprint(std::vector<PassDescription> _passDescriptions): passDescriptions(std::move(_passDescriptions)) {}

    std::vector<PassDescription> passDescriptions;
};

template <typename T>
struct ConcretePass {
    ConcretePass(std::unique_ptr<Pass<T>> _pass, int _gap): pass(std::move(_pass)), gap(_gap) {}

    std::unique_ptr<Pass<T>> pass;
    int gap;
};

// use this to actually perform the sorting
template <typename T>
class ConcreteSortingAlgorithm {
public:
    explicit ConcreteSortingAlgorithm(const SortingAlgorithmBlueprint & blueprint, const PassFactory<T> & passFactory) {
        auto passDescriptions = blueprint.passDescriptions;

        concretePasses.reserve(passDescriptions.size());

        for (const auto & pd: passDescriptions) {
            concretePasses.push_back(ConcretePass<T>(passFactory.getPass(pd.passType), pd.gap));
        }
    }

    void sort(T & container) {
        std::size_t n = container.size();

        for (const auto & cp: concretePasses) {
            cp.pass->performPass(container, n, cp.gap);
        }
    }
private:
    std::vector<ConcretePass<T>> concretePasses;
};

template <typename T>
class ConcreteSortingAlgorithmFactory {
public:
    std::unique_ptr<ConcreteSortingAlgorithm<T>> getSortingAlgorithm(const SortingAlgorithmBlueprint & blueprint) {
        return std::make_unique<ConcreteSortingAlgorithm<T>>(blueprint, passFactory);
    }
private:
    PassFactory<T> passFactory;
};