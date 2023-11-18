// value.h
// object can store either a bool or a double

#pragma once
#include <iostream>
#include <vector>
#include <variant>

/**
struct Value : public std::variant<
    double,
    bool,
    std::vector<Value*>
>
{
    double double_value() {
        return std::get<double>(*this);
    }
    bool bool_value() {
        return std::get<double>(*this);
    }
    std::vector<Value*> arr_value() {
        return std::get<std::vector<Value*>>(*this);
    }
}
*/

struct Value {
    enum TypeTag {
        DOUBLE,
        BOOL,
        ARRAY
    };

    TypeTag type;
    union {
        double double_value;
        bool bool_value;
        std::vector<Value*> arr_value;
    };

    ~Value() {}

    Value(Value const& value) {
        type = value.type;
        if (value.type == BOOL) {
            bool_value = value.bool_value;
        }
        else if (value.type == DOUBLE) {
            double_value = value.double_value;
        }
        else {
            arr_value = value.arr_value;
        }
    }

    Value& operator= (const Value& value) {
        type = value.type;
        if (value.type == BOOL) {
            bool_value = value.bool_value;
        }
        else if (value.type == DOUBLE) {
            double_value = value.double_value;
        }
        else {
            arr_value = value.arr_value;
        }

        return *this;
    }

    Value(double val = 0.0) {
        double_value = val;
        type = DOUBLE;
    }

    Value(bool val) {
        bool_value = val;
        type = BOOL;
    }

    Value(std::vector<Value*> arr) {
        arr_value = arr;
        type = ARRAY;
    }
};


std::ostream& operator << (std::ostream& stream, const Value& value);