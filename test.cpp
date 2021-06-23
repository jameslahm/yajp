#include "parser.hpp"
#include <iostream>
#include <istream>
#include <memory>
#include <sstream>

int main() {
  auto parser = new Parser("123");
  parser->Parse();
}