#include "snabl/m.hpp"

namespace snabl {
  M::M():
    env(nullptr), free_env(nullptr),
    frame(nullptr), free_frame(nullptr),
    scope(nullptr), free_scope(nullptr) {
    begin_scope();
    begin_env();
  }

  Op &M::emit() {
    return ops[emit_pc++];
  }
}
