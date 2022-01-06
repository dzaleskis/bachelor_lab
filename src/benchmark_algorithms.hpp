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
BenchmarkAlgorithm SH_SMALL_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, INCERPI_SEDGEWICK_SMALL_GAPS), "SH_SMALL_ICSW");
BenchmarkAlgorithm SH_SMALL_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, PRATT_SMALL_GAPS), "SH_SMALL_PRATT");

BenchmarkAlgorithm SHPP_SMALL_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, CIURA_SMALL_GAPS), "SHPP_SMALL_CIURA");
BenchmarkAlgorithm SHPP_SMALL_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, TOKUDA_SMALL_GAPS), "SHPP_SMALL_TOKUDA");
BenchmarkAlgorithm SHPP_SMALL_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, SEDGEWICK_SMALL_GAPS), "SHPP_SMALL_SEDGEWICK");
BenchmarkAlgorithm SHPP_SMALL_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, INCERPI_SEDGEWICK_SMALL_GAPS), "SHPP_SMALL_ICSW");
BenchmarkAlgorithm SHPP_SMALL_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, PRATT_SMALL_GAPS), "SHPP_SMALL_PRATT");

// comparisons
json CMP_SMALL_1_JSON = "{\"passBlueprints\":[{\"gap\":861,\"passType\":\"bubble\"},{\"gap\":347,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_SMALL_2_JSON = "{\"passBlueprints\":[{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"bubble\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_SMALL_3_JSON = "{\"passBlueprints\":[{\"gap\":702,\"passType\":\"bubble\"},{\"gap\":275,\"passType\":\"insertion_improved\"},{\"gap\":108,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm CMP_SMALL_1 = BenchmarkAlgorithm(CMP_SMALL_1_JSON.get<AlgorithmBlueprint>(),"CMP_SMALL_1");
BenchmarkAlgorithm CMP_SMALL_2 = BenchmarkAlgorithm(CMP_SMALL_2_JSON.get<AlgorithmBlueprint>(),"CMP_SMALL_2");
BenchmarkAlgorithm CMP_SMALL_3 = BenchmarkAlgorithm(CMP_SMALL_3_JSON.get<AlgorithmBlueprint>(),"CMP_SMALL_3");

// assignments
json ASSGN_SMALL_1_JSON = "{\"passBlueprints\":[{\"gap\":702,\"passType\":\"bubble\"},{\"gap\":275,\"passType\":\"insertion_improved\"},{\"gap\":108,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_SMALL_2_JSON = "{\"passBlueprints\":[{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":17,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_SMALL_3_JSON = "{\"passBlueprints\":[{\"gap\":701,\"passType\":\"bubble\"},{\"gap\":233,\"passType\":\"insertion_improved\"},{\"gap\":108,\"passType\":\"insertion_improved\"},{\"gap\":43,\"passType\":\"insertion_improved\"},{\"gap\":20,\"passType\":\"insertion_improved\"},{\"gap\":9,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm ASSGN_SMALL_1 = BenchmarkAlgorithm(ASSGN_SMALL_1_JSON.get<AlgorithmBlueprint>(),"ASSGN_SMALL_1");
BenchmarkAlgorithm ASSGN_SMALL_2 = BenchmarkAlgorithm(ASSGN_SMALL_2_JSON.get<AlgorithmBlueprint>(),"ASSGN_SMALL_2");
BenchmarkAlgorithm ASSGN_SMALL_3 = BenchmarkAlgorithm(ASSGN_SMALL_3_JSON.get<AlgorithmBlueprint>(),"ASSGN_SMALL_3");

// time
json TIME_SMALL_1_JSON = "{\"passBlueprints\":[{\"gap\":929,\"passType\":\"bubble\"},{\"gap\":347,\"passType\":\"insertion_improved\"},{\"gap\":209,\"passType\":\"bubble\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json TIME_SMALL_2_JSON = "{\"passBlueprints\":[{\"gap\":861,\"passType\":\"bubble\"},{\"gap\":336,\"passType\":\"insertion_improved\"},{\"gap\":131,\"passType\":\"insertion_improved\"},{\"gap\":48,\"passType\":\"insertion_improved\"},{\"gap\":19,\"passType\":\"insertion_improved\"},{\"gap\":7,\"passType\":\"insertion_improved\"},{\"gap\":3,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json TIME_SMALL_3_JSON = "{\"passBlueprints\":[{\"gap\":701,\"passType\":\"bubble\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm TIME_SMALL_1 = BenchmarkAlgorithm(TIME_SMALL_1_JSON.get<AlgorithmBlueprint>(),"TIME_SMALL_1");
BenchmarkAlgorithm TIME_SMALL_2 = BenchmarkAlgorithm(TIME_SMALL_2_JSON.get<AlgorithmBlueprint>(),"TIME_SMALL_2");
BenchmarkAlgorithm TIME_SMALL_3 = BenchmarkAlgorithm(TIME_SMALL_3_JSON.get<AlgorithmBlueprint>(),"TIME_SMALL_3");

const std::vector<BenchmarkAlgorithm> ALL_SMALL_ALGORITHMS {
        SH_SMALL_CIURA,SH_SMALL_TOKUDA,SH_SMALL_SEDGEWICK,SH_SMALL_ICSW,SH_SMALL_PRATT,
        SHPP_SMALL_CIURA,SHPP_SMALL_TOKUDA,SHPP_SMALL_SEDGEWICK,SHPP_SMALL_ICSW,SHPP_SMALL_PRATT,
        CMP_SMALL_1,CMP_SMALL_2,CMP_SMALL_3,
        ASSGN_SMALL_1,ASSGN_SMALL_2,ASSGN_SMALL_3,
        TIME_SMALL_1,TIME_SMALL_2,TIME_SMALL_3
};

const std::vector<AlgorithmBlueprint> GA_SMALL_ALGORITHMS {
        CMP_SMALL_1.algorithmBlueprint,CMP_SMALL_2.algorithmBlueprint,CMP_SMALL_3.algorithmBlueprint,
        ASSGN_SMALL_1.algorithmBlueprint,ASSGN_SMALL_2.algorithmBlueprint,ASSGN_SMALL_3.algorithmBlueprint,
        TIME_SMALL_1.algorithmBlueprint,TIME_SMALL_2.algorithmBlueprint,TIME_SMALL_3.algorithmBlueprint
};

// BIG algorithms
BenchmarkAlgorithm SH_BIG_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, CIURA_BIG_GAPS), "SH_BIG_CIURA");
BenchmarkAlgorithm SH_BIG_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, TOKUDA_BIG_GAPS), "SH_BIG_TOKUDA");
BenchmarkAlgorithm SH_BIG_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, SEDGEWICK_BIG_GAPS), "SH_BIG_SEDGEWICK");
BenchmarkAlgorithm SH_BIG_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, INCERPI_SEDGEWICK_BIG_GAPS), "SH_BIG_ICSW");
BenchmarkAlgorithm SH_BIG_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, PRATT_BIG_GAPS), "SH_BIG_PRATT");

