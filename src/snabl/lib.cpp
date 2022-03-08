#include "snabl/lib.hpp"
#include "snabl/m.hpp"

namespace snabl {
  Lib::Lib(M &m): m(m) {}

  Type::Id Lib::add_type(Type type) { return m.add_type(type); }

  void Lib::bind(Sym key, Type type, any data) {
    bindings[key] = Val(type, data);
  }
}
  
