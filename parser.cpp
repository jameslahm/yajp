#include "parser.hpp"
#include "util.hpp"
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <utility>
#include <vector>

#define SKIP_SEMICOLON                                       \
  if (lexer_->current_token() == TokenType::kSemiColonToken) \
  {                                                          \
    lexer_->GetToken();                                      \
  }

#define SN shared_ptr<Node>
#define VSN vector<SN>
#define SVSN shared_ptr<VSN>

SN Parser::ParseStringLiteral()
{
  auto value = lexer_->value();
  lexer_->GetToken();
  return make_shared<StringLiteralNode>(value);
}

SN Parser::ParseNumericLiteral()
{
  auto value = strtod(lexer_->value().c_str(), nullptr);
  lexer_->GetToken();
  return make_shared<NumericLiteralNode>(value);
}

SN Parser::ParseBooleanLiteral()
{
  auto value_str = lexer_->value();
  lexer_->GetToken();
  if (value_str == "true")
  {
    return make_shared<BooleanLiteralNode>(true);
  }
  else
  {
    return make_shared<BooleanLiteralNode>(false);
  }
}

SN Parser::ParseNullLiteral()
{
  lexer_->GetToken();
  return make_shared<NullLiteralNode>();
}

void Parser::InstallBinaryOpPrecedences(
    map<BinaryOperator, int> binary_op_precedences)
{
  binary_op_precedences_ = binary_op_precedences;
}

int Parser::GetBinaryOpPrecedence(BinaryOperator op)
{
  auto iter = binary_op_precedences_.find(op);
  if (iter != binary_op_precedences_.end())
  {
    return iter->second;
  }
  else
  {
    return UNDEFINED;
  }
}

BinaryOperator Parser::GetBinaryOpFromToken(TokenType token)
{
  switch (token)
  {
  case TokenType::kAddToken:
  {
    return BinaryOperator::kAddOp;
  }
  case TokenType::kSubToken:
  {
    return BinaryOperator::kSubOp;
  }
  case TokenType::kMulToken:
  {
    return BinaryOperator::kMulOp;
  }
  case TokenType::kDivToken:
  {
    return BinaryOperator::kDivOp;
  }
  default:
  {
    UNREACHABLE;
  }
  }
}

bool Parser::CheckIsBianryOp(TokenType token)
{
  switch (token)
  {
  case TokenType::kAddToken:
  {
    return true;
  }
  case TokenType::kSubToken:
  {
    return true;
  }
  case TokenType::kMulToken:
  {
    return true;
  }
  case TokenType::kDivToken:
  {
    return true;
  }
  default:
  {
    return false;
  }
  }
}

SN Parser::ParseBinaryExpression(SN left,
                                               int precedence)
{
  while (1)
  {
    if (CheckIsBianryOp(lexer_->current_token()))
    {
      auto op = GetBinaryOpFromToken(lexer_->current_token());
      auto next_precedence = GetBinaryOpPrecedence(op);
      if (next_precedence <= precedence)
      {
        return left;
      }
      else
      {
        lexer_->GetToken();
        auto next_left = ParseUnaryExpression();
        auto next_right =
            ParseBinaryExpression(move(next_left), next_precedence);
        left =
            make_shared<BinaryExpressionNode>(op, move(left), move(next_right));
      }
    }
    else
    {
      return left;
    }
  }
}

SN Parser::ParseIdentifier()
{
  auto name = lexer_->value();
  lexer_->GetToken();
  return make_shared<IdentifierNode>(name);
}

SN Parser::ParseCallExpression(SN callee)
{
  auto arguments = ParseCallExpressionArguments();
  return make_shared<CallExpressionNode>(move(callee), move(arguments));
}

SVSN Parser::ParseCallExpressionArguments()
{
  lexer_->GetToken();
  SVSN params = make_shared<VSN>();
  while (lexer_->current_token() != TokenType::kRightParenToken)
  {
    auto param = ParseIdentifier();
    params->push_back(move(param));
    if (lexer_->current_token() == TokenType::kCommaToken)
    {
      lexer_->GetToken();
    }
  }
  lexer_->GetToken();
  return params;
}

SN Parser::ParseIdentifierOrCallExpression()
{
  auto name = lexer_->value();
  auto identifier = make_shared<IdentifierNode>(name);
  lexer_->GetToken();
  if (lexer_->current_token() == TokenType::kLeftParenToken)
  {
    return ParseCallExpression(move(identifier));
  }
  else
  {
    return identifier;
  }
}

