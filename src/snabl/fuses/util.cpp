#include "snabl/m.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
 vector<PC> drill_pc(PC pc, M &m) {
   vector<PC> out;
   out.push_back(pc);
   
    while (pc < m.emit_pc) {
      Op op = m.ops[pc];

      switch (op_code(op)) {
      case OpCode::NOP:
	out.push_back(pc++);
	break;
      case OpCode::GOTO:
	out.push_back(pc);
	pc = ops::goto_pc(op);
	break;
      case OpCode::TRACE:
	pc++;
	break;
      default:
	return out;
      }
    }

    return out;
  }
}
