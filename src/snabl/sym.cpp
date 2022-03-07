#include "snabl/sym.hpp"

namespace snabl {
  Sym::Imp::Imp(Id id, string name): id(id), name(name) {}

  Sym::Sym() {
  }

  Sym::Sym(Id id, string name): imp(make_shared<const Imp>(id, name)) {}

  bool operator==(Sym lhs, Sym rhs) { return lhs.imp->id == rhs.imp->id; }

  bool operator<(Sym lhs, Sym rhs) { return lhs.imp->id < rhs.imp->id; }

  ostream &operator <<(ostream &out, Sym val) {
    out << val.imp->name;
    return out;
  }
}
