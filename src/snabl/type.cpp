#include "snabl/m.hpp"
#include "snabl/type.hpp"

namespace snabl {
  static optional<Error> default_emit(Val val, Reg reg, Pos pos, M &m) {
    return Error(pos, "Emit not supported: ", val);
  }
  
  static bool default_is_true(Val val) {
    return true;
  }

  Type::Imp::Imp(Id id, Sym name): id(id), name(name) {
    methods.emit = default_emit;
    methods.is_true = default_is_true;
  }

  Type::Type(shared_ptr<const Imp> imp): imp(imp) {
  }
  
  Type::Type(Lib &lib, Sym name): imp(make_shared<const Imp>(lib.add_type(*this), name)) {
  }

  bool operator==(Type lhs, Type rhs) { return lhs.imp->id == rhs.imp->id; }

  bool operator!=(Type lhs, Type rhs) { return lhs.imp->id != rhs.imp->id; }

  ostream &operator <<(ostream &out, Type val) {
    out << val.imp->name;
    return out;
  }
}
