#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/fun.hpp"
#include "snabl/fuses/move_ret.hpp"

namespace snabl::fuses {
  void fun(Fun *fun, M &m) {
    while (move_ret(fun, m) ||
	   fun_entry(fun, m) ||
	   fun_tail(fun, m));
  }

  int fun_entry(Fun *fun, M &m) {
    int n = 0;
    bool done = false;
    
    while (!done && fun->start_pc < m.emit_pc) {
      Op op = m.ops[fun->start_pc];
      
      switch (op_code(op)) {
      case OpCode::GOTO:
	fun->start_pc = ops::goto_pc(op);
	cout << "Fusing " << fun << " entry: " << fun->start_pc << endl;
	n++;
	break;
      case OpCode::NOP:
	fun->start_pc++;
	cout << "Fusing " << fun << " entry: " << fun->start_pc << endl;
	n++;
	break;
      default:
	done = true;
      }
    }

    return n;
  }

  int fun_tail(Fun *fun, M &m) {
    int n = 0;
    bool done = false;
    PC pc = fun->start_pc;
    vector<PC> rpcs;
    
    while (!done && pc < m.emit_pc) {
      Op &op = m.ops[pc];

      switch (op_code(op)) {
      case OpCode::CALLI1:
	if (ops::calli1_target(op) == fun) { rpcs.push_back(pc); }
	pc++;
	break;
      case OpCode::GOTO:
	if (rpcs.empty()) {
	  pc++;
	} else {
	  pc = ops::goto_pc(op);
	}
	
	break;
      case OpCode::NOP:
	pc++;
	break;
      case OpCode::RET:
	for (PC rpc: rpcs) {
	  cout << "Fusing " << fun << " FUN TAIL: " << rpc << endl;
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
