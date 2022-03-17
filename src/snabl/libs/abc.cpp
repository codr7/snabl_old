#include "snabl/libs/abc.hpp"
#include "snabl/m.hpp"

namespace snabl::libs {
  Abc::Abc(M &m):
    Lib(m, m.sym("abc")),
    bool_type(*this, m.sym("Bool")),
    fun_type(*this, m.sym("Fun")),
    int_type(*this, m.sym("Int")),
    macro_type(*this, m.sym("Macro")),
    meta_type(*this, m.sym("Meta")),
    nil_type(*this, m.sym("Nil")),
    reg_type(*this, m.sym("Reg")) {
    bind(m.sym("Bool"), meta_type, bool_type);
    bind(m.sym("Fun"), meta_type, fun_type);
    bind(m.sym("Int"), meta_type, int_type);
    bind(m.sym("Macro"), meta_type, macro_type);
    bind(m.sym("Meta"), meta_type, meta_type);
    bind(m.sym("Nil"), meta_type, nil_type);
    bind(m.sym("Reg"), meta_type, reg_type);

    bind(m.sym("_"), nil_type, nullptr);
    bind(m.sym("T"), bool_type, true);
    bind(m.sym("F"), bool_type, false);
    
    bind_fun(m.sym("debug"),
	     {},
	     bool_type,
	     [this](Fun &fun, Reg ret_reg, PC ret_pc, M &m) {
	       m.debug = !m.debug;
	       m.state->regs[ret_reg] = Val(bool_type, m.debug);
	       return Fun::Result(ret_pc, nullopt);
	     });

    bind_fun(m.sym("+"),
	     {{m.sym("x"), int_type}, {m.sym("y"), int_type}},
	     int_type,
	     [this](Fun &fun, Reg ret_reg, PC ret_pc, M &m) {
	       Val *rs = m.state->regs.begin();
	       rs[ret_reg] = Val(int_type, rs[1].as<types::Int::DataType>() + rs[2].as<types::Int::DataType>());
	       return Fun::Result(ret_pc, nullopt);
	     });

    bind_fun(m.sym("-"),
	     {{m.sym("x"), int_type}, {m.sym("y"), int_type}},
	     int_type,
	     [this](Fun &fun, Reg ret_reg, PC ret_pc, M &m) {
	       Val *rs = m.state->regs.begin();
	       rs[ret_reg] = Val(int_type, rs[1].as<types::Int::DataType>() - rs[2].as<types::Int::DataType>());
	       return Fun::Result(ret_pc, nullopt);
	     });

    bind_fun(m.sym("<"),
	     {{m.sym("x"), int_type}, {m.sym("y"), int_type}},
	     bool_type,
	     [this](Fun &fun, Reg ret_reg, PC ret_pc, M &m) {
	       Val *rs = m.state->regs.begin();
	       rs[ret_reg] = Val(int_type, rs[1].as<types::Int::DataType>() < rs[2].as<types::Int::DataType>());
	       return Fun::Result(ret_pc, nullopt);
	     });
  }
}
