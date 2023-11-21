#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <memory>
#include <vector>

class Value {
public:
    // Enum to represent different types
    enum Type {
        DOUBLE,
        ARRAY,
        BOOLEAN
    };

   
    Value(double val) : type(DOUBLE), double_value(val) {}   // Constructors
    Value(const std::vector<Value>& arr) : type(ARRAY), arr_value(std::make_shared<std::vector<Value>>(arr)) {}
    Value(bool val) : type(BOOLEAN), bool_value(val) {}

    // Copy constructor
    Value(const Value& other) : type(other.type) {
        if (type == DOUBLE) {
            double_value = other.double_value;
        }
        else if (type == ARRAY) {
            arr_value = std::make_shared<std::vector<Value>>(*(other.arr_value));
        }
        else {
            bool_value = other.bool_value;
        }
    }

   
    ~Value() = default;  // Destructor

    
    Value& operator=(const Value& other) { // Assignment operator
        if (this != &other) {
            type = other.type;
            if (type == DOUBLE) {
                double_value = other.double_value;
            }
            else if (type == ARRAY) {
                arr_value = std::make_shared<std::vector<Value>>(*(other.arr_value));
            }
            else {
                bool_value = other.bool_value;
            }
        }
        return *this;
    }

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& stream, const Value& value);

private:
    Type type;

    union {
        double double_value;
        std::shared_ptr<std::vector<Value>> arr_value;
        bool bool_value;
    };
};

#endif