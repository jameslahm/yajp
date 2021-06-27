#include "lexer.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iterator>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <ostream>
#include <pthread.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;
// #include <magic_enum.hpp>

enum class NodeType
{
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

class Node
{
  NodeType type_;

public:
  Node(NodeType type) : type_(type) {}
  virtual ~Node() {}

  friend ostream &operator<<(ostream &out, Node &node)
  {
    // out << magic_enum::enum_name(node.type_) << endl;
    return out;
  }

  NodeType type() const { return type_; }

  virtual string GenJs() const { return ""; };

  static auto GenJsForVector(const vector<shared_ptr<Node>> &body,
                             string delim = "\n", string prefix = "")
  {
    vector<string> body_str;
    transform(body.begin(), body.end(), back_inserter(body_str),
              [prefix](shared_ptr<Node> node)
              {
                return fmt::format("{}{}", prefix, node->GenJs());
              });
    return fmt::format("{}", fmt::join(body_str, delim));
  }
};

class IdentifierNode : public Node
{
  string name_;

public:
  IdentifierNode(string name) : Node(NodeType::kIdentifier), name_(name) {}
  string name() const { return name_; }
  void set_name(string name) { name_ = name; }
  string GenJs() const override { return name_; }
};

class NullLiteralNode : public Node
{
public:
  NullLiteralNode() : Node(NodeType::kNullLiteral) {}
  string GenJs() const override { return "null"; }
};

class StringLiteralNode : public Node
{
  string value_;

public:
  StringLiteralNode(string value)
      : Node(NodeType::kStringLiteral), value_(value) {}
  string value() const { return value_; }
  string GenJs() const override { return fmt::format("\"{}\"", value_); }
};

class BooleanLiteralNode : public Node
{
  bool value_;

public:
  BooleanLiteralNode(bool value)
      : Node(NodeType::kBooleanLiteral), value_(value) {}
  bool value() const { return value_; }
  string GenJs() const override
  {
    if (value_)
    {
      return "true";
    }
    else
    {
      return "false";
    }
  }
};

class NumericLiteralNode : public Node
{
  double value_;

public:
  NumericLiteralNode(double value)
      : Node(NodeType::kNumericLiteral), value_(value) {}
  double value() const { return value_; }
  string GenJs() const override { return to_string(value_); }
};

class UnaryOperator
{
  string source_;

public:
  UnaryOperator(string source) : source_(source) {}
  string GenJs() const { return source_; }
  string source() const {
    return source_;
  }
  const static UnaryOperator kSubOp;
  const static UnaryOperator kAddOp;
  const static UnaryOperator kExclaOp;
  const static UnaryOperator kNegOp;
  const static UnaryOperator kTypeOfOp;
  const static UnaryOperator kVoidOp;
  const static UnaryOperator kDeleteOp;
  const static UnaryOperator kThrowOp;
};

inline const UnaryOperator UnaryOperator::kSubOp = {"-"};
inline const UnaryOperator UnaryOperator::kAddOp = {"+"};
inline const UnaryOperator UnaryOperator::kExclaOp = {"!"};
inline const UnaryOperator UnaryOperator::kNegOp = {"~"};
inline const UnaryOperator UnaryOperator::kTypeOfOp = {"typeof"};
inline const UnaryOperator UnaryOperator::kVoidOp = {"void"};
inline const UnaryOperator UnaryOperator::kDeleteOp = {"delete"};
inline const UnaryOperator UnaryOperator::kThrowOp = {"throw"};

class UnaryExpressionNode : public Node
{
  UnaryOperator op_;
  shared_ptr<Node> argument_;

public:
  UnaryExpressionNode(UnaryOperator op, shared_ptr<Node> argument)
      : Node(NodeType::kUnaryExpression), op_(op), argument_(move(argument)) {}

  UnaryOperator op() const { return op_; }
  shared_ptr<Node> argument() const { return argument_; }

  string GenJs() const override
  {
    auto op_str = op_.GenJs();
    auto argument_str = argument_->GenJs();
    return fmt::format("{} {}", op_str, argument_str);
  }
};

class BinaryOperator
{
  string source_;

public:
  BinaryOperator(string source) : source_(source) {}

public:
  string GenJs() const { return source_; }
  bool operator<(const BinaryOperator &rhs) const
  {
    return source_ < rhs.source_;
  }
  string source() const {
    return source_;
  }

