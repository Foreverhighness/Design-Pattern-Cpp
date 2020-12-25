#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
using namespace std;

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

struct Token {
  enum Type { interger, plus, minus, lparen, rparen } type;
  string text;

  explicit Token(Type type, string_view text) : type{type}, text{text} {}

  friend ostream& operator<<(ostream& os, const Token& obj) { return os << "`" << obj.text << "`"; }
};

vector<Token> lex(string_view input) {
  vector<Token> result;
  for (size_t i{0}; i < input.size(); ++i) {
    if (isspace(input[i])) continue;
    switch (input[i]) {
      case '+':
        result.emplace_back(Token::plus, "+");
        break;
      case '-':
        result.emplace_back(Token::minus, "-");
        break;
      case '(':
        result.emplace_back(Token::lparen, "(");
        break;
      case ')':
        result.emplace_back(Token::rparen, ")");
        break;
      default:
        ostringstream buffer;
        buffer << input[i];
        for (size_t j{i + 1}; j < input.size(); ++j) {
          if (isdigit(input[j])) {
            buffer << input[j];
            ++i;
          } else {
            result.emplace_back(Token::interger, buffer.str());
            break;
          }
        }
    }
  }
  return result;
}

struct Element {
  virtual ~Element() = default;
  virtual int eval() const = 0;
};

struct Interger : public Element {
  int value;
  explicit Interger(const int value) : value{value} {}
  int eval() const override { return value; }
};

struct BinaryOperation : Element {
  enum Type { addition, subtraction } type;
  shared_ptr<Element> lhs, rhs;

  int eval() const override {
    switch (type) {
      case addition:
        return lhs->eval() + rhs->eval();
        break;
      case subtraction:
        return lhs->eval() - rhs->eval();
        break;
    }
  }
};

unique_ptr<Element> parse(const vector<Token>& tokens) {
  auto result = make_unique<BinaryOperation>();
  bool have_lhs{false};
  for (size_t i{0}; i < tokens.size(); ++i) {
    auto token = tokens[i];
    switch (token.type) {
      case Token::interger: {
        int value = lexical_cast<int>(token.text);
        auto interger = make_shared<Interger>(value);
        if (!have_lhs) {
          result->lhs = interger;
          have_lhs = true;
        } else {
          result->rhs = interger;
        }
      } break;
      case Token::plus:
        result->type = BinaryOperation::addition;
        break;
      case Token::minus:
        result->type = BinaryOperation::subtraction;
        break;
      case Token::lparen: {
        int j = i;
        for (; j < tokens.size() && !(tokens[j].type == Token::rparen); ++j) {}
        vector<Token> subexpression(tokens.cbegin() + i + 1, tokens.cbegin() + j);
        auto element = parse(subexpression);
        if (!have_lhs) {
          result->lhs = std::move(element);
          have_lhs = true;
        } else {
          result->rhs = std::move(element);
        }
        i = j;
      } break;
      case Token::rparen:
        assert(false);
        break;
    }
  }
  return result;
}

void test() {
  string input{" (13 - 4) - (12 + 1) "};
  auto tokens = lex(input);

  for (auto& t : tokens)
    cout << t << " ";
  cout << endl;
  try {
    auto parsed = parse(tokens);
    cout << "`" << input << "` = " << parsed->eval() << endl;
  } catch (const exception& e) {
    cout << e.what() << endl;
  }
}

int main() {
  test();
  return 0;
}