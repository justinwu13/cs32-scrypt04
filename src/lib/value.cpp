// value.cpp

#include "value.h"


Value::~Value() {
    if (type == ARRAY) {
        arr_value.reset();
    }
    else if (type == FUNC) {
        func_value.reset();
    }
}

Value::Value(Value const& value) {
    type = value.type;
    if (value.type == BOOL) {
        bool_value = value.bool_value;
    }
    else if (value.type == DOUBLE) {
        double_value = value.double_value;
    }
    else if (value.type == FUNC) {
        func_value = value.func_value;
    }
    else if (value.type == ARRAY) {
        arr_value = value.arr_value;
    }
    else { // NULLVALUE
        // do nothing
    }
    }

Value& Value::operator= (const Value& value) {
    type = value.type;
    if (value.type == BOOL) {
        bool_value = value.bool_value;
    }
    else if (value.type == DOUBLE) {
        double_value = value.double_value;
    }
    else if (value.type == FUNC) {
        func_value = value.func_value;
    }
    else if (value.type == ARRAY) {
        arr_value = value.arr_value;
    }
    else { // NULLVALUE
        // do nothing
    }

    return *this;
}

bool Value::operator== (const Value& value) const { 
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

std::ostream& operator << (std::ostream& stream, const Value& value) {
    if (value.type == Value::DOUBLE) {
        stream << value.double_value;
    }
    else if (value.type == Value::FUNC) {
        stream << value.func_value->getName();
    }
    else if (value.type == Value::ARRAY) {
        stream << '[';
        for (unsigned int i = 0; i < value.arr_value->size(); i++) {
            stream << value.arr_value->at(i);
            if (i < value.arr_value->size() - 1) { // add commas after all elements except the last one
                stream << ", ";
            }
        }
        stream << ']';
    }
    else if (value.type == Value::BOOL) {
        stream << value.bool_value;
    }
    else {
        stream << "null";
    }

    return stream;
}