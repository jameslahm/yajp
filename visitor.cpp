#include "parser.hpp"
#include "visitor.hpp"

void Visitor::visitIdentifierNode(shared_ptr<IdentifierNode> node){

}

void Visitor::visitNullLiteralNode(shared_ptr<NullLiteralNode> node){

}

void Visitor::visitStringLiteralNode(shared_ptr<StringLiteralNode> node){

}

void Visitor::visitBooleanLiteralNode(shared_ptr<BooleanLiteralNode> node){

}

void Visitor::visitNumericLiteralNode(shared_ptr<NumericLiteralNode> node){

}

void Visitor::visitUnaryExpressionNode(shared_ptr<UnaryExpressionNode> node){
  node->argument()->Accept(*this);
}

void Visitor::visitBinaryExpressionNode(shared_ptr<BinaryExpressionNode> node){
  node->left()->Accept(*this);
  node->right()->Accept(*this);
}

void Visitor::visitExpressionStatementNode(shared_ptr<ExpressionStatementNode> node){
  node->expression()->Accept(*this);
}

void Visitor::visitBlockStatementNode(shared_ptr<BlockStatementNode> node){
  auto body = node->body();
  for(int index=0;index<body.size();index++){
    body[index]->Accept(*this);
  }
}

void Visitor::visitDebuggerStatementNode(shared_ptr<DebuggerStatementNode> node){

}

void Visitor::visitEmptyStatementNode(shared_ptr<EmptyStatementNode> node){

}

void Visitor::visitReturnStatementNode(shared_ptr<ReturnStatementNode> node){
  node->argument()->Accept(*this);
}

void Visitor::visitContinueStatementNode(shared_ptr<ContinueStatementNode> node){

}

void Visitor::visitBreakStatementNode(shared_ptr<BreakStatementNode> node){

}

void Visitor::visitIfStatementNode(shared_ptr<IfStatementNode> node){
  node->test()->Accept(*this);
  node->consequent()->Accept(*this);
  node->alternate()->Accept(*this);
}

void Visitor::visitSwitchStatementNode(shared_ptr<SwitchStatementNode> node){
  node->discriminant()->Accept(*this);
  auto cases = node->cases();
  for(int index=0;index<cases.size();index++){
    cases[index]->Accept(*this);
  }
}

void Visitor::visitSwitchCaseNode(shared_ptr<SwitchCaseNode> node){
  node->test()->Accept(*this);
  auto consequent = node->consequent();
  for(int index=0;index<consequent.size();index++){
    consequent[index]->Accept(*this);
  }
}

void Visitor::visitWhileStatementNode(shared_ptr<WhileStatementNode> node){
  node->test()->Accept(*this);
  node->body()->Accept(*this);
}

void Visitor::visitDoWhileStatementNode(shared_ptr<DoWhileStatementNode> node){
  node->test()->Accept(*this);
  node->body()->Accept(*this);
}

void Visitor::visitForStatementNode(shared_ptr<ForStatementNode> node){
  node->init()->Accept(*this);
  node->test()->Accept(*this);
  node->update()->Accept(*this);
  node->body()->Accept(*this);
}

void Visitor::visitVariableDeclarationNode(shared_ptr<VariableDeclarationNode> node){
  auto declarations = node->declarations();
  for(int index=0;index<declarations.size();index++){
    declarations[index]->Accept(*this);
  }
}

void Visitor::visitVariableDeclaratorNode(shared_ptr<VariableDeclaratorNode> node){
  node->id()->Accept(*this);
  node->init()->Accept(*this);
}

void Visitor::visitForInStatementNode(shared_ptr<ForInStatementNode> node){
  node->left()->Accept(*this);
  node->right()->Accept(*this);
  node->body()->Accept(*this);
}

void Visitor::visitForOfStatementNode(shared_ptr<ForOfStatementNode> node){
  node->left()->Accept(*this);
  node->right()->Accept(*this);
  node->body()->Accept(*this);
}

void Visitor::visitThrowStatementNode(shared_ptr<ThrowStatementNode> node){
  node->argument()->Accept(*this);
}

void Visitor::visitCatchClauseNode(shared_ptr<CatchClauseNode> node){
  node->param()->Accept(*this);
  node->body()->Accept(*this);
}

void Visitor::visitTryStatementNode(shared_ptr<TryStatementNode> node){
  node->block()->Accept(*this);
  node->handler()->Accept(*this);
  node->finalizer()->Accept(*this);
}

void Visitor::visitFunctionDeclarationNode(shared_ptr<FunctionDeclarationNode> node){
  node->id()->Accept(*this);
  auto params = node->params();
  for(int index=0;index<params.size();index++){
    params[index]->Accept(*this);
  }
  node->body()->Accept(*this);
}

void Visitor::visitFunctionExpressionNode(shared_ptr<FunctionExpressionNode> node){
  node->id()->Accept(*this);
  auto params = node->params();
  for(int index=0;index<params.size();index++){
    params[index]->Accept(*this);
  }
  node->body()->Accept(*this);
}

void Visitor::visitProgramNode(shared_ptr<ProgramNode> node){
  auto body = node->body();
  for(int index=0;index<body.size();index++){
    body[index]->Accept(*this);
  }
}

void Visitor::visitImportDeclarationNode(shared_ptr<ImportDeclarationNode> node){
  auto specifiers = node->specifiers();
  for(int index=0;index<specifiers.size();index++){
    specifiers[index]->Accept(*this);
  }
  node->source()->Accept(*this);
}

void Visitor::visitImportSpecifierNode(shared_ptr<ImportSpecifierNode> node){
  node->imported()->Accept(*this);
  node->local()->Accept(*this);
}

void Visitor::visitImportDefaultSpecifierNode(shared_ptr<ImportDefaultSpecifierNode> node){
  node->local()->Accept(*this);
}

void Visitor::visitImportNamespaceSpecifierNode(shared_ptr<ImportNamespaceSpecifierNode> node){
  node->local()->Accept(*this);
}

void Visitor::visitExportSpecifierNode(shared_ptr<ExportSpecifierNode> node){
  node->exported()->Accept(*this);
  node->local()->Accept(*this);
}

void Visitor::visitExportDefaultSpecifierNode(shared_ptr<ExportDefaultSpecifierNode> node){
  node->local()->Accept(*this);
}

void Visitor::visitExportNamespaceSpecifierNode(shared_ptr<ExportNamespaceSpecifierNode> node){
  node->local()->Accept(*this);
}

void Visitor::visitExportNamedDeclarationNode(shared_ptr<ExportNamedDeclarationNode> node){
  node->declaration()->Accept(*this);
  auto specifiers = node->specifiers();
  for(int index=0;index<specifiers.size();index++){
    specifiers[index]->Accept(*this);
  }
  node->source()->Accept(*this);
}

void Visitor::visitExportDefaultDeclarationNode(shared_ptr<ExportDefaultDeclarationNode> node){
  node->declaration()->Accept(*this);
}

void Visitor::visitExportAllDeclarationNode(shared_ptr<ExportAllDeclarationNode> node){
  node->source()->Accept(*this);
}

void Visitor::visitCallExpressionNode(shared_ptr<CallExpressionNode> node){
  node->callee()->Accept(*this);
  auto arguments = node->arguments();
  for(int index=0;index<arguments.size();index++){
    arguments[index]->Accept(*this);
  }
}

void Visitor::visitParenthesizedExpressionNode(shared_ptr<ParenthesizedExpressionNode> node){
  node->expression()->Accept(*this);
}