  const static BinaryOperator kEqualEqualOp;
  const static BinaryOperator kNotEqualOp;
  const static BinaryOperator kEqualEqualEqualOp;
  const static BinaryOperator kNotEqualEqualOp;
  const static BinaryOperator kLessThanOp;
  const static BinaryOperator kLessEqualOp;
  const static BinaryOperator kGreaterThanOp;
  const static BinaryOperator kGreaterEqualOp;
  const static BinaryOperator kLessLessOp;
  const static BinaryOperator kGreaterGreaterOp;
  const static BinaryOperator kGreaterGreaterGreaterOp;
  const static BinaryOperator kAddOp;
  const static BinaryOperator kSubOp;
  const static BinaryOperator kMulOp;
  const static BinaryOperator kDivOp;
  const static BinaryOperator kModOp;
};

inline const BinaryOperator BinaryOperator::kEqualEqualOp{"=="};
inline const BinaryOperator BinaryOperator::kNotEqualOp{"!="};
inline const BinaryOperator BinaryOperator::kEqualEqualEqualOp{"==="};
inline const BinaryOperator BinaryOperator::kNotEqualEqualOp{"!=="};
inline const BinaryOperator BinaryOperator::kLessThanOp{"<"};
inline const BinaryOperator BinaryOperator::kLessEqualOp{"<="};
inline const BinaryOperator BinaryOperator::kGreaterThanOp{">"};
inline const BinaryOperator BinaryOperator::kGreaterEqualOp{">="};
inline const BinaryOperator BinaryOperator::kLessLessOp{"<<"};
inline const BinaryOperator BinaryOperator::kGreaterGreaterOp{">>"};
inline const BinaryOperator BinaryOperator::kGreaterGreaterGreaterOp{">>>"};
inline const BinaryOperator BinaryOperator::kAddOp{"+"};
inline const BinaryOperator BinaryOperator::kSubOp{"-"};
inline const BinaryOperator BinaryOperator::kMulOp{"*"};
inline const BinaryOperator BinaryOperator::kDivOp{"/"};
inline const BinaryOperator BinaryOperator::kModOp{"%"};

/*
interface BinaryExpression <: Expression {
  type: "BinaryExpression";
  operator: BinaryOperator;
  left: Expression | PrivateName;
  right: Expression;
}
*/
class BinaryExpressionNode : public Node
{
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
  string GenJs() const override
  {
    auto left_str = left_->GenJs();
    auto right_str = right()->GenJs();
    auto op_str = op_.GenJs();
    return fmt::format("{} {} {}", left_str, op_str, right_str);
  }
};

class ExpressionStatementNode : public Node
{
  shared_ptr<Node> expression_;

public:
  ExpressionStatementNode(shared_ptr<Node> expression)
      : Node(NodeType::kExpressionStatement), expression_(move(expression)) {}
  shared_ptr<Node> expression() const { return expression_; }
  string GenJs() const override { return expression_->GenJs(); }
};

class BlockStatementNode : public Node
{
  vector<shared_ptr<Node>> body_;

public:
  BlockStatementNode(vector<shared_ptr<Node>> body)
      : Node(NodeType::kBlockStatement), body_(move(body)) {}
  vector<shared_ptr<Node>> body() const { return body_; }
  string GenJs() const override
  {
    auto body_str = GenJsForVector(body_, "\n", "\t");
    return fmt::format("{{\n {} \n}}", body_str);
  }
};

class DebuggerStatementNode : public Node
{
public:
  DebuggerStatementNode() : Node(NodeType::kDebuggerStatement) {}
  string GenJs() const override { return "debugger"; }
};

class EmptyStatementNode : public Node
{
public:
  EmptyStatementNode() : Node(NodeType::kEmptyStatement) {}
  string GenJs() const override { return ""; }
};

class ReturnStatementNode : public Node
{
  shared_ptr<Node> argument_;

public:
  ReturnStatementNode(shared_ptr<Node> argument)
      : Node(NodeType::kReturnStatement), argument_(move(argument)) {}
  shared_ptr<Node> argument() const { return argument_; }
  string GenJs() const override
  {
    auto argument_str = argument_->GenJs();
    return fmt::format("return {}", argument_str);
  }
};

class ContinueStatementNode : public Node
{

public:
  ContinueStatementNode() : Node(NodeType::kContinueStatement) {}
  string GenJs() const override { return "continue"; }
};

class BreakStatementNode : public Node
{
public:
  BreakStatementNode() : Node(NodeType::kBreakStatement) {}
  string GenJs() const override { return "braek"; }
};

class IfStatementNode : public Node
{
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
  string GenJs() const override
  {
    auto test_str = test_->GenJs();
    auto consequent_str = consequent_->GenJs();
    auto alternate_str = alternate_->GenJs();
    return fmt::format("if ({}) {} else {}", test_str, consequent_str,
                       alternate_str);
  }
};

class SwitchStatementNode : public Node
{
  shared_ptr<Node> discriminant_;
  vector<shared_ptr<Node>> cases_;

public:
  SwitchStatementNode(shared_ptr<Node> discriminant,
                      vector<shared_ptr<Node>> cases)
      : Node(NodeType::kSwitchStatement), discriminant_(move(discriminant)),
        cases_(move(cases)) {}
  vector<shared_ptr<Node>> cases() const { return cases_; }
  shared_ptr<Node> discriminant() const { return discriminant_; }
  string GenJs() const override
  {
    auto discriminant_str = discriminant_->GenJs();
    vector<string> cases_str;
    transform(cases_.begin(), cases_.end(), back_inserter(cases_str),
              [](shared_ptr<Node> node)
              { return node->GenJs(); });
    return fmt::format("switch ({}) {{\n {} \n}}", discriminant_str,
                       fmt::join(cases_str, "\n"));
  }
};

class SwitchCaseNode : public Node
{
  shared_ptr<Node> test_;
  vector<shared_ptr<Node>> consequent_;

public:
  SwitchCaseNode(shared_ptr<Node> test, vector<shared_ptr<Node>> consequent)
      : Node(NodeType::kSwitchCase), test_(move(test)),
        consequent_(move(consequent)) {}
  shared_ptr<Node> test() const { return test_; }
  vector<shared_ptr<Node>> consequent() const { return consequent_; }
  string GenJs() const override
  {
    auto test_str = test_->GenJs();
    auto consequent_str = GenJsForVector(consequent_);
    return fmt::format("case ({}): {{\n {} \n}}", consequent_str);
  }
};

class WhileStatementNode : public Node
{
  shared_ptr<Node> test_;
  shared_ptr<Node> body_;

public:
  WhileStatementNode(shared_ptr<Node> test, shared_ptr<Node> body)
      : Node(NodeType::kWhileStatement), test_(move(test)), body_(move(body)) {}
  shared_ptr<Node> test() const { return test_; }
  shared_ptr<Node> body() const { return body_; }
  string GenJs() const override
  {
    auto test_str = test_->GenJs();
    auto body_str = body_->GenJs();
    return fmt::format("while ({}) {}", test_str, body_str);
  }
};

class DoWhileStatementNode : public Node
{
  shared_ptr<Node> test_;
  shared_ptr<Node> body_;

public:
  DoWhileStatementNode(shared_ptr<Node> test, shared_ptr<Node> body)
      : Node(NodeType::kDoWhileStatement), test_(move(test)),
        body_(move(body)) {}
  shared_ptr<Node> test() const { return test_; }
  shared_ptr<Node> body() const { return body_; }
  string GenJs() const override
  {
    auto test_str = test_->GenJs();
    auto body_str = body_->GenJs();
    return fmt::format("do {} while ({})", test_str, body_str);
  }
};

class ForStatementNode : public Node
{
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
  string GenJs() const override
  {
    auto init_str = init_->GenJs();
    auto test_str = test_->GenJs();
    auto update_str = update_->GenJs();
    auto body_str = body_->GenJs();
    return fmt::format("for ({};{};{}) {}", init_str, test_str, update_str,
                       body_str);
  }
};

class VariableDeclarationKind
{
  string source_;

public:
  VariableDeclarationKind(string source) : source_(source) {}

