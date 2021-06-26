#include "parser.hpp"
#include <iostream>
#include <istream>
#include <memory>
#include <sstream>

int main() {
  auto parser = new Parser(""
                           "import sayHello from 'hello';"
                           "sayHello();"
                           "function sayHi(){"
                           " const a = 1 + 1;"
                           " return a;"
                           "}");
  auto program = parser->Parse();
  // cout << program.get()->type() << endl;
}