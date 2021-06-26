#include "lexer.hpp"
#include <locale>
#include <map>
#include <memory>
#include <ostream>
#include <pthread.h>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;
// #include <magic_enum.hpp>

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
  kForOfStatement,
  kThrowStatement,
  kTryStatement,
  kCatchClause,
  kFunctionDeclaration,
  kFunctionExpression,
  kProgram,
  kImportDeclaration,
  kImportSpecifier,
  kImportDefaultSpecifier,
  kImportNamespaceSpecifier,
  kExportSpecifier,
  kExportNamespaceSpecifier,
  kExportDefaultSpecifier,
  kExportNamedDeclaration,
  kExportDefaultDeclaration,
  kExportAllDeclaration,
  kCallExpression,
  kParenthesizedExpression
};

class Node {
  NodeType type_;

public:
  Node(NodeType type) : type_(type) {}
  virtual ~Node() {}

  friend ostream &operator<<(ostream &out, Node &node) {
    // out << magic_enum::enum_name(node.type_) << endl;
    return out;
  }

  NodeType type() const { return type_; }

  virtual string GenJs() { return ""; };
};

class IdentifierNode : public Node {
  string name_;

public:
  IdentifierNode(string name) : Node(NodeType::kIdentifier), name_(name) {}
  string name() const { return name_; }
  void set_name(string name) { name_ = name; }
  string GenJs() override { return name_; }
};

class NullLiteralNode : public Node {
public:
  NullLiteralNode() : Node(NodeType::kNullLiteral) {}
  string GenJs() override { return "null"; }
};

class StringLiteralNode : public Node {
  string value_;

public:
  StringLiteralNode(string value)
      : Node(NodeType::kStringLiteral), value_(value) {}
  string value() const { return value_; }
  string GenJs() override { return value_; }
};

class BooleanLiteralNode : public Node {
  bool value_;

public:
  BooleanLiteralNode(bool value)
      : Node(NodeType::kBooleanLiteral), value_(value) {}
  bool value() const { return value_; }
  string GenJs() override {
    if (value_) {
      return "true";
    } else {
      return "false";
    }
  }
};

class NumericLiteralNode : public Node {
  double value_;

public:
  NumericLiteralNode(double value)
      : Node(NodeType::kNumericLiteral), value_(value) {}
  double value() const { return value_; }
  string GenJs() override { return to_string(value_); }
};

// enum class UnaryOperator {
//   kSubOp,
//   kAddOp,
//   kExclaOp,
//   kNegOp,
//   kTypeOfOp,
//   kVoidOp,
//   kDeleteOp,
//   kThrowOp
// };

class UnaryOperator {
  string source_;

public:
  UnaryOperator(string source) : source_(source) {}
  string genJs(){
    return source_;
  }
};

inline const static UnaryOperator kSubOp{"-"};
inline const static UnaryOperator kAddOp{"+"};
inline const static UnaryOperator kExclaOp{"!"};
inline const static UnaryOperator kNegOp{"-"};
inline const static UnaryOperator kTypeOfOp{"typeof"};
inline const static UnaryOperator kVoidOp{"void"};
inline const static UnaryOperator kDeleteOp{"delete"};
inline const static UnaryOperator kThrowOp{"throw"};


class UnaryExpressionNode : public Node {
  UnaryOperator op_;
  shared_ptr<Node> argument_;

public:
  UnaryExpressionNode(UnaryOperator op, shared_ptr<Node> argument)
      : Node(NodeType::kUnaryExpression), op_(op), argument_(move(argument)) {}

  UnaryOperator op() const { return op_; }
  shared_ptr<Node> argument() const { return argument_; }

  string GenJs() override {}
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
  shared_ptr<Node> right() const { return right_; }
  BinaryOperator op() const { return op_; }
  void set_left(shared_ptr<Node> left) { left_ = move(left); }
  void set_right(shared_ptr<Node> right) { right_ = move(right); }
};

class ExpressionStatementNode : public Node {
  shared_ptr<Node> expression_;

public:
  ExpressionStatementNode(shared_ptr<Node> expression)
      : Node(NodeType::kExpressionStatement), expression_(move(expression)) {}
  shared_ptr<Node> expression() const { return expression_; }
};

class BlockStatementNode : public Node {
  vector<shared_ptr<Node>> body_;

public:
  BlockStatementNode(vector<shared_ptr<Node>> body)
      : Node(NodeType::kBlockStatement), body_(move(body)) {}
  vector<shared_ptr<Node>> body() const { return body_; }
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
  shared_ptr<Node> argument() const { return argument_; }
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
  shared_ptr<Node> test() const { return test_; }
  shared_ptr<Node> consequent() const { return consequent_; }
  shared_ptr<Node> alternate() const { return alternate_; }
};