  string GenJs() const { return source_; }

  const static VariableDeclarationKind kLet;
  const static VariableDeclarationKind kConst;
  const static VariableDeclarationKind kVar;
};

inline const VariableDeclarationKind VariableDeclarationKind::kConst{"const"};
inline const VariableDeclarationKind VariableDeclarationKind::kLet("let");
inline const VariableDeclarationKind VariableDeclarationKind::kVar("var");

class VariableDeclaratorNode : public Node
{
  shared_ptr<Node> id_;
  shared_ptr<Node> init_;

public:
  VariableDeclaratorNode(shared_ptr<Node> id, shared_ptr<Node> init)
      : Node(NodeType::kVariableDeclarator), id_(move(id)), init_(move(init)) {}
  shared_ptr<Node> id() const { return id_; }
  shared_ptr<Node> init() const { return init_; }
  string GenJs() const override
  {
    auto id_str = id_->GenJs();
    auto init_str = init_->GenJs();
    return fmt::format("{} = {}", id_str, init_str);
  }
};

class VariableDeclarationNode : public Node
{
  VariableDeclarationKind kind_;
  vector<shared_ptr<Node>> declarations_;

public:
  VariableDeclarationNode(VariableDeclarationKind kind,
                          vector<shared_ptr<Node>> declarations)
      : Node(NodeType::kVariableDeclaration), kind_(kind),
        declarations_(move(declarations)) {}
  VariableDeclarationKind kind() const { return kind_; }
  vector<shared_ptr<Node>> declarations() const { return declarations_; }
  string GenJs() const override
  {
    auto kind_str = kind_.GenJs();
    auto body_str = GenJsForVector(declarations_, " ");
    return fmt::format("{} {}", kind_str, body_str);
  }
};

class ForInStatementNode : public Node
{
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
  string GenJs() const override
  {
    auto left_str = left_->GenJs();
    auto right_str = right_->GenJs();
    auto body_str = body_->GenJs();
    return fmt::format("for ({} in {}) {}", left_str, right_str, body_str);
  }
};

class ForOfStatementNode : public Node
{
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
  string GenJs() const override
  {
    auto left_str = left_->GenJs();
    auto right_str = right_->GenJs();
    auto body_str = body_->GenJs();
    auto await_str = await_ ? "await " : "";
    return fmt::format("for ({} of {}) {}", left_str, right_str, body_str);
  }
};

class ThrowStatementNode : public Node
{
  shared_ptr<Node> argument_;

public:
  ThrowStatementNode(shared_ptr<Node> argument)
      : Node(NodeType::kThrowStatement), argument_(move(argument)) {}
  shared_ptr<Node> argument() const { return argument_; }
  string GenJs() const override
  {
    auto argument_str = argument_->GenJs();
    return fmt::format("throw {}", argument_str);
  }
};

class CatchClauseNode : public Node
{
  shared_ptr<Node> param_;
  shared_ptr<Node> body_;

public:
  CatchClauseNode(shared_ptr<Node> param, shared_ptr<Node> body)
      : Node(NodeType::kCatchClause), param_(move(param)), body_(move(body)) {}
  shared_ptr<Node> param() const { return param_; }
  shared_ptr<Node> body() const { return body_; }
  string GenJs() const override
  {
    auto param_str = param_->GenJs();
    auto body_str = body_->GenJs();
    return fmt::format("catch ({}) {}", param_str, body_str);
  }
};

class TryStatementNode : public Node
{
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
  string GenJs() const override
  {
    auto block_str = block_->GenJs();
    auto handler_str = handler_->GenJs();
    auto finalizer_str = finalizer_->GenJs();
    return fmt::format("try {} {} finally {}", block_str, handler_str,
                       finalizer_str);
  }
};

class FunctionDeclarationNode : public Node
{
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
  string GenJs() const override
  {
    auto id_str = id_->GenJs();
    auto params_str = GenJsForVector(params_, " ");
    auto body_str = body_->GenJs();
    auto generator_str = generator_ ? "*" : "";
    auto async_str = async_ ? "async " : "";
    return fmt::format("{}function{} {}({}) {}", async_str, generator_str,
                       id_str, params_str, body_str);
  }
};

class FunctionExpressionNode : public Node
{
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
  string GenJs() const override
  {
    auto id_str = id_->GenJs();
    auto params_str = GenJsForVector(params_, " ");
    auto body_str = body_->GenJs();
    auto generator_str = generator_ ? "*" : "";
    auto async_str = async_ ? "async " : "";
    return fmt::format("{} function{} {}{} {}", async_str, generator_str,
                       id_str, params_str, body_str);
  }
};

class SoureType
{
  string source_;

public:
  SoureType(string source) : source_(source)
  {
  }

