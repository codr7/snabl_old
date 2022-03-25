#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/move_ret.hpp"

namespace snabl::fuses {
  int move_ret(Fun *fun, PC start_pc, M &m) {
    int n = 0;
    bool done = false;
    vector<PC> ret_pcs;
    
    for (PC pc = start_pc; !done && pc < m.emit_pc;) {
      Op op = m.ops[pc];
      
      switch (op_code(op)) {
      case OpCode::NOP:
	ret_pcs.push_back(pc);
	pc++;
	break;
      case OpCode::GOTO:
	ret_pcs.push_back(pc);
	pc = ops::goto_pc(op);
	break;
      case OpCode::RET:
	for (PC rpc: ret_pcs) {
	  cout << "Fusing " << fun << " MOVE RET: " << rpc << endl;
	  ops::RET(m.ops[rpc]);
	  n++;
	}
	
	done = true;
	break;
      default:
	ret_pcs.clear();
	pc += op_len(op);
      }
    }

    return n;
  }
}
