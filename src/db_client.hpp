//#pragma once
//#include <string>
//#include "sqlite3pp.h"
//#include "fitness.hpp"
//
//const char* DB_PATH = "C:\\Users\\deividas.zaleskis\\uni\\project\\course_project_lab\\db\\results.db";
//

//
//void save_result_to_db(const ResultEntity & result) {
//    try {
//        SQLite::Database db(DB_PATH);
//
//        auto queryFormat = "INSERT INTO "
//                          "genetic_algorithm_result "
//                          "(avg_inversions, avg_comparisons, avg_assignments, fitness, algorithm_json) "
//                          "values(?,?,?,?,?);";
//
//        SQLite::Statement query(db, queryFormat);
//        query.bind(1, result.avg_inversions);
//        query.bind(2, result.avg_comparisons);
//        query.bind(3, result.avg_assignments);
//        query.bind(4, result.fitness);
//        query.bind(5, result.algorithm_json);
//
//        query.exec();
//
//    } catch (std::exception& e) {
//        std::cout << e.what() << std::endl;
//    }
//}