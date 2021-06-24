#include "lexer.hpp"
#include <locale>
#include <map>
#include <memory>
#include <pthread.h>
#include <string>
#include <type_traits>
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
  unique_ptr<Node> argument_;

public:
  UnaryExpressionNode(UnaryOperator op, unique_ptr<Node> argument)
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
  unique_ptr<Node> left_;
  unique_ptr<Node> right_;

public:
  BinaryExpressionNode(BinaryOperator op, unique_ptr<Node> left,
                       unique_ptr<Node> right)
      : Node(NodeType::kBinaryExpression), op_(op), left_(move(left)),
        right_(move(right)) {}
};

class ExpressionStatementNode : public Node {
  unique_ptr<Node> expression_;

public:
  ExpressionStatementNode(unique_ptr<Node> expression)
      : Node(NodeType::kExpressionStatement), expression_(move(expression)) {}
};

class BlockStatementNode : public Node {
  vector<unique_ptr<Node>> body_;

public:
  BlockStatementNode(vector<unique_ptr<Node>> body)
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
  unique_ptr<Node> argument_;

public:
  ReturnStatementNode(unique_ptr<Node> argument)
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
  unique_ptr<Node> test_;
  unique_ptr<Node> consequent_;
  unique_ptr<Node> alternate_;

public:
  IfStatementNode(unique_ptr<Node> test, unique_ptr<Node> consequent,
                  unique_ptr<Node> alternate)
      : Node(NodeType::kIfStatement), test_(move(test)),
        consequent_(move(consequent)), alternate_(move(alternate)) {}
};

class SwitchStatementNode : public Node {
  unique_ptr<Node> discriminant_;
  vector<unique_ptr<Node>> cases_;

public:
  SwitchStatementNode(unique_ptr<Node> discriminant,
                      vector<unique_ptr<Node>> cases)
      : Node(NodeType::kSwitchStatement), discriminant_(move(discriminant)),
        cases_(move(cases)) {}
};

class SwitchCaseNode : public Node {
  unique_ptr<Node> test_;
  vector<unique_ptr<Node>> consequent_;

public:
  SwitchCaseNode(unique_ptr<Node> test, vector<unique_ptr<Node>> consequent)
      : Node(NodeType::kSwitchCase), test_(move(test)),
        consequent_(move(consequent)) {}
};

class WhileStatementNode : public Node {
  unique_ptr<Node> test_;
  unique_ptr<Node> body_;

public:
  WhileStatementNode(unique_ptr<Node> test, unique_ptr<Node> body)
      : Node(NodeType::kWhileStatement), test_(move(test)), body_(move(body)) {}
};

class DoWhileStatementNode : public Node {
  unique_ptr<Node> test_;
  unique_ptr<Node> body_;

public:
  DoWhileStatementNode(unique_ptr<Node> test, unique_ptr<Node> body)
      : Node(NodeType::kDoWhileStatement), test_(move(test)),
        body_(move(body)) {}
};

class ForStatementNode : public Node {
  unique_ptr<Node> init_;
  unique_ptr<Node> test_;
  unique_ptr<Node> update_;
  unique_ptr<Node> body_;

public:
  ForStatementNode(unique_ptr<Node> init, unique_ptr<Node> test,
                   unique_ptr<Node> update, unique_ptr<Node> body)
      : Node(NodeType::kForStatement), init_(move(init)), test_(move(test)),
        update_(move(update)), body_(move(body)) {}
};

enum class VariableDeclarationKind { kVar, kLet, kConst };

class VariableDeclaratorNode : public Node {
  unique_ptr<Node> id_;
  unique_ptr<Node> init_;

public:
  VariableDeclaratorNode(unique_ptr<Node> id, unique_ptr<Node> init)
      : Node(NodeType::kVariableDeclarator), id_(move(id)), init_(move(init)) {}
};

