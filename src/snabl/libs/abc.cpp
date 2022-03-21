#include <iostream>

#include "snabl/forms/id.hpp"
#include "snabl/forms/slice.hpp"
#include "snabl/libs/abc.hpp"
#include "snabl/m.hpp"

#include "snabl/types/bool.hpp"
#include "snabl/types/fun.hpp"
#include "snabl/types/int.hpp"
#include "snabl/types/macro.hpp"
#include "snabl/types/meta.hpp"
#include "snabl/types/nil.hpp"
#include "snabl/types/reg.hpp"

namespace snabl::libs {
  Abc::Abc(M &m):
    Lib(m, m.sym("abc")),
    bool_type(*this, make_shared<types::Bool>(type_id(), m.sym("Bool"))),
    fun_type(*this, make_shared<types::Fun>(type_id(), m.sym("Fun"))),
    int_type(*this, make_shared<types::Int>(type_id(), m.sym("Int"))),
    macro_type(*this, make_shared<types::Macro>(type_id(), m.sym("Macro"))),
    meta_type(*this, make_shared<types::Meta>(type_id(), m.sym("Meta"))),
    nil_type(*this, make_shared<types::Nil>(type_id(), m.sym("Nil"))),
    reg_type(*this, make_shared<types::Reg>(type_id(), m.sym("Reg"))) {
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

    bind_macro(m.sym("fun:"), 4,
	       [this](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 Sym id = args.front().as<forms::Id>().name;
		 args.pop_front();

		 deque<Form> arg_forms = args.front().as<forms::Slice>().items;
		 args.pop_front();
		 vector<Fun::Arg> fargs;
		 
		 Sym ret_type_id = args.front().as<forms::Id>().name;
		 args.pop_front();
		 optional<Val> ret_type = m.scope->find(ret_type_id);

		 Fun *f = m.lib->bind_fun(id, fargs, ret_type->as<Type>(), nullptr);
		 m.scope->bind(id, fun_type, f);
		 return f->emit(args, m);
	       });
    
    bind_macro(m.sym("if"), 2,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 Reg cond = m.scope->reg_count++;
		 if (auto err = args[0].emit(cond, m); err) { return err; }
		 Op &branch = m.emit();
		 if (auto err = args[1].emit(reg, m); err) { return err; }

		 if (args.size() > 2) {
		   Op &skip = m.emit();
		   ops::BRANCH(branch, cond, reg, m.emit_pc);
		   if (auto err = args[2].emit(reg, m); err) { return err; }
		   ops::GOTO(skip, m.emit_pc);
		 } else {
		   ops::BRANCH(branch, cond, reg, m.emit_pc);		   
		 }
		 
		 return nullopt;
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
