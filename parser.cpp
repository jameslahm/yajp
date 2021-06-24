#include "parser.hpp"
#include "util.hpp"
#include <cstdlib>
#include <memory>
#include <utility>
#include <vector>

#define SKIP_SEMICOLON                                                         \
  if (lexer_->current_token() == TokenType::kSemiColonToken) {                 \
    lexer_->GetToken();                                                        \
  }

shared_ptr<Node> Parser::ParseStringLiteral() {
  auto value = lexer_->value();
  lexer_->GetToken();
  return make_shared<StringLiteralNode>(value);
}

shared_ptr<Node> Parser::ParseNumericLiteral() {
  auto value = strtod(lexer_->value().c_str(), nullptr);
  lexer_->GetToken();
  return make_shared<NumericLiteralNode>(value);
}

shared_ptr<Node> Parser::ParseBooleanLiteral() {
  auto value_str = lexer_->value();
  lexer_->GetToken();
  if (value_str == "true") {
    return make_shared<BooleanLiteralNode>(true);
  } else {
    return make_shared<BooleanLiteralNode>(false);
  }
}

shared_ptr<Node> Parser::ParseNullLiteral() {
  lexer_->GetToken();
  return make_shared<NullLiteralNode>();
}

void Parser::InstallBinaryOpPrecendences(
    map<BinaryOperator, int> binary_op_precendences) {
  binary_op_precendences_ = binary_op_precendences;
}

int Parser::GetBinaryOpPrecendence(BinaryOperator op) {
  auto iter = binary_op_precendences_.find(op);
  if (iter != binary_op_precendences_.end()) {
    return iter->second;
  } else {
    return UNDEFINED;
  }
}

BinaryOperator Parser::GetBinaryOpFromToken(TokenType token) {
  switch (token) {
  case TokenType::kAddToken: {
    return BinaryOperator::kAddOp;
  }
  case TokenType::kSubToken: {
    return BinaryOperator::kSubOp;
  }
  case TokenType::kMulToken: {
    return BinaryOperator::kMulOp;
  }
  case TokenType::kDivToken: {
    return BinaryOperator::kDivOp;
  }
  default: {
    UNREACHABLE;
  }
  }
}

bool Parser::CheckIsBianryOp(TokenType token) {
  switch (token) {
  case TokenType::kAddToken: {
    return true;
  }
  case TokenType::kSubToken: {
    return true;
  }
  case TokenType::kMulToken: {
    return true;
  }
  case TokenType::kDivToken: {
    return true;
  }
  default: {
    return false;
  }
  }
}

shared_ptr<Node> Parser::ParseBinaryExpression(shared_ptr<Node> left,
                                               int precendence) {
  while (1) {
    if (CheckIsBianryOp(lexer_->current_token())) {
      auto op = GetBinaryOpFromToken(lexer_->current_token());
      auto next_precendence = GetBinaryOpPrecendence(op);
      if (next_precendence <= precendence) {
        return left;
      } else {
        lexer_->GetToken();
        auto next_left = ParseUnaryExpression();
        auto next_right =
            ParseBinaryExpression(move(next_left), next_precendence);
        left =
            make_shared<BinaryExpressionNode>(op, move(left), move(next_right));
      }
    } else {
      return left;
    }
  }
}

shared_ptr<Node> Parser::ParseIdentifier() {
  auto name = lexer_->value();
  lexer_->GetToken();
  return make_shared<IdentifierNode>(name);
}

shared_ptr<Node> Parser::ParseUnaryExpression() {
  switch (lexer_->current_token()) {
  case TokenType::kIdentifierToken: {
    return ParseIdentifier();
  }
  case TokenType::kNumericToken: {
    return ParseNumericLiteral();
  }
  case TokenType::kStringToken: {
    return ParseStringLiteral();
  }
  case TokenType::kAddToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kAddOp, move(expr));
  }
  case TokenType::kSubToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kSubOp, move(expr));
  }
  case TokenType::kExclaToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kExclaOp,
                                            move(expr));
  }
  case TokenType::kNegToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kNegOp, move(expr));
  }
  case TokenType::kTypeOfToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kTypeOfOp,
                                            move(expr));
  }
  case TokenType::kVoidToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kVoidOp, move(expr));
  }
  case TokenType::kDeleteToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kDeleteOp,
                                            move(expr));
  }
  case TokenType::kThrowToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kThrowOp,
                                            move(expr));
  }
  default: {
    return nullptr;
  }
  }
}

