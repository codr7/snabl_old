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
	PC else_pc = drill_pc(ops::branch_else(b), m);

	if (else_pc != ops::branch_else(b)) {
	  cout << "Fusing " << fun << " BRANCH ELSE: " << else_pc << endl;
	  n++;
	}

	ops::BRANCH(b, ops::branch_cond(b), ops::branch_reg(b), else_pc);
      }

      pc += op_len(b);
    }

    return n;
  }
}
