#pragma once
#include <array>
#include <functional>
#include "json.hpp"
#include "iterator_utils.hpp"

namespace ga_solution {

    using json = nlohmann::json;

	constexpr const double EQUAL_PROBABILITY = 0.5;

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
	struct solution_base {
        constexpr static const double MUT_SCALE = 0.2;
        constexpr static const double MUTATION_PROBABILITY = 0.5;
        constexpr static const int MUT_CEILING = 100;
		std::vector<int> gaps = std::vector<int>(GAP_COUNT);

        solution_base() = default;

        explicit solution_base(std::vector<int> x) {
            iterator_utils::fill_random(gaps.begin(), gaps.end(), MIN_GAP_VALUE, MAX_GAP_VALUE);
            std::copy_backward(x.begin(), x.end(), gaps.end());
            normalize();
        }

		void init_genes(const std::function<double(void)> &rnd01) {
			for (int & gap : gaps) {
				gap = std::floor(rnd01() * MAX_GAP_VALUE);
			}
			normalize();
		}

		void normalize() {
			std::sort(gaps.begin(), gaps.end(), std::greater<int>());
			// hack for correct sequences
			gaps[gaps.size() - 1] = 1;
		}

		solution_base crossover(
		const solution_base& X2,
		const std::function<double(void)> &rnd01) const {
			solution_base X_new;

			// using uniform distribution
			for (int i = 0; i < X_new.gaps.size(); i++) {
				double prob = rnd01();
				if (prob < EQUAL_PROBABILITY) {
					X_new.gaps[i] = this->gaps[i];
				} else {
					X_new.gaps[i] = X2.gaps[i];
				}
			}

			X_new.normalize();

			return X_new;
		}

		solution_base mutate(
		const std::function<double(void)> &rnd01,
		double shrink_scale) const {
			solution_base X_new = *this;
			const double mut_radius = MUT_SCALE * shrink_scale;

			for (int i = 0; i < gaps.size(); i++) {
				bool do_mutate = rnd01() < MUTATION_PROBABILITY;
				if (do_mutate) {
					int new_gap;
					do {
						double raw_mutation = mut_radius * (rnd01()-rnd01()); // value is between -1.0 and 1.0
						int adjusted_mutation = std::floor(raw_mutation * MUT_CEILING); // scale the value
						new_gap = gaps[i] + adjusted_mutation;
					} while(!is_gap_valid(new_gap));
					X_new.gaps[i] = new_gap;
				}
			}

			X_new.normalize();

			return X_new;
		}

		inline bool is_gap_valid(int gap) const {
			return gap >= MIN_GAP_VALUE && gap <= MAX_GAP_VALUE;
		}
	};

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
    struct solution_extended {
        constexpr static const double MUTATION_PROBABILITY = 0.5;
        constexpr static const int TAIL_SEQ_LEN = 7;

        std::vector<int> gaps = std::vector<int>(GAP_COUNT);

        solution_extended() = default;

        explicit solution_extended(std::vector<int> x) {
            std::copy(x.begin(), x.end(), gaps.begin());
        }

        void init_genes(const std::function<double(void)> &rnd01) {
            for (int i = 0; i < gaps.size(); i++) {
                gaps[i] = generate_gap(i);
            }
        }

        void normalize() {
            std::sort(gaps.begin(), gaps.end(), std::greater<int>());
        }

        solution_extended crossover(
                const solution_extended& X2,
                const std::function<double(void)> &rnd01) const {
            solution_extended X_new;

            // using uniform distribution
            for (int i = 0; i < X_new.gaps.size(); i++) {
                if (rnd01() < EQUAL_PROBABILITY) {
                    X_new.gaps[i] = this->gaps[i];
                } else {
                    X_new.gaps[i] = X2.gaps[i];
                }
            }

            X_new.normalize();

            return X_new;
        }

        solution_extended mutate(
                const std::function<double(void)> &rnd01,
                double shrink_scale) const {
            solution_extended X_new = solution_extended(gaps);
            const double mut_radius = 0.2 * shrink_scale;

            for (int i = 0; i < gaps.size() - TAIL_SEQ_LEN; i++) {
                if (rnd01() < MUTATION_PROBABILITY) {
                    X_new.gaps[i] = mutate_gap(i, gaps[i], rnd01, mut_radius);
                    break;
                }
            }

            return X_new;
        }

        inline int mutate_gap(int index, int value, const std::function<double(void)> &rnd01, double mut_radius) const {
            double raw_mutation = mut_radius * (rnd01()-rnd01()); // value is between -1.0 and 1.0
            int adjusted_mutation = std::floor(raw_mutation * get_mut_ceiling(index));

            if (adjusted_mutation >= 0) {
                // do positive mutation
                return std::min(get_higher_bound(index), value + adjusted_mutation);
            } else {
                // do negative mutation
                return std::max(get_lower_bound(index), value + adjusted_mutation);
            }
        }

        inline int generate_gap(int index) const {
            const static std::vector<int> tail_seq = std::vector<int> {974,669,137,40,13,5,1};

            int diff = gaps.size() - tail_seq.size();

            if (index >= diff) {
                return tail_seq[index - diff];
            }
            return random_utils::get_random_int(get_lower_bound(index), get_higher_bound(index));
        }

        inline int get_mut_ceiling(int index) const {
            static const std::array<int, 8> mut_ceilings {30000, 10000, 5000,
                                                          2500, 1000, 500,
                                                          250, 100};
            return mut_ceilings[index];
        }

        inline int get_lower_bound(int index) const {
            static const std::array<int, 8> lower_bounds {300000, 100000, 50000,
                                                          25000, 10000, 5000,
                                                          2500, 1000};
            return lower_bounds[index];
        }

        inline int get_higher_bound(int index) const {
            static const std::array<int, 8> higher_bounds {600000, 300000-1, 100000-1,
                                                           50000-1, 25000-1, 10000-1,
                                                           5000-1, 2500-1};
            return higher_bounds[index];
        }
    };

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
    void to_json(json& j, const solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
		j = json{
			{"gaps", sol.gaps},
		};
	}

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
	void from_json(const json& j, solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
        j.at("gaps").get_to(sol.gaps);
    }


    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
    void to_json(json& j, const solution_extended<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
        j = json{
                {"gaps", sol.gaps},
        };
    }

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
    void from_json(const json& j, solution_extended<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
        j.at("gaps").get_to(sol.gaps);
    }
}