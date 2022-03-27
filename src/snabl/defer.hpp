#ifndef SNABL_DEFER_HPP
#define SNABL_DEFER_HPP

namespace snabl {
  template <typename T>
  struct Defer {
    T body;
    Defer(T body) : body(body) {}
    ~Defer() { body(); }
  };

  template <typename T>
  Defer<T> defer(T body) { return Defer<T>(body); }
}

#endif
