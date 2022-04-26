#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/state.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
  int state(Fun *fun, M &m) {
    int n = 0;

    for (PC pc = fun->start_pc; pc < m.emit_pc;) {
      PC pc1 = pc;
      Op &op1 = m.ops[pc1];

      if (op_code(op1) == OpCode::STATE_BEG) {
	vector<PC> pcs;
	int count = ops::state_beg_count(op1);
	pc = ops::state_beg_next(op1);
	
	for (;;) {
	  pc = drill_pc(pc, m).first;
	  Op op2 = m.ops[pc];
	  
	  if (op_code(op2) == OpCode::STATE_BEG) {
	    pcs.push_back(pc);
	    count += ops::state_beg_count(op2);
	    pc = ops::state_beg_next(op2);
	  } else {
	    break;
	  }
	}

	if (pc != ops::state_beg_next(op1) || count != ops::state_beg_count(op1)) {
	  ops::STATE_BEG(op1, pc, count);
	  cout << "Fusing " << fun << " STATE: ";
	  op_trace(pc, cout, m);
	  n++;
	}

	for (PC pc: pcs) { ops::NOP(m.ops[pc]); }
      }

      pc += op_len(m.ops[pc]);
    }

    return n;
  }
}
