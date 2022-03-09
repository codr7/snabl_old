#ifndef SNABL_FORM_HPP
#define SNABL_FORM_HPP

#include <string>
#include <optional>
#include <ostream>

#include "snabl/state.hpp"
#include "snabl/error.hpp"
#include "snabl/pos.hpp"

namespace snabl {
  using namespace std;

  struct M;
  
  struct Form {
    struct Imp {
      Imp(Pos pos);
      
      Pos pos;

      virtual void dump(ostream& out) const = 0;
      virtual optional<Error> emit(Reg reg, M &m) const = 0;
    };

    shared_ptr<const Imp> imp;
    
    Form(shared_ptr<const Imp> imp = nullptr);
    void dump(ostream& out);
    optional<Error> emit(Reg reg, M &m);
  };
}

#endif