  const static SoureType kModule;
  const static SoureType kScript;
};

inline const SoureType SoureType::kModule{"module"};
inline const SoureType SoureType::kScript("script");

class ProgramNode : public Node
{
  SoureType source_type_;
  vector<shared_ptr<Node>> body_;

public:
  ProgramNode(SoureType source_type, vector<shared_ptr<Node>> body)
      : Node(NodeType::kProgram), source_type_(source_type), body_(move(body))
  {
  }
  SoureType source_type() const { return source_type_; }
  vector<shared_ptr<Node>> body() const { return body_; }
  string GenJs() const override
  {
    auto body_str = GenJsForVector(body_);
    return fmt::format("{}", body_str);
  }
};

class ImportKind
{
  string source_;

public:
  ImportKind(string source) : source_(source)
  {
  }

  string GenJs() const
  {
    return source_;
  }

  const static ImportKind kType;
  const static ImportKind kTypeOf;
  const static ImportKind kValue;
  const static ImportKind kNull;
};

inline const ImportKind ImportKind::kType("type");
inline const ImportKind ImportKind::kTypeOf("typeof");
inline const ImportKind ImportKind::kValue("value");
inline const ImportKind ImportKind::kNull("null");

class ImportDeclarationNode : public Node
{
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
  string GenJs() const override
  {
    auto import_kind_str = import_kind_.GenJs();
    auto specifiers = GenJsForVector(specifiers_, ",");
    auto source_str = source_->GenJs();
    return fmt::format("import {} from {}", specifiers, source_str);
  }
};

class ImportSpecifierNode : public Node
{
  shared_ptr<Node> imported_;
  shared_ptr<Node> local_;

public:
  ImportSpecifierNode(shared_ptr<Node> imported, shared_ptr<Node> local)
      : Node(NodeType::kImportSpecifier), imported_(move(imported)),
        local_(move(local)) {}
  shared_ptr<Node> imported() const { return imported_; }
  shared_ptr<Node> local() const { return local_; }
  string GenJs() const override
  {
    auto imported_str = imported_->GenJs();
    auto local_str = local_->GenJs();
    if (imported_str == local_str)
    {
      return fmt::format("{{ {} }}", local_str);
    }
    else
    {
      return fmt::format("{{ {} as {} }}", imported_str, local_str);
    }
  }
};

class ImportDefaultSpecifierNode : public Node
{
  shared_ptr<Node> local_;

public:
  ImportDefaultSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kImportDefaultSpecifier), local_(move(local)) {}
  shared_ptr<Node> local() const { return local_; }
  string GenJs() const override
  {
    auto local_str = local_->GenJs();
    return fmt::format("{}", local_str);
  }
};