shared_ptr<Node> Parser::ParseExpression() {
  auto left = ParseUnaryExpression();
  return ParseBinaryExpression(move(left), -1);
}

shared_ptr<Node> Parser::ParseExpressionStatement() {
  auto expression = ParseExpression();
  SKIP_SEMICOLON;
  return make_shared<ExpressionStatementNode>(move(expression));
}

shared_ptr<Node> Parser::ParseEmptyStatement() {
  SKIP_SEMICOLON;
  return make_shared<EmptyStatementNode>();
}

shared_ptr<Node> Parser::ParseDebuggerStatement() {
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_shared<DebuggerStatementNode>();
}

shared_ptr<Node> Parser::ParseStatement() {
  switch (lexer_->current_token()) {
  case TokenType::kSemiColonToken: {
    return ParseEmptyStatement();
  }
  default: {
    return ParseExpressionStatement();
  }
  }
}

shared_ptr<Node> Parser::ParseBlockStatement() {
  vector<shared_ptr<Node>> body;
  while (lexer_->current_token() != TokenType::kRightBraceToken) {
    auto statement = ParseStatement();
    body.push_back(move(statement));
  }
  lexer_->GetToken();
  return make_shared<BlockStatementNode>(move(body));
}

shared_ptr<Node> Parser::ParseReturnStatement() {
  lexer_->GetToken();
  auto argument = ParseExpression();
  SKIP_SEMICOLON;
  return make_shared<ReturnStatementNode>(move(argument));
}

shared_ptr<Node> Parser::ParseContinueStatement() {
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_shared<ContinueStatementNode>();
}

shared_ptr<Node> Parser::ParseBreakStatement() {
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_shared<BreakStatementNode>();
}

shared_ptr<Node> Parser::ParseIfStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  auto consequent = ParseStatement();
  shared_ptr<Node> alternate = nullptr;
  if (lexer_->current_token() == TokenType::kElseToken) {
    lexer_->GetToken();
    alternate = ParseStatement();
  }
  return make_shared<IfStatementNode>(move(test), move(consequent),
                                      move(alternate));
}

shared_ptr<Node> Parser::ParseSwitchNodeStatement() {
  lexer_->GetToken();
  shared_ptr<Node> test = nullptr;
  if (lexer_->current_token() != TokenType::kColonToken) {
    test = ParseExpression();
  }
  lexer_->GetToken();
  vector<shared_ptr<Node>> consequent;
  while (lexer_->current_token() != TokenType::kCaseToken &&
         lexer_->current_token() != TokenType::kDefaultToken &&
         lexer_->current_token() != TokenType::kRightBraceToken) {
    auto statement = ParseStatement();
    consequent.push_back(move(statement));
  }
  return make_shared<SwitchCaseNode>(move(test), move(consequent));
}

shared_ptr<Node> Parser::ParseSwitchStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  auto discriminant = ParseExpression();
  lexer_->GetToken();
  lexer_->GetToken();

  vector<shared_ptr<Node>> cases;
  while (lexer_->current_token() == TokenType::kCaseToken ||
         lexer_->current_token() == TokenType::kDefaultToken) {
    cases.push_back(ParseSwitchNodeStatement());
  }
  lexer_->GetToken();
  return make_shared<SwitchStatementNode>(move(discriminant), move(cases));
}

shared_ptr<Node> Parser::ParseWhileStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  auto body = ParseStatement();
  return make_shared<WhileStatementNode>(move(test), move(body));
}

