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

    Value();
    Value(double val);
    Value(bool val);
    Value(std::vector<Value>& arr);
    Value(Function& func);

    Value(Value const& value);
    Value& operator= (const Value& value);
    bool operator== (const Value& value) const;
};


std::ostream& operator << (std::ostream& stream, const Value& value);