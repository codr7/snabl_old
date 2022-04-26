#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/moves.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
  int moves(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if (op_code(op1) == OpCode::MOVE) {
	int len = 1;
	vector<PC> pcs;
	
	for (;;) {
	  PC pc2 = drill_pc(pc1+1, m).back();
	  Op op2 = m.ops[pc2];
	  
	  if (op_code(op2) == OpCode::COPY &&
	      ops::copy_src(op2) == ops::copy_src(op1)+1 &&
	      ops::copy_dst(op2) == ops::copy_dst(op1)+1) {
	    pcs.push_back(pc2++);
	    len++;
	  } else {
	    break;
	  }

	  pc1 = pc2;
	}

	if (len > 1) {
	  ops::COPYS(op1, ops::copy_dst(op1), ops::copy_src(op1), len);
	  
	  for (PC pc: pcs) {
	    cout << "Fusing " << fun << " COPYS: ";
	    op_trace(pc, cout, m);
	    ops::NOP(m.ops[pc]);
	  }
	}
      }

      pc1 += op_len(op1);
    }

    return n;
  }
}
