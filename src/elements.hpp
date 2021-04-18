#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <exception>
#include "random_utils.hpp"
#include <mutex>
#include <shared_mutex>

struct stats {
    unsigned int assignments = 0;
    unsigned int comparisons = 0;
};

typedef std::shared_mutex measure_lock;
typedef std::unique_lock<measure_lock> measure_write_lock;
typedef std::shared_lock<measure_lock> measure_read_lock;

measure_lock lock;

class measure {
    public:
        void incr_assignments(std::string measure_key) {
            auto report = threadsafe_report(measure_key);
            report->assignments += 1;
        }

        void incr_comparisons(std::string measure_key) {
            auto report = threadsafe_report(measure_key);
            report->comparisons += 1;
        }

        stats get_report(std::string measure_key) {
            auto report = threadsafe_report(measure_key);
            return (*report);
        }

        std::string init_report() {
            std::string measure_key = std::to_string(random_utils::get_random());
            measure_write_lock w_lock(lock);
            reports.insert(std::make_pair(measure_key, new stats()));
            return measure_key;
        }

    private:
        stats* threadsafe_report(std::string measure_key) {
            measure_read_lock r_lock(lock);
            return reports.at(measure_key);
        }

        std::unordered_map<std::string, stats*> reports;
};

measure global_measure;

template <typename T_VAL>
class Element {

public:
    Element(T_VAL value, std::string measure_key){
        
        this->wrapped = value;
        this->key = measure_key;
    }
    
    T_VAL get_wrapped() const {
        
        return this->wrapped;
    }

    std::string get_measure_key() const {
        
        return this->measure_key;
    }

    Element<T_VAL> & operator = (const Element<T_VAL> & b) {
        
        global_measure.incr_assignments(this->measure_key);
        this->wrapped = b.get_wrapped();
        return *this;
    }
    
    Element<T_VAL> & operator = (const T_VAL & b) {
        
        global_measure.incr_assignments(this->measure_key);
        this->wrapped = b;
        return *this;
    }
    
private:
    Element() {}

    T_VAL wrapped;
    std::string measure_key;
};


template <typename T_VAL>
bool operator > (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left->get_measure_key());
    return left.get_wrapped() > right.get_wrapped();
}

template <typename T_VAL>
bool operator >= (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left->get_measure_key());
    return left.get_wrapped() >= right.get_wrapped();
}

template <typename T_VAL>
bool operator < (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left->get_measure_key());
    return left.get_wrapped() < right.get_wrapped();
}

template <typename T_VAL>
bool operator <= (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left->get_measure_key());
    return left.get_wrapped() <= right.get_wrapped();
}

template <typename T_VAL>
bool operator == (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left->get_measure_key());
    return left.get_wrapped() == right.get_wrapped();
}

template <typename T_VAL>
bool operator != (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left->get_measure_key());
    return left.get_wrapped() != right.get_wrapped();
}