class SwitchStatementNode : public Node {
  shared_ptr<Node> discriminant_;
  vector<shared_ptr<Node>> cases_;

public:
  SwitchStatementNode(shared_ptr<Node> discriminant,
                      vector<shared_ptr<Node>> cases)
      : Node(NodeType::kSwitchStatement), discriminant_(move(discriminant)),
        cases_(move(cases)) {}
  vector<shared_ptr<Node>> cases() const { return cases_; }
  shared_ptr<Node> discriminant() const { return discriminant_; }
};

class SwitchCaseNode : public Node {
  shared_ptr<Node> test_;
  vector<shared_ptr<Node>> consequent_;

public:
  SwitchCaseNode(shared_ptr<Node> test, vector<shared_ptr<Node>> consequent)
      : Node(NodeType::kSwitchCase), test_(move(test)),
        consequent_(move(consequent)) {}
  shared_ptr<Node> test() const { return test_; }
  vector<shared_ptr<Node>> consequent() const { return consequent_; }
};

class WhileStatementNode : public Node {
  shared_ptr<Node> test_;
  shared_ptr<Node> body_;

public:
  WhileStatementNode(shared_ptr<Node> test, shared_ptr<Node> body)
      : Node(NodeType::kWhileStatement), test_(move(test)), body_(move(body)) {}
  shared_ptr<Node> test() const { return test_; }
  shared_ptr<Node> body() const { return body_; }
};

class DoWhileStatementNode : public Node {
  shared_ptr<Node> test_;
  shared_ptr<Node> body_;

public:
  DoWhileStatementNode(shared_ptr<Node> test, shared_ptr<Node> body)
      : Node(NodeType::kDoWhileStatement), test_(move(test)),
        body_(move(body)) {}
  shared_ptr<Node> test() const { return test_; }
  shared_ptr<Node> body() const { return body_; }
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
  shared_ptr<Node> init() const { return init_; }
  shared_ptr<Node> test() const { return test_; }
  shared_ptr<Node> update() const { return update_; }
  shared_ptr<Node> body() const { return body_; }
};

enum class VariableDeclarationKind { kVar, kLet, kConst };

class VariableDeclaratorNode : public Node {
  shared_ptr<Node> id_;
  shared_ptr<Node> init_;

public:
  VariableDeclaratorNode(shared_ptr<Node> id, shared_ptr<Node> init)
      : Node(NodeType::kVariableDeclarator), id_(move(id)), init_(move(init)) {}
  shared_ptr<Node> id() const { return id_; }
  shared_ptr<Node> init() const { return init_; }
};

class VariableDeclarationNode : public Node {
  VariableDeclarationKind kind_;
  vector<shared_ptr<Node>> declarations_;

public:
  VariableDeclarationNode(VariableDeclarationKind kind,
                          vector<shared_ptr<Node>> declarations)
      : Node(NodeType::kVariableDeclaration), kind_(kind),
        declarations_(move(declarations)) {}
  VariableDeclarationKind kind() const { return kind_; }
  vector<shared_ptr<Node>> declarations() const { return declarations_; }
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
  shared_ptr<Node> left() const { return left_; }
  shared_ptr<Node> right() const { return right_; }
  shared_ptr<Node> body() const { return body_; }
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
  shared_ptr<Node> left() const { return left_; }
  shared_ptr<Node> right() const { return right_; }
  shared_ptr<Node> body() const { return body_; }
  bool await() const { return await_; }
};

class ThrowStatementNode : public Node {
  shared_ptr<Node> argument_;

public:
  ThrowStatementNode(shared_ptr<Node> argument)
      : Node(NodeType::kThrowStatement), argument_(move(argument)) {}
  shared_ptr<Node> argument() const { return argument_; }
};

class CatchClauseNode : public Node {
  shared_ptr<Node> param_;
  shared_ptr<Node> body_;

public:
  CatchClauseNode(shared_ptr<Node> param, shared_ptr<Node> body)
      : Node(NodeType::kCatchClause), param_(move(param)), body_(move(body)) {}
  shared_ptr<Node> param() const { return param_; }
  shared_ptr<Node> body() const { return body_; }
};

