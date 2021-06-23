#include "error.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>
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
  kContinueToken
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
        stream_ >> current_char_;
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
          stream_ >> current_char_;
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

      if(value_ == "debugger"){
        current_token_ = TokenType::kDebuggerToken;
        return current_token_;
      }

      if(value_ == "return") {
        current_token_ = TokenType::kReturnToken;
        return current_token_;
      }

      if(value_ == "continue") {
        current_token_ = TokenType::kContinueToken;
        return current_token_;
      }

      if(value_=="break") {
        current_token_ = TokenType::kBreakToken;
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
          stream_ >> current_char_;
        }
      }
      current_token_ = TokenType::kNumericToken;
      return current_token_;
    }

    if (current_char_ == '"') {
      stream_ >> current_char_;
      while (current_char_ != '"') {
        if (stream_.eof()) {
          current_char_ = EOF;
          break;
        } else {
          value_ += current_char_;
          stream_ >> current_char_;
        }
      }
      stream_ >> current_char_;
      current_token_ = TokenType::kStringToken;
      return current_token_;
    }

    if (current_char_ == '+') {
      stream_ >> current_char_;
      current_token_ = TokenType::kAddToken;
      return current_token_;
    }

    if (current_char_ == '-') {
      stream_ >> current_char_;
      current_token_ = TokenType::kSubToken;
      return current_token_;
    }

    if (current_char_ == '*') {
      stream_ >> current_char_;
      current_token_ = TokenType::kMulToken;
      return current_token_;
    }

    if (current_char_ == '/') {
      stream_ >> current_char_;
      current_token_ = TokenType::kDivToken;
      return current_token_;
    }

    if (current_char_ == '!') {
      stream_ >> current_char_;
      current_token_ = TokenType::kExclaToken;
      return current_token_;
    }

    if (current_char_ == '~') {
      stream_ >> current_char_;
      current_token_ = TokenType::kNegToken;
      return current_token_;
    }

    if(current_char_ == '{') {
      stream_ >> current_char_;
      current_token_ = TokenType::kLeftBraceToken;
      return current_token_;
    }

    if(current_char_ == '}') {
      stream_ >> current_char_;
      current_token_ = TokenType::kRightBraceToken;
      return current_token_;
    }

    if(current_char_ == ';') {
      stream_ >> current_char_;
      current_token_ = TokenType::kSemiColonToken;
      return current_token_;
    }

    if (current_char_ == EOF) {
      current_token_ = TokenType::kEofToken;
      return current_token_;
    }

    cout << kUnkownTokenType << " " << current_char_ << endl;
    stream_ >> current_char_;
    return GetToken();
  }

  string value() { return value_; }

  TokenType current_token() { return current_token_; }
};