BenchmarkAlgorithm SHPP_BIG_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, CIURA_BIG_GAPS), "SHPP_BIG_CIURA");
BenchmarkAlgorithm SHPP_BIG_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, TOKUDA_BIG_GAPS), "SHPP_BIG_TOKUDA");
BenchmarkAlgorithm SHPP_BIG_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, SEDGEWICK_BIG_GAPS), "SHPP_BIG_SEDGEWICK");
BenchmarkAlgorithm SHPP_BIG_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, INCERPI_SEDGEWICK_BIG_GAPS), "SHPP_BIG_ICSW");
BenchmarkAlgorithm SHPP_BIG_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, PRATT_BIG_GAPS), "SHPP_BIG_PRATT");

// comparisons
json CMP_BIG_1_JSON = "{\"passBlueprints\":[{\"gap\":64769,\"passType\":\"brick\"},{\"gap\":40412,\"passType\":\"bubble\"},{\"gap\":17961,\"passType\":\"insertion_improved\"},{\"gap\":8929,\"passType\":\"insertion_improved\"},{\"gap\":2660,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":275,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_BIG_2_JSON = "{\"passBlueprints\":[{\"gap\":90927,\"passType\":\"insertion_improved\"},{\"gap\":30301,\"passType\":\"insertion_improved\"},{\"gap\":12072,\"passType\":\"insertion_improved\"},{\"gap\":5137,\"passType\":\"insertion_improved\"},{\"gap\":2660,\"passType\":\"insertion\"},{\"gap\":1182,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_BIG_3_JSON = "{\"passBlueprints\":[{\"gap\":40412,\"passType\":\"bubble\"},{\"gap\":20975,\"passType\":\"bubble\"},{\"gap\":7983,\"passType\":\"insertion\"},{\"gap\":5137,\"passType\":\"insertion\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":931,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"bubble\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm CMP_BIG_1 = BenchmarkAlgorithm(CMP_BIG_1_JSON.get<AlgorithmBlueprint>(),"CMP_BIG_1");
BenchmarkAlgorithm CMP_BIG_2 = BenchmarkAlgorithm(CMP_BIG_2_JSON.get<AlgorithmBlueprint>(),"CMP_BIG_2");
BenchmarkAlgorithm CMP_BIG_3 = BenchmarkAlgorithm(CMP_BIG_3_JSON.get<AlgorithmBlueprint>(),"CMP_BIG_3");

// assignments
json ASSGN_BIG_1_JSON = "{\"passBlueprints\":[{\"gap\":90927,\"passType\":\"bubble\"},{\"gap\":30301,\"passType\":\"insertion_improved\"},{\"gap\":17961,\"passType\":\"insertion_improved\"},{\"gap\":5985,\"passType\":\"insertion_improved\"},{\"gap\":3548,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_BIG_2_JSON = "{\"passBlueprints\":[{\"gap\":64769,\"passType\":\"shake\"},{\"gap\":36289,\"passType\":\"insertion_improved\"},{\"gap\":13467,\"passType\":\"insertion_improved\"},{\"gap\":7983,\"passType\":\"insertion_improved\"},{\"gap\":3548,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_BIG_3_JSON ="{\"passBlueprints\":[{\"gap\":68178,\"passType\":\"bubble\"},{\"gap\":30301,\"passType\":\"insertion_improved\"},{\"gap\":13467,\"passType\":\"insertion_improved\"},{\"gap\":3326,\"passType\":\"insertion_improved\"},{\"gap\":2161,\"passType\":\"insertion_improved\"},{\"gap\":1182,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":275,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm ASSGN_BIG_1 = BenchmarkAlgorithm(ASSGN_BIG_1_JSON.get<AlgorithmBlueprint>(),"ASSGN_BIG_1");
BenchmarkAlgorithm ASSGN_BIG_2 = BenchmarkAlgorithm(ASSGN_BIG_2_JSON.get<AlgorithmBlueprint>(),"ASSGN_BIG_2");
BenchmarkAlgorithm ASSGN_BIG_3 = BenchmarkAlgorithm(ASSGN_BIG_3_JSON.get<AlgorithmBlueprint>(),"ASSGN_BIG_3");


// time
json TIME_BIG_1_JSON = "{\"passBlueprints\":[{\"gap\":90927,\"passType\":\"bubble\"},{\"gap\":30301,\"passType\":\"insertion_improved\"},{\"gap\":13467,\"passType\":\"insertion_improved\"},{\"gap\":7983,\"passType\":\"insertion_improved\"},{\"gap\":2660,\"passType\":\"insertion_improved\"},{\"gap\":931,\"passType\":\"insertion_improved\"},{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":17,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json TIME_BIG_2_JSON = "{\"passBlueprints\":[{\"gap\":66663,\"passType\":\"bubble\"},{\"gap\":30301,\"passType\":\"bubble\"},{\"gap\":17961,\"passType\":\"insertion_improved\"},{\"gap\":5985,\"passType\":\"insertion_improved\"},{\"gap\":3548,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":17,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json TIME_BIG_3_JSON = "{\"passBlueprints\":[{\"gap\":90927,\"passType\":\"insertion\"},{\"gap\":40412,\"passType\":\"insertion_improved\"},{\"gap\":17961,\"passType\":\"insertion_improved\"},{\"gap\":7983,\"passType\":\"insertion_improved\"},{\"gap\":3548,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":347,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm TIME_BIG_1 = BenchmarkAlgorithm(TIME_BIG_1_JSON.get<AlgorithmBlueprint>(),"TIME_BIG_1");
BenchmarkAlgorithm TIME_BIG_2 = BenchmarkAlgorithm(TIME_BIG_2_JSON.get<AlgorithmBlueprint>(),"TIME_BIG_2");
BenchmarkAlgorithm TIME_BIG_3 = BenchmarkAlgorithm(TIME_BIG_3_JSON.get<AlgorithmBlueprint>(),"TIME_BIG_3");


const std::vector<BenchmarkAlgorithm> ALL_BIG_ALGORITHMS {
        SH_BIG_CIURA,SH_BIG_TOKUDA,SH_BIG_SEDGEWICK,SH_BIG_ICSW,SH_BIG_PRATT,
        SHPP_BIG_CIURA,SHPP_BIG_TOKUDA,SHPP_BIG_SEDGEWICK,SHPP_BIG_ICSW,SHPP_BIG_PRATT,
        CMP_BIG_1,CMP_BIG_2,CMP_BIG_3,
        ASSGN_BIG_1,ASSGN_BIG_2,ASSGN_BIG_3,
        TIME_BIG_1,TIME_BIG_2,TIME_BIG_3
};
