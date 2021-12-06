/*
 *  UCF COP3330 Fall 2021 Assignment 6 Solution
 *  Copyright 2021 Michael Vuolo
 */

/*
	calculator08buggy.cpp | From: Programming -- Principles and Practice Using C++, by Bjarne Stroustrup
	We have inserted 3 bugs that the compiler will catch and 3 that it won't.
*/

#include "std_lib_facilities.h"

class Token {
  public:
    char kind;
  double value;
  string name;
  Token(char ch): kind(ch), value(0) {}
  Token(char ch, double val): kind(ch), value(val) {}
  Token(char ch, string n): kind(ch), name(n) {}
};

class Token_stream {
  public:
    Token_stream();
  Token get();
  void putback(Token t);
  void ignore(char c);
  private:
    bool full;
  Token buffer;
};

// token streak constructor
Token_stream::Token_stream(): full(false), buffer(0) {}

void Token_stream::putback(Token t) {
  if (full) error("putback() into a full buffer");
  buffer = t;
  full = true;
}

const char number = '8';
const char quit = 'q';
const char print = ';';
const char name = 'a';
const char
let = 'L';
const char con = 'C';
const string declkey = "let";
const string constkey = "const";

Token Token_stream::get() {
  if (full) {
    full = false;
    return buffer;
  }

  char ch;
  cin >> ch;

  switch (ch) {
  case quit:
  case print:
  case '(':
  case ')':
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '=':
    return Token(ch);
  case '.':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9': {
    cin.putback(ch);
    double val;
    cin >> val;
    return Token(number, val);
  }
  default:
    if (isalpha(ch)) {
      string s;
      s += ch;
      while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) s += ch; // letters digits and underscores
      cin.putback(ch);
      if (s == declkey) return Token(let);
      if (s == constkey) return Token(con);
      return Token(name, s);
    }
    error("Bad token");
  }
}

void Token_stream::ignore(char c) {
  if (full && c == buffer.kind) {
    full = false;
    return;
  }
  full = false;

  char ch = 0;
  while (cin >> ch)
    if (ch == c) return;
}

class Variable {
  public:
    string name;
  double value;
  bool
  var;
  Variable(string n, double v, bool va = true): name(n), value(v),
    var (va) {}
};

vector < Variable > names;

double get_value(string s) {
  for (int i = 0; i < names.size(); ++i)
    if (names[i].name == s) return names[i].value;
  error("get: undefined variable ", s);
}

void set_value(string s, double d) {
  for (int i = 0; i < names.size(); ++i)
    if (names[i].name == s) {
      if (names[i].var == false) error(s, " is a constant");
      names[i].value = d;
      return;
    }
  error("set: undefined variable ", s);
}

bool is_declared(string s) {
  for (int i = 0; i < names.size(); ++i)
    if (names[i].name == s) return true;
  return false;
}

double define_name(string s, double val, bool
  var = true) {
  if (is_declared(s)) error(s, " declared twice");
  names.push_back(Variable(s, val,
    var));
  return val;
}

Token_stream ts;

double expression();

double primary() {
  Token t = ts.get();
  switch (t.kind) {
  case '(': {
    double d = expression();
    t = ts.get();
    if (t.kind != ')') error("')' expected");
    return d;
  }
  case number:
    return t.value;
  case name: {
    Token next = ts.get();
    if (next.kind == '=') {
      double d = expression();
      set_value(t.name, d);
      return d;
    } else {
      ts.putback(next);
      return get_value(t.name);
    }
  }
  case '-':
    return -primary();
  case '+':
    return primary();
  default:
    error("primary expected");
  }
}

double term() {
  double left = primary();
  Token t = ts.get(); // get next token

  while (true) {
    switch (t.kind) {
    case '*':
      left *= primary();
      t = ts.get();
      break;
    case '/': {
      double d = primary();
      if (d == 0) error("divide by zero");
      left /= d;
      t = ts.get();
      break;
    }
    case '%': {
      int i1 = narrow_cast < int > (left);
      int i2 = narrow_cast < int > (term());
      if (i2 == 0) error("%: divide by zero");
      left = i1 % i2;
      t = ts.get();
      break;
    }
    default:
      ts.putback(t);
      return left;
    }
  }
}

double expression() {
  double left = term();
  Token t = ts.get();

  while (true) {
    switch (t.kind) {
    case '+':
      left += term();
      t = ts.get();
      break;
    case '-':
      left -= term();
      t = ts.get();
      break;
    default:
      ts.putback(t);
      return left;
    }
  }
}

double declaration(Token k) {
  Token t = ts.get();
  if (t.kind != name) error("name expected in declaration");
  string var_name = t.name;

  Token t2 = ts.get();
  if (t2.kind != '=') error("= missing in declaration of ", var_name);

  double d = expression();
  define_name(var_name, d, k.kind ==
    let);
  return d;
}

double statement() {
  Token t = ts.get();
  switch (t.kind) {
  case let:
  case con:
    return declaration(t.kind);
  default:
    ts.putback(t);
    return expression();
  }
}

void clean_up_mess() {
  ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate() {
  while (cin)
    try {
      cout << prompt;
      Token t = ts.get();
      while (t.kind == print) t = ts.get();
      if (t.kind == quit) return;
      ts.putback(t);
      cout << result << statement() << endl;
    }
  catch (exception & e) {
    cerr << e.what() << endl;
    clean_up_mess();
  }
}

int main()
try {
  // predefine names:
  define_name("pi", 3.1415926535, false); // these pre-defiend names are constants
  define_name("e", 2.7182818284, false);

  calculate();

  keep_window_open(); // cope with Windows console mode
  return 0;
} catch (exception & e) {
  cerr << e.what() << endl;
  keep_window_open("~~");
  return 1;
} catch (...) {
  cerr << "exception \n";
  keep_window_open("~~");
  return 2;
}