class ImportNamespaceSpecifierNode : public Node
{
  shared_ptr<Node> local_;

public:
  ImportNamespaceSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kImportNamespaceSpecifier), local_(move(local)) {}
  shared_ptr<Node> local() const { return local_; }
  string GenJs() const override
  {
    auto local_str = local_->GenJs();
    return fmt::format("* as {}", local_str);
  }
};

class ExportSpecifierNode : public Node
{
  shared_ptr<Node> exported_;
  shared_ptr<Node> local_;

public:
  ExportSpecifierNode(shared_ptr<Node> exported, shared_ptr<Node> local)
      : Node(NodeType::kExportSpecifier), exported_(move(exported)),
        local_(move(local)) {}
  shared_ptr<Node> exported() const { return exported_; }
  shared_ptr<Node> local() const { return local_; }
  string GenJs() const override
  {
    auto exported_str = exported_->GenJs();
    auto local_str = local_->GenJs();
    if (exported_str == local_str)
    {
      return fmt::format("{} as {}", local_str, exported_str);
    }
    else
    {
      return fmt::format("{}", local_str);
    }
  }
};

class ExportDefaultSpecifierNode : public Node
{
  shared_ptr<Node> local_;

public:
  ExportDefaultSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kExportDefaultSpecifier), local_(move(local)) {}

  shared_ptr<Node> local() const { return local_; }
  string GenJs() const override
  {
    auto local_str = local_->GenJs();
    return fmt::format("default {}", local_str);
  }
};

