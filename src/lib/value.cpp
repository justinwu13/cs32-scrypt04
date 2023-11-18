// value.cpp

#include "value.h"

std::ostream& operator << (std::ostream& stream, const Value& value) {
    if (value.type == Value::DOUBLE)
    {
        stream << value.double_value;
    }
    else if (value.type == Value::ARRAY)
    {
        stream << '[';
        for (unsigned int i = 0; i < value.arr_value.size(); i++) {
            stream << *value.arr_value.at(i);
            if (i < value.arr_value.size() - 1) { // add commas after all elements except the last one
                stream << ", ";
            }
        }
        stream << ']';
    }
    else
    {
        stream << value.bool_value;
    }

    return stream;
}