class TryStatementNode : public Node {
  shared_ptr<Node> block_;
  shared_ptr<Node> handler_;
  shared_ptr<Node> finalizer_;

public:
  TryStatementNode(shared_ptr<Node> block, shared_ptr<Node> handler,
                   shared_ptr<Node> finalizer)
      : Node(NodeType::kTryStatement), block_(move(block)),
        handler_(move(handler)), finalizer_(move(finalizer)) {}
  shared_ptr<Node> block() const { return block_; }
  shared_ptr<Node> handler() const { return handler_; }
  shared_ptr<Node> finalizer() const { return finalizer_; }
};

class FunctionDeclarationNode : public Node {
  shared_ptr<Node> id_;
  vector<shared_ptr<Node>> params_;
  shared_ptr<Node> body_;
  bool generator_;
  bool async_;

public:
  FunctionDeclarationNode(shared_ptr<Node> id, vector<shared_ptr<Node>> params,
                          shared_ptr<Node> body, bool generator, bool async)
      : Node(NodeType::kFunctionDeclaration), id_(move(id)),
        params_(move(params)), body_(move(body)), generator_(generator),
        async_(async) {}
  shared_ptr<Node> id() const { return id_; }
  vector<shared_ptr<Node>> params() const { return params_; }
  shared_ptr<Node> body() const { return body_; }
  bool generator() const { return generator_; }
  bool async() const { return async_; }
};

class FunctionExpressionNode : public Node {
  shared_ptr<Node> id_;
  vector<shared_ptr<Node>> params_;
  shared_ptr<Node> body_;
  bool generator_;
  bool async_;

public:
  FunctionExpressionNode(shared_ptr<Node> id, vector<shared_ptr<Node>> params,
                         shared_ptr<Node> body, bool generator, bool async)
      : Node(NodeType::kFunctionExpression), id_(move(id)),
        params_(move(params)), body_(move(body)), generator_(generator),
        async_(async) {}
  shared_ptr<Node> id() const { return id_; }
  vector<shared_ptr<Node>> params() const { return params_; }
  shared_ptr<Node> body() const { return body_; }
  bool generator() const { return generator_; }
  bool async() const { return async_; }
};

enum class SoureType { kScript, kModule };

class ProgramNode : public Node {
  SoureType source_type_;
  vector<shared_ptr<Node>> body_;

public:
  ProgramNode(SoureType source_type, vector<shared_ptr<Node>> body)
      : Node(NodeType::kProgram), source_type_(source_type), body_(move(body)) {
  }
  SoureType source_type() const { return source_type_; }
  vector<shared_ptr<Node>> body() const { return body_; }
};

enum class ImportKind { kType, kTypeOf, kValue, kNull };

class ImportDeclarationNode : public Node {
  ImportKind import_kind_;
  vector<shared_ptr<Node>> specifiers_;
  shared_ptr<Node> source_;

public:
  ImportDeclarationNode(ImportKind import_kind,
                        vector<shared_ptr<Node>> specifiers,
                        shared_ptr<Node> source)
      : Node(NodeType::kImportDeclaration), import_kind_(import_kind),
        specifiers_(move(specifiers)), source_(move(source)) {}
  ImportKind import_kind() const { return import_kind_; }
  vector<shared_ptr<Node>> specifiers() const { return specifiers_; }
  shared_ptr<Node> source() const { return source_; }
};

class ImportSpecifierNode : public Node {
  shared_ptr<Node> imported_;
  shared_ptr<Node> local_;

public:
  ImportSpecifierNode(shared_ptr<Node> imported, shared_ptr<Node> local)
      : Node(NodeType::kImportSpecifier), imported_(move(imported)),
        local_(move(local)) {}
  shared_ptr<Node> imported() const { return imported_; }
  shared_ptr<Node> local() const { return local_; }
};

class ImportDefaultSpecifierNode : public Node {
  shared_ptr<Node> local_;

public:
  ImportDefaultSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kImportDefaultSpecifier), local_(move(local)) {}
  shared_ptr<Node> local() const { return local_; }
};

class ImportNamespaceSpecifierNode : public Node {
  shared_ptr<Node> local_;

public:
  ImportNamespaceSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kImportNamespaceSpecifier), local_(move(local)) {}
  shared_ptr<Node> local() const { return local_; }
};

class ExportSpecifierNode : public Node {
  shared_ptr<Node> exported_;
  shared_ptr<Node> local_;

public:
  ExportSpecifierNode(shared_ptr<Node> exported, shared_ptr<Node> local)
      : Node(NodeType::kExportSpecifier), exported_(move(exported)),
        local_(move(local)) {}
  shared_ptr<Node> exported() const { return exported_; }
  shared_ptr<Node> local() const { return local_; }
};

