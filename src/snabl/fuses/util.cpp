#include "snabl/m.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
  PC drill_pc(PC pc, M &m) {
    while (pc < m.emit_pc) {
      Op op = m.ops[pc];

      switch (op_code(op)) {
      case OpCode::NOP:
	pc++;
	break;
      case OpCode::GOTO:
	pc = ops::goto_pc(op);
	break;
      default:
	return pc;
      }
    }

    return pc;
  }
}
