#include "lexer.hpp"
#include <locale>
#include <map>
#include <memory>
#include <pthread.h>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;

enum class NodeType {
  kIdentifier,
  kNullLiteral,
  kStringLiteral,
  kNumericLiteral,
  kBooleanLiteral,
  kUnaryExpression,
  kBinaryExpression,
  kExpressionStatement,
  kBlockStatement,
  kEmptyStatement,
  kDebuggerStatement,
  kReturnStatement,
  kContinueStatement,
  kBreakStatement,
  kIfStatement,
  kSwitchStatement,
  kSwitchCase,
  kWhileStatement,
  kDoWhileStatement,
  kForStatement,
  kVariableDeclaration,
  kVariableDeclarator,
  kForInStatement,
  kForOfStatement
};

class Node {
  NodeType type_;

public:
  Node(NodeType type) : type_(type) {}
  virtual ~Node() {}
};

class IdentifierNode : public Node {
  string name_;

public:
  IdentifierNode(string name) : Node(NodeType::kIdentifier), name_(name) {}
  string name() const { return name_; }
  void set_name(string name) { name_ = name; }
};

class NullLiteralNode : public Node {
public:
  NullLiteralNode() : Node(NodeType::kNullLiteral) {}
};

class StringLiteralNode : public Node {
  string value_;

public:
  StringLiteralNode(string value)
      : Node(NodeType::kStringLiteral), value_(value) {}
};

class BooleanLiteralNode : public Node {
  bool value_;

public:
  BooleanLiteralNode(bool value)
      : Node(NodeType::kBooleanLiteral), value_(value) {}
};

class NumericLiteralNode : public Node {
  double value_;

public:
  NumericLiteralNode(double value)
      : Node(NodeType::kNumericLiteral), value_(value) {}
};

enum class UnaryOperator {
  kSubOp,
  kAddOp,
  kExclaOp,
  kNegOp,
  kTypeOfOp,
  kVoidOp,
  kDeleteOp,
  kThrowOp
};

class UnaryExpressionNode : public Node {
  UnaryOperator op_;
  shared_ptr<Node> argument_;

public:
  UnaryExpressionNode(UnaryOperator op, shared_ptr<Node> argument)
      : Node(NodeType::kUnaryExpression), op_(op), argument_(move(argument)) {}

  UnaryOperator op() { return op_; }
};

enum class BinaryOperator {
  kEqualEqualOp,
  kNotEqualOp,
  kEqualEqualEqualOp,
  kNotEqualEqualOp,
  kLessThanOp,
  kLessEqualOp,
  kGreaterThanOp,
  kGreaterEqualOp,
  kLessLessOp,
  kGreaterGreaterOp,
  kGreaterGreaterGreaterOp,
  kAddOp,
  kSubOp,
  kMulOp,
  kDivOp,
  kModOp,
};

/*
interface BinaryExpression <: Expression {
  type: "BinaryExpression";
  operator: BinaryOperator;
  left: Expression | PrivateName;
  right: Expression;
}
*/
class BinaryExpressionNode : public Node {
  BinaryOperator op_;
  shared_ptr<Node> left_;
  shared_ptr<Node> right_;

public:
  BinaryExpressionNode(BinaryOperator op, shared_ptr<Node> left,
                       shared_ptr<Node> right)
      : Node(NodeType::kBinaryExpression), op_(op), left_(move(left)),
        right_(move(right)) {}
  shared_ptr<Node> left() const { return left_; }
  shared_ptr<Node> right() { return right_; }
  BinaryOperator op() { return op_; }
  void set_left(shared_ptr<Node> left) { left_ = move(left); }
};

class ExpressionStatementNode : public Node {
  shared_ptr<Node> expression_;

public:
  ExpressionStatementNode(shared_ptr<Node> expression)
      : Node(NodeType::kExpressionStatement), expression_(move(expression)) {}
};

