#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/all.hpp"
#include "snabl/fuses/branch.hpp"
#include "snabl/fuses/circle.hpp"
#include "snabl/fuses/copys.hpp"
#include "snabl/fuses/entry.hpp"
#include "snabl/fuses/ret.hpp"
#include "snabl/fuses/state.hpp"
#include "snabl/fuses/tail_call.hpp"

namespace snabl::fuses {
  void all(Fun *fun, M &m) {
    while (branch(fun, m) ||
	   circle(fun, m) ||
	   copys(fun, m) ||
	   entry(fun, m) ||
	   ret(fun, m) ||
	   state(fun, m) ||
	   tail_call(fun, m));
  }
}
