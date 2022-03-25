#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/entry.hpp"

namespace snabl::fuses {
  int entry(Fun *fun, M &m) {
    int n = 0;
    bool done = false;
    
    while (!done && fun->start_pc < m.emit_pc) {
      Op op = m.ops[fun->start_pc];
      
      switch (op_code(op)) {
      case OpCode::GOTO:
	fun->start_pc = ops::goto_pc(op);
	cout << "Fusing " << fun << " ENTRY: " << fun->start_pc << endl;
	n++;
	break;
      case OpCode::NOP:
	fun->start_pc++;
	cout << "Fusing " << fun << " ENTRY: " << fun->start_pc << endl;
	n++;
	break;
      default:
	done = true;
      }
    }

    return n;
  }
}
