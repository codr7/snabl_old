#include "snabl/m.hpp"

namespace snabl {
  M::M():
    state(nullptr), free_state(nullptr),
    frame(nullptr), free_frame(nullptr),
    scope(nullptr), free_scope(nullptr) {
    begin_scope();
    begin_state(nullptr);
  }

  Op &M::emit(int n) {
    Op &op = ops[emit_pc++];
    emit_pc += n-1;
    return op;
  }

  Type::Id M::add_type(Type type) {
    Type::Id id = types.size();
    types.push_back(type);
    return id;
  }

  Sym M::sym(string name) {
    auto found = sym_lookup.find(name);
      
    if (found != sym_lookup.end()) {
      return found->second;
    }

    Sym s(syms.size(), name);
    syms.push_back(s);
    sym_lookup[name] = s;
    return s;
  }

  void M::dump_ops(PC start_pc, ostream &out) {
    for (int i = 0; i < emit_pc; i++) {
      Op op = ops[i];
      out << op << endl;
    }
  }
}
