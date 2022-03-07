#include "snabl/m.hpp"
#include "snabl/type.hpp"

namespace snabl {
  Type::Imp::Imp(Id id, Sym name): id(id), name(name) {
  }

  Type::Type(shared_ptr<const Imp> imp): imp(imp) {
  }
  
  Type::Type(M *m, Sym name): imp(make_shared<const Imp>(m->add_type(*this), name)) {
  }

  bool operator==(Type lhs, Type rhs) { return lhs.imp->id == rhs.imp->id; }

  bool operator<(Type lhs, Type rhs) { return lhs.imp->id < rhs.imp->id; }

  ostream &operator <<(ostream &out, Type val) {
    out << val.imp->name;
    return out;
  }
}
