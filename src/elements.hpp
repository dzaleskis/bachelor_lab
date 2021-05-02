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
        void incr_assignments(int measure_key) {
            auto report = threadsafe_report(measure_key);
            report->assignments += 1;
        }

        void incr_comparisons(int measure_key) {
            auto report = threadsafe_report(measure_key);
            report->comparisons += 1;
        }

        stats get_report(int measure_key) {
            auto report = threadsafe_report(measure_key);
            return (*report);
        }

        int init_report() {
            int measure_key = random_utils::get_random();
            measure_write_lock w_lock(lock);
            reports.insert(std::make_pair(measure_key, new stats()));
            return measure_key;
        }

    private:
        stats* threadsafe_report(int measure_key) {
            measure_read_lock r_lock(lock);
            return reports.at(measure_key);
        }

        std::unordered_map<int, stats*> reports;
};

measure global_measure;

template <typename T_VAL>
class Element {

public:
    Element() {}

    Element(T_VAL value, int measure_key){
        
        this->wrapped = value;
        this->measure_key = measure_key;
    }
    
    T_VAL value() const {
        
        return this->wrapped;
    }

    int get_measure_key() const {
        
        return this->measure_key;
    }

    Element<T_VAL> & operator = (const Element<T_VAL> & b) {
        
        this->wrapped = b.value();
        this->measure_key = b.get_measure_key();
        global_measure.incr_assignments(this->measure_key);
        return *this;
    }
    
    Element<T_VAL> & assign(const T_VAL & b) {
        
        this->wrapped = b;
        global_measure.incr_assignments(this->measure_key);
        return *this;
    }
    
private:

    T_VAL wrapped;
    int measure_key;
};


template <typename T_VAL>
bool operator > (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left.get_measure_key());
    return left.value() > right.value();
}

template <typename T_VAL>
bool operator >= (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left.get_measure_key());
    return left.value() >= right.value();
}

template <typename T_VAL>
bool operator < (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left.get_measure_key());
    return left.value() < right.value();
}

template <typename T_VAL>
bool operator <= (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left.get_measure_key());
    return left.value() <= right.value();
}

template <typename T_VAL>
bool operator == (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left.get_measure_key());
    return left.value() == right.value();
}

template <typename T_VAL>
bool operator != (const Element<T_VAL> left, const Element<T_VAL> right) {
    
    global_measure.incr_comparisons(left.get_measure_key());
    return left.value() != right.value();
}