SN Parser::ParseUnaryExpression()
{
  switch (lexer_->current_token())
  {
  case TokenType::kLeftParenToken:
  {
    lexer_->GetToken();
    auto expression = ParseExpression();
    lexer_->GetToken();
    return make_shared<ParenthesizedExpressionNode>(move(expression));
  }
  case TokenType::kIdentifierToken:
  {
    return ParseIdentifierOrCallExpression();
  }
  case TokenType::kNumericToken:
  {
    return ParseNumericLiteral();
  }
  case TokenType::kStringToken:
  {
    return ParseStringLiteral();
  }
  case TokenType::kAddToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kAddOp, move(expr));
  }
  case TokenType::kSubToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kSubOp, move(expr));
  }
  case TokenType::kExclaToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kExclaOp,
                                            move(expr));
  }
  case TokenType::kNegToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kNegOp, move(expr));
  }
  case TokenType::kTypeOfToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kTypeOfOp,
                                            move(expr));
  }
  case TokenType::kVoidToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kVoidOp, move(expr));
  }
  case TokenType::kDeleteToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kDeleteOp,
                                            move(expr));
  }
  case TokenType::kThrowToken:
  {
    lexer_->GetToken();
    auto expr = ParseUnaryExpression();
    return make_shared<UnaryExpressionNode>(UnaryOperator::kThrowOp,
                                            move(expr));
  }
  default:
  {
    return nullptr;
  }
  }
}

SN Parser::ParseExpression()
{
  auto left = ParseUnaryExpression();
  return ParseBinaryExpression(move(left), -1);
}

SN Parser::ParseExpressionStatement()
{
  auto expression = ParseExpression();
  SKIP_SEMICOLON;
  return make_shared<ExpressionStatementNode>(move(expression));
}

SN Parser::ParseEmptyStatement()
{
  SKIP_SEMICOLON;
  return make_shared<EmptyStatementNode>();
}

SN Parser::ParseDebuggerStatement()
{
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_shared<DebuggerStatementNode>();
}

SN Parser::ParseStatement()
{
  switch (lexer_->current_token())
  {
  case TokenType::kSemiColonToken:
  {
    return ParseEmptyStatement();
  }
  case TokenType::kFunctionToken:
  {
    return ParseFunctionDeclaration();
  }
  case TokenType::kVarToken:
  case TokenType::kConstToken:
  case TokenType::kLetToken:
  {
    return ParseVariableDeclaration();
  }
  case TokenType::kLeftBraceToken:
  {
    return ParseBlockStatement();
  }
  case TokenType::kReturnToken:
  {
    return ParseReturnStatement();
  }
  case TokenType::kThrowToken:
  {
    return ParseThrowStatement();
  }
  case TokenType::kContinueToken:
  {
    return ParseContinueStatement();
  }
  case TokenType::kBreakToken:
  {
    return ParseBreakStatement();
  }
  default:
  {
    return ParseExpressionStatement();
  }
  }
}

SN Parser::ParseBlockStatement()
{
  lexer_->GetToken();
  SVSN body = make_shared<VSN>();
  while (lexer_->current_token() != TokenType::kRightBraceToken)
  {
    auto statement = ParseStatement();
    body->push_back(move(statement));
  }
  lexer_->GetToken();
  return make_shared<BlockStatementNode>(move(body));
}

SN Parser::ParseReturnStatement()
{
  lexer_->GetToken();
  auto argument = ParseExpression();
  SKIP_SEMICOLON;
  return make_shared<ReturnStatementNode>(move(argument));
}

SN Parser::ParseContinueStatement()
{
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_shared<ContinueStatementNode>();
}

SN Parser::ParseBreakStatement()
{
  lexer_->GetToken();
  SKIP_SEMICOLON;
  return make_shared<BreakStatementNode>();
}

SN Parser::ParseIfStatement()
{
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  auto consequent = ParseStatement();
  SN alternate = nullptr;
  if (lexer_->current_token() == TokenType::kElseToken)
  {
    lexer_->GetToken();
    alternate = ParseStatement();
  }
  return make_shared<IfStatementNode>(move(test), move(consequent),
                                      move(alternate));
}

SN Parser::ParseSwitchNodeStatement()
{
  lexer_->GetToken();
  SN test = nullptr;
  if (lexer_->current_token() != TokenType::kColonToken)
  {
    test = ParseExpression();
  }
  lexer_->GetToken();
  SVSN consequent = make_shared<VSN>();
  while (lexer_->current_token() != TokenType::kCaseToken &&
         lexer_->current_token() != TokenType::kDefaultToken &&
         lexer_->current_token() != TokenType::kRightBraceToken)
  {
    auto statement = ParseStatement();
    consequent->push_back(move(statement));
  }
  return make_shared<SwitchCaseNode>(move(test), move(consequent));
}

