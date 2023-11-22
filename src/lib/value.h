// value.h
// object that stores any value type, including numbers, booleans, arrays, and functions

#pragma once
#include <iostream>
#include <vector>
#include <variant>
#include <memory>
#include "function.h"

struct Value {
    enum TypeTag {
        NULLVALUE = -1,
        DOUBLE,
        BOOL,
        ARRAY,
        FUNC
    };

    TypeTag type;
    double double_value;
    bool bool_value;
    std::shared_ptr<std::vector<Value>> arr_value;
    std::shared_ptr<Function> func_value;

    ~Value();

    Value() {
        type = NULLVALUE;
    }

    Value(Value const& value);

    Value& operator= (const Value& value);

    bool operator== (const Value& value) const;

    Value(double val) {
        double_value = val;
        type = DOUBLE;
    }

    Value(bool val) {
        bool_value = val;
        type = BOOL;
    }

    Value(std::vector<Value>& arr) {
        arr_value = std::make_shared<std::vector<Value>>(arr);
        type = ARRAY;
    }

    Value(Function& func) {
        func_value = std::make_shared<Function>(func);
        type = FUNC;
    }
};


std::ostream& operator << (std::ostream& stream, const Value& value);