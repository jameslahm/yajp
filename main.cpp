#include "parser.hpp"
#include <emscripten/bind.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace emscripten;

#define BINDING_PROPERTY(T,P) \
  .property(#P,&T::P)

#define BINDING_CONSTRUCTOR(...) \
  .constructor<__VA_ARGS__>()

#define BINDING_NODE(T) \
  class_<T,base<Node>>(#T)

// Binding code
EMSCRIPTEN_BINDINGS(nodes) {
  #define BN BINDING_NODE
  #define BP BINDING_PROPERTY
  #define BC BINDING_CONSTRUCTOR
  #define SN shared_ptr<Node>
  #define VSN vector<shared_ptr<Node>>

  class_<Node>("Node")
  .constructor<NodeType>()
  .smart_ptr<std::shared_ptr<Node>>("Node")
  .property("type",&Node::type);

  BN(IdentifierNode) 
  BC(string)
  BP(IdentifierNode,name);

  BN(NumericLiteralNode)
  BC(double)
  BP(NumericLiteralNode,value);

  BN(NullLiteralNode)
  BC();

  BN(StringLiteralNode)
  BC(string)
  BP(StringLiteralNode,value);

  BN(BooleanLiteralNode)
  BC(bool)
  BP(BooleanLiteralNode,value);

  BN(UnaryExpressionNode)
  BC(UnaryOperator,SN)
  BP(UnaryExpressionNode,op);

  BN(BinaryExpressionNode)
  BC(BinaryOperator,SN,SN)
  BP(BinaryExpressionNode,op)
  BP(BinaryExpressionNode,left)
  BP(BinaryExpressionNode,right);

  BN(ExpressionStatementNode)
  BC(SN)
  BP(ExpressionStatementNode,expression);

  BN(BlockStatementNode)
  BC(VSN)
  BP(BlockStatementNode,body);

  BN(DebuggerStatementNode)
  BC();

  BN(EmptyStatementNode)
  BC();

  BN(ReturnStatementNode)
  BC(SN);

  BN(ContinueStatementNode)
  BC();

  BN(BreakStatementNode)
  BC();

  BN(IfStatementNode)
  BC(SN,SN,SN)
  BP(IfStatementNode,test)
  BP(IfStatementNode,consequent)
  BP(IfStatementNode,alternate);
  
  BN(SwitchStatementNode)
  BC(SN,VSN)
  BP(SwitchStatementNode,cases)
  BP(SwitchStatementNode,discriminant);

  BN(SwitchCaseNode)
  BC(SN,VSN)
  BP(SwitchCaseNode,consequent)
  BP(SwitchCaseNode,test);
  
  BN(WhileStatementNode)
  BC(SN,SN)
  BP(WhileStatementNode,test)
  BP(WhileStatementNode,body);

  BN(DoWhileStatementNode)
  BC(SN,SN)
  BP(DoWhileStatementNode,test)
  BP(DoWhileStatementNode,body);

  BN(ForStatementNode)
  BC(SN,SN,SN,SN)
  BP(ForStatementNode,init)
  BP(ForStatementNode,test);

  BN(VariableDeclarationNode)
  BC(VariableDeclarationKind,VSN)
  BP(VariableDeclarationNode,kind)
  BP(VariableDeclarationNode,declarations);

  BN(VariableDeclaratorNode)
  BC(SN,SN)
  BP(VariableDeclaratorNode,id)
  BP(VariableDeclaratorNode,init);

  BN(ForInStatementNode)
  BC(SN,SN,SN)
  BP(ForInStatementNode,left)
  BP(ForInStatementNode,right)
  BP(ForInStatementNode,body);

  BN(ForOfStatementNode)
  BC(SN,SN,SN,bool)
  BP(ForOfStatementNode,left)
  BP(ForOfStatementNode,right)
  BP(ForOfStatementNode,body)
  BP(ForOfStatementNode,await);

  BN(ProgramNode)
  BC(SoureType,VSN)
  BP(ProgramNode,source_type)
  BP(ProgramNode,body);

  BN(ImportDeclarationNode)
  BC(ImportKind,VSN,SN)
  BP(ImportDeclarationNode,specifiers)
  BP(ImportDeclarationNode,source);

  BN(ImportSpecifierNode)
  BC(SN,SN)
  BP(ImportSpecifierNode,imported)
  BP(ImportSpecifierNode,local);

  BN(ImportDefaultSpecifierNode)
  BC(SN)
  BP(ImportDefaultSpecifierNode,local);
  
  BN(ImportNamespaceSpecifierNode)
  BC(SN)
  BP(ImportNamespaceSpecifierNode,local);

  BN(ExportSpecifierNode)
  BC(SN,SN)
  BP(ExportSpecifierNode,exported)
  BP(ExportSpecifierNode,local);

  BN(ExportDefaultSpecifierNode)
  BC(SN)
  BP(ExportDefaultSpecifierNode,local);

  BN(ExportNamespaceSpecifierNode)
  BC(SN)
  BP(ExportNamespaceSpecifierNode,local);

  BN(ExportNamedDeclarationNode)
  BC(SN,VSN,SN)
  BP(ExportNamedDeclarationNode,declaration)
  BP(ExportNamedDeclarationNode,source)
  BP(ExportNamedDeclarationNode,specifiers);

  BN(ExportDefaultDeclarationNode)
  BC(SN)
  BP(ExportDefaultDeclarationNode,declaration);

  BN(ExportAllDeclarationNode)
  BC(SN)
  BP(ExportAllDeclarationNode,source);

  BN(CallExpressionNode)
  BC(SN,VSN)
  BP(CallExpressionNode,callee)
  BP(CallExpressionNode,arguments);

  BN(ParenthesizedExpressionNode)
  BC(SN)
  BP(ParenthesizedExpressionNode,expression);

  class_<Parser>("Parser").constructor<string>().function("Parse",
                                                          &Parser::Parse);

  #undef BN
  #undef BP
  #undef BC
}

EMSCRIPTEN_BINDINGS(stl_wrappers) {
    register_vector<shared_ptr<Node>>("vector<shared_ptr<Node>>");
}

#define BINDING_NODE_TYPE_ENUM(V) \
  .value(#V,NodeType::V)

EMSCRIPTEN_BINDINGS(node_types){
  enum_<NodeType>("NodeType")
    BINDING_NODE_TYPE_ENUM(kIdentifier)
    BINDING_NODE_TYPE_ENUM(kNullLiteral)
    BINDING_NODE_TYPE_ENUM(kStringLiteral)
    BINDING_NODE_TYPE_ENUM(kNumericLiteral)
    BINDING_NODE_TYPE_ENUM(kBooleanLiteral)
    BINDING_NODE_TYPE_ENUM(kUnaryExpression)
    BINDING_NODE_TYPE_ENUM(kBinaryExpression)
    BINDING_NODE_TYPE_ENUM(kExpressionStatement)
    BINDING_NODE_TYPE_ENUM(kBlockStatement)
    BINDING_NODE_TYPE_ENUM(kEmptyStatement)
    BINDING_NODE_TYPE_ENUM(kDebuggerStatement)
    BINDING_NODE_TYPE_ENUM(kReturnStatement)
    BINDING_NODE_TYPE_ENUM(kContinueStatement)
    BINDING_NODE_TYPE_ENUM(kBreakStatement)
    BINDING_NODE_TYPE_ENUM(kIfStatement)
    BINDING_NODE_TYPE_ENUM(kSwitchStatement)
    BINDING_NODE_TYPE_ENUM(kSwitchCase)
    BINDING_NODE_TYPE_ENUM(kWhileStatement)
    BINDING_NODE_TYPE_ENUM(kDoWhileStatement)
    BINDING_NODE_TYPE_ENUM(kForStatement)
    BINDING_NODE_TYPE_ENUM(kVariableDeclaration)
    BINDING_NODE_TYPE_ENUM(kVariableDeclarator)
    BINDING_NODE_TYPE_ENUM(kForInStatement)
    BINDING_NODE_TYPE_ENUM(kForOfStatement)
    BINDING_NODE_TYPE_ENUM(kThrowStatement)
    BINDING_NODE_TYPE_ENUM(kTryStatement)
    BINDING_NODE_TYPE_ENUM(kCatchClause)
    BINDING_NODE_TYPE_ENUM(kFunctionDeclaration)
    BINDING_NODE_TYPE_ENUM(kFunctionExpression)
    BINDING_NODE_TYPE_ENUM(kProgram)
    BINDING_NODE_TYPE_ENUM(kImportDeclaration)
    BINDING_NODE_TYPE_ENUM(kImportSpecifier)
    BINDING_NODE_TYPE_ENUM(kImportDefaultSpecifier)
    BINDING_NODE_TYPE_ENUM(kImportNamespaceSpecifier)
    BINDING_NODE_TYPE_ENUM(kExportSpecifier)
    BINDING_NODE_TYPE_ENUM(kExportNamespaceSpecifier)
    BINDING_NODE_TYPE_ENUM(kExportDefaultSpecifier)
    BINDING_NODE_TYPE_ENUM(kExportNamedDeclaration)
    BINDING_NODE_TYPE_ENUM(kExportDefaultDeclaration)
    BINDING_NODE_TYPE_ENUM(kExportAllDeclaration);
}

#define BINDING_BINARY_OP_ENUM(V) \
  .value(#V, BinaryOperator::V)

EMSCRIPTEN_BINDINGS(binary_ops){
  enum_<BinaryOperator>("BinaryOperator")
    BINDING_BINARY_OP_ENUM(kEqualEqualOp)
    BINDING_BINARY_OP_ENUM(kNotEqualOp)
    BINDING_BINARY_OP_ENUM(kEqualEqualEqualOp)
    BINDING_BINARY_OP_ENUM(kNotEqualEqualOp)
    BINDING_BINARY_OP_ENUM(kLessThanOp)
    BINDING_BINARY_OP_ENUM(kLessEqualOp)
    BINDING_BINARY_OP_ENUM(kGreaterThanOp)
    BINDING_BINARY_OP_ENUM(kGreaterEqualOp)
    BINDING_BINARY_OP_ENUM(kLessLessOp)
    BINDING_BINARY_OP_ENUM(kGreaterGreaterOp)
    BINDING_BINARY_OP_ENUM(kGreaterGreaterGreaterOp)
    BINDING_BINARY_OP_ENUM(kAddOp)
    BINDING_BINARY_OP_ENUM(kSubOp)
    BINDING_BINARY_OP_ENUM(kMulOp)
    BINDING_BINARY_OP_ENUM(kDivOp)
    BINDING_BINARY_OP_ENUM(kModOp);
}

#define BINDING_UUNARY_OP_ENUM(V) \
  .value(#V, UnaryOperator::V)

EMSCRIPTEN_BINDINGS(unary_ops){
  enum_<UnaryOperator>("UnaryOperator")
    BINDING_UUNARY_OP_ENUM(kSubOp)
    BINDING_UUNARY_OP_ENUM(kAddOp)
    BINDING_UUNARY_OP_ENUM(kExclaOp)
    BINDING_UUNARY_OP_ENUM(kNegOp)
    BINDING_UUNARY_OP_ENUM(kTypeOfOp)
    BINDING_UUNARY_OP_ENUM(kVoidOp)
    BINDING_UUNARY_OP_ENUM(kDeleteOp)
    BINDING_UUNARY_OP_ENUM(kThrowOp);
}