SN Parser::ParseSwitchStatement()
{
  lexer_->GetToken();
  lexer_->GetToken();
  auto discriminant = ParseExpression();
  lexer_->GetToken();
  lexer_->GetToken();

  SVSN cases = make_shared<VSN>();
  while (lexer_->current_token() == TokenType::kCaseToken ||
         lexer_->current_token() == TokenType::kDefaultToken)
  {
    cases->push_back(ParseSwitchNodeStatement());
  }
  lexer_->GetToken();
  return make_shared<SwitchStatementNode>(move(discriminant), move(cases));
}

SN Parser::ParseWhileStatement()
{
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  auto body = ParseStatement();
  return make_shared<WhileStatementNode>(move(test), move(body));
}

SN Parser::ParseDoWhileStatement()
{
  lexer_->GetToken();
  auto body = ParseStatement();
  lexer_->GetToken();
  lexer_->GetToken();
  auto test = ParseExpression();
  lexer_->GetToken();
  return make_shared<DoWhileStatementNode>(move(test), move(body));
};

VariableDeclarationKind
Parser::GetVariableDeclarationKindFromToken(TokenType token)
{
  switch (token)
  {
  case TokenType::kConstToken:
  {
    return VariableDeclarationKind::kConst;
  }
  case TokenType::kLetToken:
  {
    return VariableDeclarationKind::kLet;
  }
  case TokenType::kVarToken:
  {
    return VariableDeclarationKind::kVar;
  }
  default:
  {
    UNREACHABLE;
  }
  }
}

SN Parser::ParseVariableDeclarator()
{
  auto id = ParseIdentifier();
  SN init = nullptr;
  if (lexer_->current_token() == TokenType::kEqualToken)
  {
    lexer_->GetToken();
    init = ParseExpression();
  }
  return make_shared<VariableDeclaratorNode>(move(id), move(init));
}

SN Parser::ParseVariableDeclaration()
{
  auto kind = GetVariableDeclarationKindFromToken(lexer_->current_token());
  lexer_->GetToken();
  SVSN declarations = make_shared<VSN>();
  while (1)
  {
    auto declaration = ParseVariableDeclarator();
    declarations->push_back(move(declaration));
    if (lexer_->current_token() == TokenType::kCommaToken)
    {
      lexer_->GetToken();
      continue;
    }
    else
    {
      break;
    }
  }
  SKIP_SEMICOLON;
  return make_shared<VariableDeclarationNode>(kind, move(declarations));
}

bool Parser::CheckIsVariableDeclaration(TokenType token)
{
  switch (token)
  {
  case TokenType::kVarToken:
  case TokenType::kLetToken:
  case TokenType::kConstToken:
  {
    return true;
  }
  default:
  {
    return false;
  }
  }
}

SN Parser::ParseForStatement()
{
  lexer_->GetToken();
  lexer_->GetToken();
  SN init = nullptr;
  if (lexer_->current_token() != TokenType::kSemiColonToken)
  {
    if (CheckIsVariableDeclaration(lexer_->current_token()))
    {
      init = ParseVariableDeclaration();
    }
    else
    {
      init = ParseExpression();
    }
  }
  lexer_->GetToken();
  SN test = nullptr;
  if (lexer_->current_token() != TokenType::kSemiColonToken)
  {
    test = ParseExpression();
  }
  SN update = nullptr;
  lexer_->GetToken();
  if (lexer_->current_token() != TokenType::kRightBraceToken)
  {
    update = ParseExpression();
  }
  auto body = ParseStatement();
  return make_shared<ForStatementNode>(move(init), move(test), move(update),
                                       move(body));
}

SN Parser::ParseForInStatementOrForOfStatement()
{
  lexer_->GetToken();
  bool await = false;
  if (lexer_->current_token() == TokenType::kAwaitToken)
  {
    await = true;
    lexer_->GetToken();
  }
  lexer_->GetToken();
  SN left = nullptr;
  if (CheckIsVariableDeclaration(lexer_->current_token()))
  {
    left = ParseVariableDeclaration();
  }
  else
  {
    left = ParseExpression();
  }

  switch (lexer_->current_token())
  {
  case TokenType::kInToken:
  {
    return ParseForInStatement(move(left));
  }
  case TokenType::kOfToken:
  {
    return ParseForOfStatement(move(left), await);
  }
  default:
  {
    UNREACHABLE;
  }
  }
}

SN Parser::ParseForInStatement(SN left)
{
  lexer_->GetToken();
  auto right = ParseExpression();
  auto body = ParseStatement();
  return make_shared<ForInStatementNode>(move(left), move(right), move(body));
}

