#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/fun.hpp"
#include "snabl/fuses/move_ret.hpp"

namespace snabl::fuses {
  void fun(Fun *fun, PC start_pc, M &m) {
    while (move_ret(fun, start_pc, m));
  }

  PC fun_entry(Fun *fun, PC start_pc, M &m) {
    bool done = false;
    
    while (!done && start_pc < m.emit_pc) {
      Op op = m.ops[start_pc];
      
      switch (op_code(op)) {
      case OpCode::GOTO:
	start_pc = ops::goto_pc(op);
	cout << "Fusing " << fun << " entry: " << start_pc << endl;
	break;
      case OpCode::NOP:
	start_pc++;
	cout << "Fusing " << fun << " entry: " << start_pc << endl;
	break;
      default:
	done = true;
      }
    }
    
    return start_pc;
  }
}
