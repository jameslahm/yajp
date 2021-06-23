#include "parser.hpp"
#include <emscripten/bind.h>
#include <iostream>
#include <memory>
#include <string>

using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(my_class_example) {
  class_<Node>("Node").constructor<NodeType>();
  class_<IdentifierNode, base<Node>>("IdentifierNode").constructor<string>();
  class_<NumericLiteralNode, base<Node>>("NumbericLiteralNode").constructor<double>();
  class_<NullLiteralNode, base<Node>>("NullLiteralNode").constructor();
  class_<StringLiteralNode, base<Node>>("StringLiteralNode").constructor<string>();
  class_<BooleanLiteralNode, base<Node>>("BooleanLiteralNode").constructor<bool>();
  class_<UnaryExpressionNode, base<Node>>("UnaryExpressionNode").constructor<UnaryOperator,unique_ptr<Node>>();
  class_<BinaryExpressionNode, base<Node>>("BianryExpressionNode").constructor<BinaryOperator, unique_ptr<Node>, unique_ptr<Node>>();
  class_<Parser>("Parser").constructor<string>().function("Parse",
                                                          &Parser::Parse);
}