SN Parser::ParseForOfStatement(SN left,
                                             bool await)
{
  lexer_->GetToken();
  auto right = ParseExpression();
  auto body = ParseStatement();
  return make_shared<ForOfStatementNode>(move(left), move(right), move(body),
                                         await);
}

SN Parser::ParseThrowStatement()
{
  lexer_->GetToken();
  auto argument = ParseExpression();
  return make_shared<ThrowStatementNode>(move(argument));
}

SN Parser::ParseCatchClause()
{
  lexer_->GetToken();
  lexer_->GetToken();
  auto param = ParseIdentifier();
  lexer_->GetToken();
  auto body = ParseStatement();
  return make_shared<CatchClauseNode>(move(param), move(body));
}

SN Parser::ParseTryStatement()
{
  lexer_->GetToken();
  auto block = ParseStatement();
  SN handler = nullptr;
  SN finalizer = nullptr;
  if (lexer_->current_token() == TokenType::kCatchToken)
  {
    handler = ParseCatchClause();
  }
  if (lexer_->current_token() == TokenType::kFinallyToken)
  {
    lexer_->GetToken();
    finalizer = ParseStatement();
  }
  return make_shared<TryStatementNode>(move(block), move(handler),
                                       move(finalizer));
}

SVSN Parser::ParseFunctionParams()
{
  lexer_->GetToken();
  SVSN params = make_shared<VSN>();
  while (lexer_->current_token() != TokenType::kRightParenToken)
  {
    auto param = ParseIdentifier();
    params->push_back(move(param));
    if (lexer_->current_token() == TokenType::kCommaToken)
    {
      lexer_->GetToken();
    }
  }
  lexer_->GetToken();
  return params;
}

SN Parser::ParseFunctionDeclaration()
{
  bool generator = false;
  bool async = false;
  if (lexer_->current_token() == TokenType::kAsyncToken)
  {
    async = true;
    lexer_->GetToken();
  }
  lexer_->GetToken();
  if (lexer_->current_token() == TokenType::kMulToken)
  {
    generator = true;
    lexer_->GetToken();
  }
  auto id = ParseIdentifier();
  auto params = ParseFunctionParams();
  auto body = ParseStatement();
  return make_shared<FunctionDeclarationNode>(move(id), move(params),
                                              move(body), generator, async);
}

SN Parser::ParseFunctionExpression()
{
  bool generator = false;
  bool async = false;
  if (lexer_->current_token() == TokenType::kAsyncToken)
  {
    async = true;
    lexer_->GetToken();
  }
  lexer_->GetToken();
  if (lexer_->current_token() == TokenType::kMulToken)
  {
    generator = true;
    lexer_->GetToken();
  }
  SN id = nullptr;
  if (lexer_->current_token() != TokenType::kLeftParenToken)
  {
    id = ParseIdentifier();
  }
  auto params = ParseFunctionParams();
  auto body = ParseStatement();
  return make_shared<FunctionDeclarationNode>(move(id), move(params),
                                              move(body), generator, async);
}

SN Parser::ParseImportSpecifier()
{
  auto imported = ParseIdentifier();
  SN local = imported;
  if (lexer_->current_token() == TokenType::kAsToken)
  {
    lexer_->GetToken();
    local = ParseIdentifier();
  }
  return make_shared<ImportSpecifierNode>(move(imported), move(local));
}

SN Parser::ParseImportDefaultSpecifier()
{
  auto local = ParseIdentifier();
  return make_shared<ImportDefaultSpecifierNode>(move(local));
}

SN Parser::ParseImportNamespaceSpecifier()
{
  lexer_->GetToken();
  lexer_->GetToken();
  auto local = ParseIdentifier();
  return make_shared<ImportNamespaceSpecifierNode>(move(local));
}

SN Parser::ParseImportDeclaration()
{
  lexer_->GetToken();
  SVSN specifiers = make_shared<VSN>();
  while (lexer_->current_token() != TokenType::kFromToken)
  {
    if (lexer_->current_token() == TokenType::kMulToken)
    {
      auto specifier = ParseImportNamespaceSpecifier();
      specifiers->push_back(move(specifier));
    }
    else if (lexer_->current_token() == TokenType::kIdentifierToken)
    {
      auto specifier = ParseImportDefaultSpecifier();
      specifiers->push_back(move(specifier));
    }
    else if (lexer_->current_token() == TokenType::kLeftBraceToken)
    {
      lexer_->GetToken();
      while (lexer_->current_token() != TokenType::kRightBraceToken)
      {
        auto specifier = ParseImportSpecifier();
        specifiers->push_back(move(specifier));
        if (lexer_->current_token() == TokenType::kCommaToken)
        {
          lexer_->GetToken();
        }
      }
    }
    if (lexer_->current_token() == TokenType::kCommaToken)
    {
      lexer_->GetToken();
    }
  }
  lexer_->GetToken();
  auto source = ParseStringLiteral();
  SKIP_SEMICOLON;
  return make_shared<ImportDeclarationNode>(ImportKind::kValue,
                                            move(specifiers), source);
}

