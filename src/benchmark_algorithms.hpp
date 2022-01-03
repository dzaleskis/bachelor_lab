#pragma once

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
json CMP_SMALL_1_JSON = "{\"passBlueprints\":[{\"gap\":918,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":46,\"passType\":\"insertion_improved\"},{\"gap\":17,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_SMALL_2_JSON = "{\"passBlueprints\":[{\"gap\":861,\"passType\":\"bubble\"},{\"gap\":347,\"passType\":\"insertion_improved\"},{\"gap\":103,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":20,\"passType\":\"insertion_improved\"},{\"gap\":9,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_SMALL_3_JSON = "{\"passBlueprints\":[{\"gap\":701,\"passType\":\"bubble\"},{\"gap\":347,\"passType\":\"bubble\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm CMP_SMALL_1 = BenchmarkAlgorithm(CMP_SMALL_1_JSON.get<AlgorithmBlueprint>(),"CMP_SMALL_1");
BenchmarkAlgorithm CMP_SMALL_2 = BenchmarkAlgorithm(CMP_SMALL_2_JSON.get<AlgorithmBlueprint>(),"CMP_SMALL_2");
BenchmarkAlgorithm CMP_SMALL_3 = BenchmarkAlgorithm(CMP_SMALL_3_JSON.get<AlgorithmBlueprint>(),"CMP_SMALL_3");

// assignments
json ASSGN_SMALL_1_JSON = "{\"passBlueprints\":[{\"gap\":702,\"passType\":\"insertion_improved\"},{\"gap\":275,\"passType\":\"insertion_improved\"},{\"gap\":108,\"passType\":\"insertion_improved\"},{\"gap\":43,\"passType\":\"insertion_improved\"},{\"gap\":17,\"passType\":\"insertion_improved\"},{\"gap\":7,\"passType\":\"insertion_improved\"},{\"gap\":3,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_SMALL_2_JSON = "{\"passBlueprints\":[{\"gap\":702,\"passType\":\"bubble\"},{\"gap\":233,\"passType\":\"insertion_improved\"},{\"gap\":103,\"passType\":\"insertion_improved\"},{\"gap\":46,\"passType\":\"insertion_improved\"},{\"gap\":17,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_SMALL_3_JSON = "{\"passBlueprints\":[{\"gap\":701,\"passType\":\"bubble\"},{\"gap\":233,\"passType\":\"insertion_improved\"},{\"gap\":108,\"passType\":\"insertion_improved\"},{\"gap\":43,\"passType\":\"insertion_improved\"},{\"gap\":19,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
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

// mid algorithms
BenchmarkAlgorithm SH_MID_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, CIURA_MID_GAPS), "SH_MID_CIURA");
BenchmarkAlgorithm SH_MID_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, TOKUDA_MID_GAPS), "SH_MID_TOKUDA");
BenchmarkAlgorithm SH_MID_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, SEDGEWICK_MID_GAPS), "SH_MID_SEDGEWICK");
BenchmarkAlgorithm SH_MID_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, INCERPI_SEDGEWICK_MID_GAPS), "SH_MID_ICSW");
BenchmarkAlgorithm SH_MID_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, PRATT_MID_GAPS), "SH_MID_PRATT");

BenchmarkAlgorithm SHPP_MID_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, CIURA_MID_GAPS), "SHPP_MID_CIURA");
BenchmarkAlgorithm SHPP_MID_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, TOKUDA_MID_GAPS), "SHPP_MID_TOKUDA");
BenchmarkAlgorithm SHPP_MID_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, SEDGEWICK_MID_GAPS), "SHPP_MID_SEDGEWICK");
BenchmarkAlgorithm SHPP_MID_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, INCERPI_SEDGEWICK_MID_GAPS), "SHPP_MID_ICSW");
BenchmarkAlgorithm SHPP_MID_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, PRATT_MID_GAPS), "SHPP_MID_PRATT");

// comparisons
json CMP_MID_1_JSON = "{\"passBlueprints\":[{\"gap\":7983,\"passType\":\"insertion_improved\"},{\"gap\":3548,\"passType\":\"bubble\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":3,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_MID_2_JSON = "{\"passBlueprints\":[{\"gap\":5137,\"passType\":\"insertion_improved\"},{\"gap\":1577,\"passType\":\"brick\"},{\"gap\":1577,\"passType\":\"insertion_improved\"},{\"gap\":701,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":132,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json CMP_MID_3_JSON = "{\"passBlueprints\":[{\"gap\":5985,\"passType\":\"insertion_improved\"},{\"gap\":2660,\"passType\":\"bubble\"},{\"gap\":1182,\"passType\":\"insertion_improved\"},{\"gap\":525,\"passType\":\"insertion_improved\"},{\"gap\":233,\"passType\":\"insertion_improved\"},{\"gap\":103,\"passType\":\"insertion_improved\"},{\"gap\":46,\"passType\":\"insertion_improved\"},{\"gap\":20,\"passType\":\"insertion_improved\"},{\"gap\":9,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm CMP_MID_1 = BenchmarkAlgorithm(CMP_MID_1_JSON.get<AlgorithmBlueprint>(),"CMP_MID_1");
BenchmarkAlgorithm CMP_MID_2 = BenchmarkAlgorithm(CMP_MID_2_JSON.get<AlgorithmBlueprint>(),"CMP_MID_2");
BenchmarkAlgorithm CMP_MID_3 = BenchmarkAlgorithm(CMP_MID_3_JSON.get<AlgorithmBlueprint>(),"CMP_MID_3");