class ExportNamespaceSpecifierNode : public Node
{
  shared_ptr<Node> local_;

public:
  ExportNamespaceSpecifierNode(shared_ptr<Node> local)
      : Node(NodeType::kExportNamespaceSpecifier), local_(move(local)) {}
  shared_ptr<Node> local() const { return local_; }
  string GenJs() const override
  {
    auto local_str = local_->GenJs();
    return fmt::format("* as {}", local_str);
  }
};

class ExportNamedDeclarationNode : public Node
{
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
  string GenJs() const override
  {
    if (declaration_)
    {
      return fmt::format("export {}", declaration_->GenJs());
    }
    else
    {
      auto specifiers_str = GenJsForVector(specifiers_, " ");
      auto source_str = source_ ? source_->GenJs() : "";
      if (source_str.empty())
      {
        return fmt::format("export {}", specifiers_str);
      }
      else
      {
        return fmt::format("export {} from {}", specifiers_str, source_str);
      }
    }
  }
};

class ExportDefaultDeclarationNode : public Node
{
  shared_ptr<Node> declaration_;

public:
  ExportDefaultDeclarationNode(shared_ptr<Node> declaration)
      : Node(NodeType::kExportDefaultDeclaration),
        declaration_(move(declaration)) {}
  shared_ptr<Node> declaration() const { return declaration_; }
  string GenJs() const override
  {
    auto declaration_str = declaration_->GenJs();
    return fmt::format("export default {}", declaration_str);
  }
};

class ExportAllDeclarationNode : public Node
{
  shared_ptr<Node> source_;

public:
  ExportAllDeclarationNode(shared_ptr<Node> source)
      : Node(NodeType::kExportAllDeclaration), source_(move(source)) {}
  shared_ptr<Node> source() const { return source_; }
  string GenJs() const
  {
    auto source_str = source_->GenJs();
    return fmt::format("export * from {}", source_str);
  }
};

class CallExpressionNode : public Node
{
  vector<shared_ptr<Node>> arguments_;
  shared_ptr<Node> callee_;

public:
  CallExpressionNode(shared_ptr<Node> callee,
                     vector<shared_ptr<Node>> arguments)
      : Node(NodeType::kCallExpression), callee_(callee),
        arguments_(move(arguments)) {}
  vector<shared_ptr<Node>> arguments() const { return arguments_; }
  shared_ptr<Node> callee() const { return callee_; }
  string GenJs() const
  {
    auto callee_str = callee_->GenJs();
    auto arguments_str = GenJsForVector(arguments_, ", ");
    return fmt::format("{}({})", callee_str, arguments_str);
  }
};

class ParenthesizedExpressionNode : public Node
{
  shared_ptr<Node> expression_;

public:
  ParenthesizedExpressionNode(shared_ptr<Node> expression)
      : Node(NodeType::kParenthesizedExpression),
        expression_(move(expression)) {}
  shared_ptr<Node> expression() const { return expression_; }
  string GenJs() const
  {
    auto expression_str = expression_->GenJs();
    return fmt::format("({})", expression_str);
  }
};

class Parser
{
  shared_ptr<Lexer> lexer_;
  map<BinaryOperator, int> binary_op_precendences_;

  map<BinaryOperator, int> kDefaultBinaryOpPrecendences = {
      {BinaryOperator::kLessThanOp, 5},
      {BinaryOperator::kLessLessOp, 5},
      {BinaryOperator::kAddOp, 10},
      {BinaryOperator::kSubOp, 10},
      {BinaryOperator::kMulOp, 20},
      {BinaryOperator::kDivOp, 20},
  };

public:
  Parser(shared_ptr<Lexer> lexer) : lexer_(move(lexer))
  {
    InstallBinaryOpPrecendences(kDefaultBinaryOpPrecendences);
  }

  Parser(string source) : lexer_(new Lexer(source))
  {
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
