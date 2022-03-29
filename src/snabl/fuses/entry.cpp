#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/entry.hpp"

namespace snabl::fuses {
  int entry(Fun *fun, M &m) {
    int n = 0;
    bool done = false;
    PC pc = fun->start_pc;
    
    while (!done && pc < m.emit_pc) {
      Op op = m.ops[pc];
      
      switch (op_code(op)) {
      case OpCode::GOTO:
	pc = fun->start_pc = ops::goto_pc(op);
	cout << "Fusing " << fun << " ENTRY: ";
	op_trace(pc, cout, m);
	n++;
	break;
      case OpCode::NOP:
	pc = ++fun->start_pc;
	cout << "Fusing " << fun << " ENTRY: ";
	op_trace(pc, cout, m);
	n++;
	break;
      case OpCode::TRACE:
	pc++;
	break;
      default:
	done = true;
      }
    }

    return n;
  }
}
