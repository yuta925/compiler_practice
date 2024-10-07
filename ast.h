//=====================================================================
//
//     ast.h: mini-C プログラムの抽象構文木 (ヘッダ)
//
//            コンパイラ実習  2004 (c) 平岡佑介, 石浦菜岐佐
//
//=====================================================================

#ifndef INCLUDE_AST_H_
#define INCLUDE_AST_H_

#include <assert.h>
#include <iostream>
#include <string>
#include <list>

//---------------------------------------------------------------------
//   Type
//   型 (int か char) を表す列挙型
//---------------------------------------------------------------------
enum Type
{
  Type_INT,
  Type_CHAR
};

//---------------------------------------------------------------------
// Type_string
// Type の表示用文字列
//---------------------------------------------------------------------
std::string Type_string(Type t);

//---------------------------------------------------------------------
//  Operator
//  演算子を表す列挙型
//---------------------------------------------------------------------
enum Operator
{
  Operator_PLUS,  // +
  Operator_MINUS, // -
  Operator_MUL,   // *
  Operator_DIV,   // /
  Operator_MOD,   // %

  Operator_LT, // <
  Operator_GT, // >
  Operator_LE, // <=
  Operator_GE, // >=
  Operator_NE, // !=
  Operator_EQ, // ==
};

// static int tmp = 0;

//---------------------------------------------------------------------
//  Operator_string
//  Operator の表示用文字列
//---------------------------------------------------------------------
std::string Operator_string(Operator o);

//---------------------------------------------------------------------
//  class Expression
//  「式」の抽象基底
//---------------------------------------------------------------------
class Expression
{
private:
  Expression(const Expression &);
  Expression &operator=(const Expression &);

public:
  Expression() {}
  virtual ~Expression() {}
  virtual void print(std::ostream &os) const = 0;
};

//---------------------------------------------------------------------
//  class Exp_constant
//  式中の定数を表す
//---------------------------------------------------------------------
class Exp_constant : public Expression
{
private:
  Type type_;
  int value_;

public:
  Exp_constant(Type t, int i) : type_(t), value_(i) {}
  ~Exp_constant() {}
  const int value() const { return value_; }
  const Type type() const { return type_; }
  void print(std::ostream &os) const;
};

#endif // ifndef INCLUDE_AST_H_

//---------------------------------------------------------------------
//  class Exp_variable
//  式中の変数を表す
//---------------------------------------------------------------------
class Exp_variable : public Expression
{
private:
  std::string name_;

public:
  Exp_variable(const std::string &n) : name_(n) {};
  ~Exp_variable() {}
  const std::string &name() const { return name_; };
  void print(std::ostream &os) const;
};

//---------------------------------------------------------------------
//  class Exp_operation1
//  単行演算式を返す
//---------------------------------------------------------------------
class Exp_operation1 : public Expression
{
private:
  Operator operation_;
  Expression *operand_;

public:
  Exp_operation1(const Operator op, Expression *ex) : operation_(op), operand_(ex) {};
  ~Exp_operation1() { delete operand_; }
  Operator operation() const { return operation_; }
  const Expression *operand() const { return operand_; }
  void print(std::ostream &os) const;
};

//---------------------------------------------------------------------
//  class Exp_operation2
//  二項演算式を表す
//---------------------------------------------------------------------
class Exp_operation2 : public Expression
{
private:
  Operator operation_;
  Expression *operand1_;
  Expression *operand2_;

public:
  Exp_operation2(Operator op, Expression *ex1, Expression *ex2) : operation_(op), operand1_(ex1), operand2_(ex2) {};
  ~Exp_operation2()
  {
    delete operand1_;
    delete operand2_;
  };
  Operator operation() const { return operation_; }
  const Expression *operand1() const { return operand1_; }
  const Expression *operand2() const { return operand2_; }
  void print(std::ostream &os) const;
};

//---------------------------------------------------------------------
//  class Exp_function
//  関数呼び出しを表現する
//---------------------------------------------------------------------
class Exp_function : public Expression
{
private:
  std::string name_;
  std::list<Expression *> args_;

public:
  Exp_function(const std::string &nm, const std::list<Expression *> &args) : name_(nm), args_(args) {};
  ~Exp_function();
  const std::string &name() const { return name_; }
  const std::list<Expression *> &args() const { return args_; }
  void print(std::ostream &os) const;
};

//---------------------------------------------------------------------
//  class Statement
//  「文」を表す抽象基底 Statement
//---------------------------------------------------------------------
class Statement
{
public:
  Statement() {}                                                  // コンストラクタ
  virtual ~Statement() {}                                         // デストラクタ
  virtual void print(std::ostream &os, int indent = 0) const = 0; // 表示
private:
  Statement(const Statement &);            // コピーコンスタラクタは禁止
  Statement &operator=(const Statement &); // 代入演算は禁止
};

//---------------------------------------------------------------------
//  class St_assign
//  代入文を表現する
//---------------------------------------------------------------------
class St_assign : public Statement
{
private:
  Exp_variable *lhs_; // 左辺 (変数)
  Expression *rhs_;   // 右辺 (式)
public:
  St_assign(Exp_variable *lexp, Expression *rexp) : lhs_(lexp), rhs_(rexp) {}
  ~St_assign() // デストラクタ
  {
    delete lhs_; // 子の抽象構文木は delete する
    delete rhs_; // 子の抽象構文木は delete する
  }
  const Exp_variable *lhs() const { return lhs_; }    // 左辺の読み出し
  const Expression *rhs() const { return rhs_; }      // 右辺の読み出し
  void print(std::ostream &os, int indent = 0) const; // 表示
};

//---------------------------------------------------------------------
//  class St_assign
//  文の並びを表現する
//---------------------------------------------------------------------
class St_list : public Statement
{
private:
  std::list<Statement *> statements_;

public:
  St_list(const std::list<Statement *> &li) : statements_(li) {}
  ~St_list()
  {
    for (Statement *stmt : statements_)
      delete stmt;
  }
  const std::list<Statement *> &statements() const { return statements_; }
  void print(std::ostream &os, int indent = 0) const;
};

//---------------------------------------------------------------------
//  class St_if
//  if文を表現する
//---------------------------------------------------------------------
class St_if : public Statement
{
private:
  Expression *cond_;
  Statement *then_;
  Statement *else_;

public:
  St_if(Expression *cond, Statement *then, Statement *els) : cond_(cond), then_(then), else_(els) {}
  ~St_if()
  {
    delete cond_;
    delete then_;
    delete else_;
  }
  const Expression *condition() const { return cond_; }
  const Statement *then_part() const { return then_; }
  const Statement *else_part() const { return else_; }
  void print(std::ostream &os, int indent = 0) const;
};

//---------------------------------------------------------------------
//  class St_while
//  whileループを表現する
//---------------------------------------------------------------------
class St_while : public Statement
{
private:
  Expression *cond_;
  Statement *body_;

public:
  St_while(Expression *cond, Statement *body) : cond_(cond), body_(body) {}
  ~St_while()
  {
    delete cond_;
    delete body_;
  }
  const Expression *condition() const { return cond_; }
  const Statement *body() const { return body_; }
  void print(std::ostream &os, int indent = 0) const;
};

//---------------------------------------------------------------------
//  class Exp_function
//  関数呼び出しを表現する
//---------------------------------------------------------------------