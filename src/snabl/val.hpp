#ifndef SNABL_VAL_HPP
#define SNABL_VAL_HPP

#include <any>

#include "snabl/type.hpp"

namespace snabl {
  using namespace std;
  
  struct Val {
    Type type;
    any data;

    Val() {}
    Val(Type type, any data): type(type), data(data) {}

    template <typename T>
    T as() { return any_cast<T>(data); }
  };

  ostream &operator <<(ostream &out, Val val);
}

#endif
