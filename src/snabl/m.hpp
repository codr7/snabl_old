#ifndef SNABL_M_HPP
#define SNABL_M_HPP

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "snabl/alloc.hpp"
#include "snabl/env.hpp"
#include "snabl/error.hpp"
#include "snabl/frame.hpp"
#include "snabl/libs/abc.hpp"
#include "snabl/op.hpp"
#include "snabl/scope.hpp"
#include "snabl/sym.hpp"

namespace snabl {
  struct M {
    static const int OP_COUNT = 1024;
    static const int ENV_SLAB_SIZE = 32;
    static const int FRAME_SLAB_SIZE = 32;
    static const int SCOPE_SLAB_SIZE = 32;

    Alloc<Env, ENV_SLAB_SIZE> env_alloc;
    Alloc<Frame, FRAME_SLAB_SIZE> frame_alloc;
    Alloc<Scope, SCOPE_SLAB_SIZE> scope_alloc;
    
    vector<Sym> syms;
    map<string, Sym> sym_lookup;

    vector<Type> types;
    
    Env *env, *free_env;
    Frame *frame, *free_frame;
    Scope *scope, *free_scope;
        
    array<Op, OP_COUNT> ops;
    PC emit_pc = 0;

    optional<libs::Abc> abc_lib;

    M();
    Op &emit(int n = 1);
    optional<Error> eval(PC start_pc);

    Sym sym(string name) {
      auto found = sym_lookup.find(name);
      
      if (found != sym_lookup.end()) {
	return found->second;
      }

      Sym s(syms.size(), name);
      syms.push_back(s);
      sym_lookup[name] = s;
      return s;
    }

    Type::Id add_type(Type type) {
      Type::Id id = types.size();
      types.push_back(type);
      return id;
    }
    
    Env *begin_env() {
      if (free_env) {
	Env *new_env = free_env;
	free_env = free_env->outer;
	env = new(new_env) Env(env);
      } else {
	env = env_alloc.make(env);
      }
      
      return env;
    }

    Env *end_env() {
      Env *old = env;
      if (env->outer) { env->outer->regs[0] = env->regs[0]; }
      env = env->outer;
      return old;
    }

    void deref_env(Env *env) {
      if (!env->ref_count--) {
	env->outer = free_env;
	free_env = env;
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
