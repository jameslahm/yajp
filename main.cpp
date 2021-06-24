#include "parser.hpp"
#include <emscripten/bind.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(my_class_example) {
  class_<Node>("Node").constructor<NodeType>().smart_ptr<std::shared_ptr<Node>>(
      "Node");
  class_<IdentifierNode, base<Node>>("IdentifierNode").constructor<string>();
  class_<NumericLiteralNode, base<Node>>("NumbericLiteralNode")
      .constructor<double>();
  class_<NullLiteralNode, base<Node>>("NullLiteralNode").constructor();
  class_<StringLiteralNode, base<Node>>("StringLiteralNode")
      .constructor<string>();
  class_<BooleanLiteralNode, base<Node>>("BooleanLiteralNode")
      .constructor<bool>();
  class_<UnaryExpressionNode, base<Node>>("UnaryExpressionNode")
      .constructor<UnaryOperator, shared_ptr<Node>>();
  class_<BinaryExpressionNode, base<Node>>("BianryExpressionNode")
      .constructor<BinaryOperator, shared_ptr<Node>, shared_ptr<Node>>()
      .property("left",&BinaryExpressionNode::left,&BinaryExpressionNode::set_left);
  class_<ExpressionStatementNode, base<Node>>("ExpressionStatementNode")
      .constructor<shared_ptr<Node>>();
  class_<BlockStatementNode, base<Node>>("BlockStatementNode")
      .constructor<vector<shared_ptr<Node>>>();
  class_<DebuggerStatementNode, base<Node>>("DebuggerStatementNode")
      .constructor<>();
  class_<EmptyStatementNode, base<Node>>("EmptyStatementNode").constructor();
  class_<ReturnStatementNode, base<Node>>("ReturnStatementNode")
      .constructor<shared_ptr<Node>>();
  class_<ContinueStatementNode, base<Node>>("ContinueStatementNode")
      .constructor();
  class_<BreakStatementNode, base<Node>>("BreakStatementNode").constructor();
  class_<IfStatementNode, base<Node>>("IfStatementNode")
      .constructor<shared_ptr<Node>, shared_ptr<Node>, shared_ptr<Node>>();
  class_<SwitchStatementNode, base<Node>>("SwitchStatementNode")
      .constructor<shared_ptr<Node>, vector<shared_ptr<Node>>>();
  class_<SwitchCaseNode, base<Node>>("SwitchCaseNode")
      .constructor<shared_ptr<Node>, vector<shared_ptr<Node>>>();
  class_<WhileStatementNode, base<Node>>("WhileStatementNode")
      .constructor<shared_ptr<Node>, shared_ptr<Node>>();
  class_<DoWhileStatementNode, base<Node>>("DoWhileStatementNode")
      .constructor<shared_ptr<Node>, shared_ptr<Node>>();
  class_<ForStatementNode, base<Node>>("ForStatementNode")
      .constructor<shared_ptr<Node>, shared_ptr<Node>, shared_ptr<Node>,
                   shared_ptr<Node>>();
  class_<VariableDeclarationNode, base<Node>>("VariableDeclarationNode")
      .constructor<VariableDeclarationKind, vector<shared_ptr<Node>>>();
  class_<VariableDeclaratorNode, base<Node>>("VariableDeclaratorNode")
      .constructor<shared_ptr<Node>, shared_ptr<Node>>();
  class_<ForInStatementNode, base<Node>>("ForInStatementNode")
      .constructor<shared_ptr<Node>, shared_ptr<Node>, shared_ptr<Node>>();
  class_<ForOfStatementNode, base<Node>>("ForOfStatementNode")
      .constructor<shared_ptr<Node>, shared_ptr<Node>, shared_ptr<Node>,
                   bool>();
  class_<Parser>("Parser").constructor<string>().function("Parse",
                                                          &Parser::Parse);
}