class VariableDeclarationNode : public Node {
  VariableDeclarationKind kind_;
  vector<unique_ptr<Node>> declarations_;

public:
  VariableDeclarationNode(VariableDeclarationKind kind,
                          vector<unique_ptr<Node>> declarations)
      : Node(NodeType::kVariableDeclaration), kind_(kind),
        declarations_(move(declarations)) {}
};

class ForInStatementNode : public Node {
  unique_ptr<Node> left_;
  unique_ptr<Node> right_;
  unique_ptr<Node> body_;

public:
  ForInStatementNode(unique_ptr<Node> left, unique_ptr<Node> right,
                     unique_ptr<Node> body)
      : Node(NodeType::kForInStatement), left_(move(left)), right_(move(right)),
        body_(move(body)) {}
};

class ForOfStatementNode : public Node {
  unique_ptr<Node> left_;
  unique_ptr<Node> right_;
  unique_ptr<Node> body_;
  bool await_;

public:
  ForOfStatementNode(unique_ptr<Node> left, unique_ptr<Node> right,
                 unique_ptr<Node> body, bool await)
      : Node(NodeType::kForOfStatement), left_(move(left)), right_(move(right)),
        body_(move(body)) {}
};

class Parser {
  unique_ptr<Lexer> lexer_;
  map<BinaryOperator, int> binary_op_precendences_;

  map<BinaryOperator, int> kDefaultBinaryOpPrecendences = {
      {BinaryOperator::kLessThanOp, 5}, {BinaryOperator::kLessLessOp, 5},
      {BinaryOperator::kAddOp, 10},     {BinaryOperator::kSubOp, 10},
      {BinaryOperator::kMulOp, 20},     {BinaryOperator::kDivOp, 20},
  };

public:
  Parser(unique_ptr<Lexer> lexer) : lexer_(move(lexer)) {
    InstallBinaryOpPrecendences(kDefaultBinaryOpPrecendences);
  }

  Parser(string source) : lexer_(new Lexer(source)) {}

  unique_ptr<Node> Parse();
  unique_ptr<Node> ParseUnaryExpression();
  unique_ptr<Node> ParseBinaryExpression(unique_ptr<Node> left,
                                         int precendence);
  unique_ptr<Node> ParseIdentifier();
  unique_ptr<Node> ParseStringLiteral();
  unique_ptr<Node> ParseNumericLiteral();
  unique_ptr<Node> ParseNullLiteral();
  unique_ptr<Node> ParseBooleanLiteral();
  unique_ptr<Node> ParseExpression();
  unique_ptr<Node> ParseExpressionStatement();
  unique_ptr<Node> ParseBlockStatement();
  unique_ptr<Node> ParseStatement();
  unique_ptr<Node> ParseEmptyStatement();
  unique_ptr<Node> ParseDebuggerStatement();
  unique_ptr<Node> ParseReturnStatement();
  unique_ptr<Node> ParseBreakStatement();
  unique_ptr<Node> ParseContinueStatement();
  unique_ptr<Node> ParseIfStatement();
  unique_ptr<Node> ParseSwitchStatement();
  unique_ptr<Node> ParseSwitchNodeStatement();
  unique_ptr<Node> ParseWhileStatement();
  unique_ptr<Node> ParseDoWhileStatement();
  unique_ptr<Node> ParseForStatement();
  unique_ptr<Node> ParseVariableDeclaration();
  unique_ptr<Node> ParseVariableDeclarator();
  unique_ptr<Node> ParseForInStatement(unique_ptr<Node> left);
  unique_ptr<Node> ParseForOfStatement(unique_ptr<Node> left, bool await);
  unique_ptr<Node> ParseForInStatementOrForOfStatement();

  void
  InstallBinaryOpPrecendences(map<BinaryOperator, int> binary_op_precendences);
  int GetBinaryOpPrecendence(BinaryOperator op);
  BinaryOperator GetBinaryOpFromToken(TokenType token);
  bool CheckIsBianryOp(TokenType token);
  VariableDeclarationKind GetVariableDeclarationKindFromToken(TokenType token);
  bool CheckIsVariableDeclaration(TokenType token);
};
