#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/branch.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {  
  int branch(Fun *fun, M &m) {
    int n = 0;

    for (PC pc = fun->start_pc; pc < m.emit_pc;) {
      Op &b = m.ops[pc];

      if (op_code(b) == OpCode::BRANCH) {
	PC if_pc = drill_pc(ops::branch_if_pc(b), m).back();
	bool changed = false;
	
	if (if_pc != ops::branch_if_pc(b)) {
	  cout << "Fusing " << fun << " BRANCH IF: ";
	  op_trace(if_pc, cout, m);
	  changed = true;
	  n++;
	}

	PC else_pc = drill_pc(ops::branch_else_pc(b), m).back();

	if (else_pc != ops::branch_else_pc(b)) {
	  cout << "Fusing " << fun << " BRANCH ELSE: ";
	  op_trace(else_pc, cout, m);
	  changed = true;
	  n++;
	}

	if (changed) { ops::BRANCH(b, ops::branch_cond(b), ops::branch_reg(b), if_pc, else_pc); }
      }

      pc += op_len(b);
    }

    return n;
  }
}
