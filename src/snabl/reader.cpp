#include <deque>
#include <vector>

#include "snabl/forms/call.hpp"
#include "snabl/forms/id.hpp"
#include "snabl/forms/lit.hpp"
#include "snabl/forms/slice.hpp"
#include "snabl/m.hpp"
#include "snabl/reader.hpp"

namespace snabl {
  ReadResult read_form(istream &in, Pos &pos, M &m) {
    const vector<Reader> readers {read_ws, read_int, read_call, read_slice, read_id};
    
    for (Reader r: readers) {
      if (auto [f, err] = r(in, pos, m); err) {
	return ReadResult(nullopt, err);
      } else if (f) {
	return ReadResult(f, nullopt);
      }
    }

    return ReadResult(nullopt, nullopt);
  }

  ReadResult read_call(istream &in, Pos &pos, M &m) {
    char c = 0;
    if (!in.get(c)) { ReadResult(nullopt, nullopt); }

    if (c != '(') {
      in.unget();
      return ReadResult(nullopt, nullopt);
    }

    Pos fpos = pos;
    pos.column++;
    deque<Form> args;
    
    for (;;) {
      read_ws(in, pos, m);

      if (in.get(c)) {
	if (c == ')') { break; }
	in.unget();
      } else {
	break;
      }
      
      auto [bf, err] = read_form(in, pos, m);
      if (err) { return ReadResult(nullopt, err); }
      if (!bf) { break; }
      args.push_back(*bf);
    }

    if (c != ')') { return ReadResult(nullopt, Error(fpos, "Open call")); }
    pos.column++;
    Form target = args.front();
    args.pop_front();
    return ReadResult(forms::Call(fpos, target, args), nullopt);
  }

  ReadResult read_id(istream &in, Pos &pos, M &m) {
    Pos fpos(pos);
    stringstream buf;
    char c = 0;
    
    while (in.get(c)) {
      if (!isgraph(c) || (c == '|' && buf.tellp()) ||  c == '(' || c == ')' || c == '[' || c == ']') {
	in.unget();
	break;
      }

      buf << c;
      pos.column++;
    }

    if (!buf.tellp()) {
      return ReadResult(nullopt, nullopt);
    }
    
    string name = buf.str();
    return ReadResult(forms::Id(fpos,m.sym(buf.str())), nullopt);
  }

  static pair<types::Int::DataType, optional<Error>> read_int_base(istream &in, Pos &pos, int base) {
    types::Int::DataType v(0);
    
    static map<char, int8_t> char_vals = {
      {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
      {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14},
      {'f', 15}
    };
    
    auto ci(char_vals.end());
    char c = 0;
    
    while (in.get(c)) {
      if ((ci = char_vals.find(c)) == char_vals.end()) { break; }
      auto cv(ci->second);
      if (cv >= base) { return pair<types::Int::DataType, optional<Error>>(-1, Error(pos, "Invalid integer: ", c)); }
      v = v * base + cv;
      pos.column++;
    }
    
    if (!in.eof()) { in.unget();}
    return pair<types::Int::DataType, optional<Error>>(v, nullopt);
  }
  
  ReadResult read_int(istream &in, Pos &pos, M &m) {
    char c = 0;
    if (!in.get(c)) { return ReadResult(nullopt, nullopt); }
    if (c) { in.unget(); }
    if (!isdigit(c)) { return ReadResult(nullopt, nullopt); }
    Pos fpos = pos;
    auto [v, err] = read_int_base(in, pos, 10);
    if (err) { return ReadResult(nullopt, err); }
    return ReadResult(forms::Lit(fpos, m.abc_lib->int_type, v), nullopt);
  }

  ReadResult read_slice(istream &in, Pos &pos, M &m) {
    char c = 0;
    if (!in.get(c)) { ReadResult(nullopt, nullopt); }

    if (c != '[') {
      in.unget();
      return ReadResult(nullopt, nullopt);
    }

    Pos fpos = pos;
    pos.column++;
    deque<Form> items;
    
    for (;;) {
      read_ws(in, pos, m);

      if (in.get(c)) {
	if (c == ']') { break; }
	in.unget();
      } else {
	break;
      }
      
      auto [f, err] = read_form(in, pos, m);
      if (err) { return ReadResult(nullopt, err); }
      if (!f) { break; }
      items.push_back(*f);
    }

    if (c != ']') { return ReadResult(nullopt, Error(fpos, "Open slice: ", c)); }
    pos.column++;
    return ReadResult(forms::Slice(fpos, items), nullopt);
  }
  
  ReadResult read_ws(istream &in, Pos &pos, M &m) {
    char c = 0;
    
    while (in.get(c)) {
      if (!isspace(c)) {
	in.unget();
	break;
      }
	  
      switch (c) {
      case ' ':
      case '\t':
	pos.column++;
	break;
      case '\n':
	pos.line++;
	pos.column = 1;
	break;
      };
    }

    return ReadResult(nullopt, nullopt);
  }
}