SN Parser::ParseExportSpecifier()
{
  auto local = ParseIdentifier();
  SN exported = local;
  if (lexer_->current_token() == TokenType::kAsToken)
  {
    lexer_->GetToken();
    exported = ParseIdentifier();
  }
  return make_shared<ExportSpecifierNode>(move(exported), move(local));
}

SN Parser::ParseExportNamespaceSpecifier()
{
  lexer_->GetToken();
  auto exported = ParseIdentifier();
  return make_shared<ExportNamespaceSpecifierNode>(move(exported));
}

SN Parser::ParseExportNamedDeclarationOrExportAllDeclaration()
{
  SVSN specifiers = make_shared<VSN>();
  SN declaration = nullptr;
  SN source = nullptr;
  if (lexer_->current_token() == TokenType::kLeftBraceToken)
  {
    lexer_->GetToken();
    while (lexer_->current_token() != TokenType::kRightBraceToken)
    {
      auto specifier = ParseExportSpecifier();
      specifiers->push_back(move(specifier));
      if (lexer_->current_token() == TokenType::kCommaToken)
      {
        lexer_->GetToken();
      }
    }
  }
  else if (lexer_->current_token() == TokenType::kMulToken)
  {
    lexer_->GetToken();
    if (lexer_->current_token() == TokenType::kAsToken)
    {
      auto specifier = ParseExportNamespaceSpecifier();
      specifiers->push_back(move(specifier));
    }
    else
    {
      lexer_->GetToken();
      auto source = ParseIdentifier();
      return make_shared<ExportAllDeclarationNode>(move(source));
    }
  }
  else
  {
    declaration = ParseDeclaration();
  }
  if (lexer_->current_token() == TokenType::kFromToken)
  {
    lexer_->GetToken();
    source = ParseIdentifier();
  }
  SKIP_SEMICOLON;
  return make_shared<ExportNamedDeclarationNode>(
      move(declaration), move(specifiers), move(source));
}

SN Parser::ParseExportDefaultDeclaration()
{
  lexer_->GetToken();
  SN declaration = nullptr;
  if (lexer_->current_token() == TokenType::kFunctionToken)
  {
    declaration = ParseFunctionDeclaration();
  }
  else
  {
    declaration = ParseExpression();
  }
  return make_shared<ExportDefaultDeclarationNode>(move(declaration));
}

SN
Parser::ParseExportNamedDeclarationOrExportDefaultDeclaration()
{
  lexer_->GetToken();
  if (lexer_->current_token() == TokenType::kDefaultToken)
  {
    return ParseExportDefaultDeclaration();
  }
  else
  {
    return ParseExportNamedDeclarationOrExportAllDeclaration();
  }
}

SN Parser::ParseDeclaration()
{
  if (lexer_->current_token() == TokenType::kAsyncToken)
  {
    return ParseFunctionDeclaration();
  }
  if (lexer_->current_token() == TokenType::kFunctionToken)
  {
    return ParseFunctionDeclaration();
  }
  if (lexer_->current_token() == TokenType::kVarToken)
  {
    return ParseVariableDeclaration();
  }
  if (lexer_->current_token() == TokenType::kConstToken)
  {
    return ParseVariableDeclaration();
  }
  if (lexer_->current_token() == TokenType::kLetToken)
  {
    return ParseVariableDeclaration();
  }
  UNREACHABLE;
}

SN Parser::ParseProgram()
{
  SourceType source_type = SourceType::kModule;
  SVSN body = make_shared<VSN>();
  while (lexer_->current_token() != TokenType::kEofToken)
  {
    SN node;
    if (lexer_->current_token() == TokenType::kImportToken)
    {
      node = ParseImportDeclaration();
    }
    else if (lexer_->current_token() == TokenType::kExportToken)
    {
      node = ParseExportNamedDeclarationOrExportAllDeclaration();
    }
    else
    {
      node = ParseStatement();
    }
    body->push_back(move(node));
  }
  return make_shared<ProgramNode>(source_type, move(body));
}

SN Parser::Parse()
{
  lexer_->GetToken();
  return ParseProgram();
}