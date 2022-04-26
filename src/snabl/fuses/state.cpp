#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/state.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
  int state(Fun *fun, M &m) {
    int n = 0;

    for (PC pc = fun->start_pc; pc < m.emit_pc;) {
      Op &op1 = m.ops[pc];

      if (op_code(op1) == OpCode::STATE_BEG) {
	vector<PC> pcs;
	int count = ops::state_beg_count(op1);

	for (;;) {
	  pc = drill_pc(pc+1, m).first;
	  Op op2 = m.ops[pc];
	  
	  if (op_code(op2) == OpCode::STATE_BEG) {
	    pcs.push_back(pc);
	    count += ops::state_beg_count(op2);
	  } else {
	    break;
	  }
	}

	if (!pcs.empty()) {
	  ops::STATE_BEG(op1, ops::state_beg_next(m.ops[pcs.back()]), count);	  

	  for (PC pc: pcs) {
	    cout << "Fusing " << fun << " STATE: ";
	    op_trace(pc, cout, m);
	    Op &op = m.ops[pc];
	    ops::NOP(op);
	    n++;
	  }
	}
      }

      pc += op_len(m.ops[pc]);
    }

    return n;
  }
}
