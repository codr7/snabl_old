#include "snabl/m.hpp"

namespace snabl {
  M::M():
    state(nullptr), free_state(nullptr),
    frame(nullptr), free_frame(nullptr),
    scope(nullptr), free_scope(nullptr),
    home_lib(*this, sym("home")), lib(&home_lib) {
    begin_scope();
    begin_state(nullptr);
  }

  Op &M::emit(int n) {
    Op &op = ops[emit_pc++];
    emit_pc += n-1;
    return op;
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

  optional<Error> M::use(Lib &lib, const vector<Sym> &syms, Pos pos) {
    if (syms.empty()) {
      for (auto &b: lib.bindings) { scope->bind(b.first, b.second.type, b.second.data); }
    }
    
    for (Sym s: syms) {
      optional<Val> v = lib.find(s);
      if (!v) { return Error(pos, "Unknown id: ", lib.name, '.', s); }
      scope->bind(s, v->type, v->data);
    }

    return nullopt;
  }
}
