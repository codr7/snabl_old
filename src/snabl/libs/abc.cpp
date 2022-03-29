#include <iostream>

#include "snabl/forms/id.hpp"
#include "snabl/forms/lit.hpp"
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
  static Form pop_form(deque<Form> &in) {
    Form f = in.front();
    in.pop_front();
    return f;
  }

  static Sym pop_sym(deque<Form> &in) { return pop_form(in).as<forms::Id>().name; }

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

    bind_macro(m.sym("="), 2,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 if (auto err = pop_form(args).emit(1, m); err) { return err; }
		 if (auto err = pop_form(args).emit(2, m); err) { return err; }
		 ops::EQ(m.emit(), reg, 1, 2);
		 return nullopt;
	       });

    bind_macro(m.sym("bench"), 2,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 if (auto err = pop_form(args).emit(reg, m); err) { return err; }
		 Op &bench_op = m.emit();

		 for (const Form &f: args) {
		   if (auto err = f.emit(reg, m); err) { return err; }
		 }
		 
		 ops::STOP(m.emit());
		 ops::BENCH(bench_op, reg, reg, m.emit_pc);   
		 return nullopt;
	       });
    
    bind_macro(m.sym("dec"), 1,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 Sym id = args[0].as<forms::Id>().name;
		 auto val = m.scope->find(id);
		 if (!val) { return Error(pos, "Unknown id: ", id); }
		 if (val->type != m.abc_lib->reg_type) { return Error(pos, "Invalid target: ", id); }

		 types::Int::DataType delta = (args.size() == 1)
		   ? 1
		   : args[1].as<forms::Lit>()._val.as<types::Int::DataType>();

		 ops::DEC(m.emit(), reg, val->as<Reg>(), delta);
		 return nullopt;
	       });

    bind_fun(m.sym("dump"),
	     {{m.sym("x"), int_type}},
	     nil_type,
	     [](Fun &fun, PC ret_pc, M &m) {
	       cout << m.state->get(1) << endl;
	       return Fun::Result(ret_pc, nullopt);
	     });

    bind_macro(m.sym("fun:"), 4,
	       [this](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 Sym id = pop_sym(args);
		 deque<Form> arg_forms = pop_form(args).as<forms::Slice>().items;
		 vector<Fun::Arg> fargs;

		 while (!arg_forms.empty()) {
		   vector<Sym> arg_ids;
		   
		   while (!arg_forms.empty()) {
		     Sym arg_id = arg_forms.front().as<forms::Id>().name;
		     if (isupper(arg_id.imp->name[0])) { break; }
		     arg_ids.push_back(arg_id);
		     arg_forms.pop_front();
		   }
		     
		   Form type_form = pop_form(arg_forms);
		   Sym type_id = type_form.as<forms::Id>().name;
		   optional<Val> arg_type = m.scope->find(type_id);
		   if (!arg_type) { return Error(type_form.imp->pos, "Type not found: ", type_id); }
		   for (Sym arg_id: arg_ids) { fargs.emplace_back(arg_id, arg_type->as<Type>()); }
		 }
		 
		 optional<Val> ret_type = m.scope->find(pop_sym(args));
		 Fun *f = m.lib->bind_fun(id, fargs, ret_type->as<Type>(), nullptr);
		 m.scope->bind(id, fun_type, f);
		 return f->emit(args, reg, m);
	       });
    
    bind_macro(m.sym("if"), 2,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 Reg cond = m.scope->reg_count++;
		 if (auto err = args[0].emit(cond, m); err) { return err; }
		 Op &branch = m.emit();
		 PC if_pc = m.emit_pc;
		 if (auto err = args[1].emit(reg, m); err) { return err; }

		 if (args.size() > 2) {
		   Op &skip = m.emit();
		   ops::BRANCH(branch, cond, reg, if_pc, m.emit_pc);
		   if (auto err = args[2].emit(reg, m); err) { return err; }
		   ops::GOTO(skip, m.emit_pc);
		 } else {
		   ops::BRANCH(branch, cond, reg, if_pc, m.emit_pc);		   
		 }
		 
		 return nullopt;
	       });

    bind_macro(m.sym("let"), 1,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 deque<Form> bfs = pop_form(args).as<forms::Slice>().items;
		 ops::STATE_BEG(m.emit(), 1);
		 m.begin_scope();

		 while (!bfs.empty()) {
		   Sym id = pop_sym(bfs);
		   Form vf = pop_form(bfs);
		   Reg breg = m.scope->reg_count++;
		   m.scope->bind(id, m.abc_lib->reg_type, breg);
		   if (auto err = vf.emit(breg, m); err) { return err; }
		 }

		 for (const Form &f: args) {
		   if (auto err = f.emit(reg, m); err) { return err; }
		 }		 

		 m.deref_scope(m.end_scope());
		 ops::STATE_END(m.emit(), reg);
		 return nullopt;
	       });

    bind_macro(m.sym("one?"), 1,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 if (auto err = pop_form(args).emit(1, m); err) { return err; }
		 ops::ONE(m.emit(), reg, 1);
		 return nullopt;
	       });

    bind_macro(m.sym("test"), 1,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 if (auto err = pop_form(args).emit(1, m); err) { return err; }

		 ops::STATE_BEG(m.emit(), 1);
		 m.begin_scope();
		 
		 for (const Form &f: args) {
		   if (auto err = f.emit(2, m); err) { return err; }
		 }

		 m.deref_scope(m.end_scope());
		 ops::STATE_END(m.emit(), 2);
		 ops::TEST(m.emit(), 1, 2, reg);
		 return nullopt;
	       });

    bind_fun(m.sym("trace"),
	     {},
	     bool_type,
	     [this](Fun &fun, PC ret_pc, M &m) {
	       m.trace = !m.trace;
	       m.state->set(0, Val(bool_type, m.trace));
	       return Fun::Result(ret_pc, nullopt);
	     });
    
    bind_macro(m.sym("z?"), 1,
	       [](Macro &macro, deque<Form> args, Reg reg, Pos pos, M &m) -> Macro::Result {
		 if (auto err = pop_form(args).emit(1, m); err) { return err; }
		 ops::Z(m.emit(), reg, 1);
		 return nullopt;
	       });

    
    bind_fun(m.sym("+"),
	     {{m.sym("x"), int_type}, {m.sym("y"), int_type}},
	     int_type,
	     [this](Fun &fun, PC ret_pc, M &m) {	       
	       m.state->set(0, Val(int_type,
				   static_cast<types::Int::DataType>(m.state->get(1).as<types::Int::DataType>() +
								     m.state->get(2).as<types::Int::DataType>())));
			    
	       return Fun::Result(ret_pc, nullopt);
	     });

    bind_fun(m.sym("-"),
	     {{m.sym("x"), int_type}, {m.sym("y"), int_type}},
	     int_type,
	     [this](Fun &fun, PC ret_pc, M &m) {
	       m.state->set(0, Val(int_type,
				   static_cast<types::Int::DataType>(m.state->get(1).as<types::Int::DataType>() -
								     m.state->get(2).as<types::Int::DataType>())));
			    
	       return Fun::Result(ret_pc, nullopt);
	     });

    bind_fun(m.sym("<"),
	     {{m.sym("x"), int_type}, {m.sym("y"), int_type}},
	     bool_type,
	     [this](Fun &fun, PC ret_pc, M &m) {
	       m.state->set(0,
			    Val(bool_type,
				m.state->get(1).as<types::Int::DataType>() < m.state->get(2).as<types::Int::DataType>()));
	       
	       return Fun::Result(ret_pc, nullopt);
	     });
  }
}
