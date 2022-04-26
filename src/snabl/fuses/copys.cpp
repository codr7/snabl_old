#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/copys.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
  int copys(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc = fun->start_pc; pc < m.emit_pc;) {
      Op &op1 = m.ops[pc];

      if (op_code(op1) == OpCode::COPY) {
	vector<PC> pcs;
	
	for (;;) {
	  pc = drill_pc(pc+1, m).first;
	  Op op2 = m.ops[pc];
	  
	  if (op_code(op2) == OpCode::COPY &&
	      ops::copy_src(op2) == ops::copy_src(op1)+1 &&
	      ops::copy_dst(op2) == ops::copy_dst(op1)+1) {
	    pcs.push_back(pc);
	  } else {
	    break;
	  }
	}

	if (!pcs.empty()) {
	  ops::COPYS(op1, ops::copy_dst(op1), ops::copy_src(op1), pcs.size());
	  
	  for (PC pc: pcs) {
	    cout << "Fusing " << fun << " COPYS: ";
	    op_trace(pc, cout, m);
	    ops::NOP(m.ops[pc]);
	  }
	}
      }

      pc += op_len(m.ops[pc]);
    }

    return n;
  }
}
