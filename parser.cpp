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

unique_ptr<Node> Parser::ParseStringLiteral() {
  auto value = lexer_->value();
  lexer_->GetToken();
  return make_unique<StringLiteralNode>(value);
}

unique_ptr<Node> Parser::ParseNumericLiteral() {
  auto value = strtod(lexer_->value().c_str(), nullptr);
  lexer_->GetToken();
  return make_unique<NumericLiteralNode>(value);
}

unique_ptr<Node> Parser::ParseBooleanLiteral() {
  auto value_str = lexer_->value();
  lexer_->GetToken();
  if (value_str == "true") {
    return make_unique<BooleanLiteralNode>(true);
  } else {
    return make_unique<BooleanLiteralNode>(false);
  }
}

unique_ptr<Node> Parser::ParseNullLiteral() {
  lexer_->GetToken();
  return make_unique<NullLiteralNode>();
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

unique_ptr<Node> Parser::ParseBinaryExpression(unique_ptr<Node> left,
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
            make_unique<BinaryExpressionNode>(op, move(left), move(next_right));
      }
    } else {
      return left;
    }
  }
}

unique_ptr<Node> Parser::ParseIdentifier() {
  auto name = lexer_->value();
  lexer_->GetToken();
  return make_unique<IdentifierNode>(name);
}

unique_ptr<Node> Parser::ParseUnaryExpression() {
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
    return make_unique<UnaryExpressionNode>(UnaryOperator::kAddOp, move(expr));
  }
  case TokenType::kSubToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_unique<UnaryExpressionNode>(UnaryOperator::kSubOp, move(expr));
  }
  case TokenType::kExclaToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_unique<UnaryExpressionNode>(UnaryOperator::kExclaOp,
                                            move(expr));
  }
  case TokenType::kNegToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_unique<UnaryExpressionNode>(UnaryOperator::kNegOp, move(expr));
  }
  case TokenType::kTypeOfToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_unique<UnaryExpressionNode>(UnaryOperator::kTypeOfOp,
                                            move(expr));
  }
  case TokenType::kVoidToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_unique<UnaryExpressionNode>(UnaryOperator::kVoidOp, move(expr));
  }
  case TokenType::kDeleteToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_unique<UnaryExpressionNode>(UnaryOperator::kDeleteOp,
                                            move(expr));
  }
  case TokenType::kThrowToken: {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_unique<UnaryExpressionNode>(UnaryOperator::kThrowOp,
                                            move(expr));
  }
  default: {
    return nullptr;
  }
  }
}

unique_ptr<Node> Parser::ParseExpression() {
  auto left = ParseUnaryExpression();
  return ParseBinaryExpression(move(left), -1);
}

unique_ptr<Node> Parser::ParseExpressionStatement() {
  auto expression = ParseExpression();
  SKIP_SEMICOLON;
  return make_unique<ExpressionStatementNode>(move(expression));
}

unique_ptr<Node> Parser::ParseEmptyStatement() {
  SKIP_SEMICOLON;
  return make_unique<EmptyStatementNode>();
}

unique_ptr<Node> Parser::ParseDebuggerStatement() {
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_unique<DebuggerStatementNode>();
}

unique_ptr<Node> Parser::ParseStatement() {
  switch (lexer_->current_token()) {
  case TokenType::kSemiColonToken: {
    return ParseEmptyStatement();
  }
  default: {
    return ParseExpressionStatement();
  }
  }
}

unique_ptr<Node> Parser::ParseBlockStatement() {
  vector<unique_ptr<Node>> body;
  while (lexer_->current_token() != TokenType::kRightBraceToken) {
    auto statement = ParseStatement();
    body.push_back(move(statement));
  }
  lexer_->GetToken();
  return make_unique<BlockStatementNode>(move(body));
}

unique_ptr<Node> Parser::ParseReturnStatement() {
  lexer_->GetToken();
  auto argument = ParseExpression();
  SKIP_SEMICOLON;
  return make_unique<ReturnStatementNode>(move(argument));
}

unique_ptr<Node> Parser::ParseContinueStatement() {
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_unique<ContinueStatementNode>();
}

unique_ptr<Node> Parser::ParseBreakStatement() {
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_unique<BreakStatementNode>();
}

unique_ptr<Node> Parser::ParseIfStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  auto consequent = ParseStatement();
  unique_ptr<Node> alternate = nullptr;
  if (lexer_->current_token() == TokenType::kElseToken) {
    lexer_->GetToken();
    alternate = ParseStatement();
  }
  return make_unique<IfStatementNode>(move(test), move(consequent),
                                      move(alternate));
}

