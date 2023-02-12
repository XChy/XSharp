#include "XSharp/XString.h"

struct Symbol {
    XString name;
    enum Type { Var, Class, Function };
};
