#pragma once
#include <vector>
#include "pass.hpp"
#include "random_utils.hpp"
#include "sorting_algorithm.hpp"
#include "gaps.hpp"

namespace genetic {
    constexpr double EQUAL_PROBABILITY = 0.5;
    constexpr double MUTATION_PROBABILITY = EQUAL_PROBABILITY;
    constexpr double MUTATION_SCALE = 0.2;
    constexpr double MUTATION_MAX = 2;

    inline bool withProbability(double probability, const std::function<double(void)> &rnd01) {
        return probability < rnd01();
    }

    inline std::vector<int> get_random_gaps(const std::function<double(void)> &rnd01) {
        int random_gap_index = std::floor(rnd01() * ALL_GAPS.size());
        auto gaps = ALL_GAPS[random_gap_index];

        return std::move(gaps);
    }

    inline PassType get_random_pass(const std::function<double(void)> &rnd01) {
        int random_pass_index = std::floor(rnd01() * ALL_PASSES.size());
        auto pass = ALL_PASSES[random_pass_index];

        return pass;
    }

    inline int get_mutated_gap(const std::function<double(void)> &rnd01, double mut_radius, int old_gap) {
        double raw_mutation = mut_radius * (rnd01()-rnd01()); // belongs to [-1, 1]
        int scaled_mutation = std::floor(raw_mutation * MUTATION_MAX); // belongs to [-MUTATION_MAX, MUTATION_MAX]
        int new_gap = std::max(1, old_gap + scaled_mutation); // ensure gap is positive

        return new_gap;
    }

    // TODO: maybe composition makes more sense here? more typing though :)
    struct Solution: public AlgorithmBlueprint {
        Solution() = default;

        void init_genes(const std::function<double(void)> &rnd01) {
            auto gaps = get_random_gaps(rnd01);
            passBlueprints.reserve(gaps.size());

            for (int & gap : gaps) {
                auto pass = get_random_pass(rnd01);
                passBlueprints.emplace_back(pass, gap);
            }

            normalize();
        }

        inline void normalize() {
            std::sort(passBlueprints.begin(), passBlueprints.end(), std::greater<>());
        }

        Solution crossover(const Solution& X2, const std::function<double(void)> &rnd01) const {
            Solution X_new;
            X_new.passBlueprints.reserve(passBlueprints.size());

            // using uniform distribution
            for (int i = 0; i < X_new.passBlueprints.size(); i++) {
                if (withProbability(EQUAL_PROBABILITY, rnd01)) {
                    X_new.passBlueprints[i] = this->passBlueprints[i];
                } else {
                    X_new.passBlueprints[i] = X2.passBlueprints[i];
                }
            }

            X_new.normalize();

            return std::move(X_new);
        }

        Solution mutate(const std::function<double(void)> &rnd01, double shrink_scale) const {
            Solution X_new;
            X_new.passBlueprints.reserve(passBlueprints.size());

            const double mut_radius = MUTATION_SCALE * shrink_scale;

            for (int i = 0; i < passBlueprints.size(); i++) {
                auto oldBlueprint = this->passBlueprints[i];

                if (withProbability(MUTATION_PROBABILITY, rnd01)) {
                    // create new blueprint here
                    if (withProbability(EQUAL_PROBABILITY, rnd01)) {
                        // mutate the gap
                        auto new_gap = get_mutated_gap(rnd01, mut_radius, oldBlueprint.gap);
                        X_new.passBlueprints[i] = PassBlueprint(oldBlueprint.passType, new_gap);
                    } else {
                        // use a new pass (no way to mutate them)
                        auto new_pass = get_random_pass(rnd01);
                        X_new.passBlueprints[i] = PassBlueprint(new_pass, oldBlueprint.gap);
                    }
                } else {
                    // use old
                    X_new.passBlueprints[i] = oldBlueprint;
                }
            }

            X_new.normalize();

            return std::move(X_new);
        }

    };
}