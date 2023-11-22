// value.h
// object can store either a bool or a double

#pragma once
#include <iostream>
#include <vector>
#include <variant>
#include <memory>
#include "function.h"

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

    ~Value() {
        if (type == ARRAY) {
            arr_value.reset();
        }
        else if (type == FUNC) {
            func_value.reset();
        }
    }

    Value() {
        type = NULLVALUE;
    }

    Value(Value const& value) {
        type = value.type;
        if (value.type == BOOL) {
            bool_value = value.bool_value;
            // double_value = 0.0;
            // arr_value = nullptr;
        }
        else if (value.type == DOUBLE) {
            double_value = value.double_value;
            // bool_value = false;
            // arr_value = nullptr;
        }
        else if (value.type == FUNC) {
            func_value = value.func_value;
            // bool_value = false;
            // arr_value = nullptr;
        }
        else if (value.type == ARRAY) {
            arr_value = value.arr_value;
            // bool_value = false;
            // double_value = 0.0;
        }
    }

    Value& operator= (const Value& value) {
        type = value.type;
        if (value.type == BOOL) {
            bool_value = value.bool_value;
            // double_value = 0.0;
            // arr_value = nullptr;
        }
        else if (value.type == DOUBLE) {
            double_value = value.double_value;
            // bool_value = false;
            // arr_value = nullptr;
        }
        else if (value.type == FUNC) {
            func_value = value.func_value;
            // bool_value = false;
            // arr_value = nullptr;
        }
        else if (value.type == ARRAY) {
            arr_value = value.arr_value;
            // bool_value = false;
            // double_value = 0.0;
        }
        else { // NULLVALUE
            // do nothing
        }

        return *this;
    }

    bool operator== (const Value& value) const {
        if (this->type != value.type) {
            return false;
        }
        if (type == BOOL) {
            return bool_value == value.bool_value;
        }
        else if (type == DOUBLE) {
            return double_value == value.double_value;
        }
        else if (type == ARRAY) {
            return *arr_value == *value.arr_value;
        }
        else if (type == FUNC) {
            return func_value == value.func_value; // both pointers point to the same memory
        }
        else { // both types are null
            return true;
        }
    }

    Value(double val) {
        double_value = val;
        // bool_value = false;
        // arr_value = nullptr;
        type = DOUBLE;
    }

    Value(bool val) {
        bool_value = val;
        // double_value = 0.0;
        // arr_value = nullptr;
        type = BOOL;
    }

    Value(std::vector<Value>& arr) {
        arr_value = std::make_shared<std::vector<Value>>(arr);
        // double_value = 0.0;
        // bool_value = false;
        type = ARRAY;
    }

    Value(Function& func) {
        func_value = std::make_shared<Function>(func);
        // double_value = 0.0;
        // bool_value = false;
        type = FUNC;
    }
};


std::ostream& operator << (std::ostream& stream, const Value& value);