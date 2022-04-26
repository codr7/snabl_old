#include "snabl/m.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
  pair<PC, vector<PC>> drill_pc(PC pc, M &m) {
   vector<PC> out;
   bool done = false;
   
    while (pc < m.emit_pc && !done) {
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
	done = true;
      }
    }

    return make_pair(pc, out);
  }
}
