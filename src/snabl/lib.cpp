#include "snabl/lib.hpp"
#include "snabl/m.hpp"

namespace snabl {
  Lib::Lib(M &m, Sym name): m(m), name(name) {}

  Type::Id Lib::type_id() const { return m.types.size(); }

  void Lib::add_type(Type type) { m.types.push_back(type); }

  optional<Val> Lib::find(Sym name) {
    auto found = bindings.find(name);
    if (found == bindings.end()) { return nullopt; }
    return found->second;
  }

  void Lib::bind(Sym key, Type type, any data) { bindings[key] = Val(type, data); }

  Fun *Lib::bind_fun(Sym name, const vector<Fun::Arg> &args, Type ret_type, Fun::Body body) {
    Fun *fun = m.fun_alloc.make(name, args, ret_type, body);
    bind(name, m.abc_lib->fun_type, fun);
    return fun;
  }

  Macro *Lib::bind_macro(Sym name, int arg_count, Macro::Body body) {
    Macro *macro = m.macro_alloc.make(name, arg_count, body);
    bind(name, m.abc_lib->macro_type, macro);
    return macro;
  }
}
  
