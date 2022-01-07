#pragma once
#include "json.hpp"
using json = nlohmann::json;

struct BenchmarkAlgorithm {
    BenchmarkAlgorithm(AlgorithmBlueprint algorithmBlueprint, const std::string & name) {
        this->algorithmBlueprint = algorithmBlueprint;
        this->name = name;
    }

    AlgorithmBlueprint algorithmBlueprint;
    std::string name;
};

AlgorithmBlueprint construct_algorithm(PassType pass, const std::vector<int> & gaps) {
    AlgorithmBlueprint algorithmBlueprint;
    algorithmBlueprint.passBlueprints.reserve(gaps.size());

    for (int gap : gaps) {
        algorithmBlueprint.passBlueprints.emplace_back(pass, gap);
    }

    return std::move(algorithmBlueprint);
}


// small algorithms
BenchmarkAlgorithm SH_SMALL_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, CIURA_SMALL_GAPS), "SH_SMALL_CIURA");
BenchmarkAlgorithm SH_SMALL_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, TOKUDA_SMALL_GAPS), "SH_SMALL_TOKUDA");
BenchmarkAlgorithm SH_SMALL_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, SEDGEWICK_SMALL_GAPS), "SH_SMALL_SEDGEWICK");

BenchmarkAlgorithm SHPP_SMALL_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, CIURA_SMALL_GAPS), "SHPP_SMALL_CIURA");
BenchmarkAlgorithm SHPP_SMALL_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, TOKUDA_SMALL_GAPS), "SHPP_SMALL_TOKUDA");
BenchmarkAlgorithm SHPP_SMALL_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, SEDGEWICK_SMALL_GAPS), "SHPP_SMALL_SEDGEWICK");

// comparisons
json GA_SMALL_1_JSON = "{\"passBlueprints\":[{\"gap\":861,\"passType\":\"bubble\"},{\"gap\":347,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_SMALL_2_JSON = "{\"passBlueprints\":[{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"bubble\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_SMALL_3_JSON = "{\"passBlueprints\":[{\"gap\":702,\"passType\":\"bubble\"},{\"gap\":275,\"passType\":\"insertion_improved\"},{\"gap\":108,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_SMALL_4_JSON = "{\"passBlueprints\":[{\"gap\":701,\"passType\":\"bubble\"},{\"gap\":233,\"passType\":\"insertion_improved\"},{\"gap\":108,\"passType\":\"insertion_improved\"},{\"gap\":43,\"passType\":\"insertion_improved\"},{\"gap\":20,\"passType\":\"insertion_improved\"},{\"gap\":9,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_SMALL_5_JSON = "{\"passBlueprints\":[{\"gap\":929,\"passType\":\"bubble\"},{\"gap\":347,\"passType\":\"insertion_improved\"},{\"gap\":209,\"passType\":\"bubble\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm GA_SMALL_1 = BenchmarkAlgorithm(GA_SMALL_1_JSON.get<AlgorithmBlueprint>(), "GA_SMALL_1");
BenchmarkAlgorithm GA_SMALL_2 = BenchmarkAlgorithm(GA_SMALL_2_JSON.get<AlgorithmBlueprint>(), "GA_SMALL_2");
BenchmarkAlgorithm GA_SMALL_3 = BenchmarkAlgorithm(GA_SMALL_3_JSON.get<AlgorithmBlueprint>(), "GA_SMALL_3");
BenchmarkAlgorithm GA_SMALL_4 = BenchmarkAlgorithm(GA_SMALL_4_JSON.get<AlgorithmBlueprint>(), "GA_SMALL_4");
BenchmarkAlgorithm GA_SMALL_5 = BenchmarkAlgorithm(GA_SMALL_5_JSON.get<AlgorithmBlueprint>(), "GA_SMALL_5");

const std::vector<BenchmarkAlgorithm> ALL_SMALL_ALGORITHMS {
        SH_SMALL_CIURA,SH_SMALL_TOKUDA,SH_SMALL_SEDGEWICK,
        SHPP_SMALL_CIURA,SHPP_SMALL_TOKUDA,SHPP_SMALL_SEDGEWICK,
        GA_SMALL_1, GA_SMALL_2, GA_SMALL_3, GA_SMALL_4, GA_SMALL_5
};

const std::vector<AlgorithmBlueprint> GA_SMALL_ALGORITHMS {
        GA_SMALL_1.algorithmBlueprint,GA_SMALL_2.algorithmBlueprint,GA_SMALL_3.algorithmBlueprint,
        GA_SMALL_4.algorithmBlueprint,GA_SMALL_5.algorithmBlueprint,
};

// BIG algorithms
BenchmarkAlgorithm SH_BIG_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, CIURA_BIG_GAPS), "SH_BIG_CIURA");
BenchmarkAlgorithm SH_BIG_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, TOKUDA_BIG_GAPS), "SH_BIG_TOKUDA");
BenchmarkAlgorithm SH_BIG_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, SEDGEWICK_BIG_GAPS), "SH_BIG_SEDGEWICK");

