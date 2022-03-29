#ifndef SNABL_M_HPP
#define SNABL_M_HPP

#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "snabl/alloc.hpp"
#include "snabl/error.hpp"
#include "snabl/frame.hpp"
#include "snabl/fun.hpp"
#include "snabl/libs/abc.hpp"
#include "snabl/macro.hpp"
#include "snabl/op.hpp"
#include "snabl/path.hpp"
#include "snabl/scope.hpp"
#include "snabl/state.hpp"
#include "snabl/sym.hpp"

namespace snabl {
  struct M {
    bool trace = false;

    static const int OP_COUNT = 1024;
    static const int SLAB_SIZE = 64;

    Alloc<Frame, SLAB_SIZE> frame_alloc;
    Alloc<Fun, SLAB_SIZE> fun_alloc;
    Alloc<Macro, SLAB_SIZE> macro_alloc;
    Alloc<Scope, SLAB_SIZE> scope_alloc;
    Alloc<State, SLAB_SIZE> state_alloc;
    
    vector<Sym> syms;
    map<string, Sym> sym_lookup;

    vector<Type> types;
    
    State *state, *free_state;
    Frame *frame, *free_frame;
    Scope *scope, *free_scope;
        
    array<Op, OP_COUNT> ops;
    PC emit_pc = 0;

    Lib home_lib;
    optional<libs::Abc> abc_lib;
    Lib *lib;
    fs::path load_path;
    
    M();
    Op &emit(int n = 1, bool skip_trace = false);
    optional<Error> eval(PC start_pc);
    Sym sym(string name);
    void dump_ops(PC start_pc, ostream &out);
    optional<Error> include(fs::path path, Pos pos);
    optional<Error> use(Lib &lib, const vector<Sym> &syms, Pos pos);
    
    void ret_state(Reg reg1, optional<Reg> reg2 = nullopt) {
      State *old = end_state();
      state->set(reg1, move(old->get(reg1)));
      if (reg2 && *reg2 != reg1) { state->set(*reg2, move(old->get(*reg2))); }
      deref_state(old);
    }

    State *begin_state() {
      if (free_state) {
	State *new_state = free_state;
	free_state = free_state->outer;
	state = new(new_state) State(state);
      } else {
	state = state_alloc.make(state);
      }
      
      return state;
    }

    State *freeze_state() {
      State *s = begin_state();
      end_state();
      
      for (Reg i = Fun::ARG_COUNT+1; i < State::REG_COUNT; i++) {
	if (auto v = s->find(i); v) { s->set(i, *v); }
      }

      return s;
    }
    
    State *end_state() {
      State *old = state;
      state = state->outer;
      return old;
    }

    void deref_state(State *state) {
      if (!--state->ref_count) {
	if (state->outer) { deref_state(state->outer); }
	state->outer = free_state;
	free_state = state;
      }
    }

    Frame *begin_frame(Fun *target, Reg ret_reg, PC ret_pc) {
      if (free_frame) {
	Frame *new_frame = free_frame;
	free_frame = free_frame->outer;
	frame = new(new_frame) Frame(frame, target, ret_reg, ret_pc);
      } else {
	frame = frame_alloc.make(frame, target, ret_reg, ret_pc);
      }
      
      return frame;
    }

    Frame *end_frame() {
      if (frame->target->emit_reg != frame->ret_reg) {
	state->set(frame->ret_reg, move(state->get(frame->target->emit_reg)));
      }

      Frame *old = frame;
      frame = frame->outer;
      return old;
    }

    void deref_frame(Frame *frame) {
      if (!--frame->ref_count) {
	if (frame->outer) { deref_frame(frame->outer); }
	frame->outer = free_frame;
	free_frame = frame;
      }
    }

    Scope *begin_scope() {
      if (free_scope) {
	Scope *new_scope = free_scope;
	free_scope = free_scope->outer;
	scope = new(new_scope) Scope(scope);
      } else {
	scope = scope_alloc.make(scope);
      }
      
      return scope;
    }

    Scope *end_scope() {
      Scope *old = scope;
      scope = scope->outer;
      return old;
    }

    void deref_scope(Scope *scope) {
      if (!--scope->ref_count) {
	scope->outer = free_scope;
	free_scope = scope;
      }
    }
  };
}

#endif
