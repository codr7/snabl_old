#ifndef SNABL_M_HPP
#define SNABL_M_HPP

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "snabl/alloc.hpp"
#include "snabl/state.hpp"
#include "snabl/error.hpp"
#include "snabl/frame.hpp"
#include "snabl/libs/abc.hpp"
#include "snabl/op.hpp"
#include "snabl/scope.hpp"
#include "snabl/sym.hpp"

namespace snabl {
  struct M {
    static const int OP_COUNT = 1024;
    static const int STATE_SLAB_SIZE = 32;
    static const int FRAME_SLAB_SIZE = 32;
    static const int SCOPE_SLAB_SIZE = 32;

    Alloc<State, STATE_SLAB_SIZE> state_alloc;
    Alloc<Frame, FRAME_SLAB_SIZE> frame_alloc;
    Alloc<Scope, SCOPE_SLAB_SIZE> scope_alloc;
    
    vector<Sym> syms;
    map<string, Sym> sym_lookup;

    vector<Type> types;
    
    State *state, *free_state;
    Frame *frame, *free_frame;
    Scope *scope, *free_scope;
        
    array<Op, OP_COUNT> ops;
    PC emit_pc = 0;

    optional<libs::Abc> abc_lib;

    M();
    Op &emit(int n = 1);
    optional<Error> eval(PC start_pc);
    Type::Id add_type(Type type);
    Sym sym(string name);
    
    State *begin_state(State *outer) {
      if (free_state) {
	State *new_state = free_state;
	free_state = free_state->outer;
	state = new(new_state) State(state);
      } else {
	state = state_alloc.make(outer);
      }
      
      return state;
    }

    State *end_state() {
      State *old = state;
      if (state->outer) { state->outer->regs[0] = state->regs[0]; }
      state = state->outer;
      return old;
    }

    void deref_state(State *state) {
      if (!state->ref_count--) {
	state->outer = free_state;
	free_state = state;
      }
    }

    Frame *begin_frame(PC ret_pc) {
      if (free_frame) {
	Frame *new_frame = free_frame;
	free_frame = free_frame->outer;
	frame = new(new_frame) Frame(frame, ret_pc);
      } else {
	frame = frame_alloc.make(frame, ret_pc);
      }
      
      return frame;
    }

    Frame *end_frame() {
      Frame *old = frame;
      frame = frame->outer;
      return old;
    }

    void deref_frame(Frame *frame) {
      if (!frame->ref_count--) {
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
      if (!scope->ref_count--) {
	scope->outer = free_scope;
	free_scope = scope;
      }
    }
  };
}

#endif
