#include "snabl/error.hpp"

namespace snabl {
  Error::Imp::Imp(Pos pos, string message): pos(pos), message(message) {
  }
}