BenchmarkAlgorithm SHPP_BIG_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, CIURA_BIG_GAPS), "SHPP_BIG_CIURA");
BenchmarkAlgorithm SHPP_BIG_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, TOKUDA_BIG_GAPS), "SHPP_BIG_TOKUDA");
BenchmarkAlgorithm SHPP_BIG_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, SEDGEWICK_BIG_GAPS), "SHPP_BIG_SEDGEWICK");

// genetic
json GA_BIG_1_JSON = "{\"passBlueprints\":[{\"gap\":91032,\"passType\":\"insertion_improved\"},{\"gap\":40459,\"passType\":\"insertion_improved\"},{\"gap\":17982,\"passType\":\"insertion_improved\"},{\"gap\":7983,\"passType\":\"insertion_improved\"},{\"gap\":3552,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_BIG_2_JSON = "{\"passBlueprints\":[{\"gap\":84685,\"passType\":\"insertion\"},{\"gap\":36335,\"passType\":\"bubble\"},{\"gap\":20724,\"passType\":\"insertion_improved\"},{\"gap\":7692,\"passType\":\"insertion_improved\"},{\"gap\":3459,\"passType\":\"insertion_improved\"},{\"gap\":1549,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_BIG_3_JSON = "{\"passBlueprints\":[{\"gap\":64769,\"passType\":\"brick\"},{\"gap\":40412,\"passType\":\"bubble\"},{\"gap\":17961,\"passType\":\"insertion_improved\"},{\"gap\":8929,\"passType\":\"insertion_improved\"},{\"gap\":2660,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":275,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_BIG_4_JSON = "{\"passBlueprints\":[{\"gap\":84718,\"passType\":\"bubble\"},{\"gap\":40238,\"passType\":\"bubble\"},{\"gap\":17002,\"passType\":\"insertion_improved\"},{\"gap\":7594,\"passType\":\"insertion_improved\"},{\"gap\":3450,\"passType\":\"insertion_improved\"},{\"gap\":2044,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json GA_BIG_5_JSON = "{\"passBlueprints\":[{\"gap\":83276,\"passType\":\"insertion_improved\"},{\"gap\":37567,\"passType\":\"insertion_improved\"},{\"gap\":16947,\"passType\":\"insertion_improved\"},{\"gap\":7504,\"passType\":\"insertion_improved\"},{\"gap\":3475,\"passType\":\"insertion_improved\"},{\"gap\":1556,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;

BenchmarkAlgorithm GA_BIG_1 = BenchmarkAlgorithm(GA_BIG_1_JSON.get<AlgorithmBlueprint>(), "GA_BIG_1");
BenchmarkAlgorithm GA_BIG_2 = BenchmarkAlgorithm(GA_BIG_2_JSON.get<AlgorithmBlueprint>(), "GA_BIG_2");
BenchmarkAlgorithm GA_BIG_3 = BenchmarkAlgorithm(GA_BIG_3_JSON.get<AlgorithmBlueprint>(), "GA_BIG_3");
BenchmarkAlgorithm GA_BIG_4 = BenchmarkAlgorithm(GA_BIG_4_JSON.get<AlgorithmBlueprint>(), "GA_BIG_4");
BenchmarkAlgorithm GA_BIG_5 = BenchmarkAlgorithm(GA_BIG_5_JSON.get<AlgorithmBlueprint>(), "GA_BIG_5");


const std::vector<BenchmarkAlgorithm> ALL_BIG_ALGORITHMS {
        SH_BIG_CIURA, SH_BIG_TOKUDA, SH_BIG_SEDGEWICK,
        SHPP_BIG_CIURA, SHPP_BIG_TOKUDA, SHPP_BIG_SEDGEWICK,
        GA_BIG_1, GA_BIG_2, GA_BIG_3, GA_BIG_4, GA_BIG_5,
};
