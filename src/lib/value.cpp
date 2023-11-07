// value.cpp

#include "value.h"

std::ostream& operator << (std::ostream& stream, const Value& value) {
    if (value.type == Value::DOUBLE)
    {
        stream << value.double_value;
    }
    else
    {
        stream << value.bool_value;
    }

    return stream;
}