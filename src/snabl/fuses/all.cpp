#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/all.hpp"
#include "snabl/fuses/entry.hpp"
#include "snabl/fuses/ret.hpp"
#include "snabl/fuses/tail_call.hpp"

namespace snabl::fuses {
  void all(Fun *fun, M &m) {
    while (entry(fun, m) ||
	   ret(fun, m) ||
	   tail_call(fun, m));
  }
}
