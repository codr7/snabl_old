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
      Pos pos;

      Imp(Pos pos);
      virtual ~Imp();
      virtual void dump(ostream& out) const = 0;
      virtual optional<Error> emit(Reg reg, M &m) const = 0;
      virtual optional<Error> quote(Reg reg, M &m) const;
      virtual optional<Val> val(M &m) const;
    };

    shared_ptr<const Imp> imp;
    
    Form(shared_ptr<const Imp> imp = nullptr);
    void dump(ostream& out) const;
    optional<Error> emit(Reg reg, M &m) const;
    optional<Error> quote(Reg reg, M &m) const;
    optional<Val> val(M &m) const;

    template <typename T>
    const typename T::Imp &as() const { return *static_cast<const typename T::Imp *>(imp.get()); }
  };

  ostream &operator<<(ostream &out, Form val);
}

#endif
