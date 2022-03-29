#include <fstream>

#include "snabl/defer.hpp"
#include "snabl/m.hpp"
#include "snabl/reader.hpp"

namespace snabl {
  M::M():
    state(nullptr), free_state(nullptr),
    frame(nullptr), free_frame(nullptr),
    scope(nullptr), free_scope(nullptr),
    home_lib(*this, sym("home")), lib(&home_lib) {
    begin_scope();
    begin_state();
  }

  Op &M::emit(int n, bool skip_trace) {
    if (trace && !skip_trace) { ops::TRACE(emit(1, true)); }
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

  optional<Error> M::include(fs::path path, Pos pos) {
    auto in_path(path.is_absolute() ? path : load_path/path);
    ifstream in(in_path);
    if (in.fail()) { return Error(pos, "File not found: ", in_path); }
    PC start_pc = emit_pc;

    for (;;) {
      auto [f, err] = read_form(in, pos, *this);
      if (err) { return err; }
      if (!f) { break; }
      if (err = f->emit(0, *this); err) { return err;}      
    }

    ops::STOP(emit());
    auto prev_path(load_path);
    load_path = path.parent_path();
    auto restore_load_path(defer([&]() { load_path = prev_path; }));
    return eval(start_pc);
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