class BlockStatementNode : public Node {
  vector<shared_ptr<Node>> body_;

public:
  BlockStatementNode(vector<shared_ptr<Node>> body)
      : Node(NodeType::kBlockStatement), body_(move(body)) {}
};

class DebuggerStatementNode : public Node {
public:
  DebuggerStatementNode() : Node(NodeType::kDebuggerStatement) {}
};

class EmptyStatementNode : public Node {
public:
  EmptyStatementNode() : Node(NodeType::kEmptyStatement) {}
};

class ReturnStatementNode : public Node {
  shared_ptr<Node> argument_;

public:
  ReturnStatementNode(shared_ptr<Node> argument)
      : Node(NodeType::kReturnStatement), argument_(move(argument)) {}
};

class ContinueStatementNode : public Node {

public:
  ContinueStatementNode() : Node(NodeType::kContinueStatement) {}
};

class BreakStatementNode : public Node {
public:
  BreakStatementNode() : Node(NodeType::kBreakStatement) {}
};

class IfStatementNode : public Node {
  shared_ptr<Node> test_;
  shared_ptr<Node> consequent_;
  shared_ptr<Node> alternate_;

public:
  IfStatementNode(shared_ptr<Node> test, shared_ptr<Node> consequent,
                  shared_ptr<Node> alternate)
      : Node(NodeType::kIfStatement), test_(move(test)),
        consequent_(move(consequent)), alternate_(move(alternate)) {}
};

class SwitchStatementNode : public Node {
  shared_ptr<Node> discriminant_;
  vector<shared_ptr<Node>> cases_;

public:
  SwitchStatementNode(shared_ptr<Node> discriminant,
                      vector<shared_ptr<Node>> cases)
      : Node(NodeType::kSwitchStatement), discriminant_(move(discriminant)),
        cases_(move(cases)) {}
};

class SwitchCaseNode : public Node {
  shared_ptr<Node> test_;
  vector<shared_ptr<Node>> consequent_;

public:
  SwitchCaseNode(shared_ptr<Node> test, vector<shared_ptr<Node>> consequent)
      : Node(NodeType::kSwitchCase), test_(move(test)),
        consequent_(move(consequent)) {}
};

class WhileStatementNode : public Node {
  shared_ptr<Node> test_;
  shared_ptr<Node> body_;

public:
  WhileStatementNode(shared_ptr<Node> test, shared_ptr<Node> body)
      : Node(NodeType::kWhileStatement), test_(move(test)), body_(move(body)) {}
};

class DoWhileStatementNode : public Node {
  shared_ptr<Node> test_;
  shared_ptr<Node> body_;

public:
  DoWhileStatementNode(shared_ptr<Node> test, shared_ptr<Node> body)
      : Node(NodeType::kDoWhileStatement), test_(move(test)),
        body_(move(body)) {}
};

class ForStatementNode : public Node {
  shared_ptr<Node> init_;
  shared_ptr<Node> test_;
  shared_ptr<Node> update_;
  shared_ptr<Node> body_;

public:
  ForStatementNode(shared_ptr<Node> init, shared_ptr<Node> test,
                   shared_ptr<Node> update, shared_ptr<Node> body)
      : Node(NodeType::kForStatement), init_(move(init)), test_(move(test)),
        update_(move(update)), body_(move(body)) {}
};

enum class VariableDeclarationKind { kVar, kLet, kConst };

class VariableDeclaratorNode : public Node {
  shared_ptr<Node> id_;
  shared_ptr<Node> init_;

public:
  VariableDeclaratorNode(shared_ptr<Node> id, shared_ptr<Node> init)
      : Node(NodeType::kVariableDeclarator), id_(move(id)), init_(move(init)) {}
};

class VariableDeclarationNode : public Node {
  VariableDeclarationKind kind_;
  vector<shared_ptr<Node>> declarations_;

public:
  VariableDeclarationNode(VariableDeclarationKind kind,
                          vector<shared_ptr<Node>> declarations)
      : Node(NodeType::kVariableDeclaration), kind_(kind),
        declarations_(move(declarations)) {}
};