class ExportDefaultSpecifierNode : public Node {
  shared_ptr<Node> local_;

public:
  ExportDefaultSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kExportDefaultSpecifier), local_(move(local)) {}

  shared_ptr<Node> local() const { return local_; }
};

class ExportNamespaceSpecifierNode : public Node {
  shared_ptr<Node> local_;

public:
  ExportNamespaceSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kExportNamespaceSpecifier), local_(move(local)) {}
  shared_ptr<Node> local() const { return local_; }
};

class ExportNamedDeclarationNode : public Node {
  shared_ptr<Node> declaration_;
  vector<shared_ptr<Node>> specifiers_;
  shared_ptr<Node> source_;

public:
  ExportNamedDeclarationNode(shared_ptr<Node> declaration,
                             vector<shared_ptr<Node>> specifiers,
                             shared_ptr<Node> source)
      : Node(NodeType::kExportNamedDeclaration),
        declaration_(move(declaration)), specifiers_(move(specifiers)),
        source_(move(source)) {}
  shared_ptr<Node> declaration() const { return declaration_; }
  shared_ptr<Node> source() const { return source_; }
  vector<shared_ptr<Node>> specifiers() const { return specifiers_; }
};

class ExportDefaultDeclarationNode : public Node {
  shared_ptr<Node> declaration_;

public:
  ExportDefaultDeclarationNode(shared_ptr<Node> declaration)
      : Node(NodeType::kExportDefaultDeclaration),
        declaration_(move(declaration)) {}
  shared_ptr<Node> declaration() const { return declaration_; }
};

class ExportAllDeclarationNode : public Node {
  shared_ptr<Node> source_;

public:
  ExportAllDeclarationNode(shared_ptr<Node> source)
      : Node(NodeType::kExportAllDeclaration), source_(move(source)) {}
  shared_ptr<Node> source() const { return source_; }
};

class CallExpressionNode : public Node {
  vector<shared_ptr<Node>> arguments_;
  shared_ptr<Node> callee_;

public:
  CallExpressionNode(shared_ptr<Node> callee,
                     vector<shared_ptr<Node>> arguments)
      : Node(NodeType::kCallExpression), callee_(callee),
        arguments_(move(arguments)) {}
  vector<shared_ptr<Node>> arguments() const { return arguments_; }
  shared_ptr<Node> callee() const { return callee_; }
};

class ParenthesizedExpressionNode : public Node {
  shared_ptr<Node> expression_;

public:
  ParenthesizedExpressionNode(shared_ptr<Node> expression)
      : Node(NodeType::kParenthesizedExpression),
        expression_(move(expression)) {}
  shared_ptr<Node> expression() const { return expression_; }
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
  shared_ptr<Node> ParseThrowStatement();
  shared_ptr<Node> ParseTryStatement();
  shared_ptr<Node> ParseCatchClause();
  shared_ptr<Node> ParseFunctionExpression();
  shared_ptr<Node> ParseFunctionDeclaration();
  vector<shared_ptr<Node>> ParseFunctionParams();
  shared_ptr<Node> ParseImportDeclaration();
  shared_ptr<Node> ParseImportSpecifier();
  shared_ptr<Node> ParseImportDefaultSpecifier();
  shared_ptr<Node> ParseImportNamespaceSpecifier();
  shared_ptr<Node> ParseExportNamedDeclarationOrExportAllDeclaration();
  shared_ptr<Node> ParseExportSpecifier();
  shared_ptr<Node> ParseExportDefaultSpecifier();
  shared_ptr<Node> ParseExportNamespaceSpecifier();
  shared_ptr<Node> ParseExportDefaultDeclaration();
  shared_ptr<Node> ParseExportNamedDeclarationOrExportDefaultDeclaration();
  shared_ptr<Node> ParseExportAllDeclaration();
  shared_ptr<Node> ParseDeclaration();
  shared_ptr<Node> ParseProgram();
  shared_ptr<Node> ParseCallExpression(shared_ptr<Node> callee);
  shared_ptr<Node> ParseIdentifierOrCallExpression();
  vector<shared_ptr<Node>> ParseCallExpressionArguments();

  void
  InstallBinaryOpPrecendences(map<BinaryOperator, int> binary_op_precendences);
  int GetBinaryOpPrecendence(BinaryOperator op);
  BinaryOperator GetBinaryOpFromToken(TokenType token);
  bool CheckIsBianryOp(TokenType token);
  VariableDeclarationKind GetVariableDeclarationKindFromToken(TokenType token);
  bool CheckIsVariableDeclaration(TokenType token);
};
