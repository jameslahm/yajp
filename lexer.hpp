#pragma once
#include <sstream>
#include <string>
using namespace std;

enum class TokenType {
  kForToken,
  kConstToken,
  kIdentifierToken,
  kNumericToken,
  kStringToken,
  kAddToken,
  kSubToken,
  kMulToken,
  kDivToken,
  kExclaToken,
  kEofToken,
  kBooleanToken,
  kNullToken,
  kNegToken,
  kTypeOfToken,
  kVoidToken,
  kDeleteToken,
  kThrowToken,
  kLeftBraceToken,
  kRightBraceToken,
  kSemiColonToken,
  kDebuggerToken,
  kReturnToken,
  kBreakToken,
  kContinueToken,
  kElseToken,
  kIfToken,
  kSwitchToken,
  kCaseToken,
  kColonToken,
  kDefaultToken,
  kLetToken,
  kVarToken,
  kCommaToken,
  kEqualToken,
  kEqualEqualToken,
  kEqualEqualEqualToken,
  kInToken,
  kOfToken,
  kAwaitToken,
  kCatchToken,
  kFinallyToken,
  kAsyncToken,
  kFromToken,
  kImportToken,
  kExportToken,
  kAsToken,
  kFunctionToken,
  kLeftParenToken,
  kRightParenToken
};

class Lexer {
  stringstream stream_;
  int current_line_;
  int current_col_;
  TokenType current_token_;
  string value_;
  char current_char_ = ' ';

public:
  Lexer(string source) : stream_(source) {}
  TokenType GetToken() {
    value_.clear();

    while (true) {
      if (isspace(current_char_)) {
        if (stream_.eof()) {
          current_char_ = EOF;
          break;
        }
        current_char_ = stream_.get();
      } else {
        break;
      }
    }

    if (isalpha(current_char_)) {
      while (isalpha(current_char_) || isdigit(current_char_) ||
             current_char_ == '_') {
        if (stream_.eof()) {
          current_char_ = EOF;
          break;
        } else {
          value_ += current_char_;
          current_char_ = stream_.get();
        }
      }
      if (value_ == "const") {
        current_token_ = TokenType::kConstToken;
        return current_token_;
      }
      if (value_ == "true") {
        current_token_ = TokenType::kBooleanToken;
        return current_token_;
      }
      if (value_ == "false") {
        current_token_ = TokenType::kBooleanToken;
        return current_token_;
      }
      if (value_ == "null") {
        current_token_ = TokenType::kNullToken;
        return current_token_;
      }
      if (value_ == "typeof") {
        current_token_ = TokenType::kTypeOfToken;
        return current_token_;
      }
      if (value_ == "void") {
        current_token_ = TokenType::kVoidToken;
        return current_token_;
      }
      if (value_ == "delete") {
        current_token_ = TokenType::kDeleteToken;
        return current_token_;
      }
      if (value_ == "throw") {
        current_token_ = TokenType::kThrowToken;
        return current_token_;
      }

      if (value_ == "debugger") {
        current_token_ = TokenType::kDebuggerToken;
        return current_token_;
      }

      if (value_ == "return") {
        current_token_ = TokenType::kReturnToken;
        return current_token_;
      }

      if (value_ == "continue") {
        current_token_ = TokenType::kContinueToken;
        return current_token_;
      }

      if (value_ == "break") {
        current_token_ = TokenType::kBreakToken;
        return current_token_;
      }

      if (value_ == "if") {
        current_token_ = TokenType::kIfToken;
        return current_token_;
      }

      if (value_ == "else") {
        current_token_ = TokenType::kElseToken;
        return current_token_;
      }

      if (value_ == "switch") {
        current_token_ = TokenType::kSwitchToken;
        return current_token_;
      }

      if (value_ == "case") {
        current_token_ = TokenType::kCaseToken;
        return current_token_;
      }

      if (value_ == "default") {
        current_token_ = TokenType::kDefaultToken;
        return current_token_;
      }

      if (value_ == "let") {
        current_token_ = TokenType::kLetToken;
        return current_token_;
      }

      if (value_ == "var") {
        current_token_ = TokenType::kVarToken;
        return current_token_;
      }

      if (value_ == "in") {
        current_token_ = TokenType::kInToken;
        return current_token_;
      }

      if (value_ == "of") {
        current_token_ = TokenType::kOfToken;
        return current_token_;
      }

      if (value_ == "await") {
        current_token_ = TokenType::kAwaitToken;
        return current_token_;
      }

      if (value_ == "catch") {
        current_token_ = TokenType::kCatchToken;
        return current_token_;
      }

      if (value_ == "finally") {
        current_token_ = TokenType::kFinallyToken;
        return current_token_;
      }

      if (value_ == "async") {
        current_token_ = TokenType::kAsyncToken;
        return current_token_;
      }

      if (value_ == "from") {
        current_token_ = TokenType::kFromToken;
        return current_token_;
      }

      if (value_ == "import") {
        current_token_ = TokenType::kImportToken;
        return current_token_;
      }

      if (value_ == "export") {
        current_token_ = TokenType::kExportToken;
        return current_token_;
      }

      if (value_ == "as") {
        current_token_ = TokenType::kAsToken;
        return current_token_;
      }

      if (value_ == "function") {
        current_token_ = TokenType::kFunctionToken;
        return current_token_;
      }

      current_token_ = TokenType::kIdentifierToken;
      return current_token_;
    }

    if (isdigit(current_char_)) {
      // FIXME: 1.1.1
      while (isdigit(current_char_) || current_char_ == '.') {
        if (stream_.eof()) {
          current_char_ = EOF;
          break;
        } else {
          value_ += current_char_;
          current_char_ = stream_.get();
        }
      }
      current_token_ = TokenType::kNumericToken;
      return current_token_;
    }

    if (current_char_ == '"') {
      current_char_ = stream_.get();
      while (current_char_ != '"') {
        if (stream_.eof()) {
          current_char_ = EOF;
          break;
        } else {
          value_ += current_char_;
          current_char_ = stream_.get();
        }
      }
      current_char_ = stream_.get();
      current_token_ = TokenType::kStringToken;
      return current_token_;
    }

    if (current_char_ == '+') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kAddToken;
      return current_token_;
    }