unique_ptr<Node> Parser::ParseSwitchNodeStatement() {
  lexer_->GetToken();
  unique_ptr<Node> test = nullptr;
  if (lexer_->current_token() != TokenType::kColonToken) {
    test = ParseExpression();
  }
  lexer_->GetToken();
  vector<unique_ptr<Node>> consequent;
  while (lexer_->current_token() != TokenType::kCaseToken &&
         lexer_->current_token() != TokenType::kDefaultToken &&
         lexer_->current_token() != TokenType::kRightBraceToken) {
    auto statement = ParseStatement();
    consequent.push_back(move(statement));
  }
  return make_unique<SwitchCaseNode>(move(test), move(consequent));
}

unique_ptr<Node> Parser::ParseSwitchStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  auto discriminant = ParseExpression();
  lexer_->GetToken();
  lexer_->GetToken();

  vector<unique_ptr<Node>> cases;
  while (lexer_->current_token() == TokenType::kCaseToken ||
         lexer_->current_token() == TokenType::kDefaultToken) {
    cases.push_back(ParseSwitchNodeStatement());
  }
  lexer_->GetToken();
  return make_unique<SwitchStatementNode>(move(discriminant), move(cases));
}

unique_ptr<Node> Parser::ParseWhileStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  auto body = ParseStatement();
  return make_unique<WhileStatementNode>(move(test), move(body));
}

unique_ptr<Node> Parser::ParseDoWhileStatement() {
  lexer_->GetToken();
  auto body = ParseStatement();
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  return make_unique<DoWhileStatementNode>(move(test), move(body));
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

unique_ptr<Node> Parser::ParseVariableDeclarator() {
  auto id = ParseIdentifier();
  unique_ptr<Node> init = nullptr;
  if (lexer_->current_token() == TokenType::kEqualToken) {
    init = ParseExpression();
  }
  return make_unique<VariableDeclaratorNode>(move(id), move(init));
}

unique_ptr<Node> Parser::ParseVariableDeclaration() {
  auto kind = GetVariableDeclarationKindFromToken(lexer_->current_token());
  lexer_->GetToken();
  vector<unique_ptr<Node>> declarations;
  while (1) {
    auto declaration = ParseVariableDeclarator();
    declarations.push_back(move(declaration));
    if (lexer_->current_token() != TokenType::kCommaToken) {
      break;
    }
  }
  SKIP_SEMICOLON;
  return make_unique<VariableDeclarationNode>(kind, move(declarations));
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

unique_ptr<Node> Parser::ParseForStatement() {
  lexer_->GetToken();
  lexer_->GetToken();
  unique_ptr<Node> init = nullptr;
  if (lexer_->current_token() != TokenType::kSemiColonToken) {
    if (CheckIsVariableDeclaration(lexer_->current_token())) {
      init = ParseVariableDeclaration();
    } else {
      init = ParseExpression();
    }
  }
  lexer_->GetToken();
  unique_ptr<Node> test = nullptr;
  if (lexer_->current_token() != TokenType::kSemiColonToken) {
    test = ParseExpression();
  }
  unique_ptr<Node> update = nullptr;
  lexer_->GetToken();
  if (lexer_->current_token() != TokenType::kRightBraceToken) {
    update = ParseExpression();
  }
  auto body = ParseStatement();
  return make_unique<ForStatementNode>(move(init), move(test), move(update),
                                       move(body));
}

unique_ptr<Node> Parser::ParseForInStatementOrForOfStatement() {
  lexer_->GetToken();
  bool await = false;
  if(lexer_->current_token()==TokenType::kAwaitToken){
    await = true;
    lexer_->GetToken();
  }
  lexer_->GetToken();
  unique_ptr<Node> left = nullptr;
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

unique_ptr<Node> Parser::ParseForInStatement(unique_ptr<Node> left) {
  lexer_->GetToken();
  auto right = ParseExpression();
  auto body = ParseStatement();
  return make_unique<ForInStatementNode>(move(left), move(right), move(body));
}

unique_ptr<Node> Parser::ParseForOfStatement(unique_ptr<Node> left, bool await) {
  lexer_->GetToken();
  auto right = ParseExpression();
  auto body = ParseStatement();
  return make_unique<ForOfStatementNode>(move(left), move(right), move(body),await);
}

unique_ptr<Node> Parser::Parse() {
  lexer_->GetToken();
  return ParseExpression();
}
