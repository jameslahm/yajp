
#pragma once
#include <memory>
using namespace std;
#include "macro.hpp"

class IdentifierNode;
class NullLiteralNode;
class StringLiteralNode;
class BooleanLiteralNode;
class NumericLiteralNode;
class UnaryExpressionNode;
class BinaryExpressionNode;
class ExpressionStatementNode;
class BlockStatementNode;
class DebuggerStatementNode;
class EmptyStatementNode;
class ReturnStatementNode;
class ContinueStatementNode;
class BreakStatementNode;
class IfStatementNode;
class SwitchStatementNode;
class SwitchCaseNode;
class WhileStatementNode;
class DoWhileStatementNode;
class ForStatementNode;
class VariableDeclarationNode;
class VariableDeclaratorNode;
class ForInStatementNode;
class ForOfStatementNode;
class ThrowStatementNode;
class CatchClauseNode;
class TryStatementNode;
class FunctionDeclarationNode;
class FunctionExpressionNode;
class ProgramNode;
class ImportDeclarationNode;
class ImportSpecifierNode;
class ImportDefaultSpecifierNode;
class ImportNamespaceSpecifierNode;
class ExportSpecifierNode;
class ExportDefaultSpecifierNode;
class ExportNamespaceSpecifierNode;
class ExportNamedDeclarationNode;
class ExportDefaultDeclarationNode;
class ExportAllDeclarationNode;
class CallExpressionNode;
class ParenthesizedExpressionNode;

#define VISIT(N) \
  virtual void visit##N(shared_ptr<N> node);

class Visitor
{
public:
  Visitor(){}
  NODES(VISIT)
};