    if (current_char_ == '-') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kSubToken;
      return current_token_;
    }

    if (current_char_ == '*') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kMulToken;
      return current_token_;
    }

    if (current_char_ == '/') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kDivToken;
      return current_token_;
    }

    if (current_char_ == '!') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kExclaToken;
      return current_token_;
    }

    if (current_char_ == '~') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kNegToken;
      return current_token_;
    }

    if (current_char_ == '{') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kLeftBraceToken;
      return current_token_;
    }

    if (current_char_ == '}') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kRightBraceToken;
      return current_token_;
    }

    if (current_char_ == ';') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kSemiColonToken;
      return current_token_;
    }

    if (current_char_ == ':') {
      current_char_ = stream_.get();
      current_token_ = TokenType::kColonToken;
      return current_token_;
    }

    if (current_char_ == '=') {
      current_char_ = stream_.get();
      if (current_char_ == '=') {
        current_char_ = stream_.get();
        if (current_char_ == '=') {
          current_token_ = TokenType::kEqualEqualEqualToken;
          return current_token_;
        }
        current_token_ = TokenType::kEqualEqualToken;
        return current_token_;
      }
      current_token_ = TokenType::kEqualToken;
      return current_token_;
    }

    if(current_char_ == '('){
      current_char_ = stream_.get();
      current_token_ = TokenType::kLeftParenToken;
      return current_token_;
    }

    if(current_char_ == ')'){
      current_char_ = stream_.get();
      current_token_ = TokenType::kRightParenToken;
      return current_token_;
    }

    if (current_char_ == EOF) {
      current_token_ = TokenType::kEofToken;
      return current_token_;
    }

    current_char_ = stream_.get();
    return GetToken();
  }

  string value() { return value_; }

  TokenType current_token() { return current_token_; }
};
