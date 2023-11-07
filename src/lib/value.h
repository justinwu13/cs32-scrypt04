// value.h
// object can store either a bool or a double

#pragma once
#include <iostream>

struct Value {
    enum TypeTag {
        DOUBLE,
        BOOL
    };

    TypeTag type;
    union {
        double double_value;
        bool bool_value;
    };

    Value(double val = 0.0) {
        double_value = val;
        type = DOUBLE;
    }

    Value(bool val) {
        bool_value = val;
        type = BOOL;
    }


};

std::ostream& operator << (std::ostream& stream, const Value& value);