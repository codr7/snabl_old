#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/tail_call.hpp"

namespace snabl::fuses {
  int tail_call(Fun *fun, M &m) {
    int n = 0;
    bool done = false;
    PC pc = fun->start_pc;
    vector<PC> rpcs;
    
    while (!done && pc < m.emit_pc) {
      Op &op = m.ops[pc];

      switch (op_code(op)) {
      case OpCode::CALLI1:
	if (ops::calli1_target(op) == fun) {
	  rpcs.push_back(pc);
	} else {
	  rpcs.clear();
	}
	
	pc++;
	break;
      case OpCode::NOP:
      case OpCode::TRACE:
	pc++;
	break;
      case OpCode::RET:
	for (PC rpc: rpcs) {
	  cout << "Fusing " << fun << " TAIL CALL: ";
	  op_trace(rpc, cout, m);
	  ops::REC(m.ops[rpc]);
	  n++;
	}

	rpcs.clear();
	pc++;
	break;
      default:
	rpcs.clear();
	pc += op_len(op);
	break;
      }
    }

    return n;
  }
}