// assignments
json ASSGN_MID_1_JSON = "{\"passBlueprints\":[{\"gap\":5137,\"passType\":\"insertion_improved\"},{\"gap\":2111,\"passType\":\"insertion_improved\"},{\"gap\":657,\"passType\":\"insertion_improved\"},{\"gap\":525,\"passType\":\"insertion_improved\"},{\"gap\":233,\"passType\":\"insertion_improved\"},{\"gap\":103,\"passType\":\"insertion_improved\"},{\"gap\":46,\"passType\":\"insertion_improved\"},{\"gap\":13,\"passType\":\"insertion_improved\"},{\"gap\":9,\"passType\":\"insertion_improved\"},{\"gap\":3,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_MID_2_JSON = "{\"passBlueprints\":[{\"gap\":5985,\"passType\":\"insertion_improved\"},{\"gap\":2660,\"passType\":\"insertion_improved\"},{\"gap\":1182,\"passType\":\"insertion_improved\"},{\"gap\":861,\"passType\":\"insertion_improved\"},{\"gap\":301,\"passType\":\"insertion_improved\"},{\"gap\":103,\"passType\":\"insertion_improved\"},{\"gap\":48,\"passType\":\"insertion_improved\"},{\"gap\":23,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json ASSGN_MID_3_JSON ="{\"passBlueprints\":[{\"gap\":3905,\"passType\":\"insertion_improved\"},{\"gap\":1478,\"passType\":\"insertion_improved\"},{\"gap\":1182,\"passType\":\"insertion_improved\"},{\"gap\":505,\"passType\":\"insertion_improved\"},{\"gap\":213,\"passType\":\"insertion_improved\"},{\"gap\":109,\"passType\":\"insertion_improved\"},{\"gap\":41,\"passType\":\"insertion_improved\"},{\"gap\":20,\"passType\":\"insertion_improved\"},{\"gap\":9,\"passType\":\"insertion_improved\"},{\"gap\":4,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm ASSGN_MID_1 = BenchmarkAlgorithm(ASSGN_MID_1_JSON.get<AlgorithmBlueprint>(),"ASSGN_MID_1");
BenchmarkAlgorithm ASSGN_MID_2 = BenchmarkAlgorithm(ASSGN_MID_2_JSON.get<AlgorithmBlueprint>(),"ASSGN_MID_2");
BenchmarkAlgorithm ASSGN_MID_3 = BenchmarkAlgorithm(ASSGN_MID_3_JSON.get<AlgorithmBlueprint>(),"ASSGN_MID_3");


// time
json TIME_MID_1_JSON = "{\"passBlueprints\":[{\"gap\":3548,\"passType\":\"insertion_improved\"},{\"gap\":2186,\"passType\":\"bubble\"},{\"gap\":931,\"passType\":\"insertion_improved\"},{\"gap\":396,\"passType\":\"insertion_improved\"},{\"gap\":233,\"passType\":\"insertion_improved\"},{\"gap\":112,\"passType\":\"insertion_improved\"},{\"gap\":57,\"passType\":\"insertion_improved\"},{\"gap\":18,\"passType\":\"insertion_improved\"},{\"gap\":10,\"passType\":\"insertion_improved\"},{\"gap\":3,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json TIME_MID_2_JSON = "{\"passBlueprints\":[{\"gap\":3905,\"passType\":\"insertion_improved\"},{\"gap\":3326,\"passType\":\"bubble\"},{\"gap\":931,\"passType\":\"insertion_improved\"},{\"gap\":396,\"passType\":\"insertion_improved\"},{\"gap\":169,\"passType\":\"insertion_improved\"},{\"gap\":103,\"passType\":\"insertion_improved\"},{\"gap\":72,\"passType\":\"insertion_improved\"},{\"gap\":13,\"passType\":\"insertion_improved\"},{\"gap\":12,\"passType\":\"insertion_improved\"},{\"gap\":5,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
json TIME_MID_3_JSON = "{\"passBlueprints\":[{\"gap\":1311,\"passType\":\"insertion_improved\"},{\"gap\":640,\"passType\":\"insertion_improved\"},{\"gap\":312,\"passType\":\"insertion_improved\"},{\"gap\":153,\"passType\":\"insertion_improved\"},{\"gap\":99,\"passType\":\"shake\"},{\"gap\":46,\"passType\":\"insertion_improved\"},{\"gap\":22,\"passType\":\"brick\"},{\"gap\":21,\"passType\":\"insertion_improved\"},{\"gap\":5,\"passType\":\"insertion_improved\"},{\"gap\":3,\"passType\":\"insertion_improved\"},{\"gap\":1,\"passType\":\"insertion_improved\"}]}"_json;
BenchmarkAlgorithm TIME_MID_1 = BenchmarkAlgorithm(TIME_MID_1_JSON.get<AlgorithmBlueprint>(),"TIME_MID_1");
BenchmarkAlgorithm TIME_MID_2 = BenchmarkAlgorithm(TIME_MID_2_JSON.get<AlgorithmBlueprint>(),"TIME_MID_2");
BenchmarkAlgorithm TIME_MID_3 = BenchmarkAlgorithm(TIME_MID_3_JSON.get<AlgorithmBlueprint>(),"TIME_MID_3");


const std::vector<BenchmarkAlgorithm> ALL_MID_ALGORITHMS {
        SH_MID_CIURA,SH_MID_TOKUDA,SH_MID_SEDGEWICK,SH_MID_ICSW,SH_MID_PRATT,
        SHPP_MID_CIURA,SHPP_MID_TOKUDA,SHPP_MID_SEDGEWICK,SHPP_MID_ICSW,SHPP_MID_PRATT,
        CMP_MID_1,CMP_MID_2,CMP_MID_3,
        ASSGN_MID_1,ASSGN_MID_2,ASSGN_MID_3,
        TIME_MID_1,TIME_MID_2,TIME_MID_3
};
