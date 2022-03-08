#ifndef SNABL_ERROR_H
#define SNABL_ERROR_H

#include <memory>
#include <sstream>
#include <string>

#include "snabl/pos.hpp"

namespace snabl {
  using namespace std;

  template <typename Arg, typename...Args>
  string str(Arg &&arg, Args &&...args) {
    stringstream buf;
    buf << arg;
    ((buf << std::forward<Args>(args)), ...);
    return buf.str();
  }

  struct Error {
    struct Imp {
      Pos pos;
      string message;

      Imp(Pos pos, string message);
    };

    shared_ptr<const Imp> imp;

    template <typename...Args>
    Error(Pos pos, string message, Args &&...args):
      imp(make_shared<const Imp>(pos, str("Error in ", pos, ":\n", message, forward<Args>(args)...))) {}
  };
}

#endif