class ForInStatementNode : public Node {
  shared_ptr<Node> left_;
  shared_ptr<Node> right_;
  shared_ptr<Node> body_;

public:
  ForInStatementNode(shared_ptr<Node> left, shared_ptr<Node> right,
                     shared_ptr<Node> body)
      : Node(NodeType::kForInStatement), left_(move(left)), right_(move(right)),
        body_(move(body)) {}
};

class ForOfStatementNode : public Node {
  shared_ptr<Node> left_;
  shared_ptr<Node> right_;
  shared_ptr<Node> body_;
  bool await_;

public:
  ForOfStatementNode(shared_ptr<Node> left, shared_ptr<Node> right,
                     shared_ptr<Node> body, bool await)
      : Node(NodeType::kForOfStatement), left_(move(left)), right_(move(right)),
        body_(move(body)) {}
};

class Parser {
  shared_ptr<Lexer> lexer_;
  map<BinaryOperator, int> binary_op_precendences_;

  map<BinaryOperator, int> kDefaultBinaryOpPrecendences = {
      {BinaryOperator::kLessThanOp, 5}, {BinaryOperator::kLessLessOp, 5},
      {BinaryOperator::kAddOp, 10},     {BinaryOperator::kSubOp, 10},
      {BinaryOperator::kMulOp, 20},     {BinaryOperator::kDivOp, 20},
  };

public:
  Parser(shared_ptr<Lexer> lexer) : lexer_(move(lexer)) {
    InstallBinaryOpPrecendences(kDefaultBinaryOpPrecendences);
  }

  Parser(string source) : lexer_(new Lexer(source)) {
    InstallBinaryOpPrecendences(kDefaultBinaryOpPrecendences);
  }

  shared_ptr<Node> Parse();
  shared_ptr<Node> ParseUnaryExpression();
  shared_ptr<Node> ParseBinaryExpression(shared_ptr<Node> left,
                                         int precendence);
  shared_ptr<Node> ParseIdentifier();
  shared_ptr<Node> ParseStringLiteral();
  shared_ptr<Node> ParseNumericLiteral();
  shared_ptr<Node> ParseNullLiteral();
  shared_ptr<Node> ParseBooleanLiteral();
  shared_ptr<Node> ParseExpression();
  shared_ptr<Node> ParseExpressionStatement();
  shared_ptr<Node> ParseBlockStatement();
  shared_ptr<Node> ParseStatement();
  shared_ptr<Node> ParseEmptyStatement();
  shared_ptr<Node> ParseDebuggerStatement();
  shared_ptr<Node> ParseReturnStatement();
  shared_ptr<Node> ParseBreakStatement();
  shared_ptr<Node> ParseContinueStatement();
  shared_ptr<Node> ParseIfStatement();
  shared_ptr<Node> ParseSwitchStatement();
  shared_ptr<Node> ParseSwitchNodeStatement();
  shared_ptr<Node> ParseWhileStatement();
  shared_ptr<Node> ParseDoWhileStatement();
  shared_ptr<Node> ParseForStatement();
  shared_ptr<Node> ParseVariableDeclaration();
  shared_ptr<Node> ParseVariableDeclarator();
  shared_ptr<Node> ParseForInStatement(shared_ptr<Node> left);
  shared_ptr<Node> ParseForOfStatement(shared_ptr<Node> left, bool await);
  shared_ptr<Node> ParseForInStatementOrForOfStatement();

  void
  InstallBinaryOpPrecendences(map<BinaryOperator, int> binary_op_precendences);
  int GetBinaryOpPrecendence(BinaryOperator op);
  BinaryOperator GetBinaryOpFromToken(TokenType token);
  bool CheckIsBianryOp(TokenType token);
  VariableDeclarationKind GetVariableDeclarationKindFromToken(TokenType token);
  bool CheckIsVariableDeclaration(TokenType token);
};
