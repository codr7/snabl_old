#include "snabl/error.hpp"

namespace snabl {
  Error::Imp::Imp(Pos pos, string message): pos(pos), message(message) {}

  ostream &operator <<(ostream &out, Error val) {
    out << "Error in " << val.imp->pos << ':' << endl << val.imp->message;
    return out;
  }
}