shared_ptr<Node> Parser::ParseDoWhileStatement() {
  lexer_->GetToken();
  auto body = ParseStatement();
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  return make_shared<DoWhileStatementNode>(move(test), move(body));
};

VariableDeclarationKind
Parser::GetVariableDeclarationKindFromToken(TokenType token) {
  switch (token) {
  case TokenType::kConstToken: {
    return VariableDeclarationKind::kConst;
  }
  case TokenType::kLetToken: {
    return VariableDeclarationKind::kLet;
  }
  case TokenType::kVarToken: {
    return VariableDeclarationKind::kVar;
  }
  default: {
    UNREACHABLE;
  }
  }
}

shared_ptr<Node> Parser::ParseVariableDeclarator() {
  auto id = ParseIdentifier();
  shared_ptr<Node> init = nullptr;
  if (lexer_->current_token() == TokenType::kEqualToken) {
    init = ParseExpression();
  }
  return make_shared<VariableDeclaratorNode>(move(id), move(init));
}

shared_ptr<Node> Parser::ParseVariableDeclaration() {
  auto kind = GetVariableDeclarationKindFromToken(lexer_->current_token());
  lexer_->GetToken();
  vector<shared_ptr<Node>> declarations;
  while (1) {
    auto declaration = ParseVariableDeclarator();
    declarations.push_back(move(declaration));
    if (lexer_->current_token() != TokenType::kCommaToken) {
      break;
    }
  }
  SKIP_SEMICOLON;
  return make_shared<VariableDeclarationNode>(kind, move(declarations));
}

bool Parser::CheckIsVariableDeclaration(TokenType token) {
  switch (token) {
  case TokenType::kVarToken:
  case TokenType::kLetToken:
  case TokenType::kConstToken: {
    return true;
  }
  default: {
    return false;
  }
  }
}

shared_ptr<Node> Parser::ParseForStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  shared_ptr<Node> init = nullptr;
  if (lexer_->current_token() != TokenType::kSemiColonToken) {
    if (CheckIsVariableDeclaration(lexer_->current_token())) {
      init = ParseVariableDeclaration();
    } else {
      init = ParseExpression();
    }
  }
  lexer_->GetToken();
  shared_ptr<Node> test = nullptr;
  if (lexer_->current_token() != TokenType::kSemiColonToken) {
    test = ParseExpression();
  }
  shared_ptr<Node> update = nullptr;
  lexer_->GetToken();
  if (lexer_->current_token() != TokenType::kRightBraceToken) {
    update = ParseExpression();
  }
  auto body = ParseStatement();
  return make_shared<ForStatementNode>(move(init), move(test), move(update),
                                       move(body));
}

shared_ptr<Node> Parser::ParseForInStatementOrForOfStatement() {
  lexer_->GetToken();
  bool await = false;
  if(lexer_->current_token()==TokenType::kAwaitToken){
    await = true;
    lexer_->GetToken();
  }
  lexer_->GetToken();
  shared_ptr<Node> left = nullptr;
  if (CheckIsVariableDeclaration(lexer_->current_token())) {
    left = ParseVariableDeclaration();
  } else {
    left = ParseExpression();
  }

  switch (lexer_->current_token()) {
  case TokenType::kInToken: {
    return ParseForInStatement(move(left));
  }
  case TokenType::kOfToken: {
    return ParseForOfStatement(move(left),await);
  }
  default: {
    UNREACHABLE;
  }
  }
}

shared_ptr<Node> Parser::ParseForInStatement(shared_ptr<Node> left) {
  lexer_->GetToken();
  auto right = ParseExpression();
  auto body = ParseStatement();
  return make_shared<ForInStatementNode>(move(left), move(right), move(body));
}

shared_ptr<Node> Parser::ParseForOfStatement(shared_ptr<Node> left, bool await) {
  lexer_->GetToken();
  auto right = ParseExpression();
  auto body = ParseStatement();
  return make_shared<ForOfStatementNode>(move(left), move(right), move(body),await);
}

shared_ptr<Node> Parser::Parse() {
  lexer_->GetToken();
  return ParseExpression();
}
