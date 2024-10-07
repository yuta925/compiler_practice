//=====================================================================
//
//     ast.cpp: mini-C プログラムの抽象構文木 (実装)
//
//            コンパイラ実習  2004 (c) 平岡佑介, 石浦菜岐佐
//
//=====================================================================

#include "ast.h"

//---------------------------------------------------------------------
//   Type_string の実装
//---------------------------------------------------------------------
std::string Type_string(Type t)
{
  switch (t)
  {
  case Type_INT:
    return "int";
  case Type_CHAR:
    return "char";
  default:
    return "UNDEF";
  }
}

//---------------------------------------------------------------------
//   Opeartor_string の実装
//---------------------------------------------------------------------
std::string Operator_string(Operator o)
{
  switch (o)
  {
  case Operator_PLUS:
    return "+";
  case Operator_MINUS:
    return "-";
  case Operator_MUL:
    return "*";
  case Operator_DIV:
    return "/";
  case Operator_MOD:
    return "%";
  case Operator_LT:
    return "<";
  case Operator_GT:
    return ">";
  case Operator_LE:
    return "<=";
  case Operator_GE:
    return ">=";
  case Operator_NE:
    return "!=";
  case Operator_EQ:
    return "==";
  default:
    return "???";
  }
}

static int tmp = 0;

//---------------------------------------------------------------------
//  tab(int)
//    インデントを行うための関数 (ast.cpp の中だけで用いる)
//    indent で指定された段数の 2 倍のスペースを返す
//---------------------------------------------------------------------
static std::string tab(int indent)
{
  std::string tab_ = "";
  for (int i = 0; i < indent; i++)
  {
    tab_ += "  ";
  }
  return tab_;
}

//---------------------------------------------------------------------
//   Exp_constant::print の実装
//---------------------------------------------------------------------
void Exp_constant::print(std::ostream &os) const
{
  switch (type())
  {
  case Type_INT:
    os << value();
    break;
  case Type_CHAR:
  {
    os << '\'';
    if (value() == '\n')
    {
      os << '\\';
      os << 'n';
    }
    else if (value() == '\t')
    {
      os << '\\';
      os << 't';
    }
    else if (value() == '\\')
    {
      os << '\\';
      os << '\\';
    }
    else
    {
      os << (char)value();
    }
    os << '\'';
    break;
  }
  default:
    assert(0);
  }
}

//---------------------------------------------------------------------
//   Exp_variable::print の実装
//---------------------------------------------------------------------
void Exp_variable::print(std::ostream &os) const
{

  for (char c : name())
  {
    if (c == '\n')
    {
      os << '\\';
      os << 'n';
    }
    else if (c == '\t')
    {
      os << '\\';
      os << 't';
    }
    else if (c == '\\')
    {
      os << '\\';
      os << '\\';
    }
    else
    {
      os << c;
    }
  }
}

//---------------------------------------------------------------------
//   Exp_operation1::print の実装
//---------------------------------------------------------------------
void Exp_operation1::print(std::ostream &os) const
{
  os << "(" << Operator_string(operation());
  if (operand())
  {
    operand()->print(os);
  }
  else
  {
    os << "UNDEF";
  }
  os << ")";
}

//---------------------------------------------------------------------
//   Exp_operation2:print の実装
//---------------------------------------------------------------------
void Exp_operation2::print(std::ostream &os) const
{
  os << "(";
  if (operand1() && operand2())
  {
    operand1()->print(os);
    os << Operator_string(operation());
    operand2()->print(os);
  }
  else
  {
    os << "UNDEF";
  };
  os << ")";
}

//---------------------------------------------------------------------
//   ~Exp_function() の実装
//---------------------------------------------------------------------
Exp_function::~Exp_function()
{
  for (std::list<Expression *>::const_iterator it = args_.begin();
       it != args_.end(); it++)
  {
    delete *it;
  }
}

//---------------------------------------------------------------------
//   Exp_function::print の実装
//---------------------------------------------------------------------
void Exp_function::print(std::ostream &os) const
{
  os << name() << "(";
  for (std::list<Expression *>::const_iterator it = args_.begin();
       it != args_.end(); it++)
  {
    (*it)->print(os);
    if (it == args_.begin())
      os << ',';
  }
  os << ")";
}

//---------------------------------------------------------------------
//  St_assign::print の実装
//---------------------------------------------------------------------
void St_assign::print(std::ostream &os, int indent) const
{
  os << tab(indent); // インデント（1段につきスペース2個）をつける
  if (lhs())         // 左辺が NULL 出なければ
  {
    lhs()->print(os); // 左辺を表示
  }
  else
  {
    os << "UNDEF";
  }
  os << " = ";
  if (rhs()) // 右辺が NULL 出なければ
  {
    rhs()->print(os); // 右辺を表示
  }
  else
  {
    os << "UNDEF";
  }
  os << ";" << std::endl;
}

//---------------------------------------------------------------------
//  St_list::print の実装
//---------------------------------------------------------------------
void St_list::print(std::ostream &os, int indent) const
{
  for (std::list<Statement *>::const_iterator it = statements_.begin();
       it != statements_.end(); it++)
  {
    os << tab(indent);
    (*it)->print(os, indent);
  }
}

//---------------------------------------------------------------------
//  St_if::print の実装
//---------------------------------------------------------------------
void St_if::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "if ";
  if (condition())
  {
    condition()->print(os);
  }
  else
  {
    os << "UNDEF";
  }
  os << " {" << std::endl;
  if (then_part())
  {
    then_part()->print(os, indent + 1);
  }
  if (else_part())
  {
    os << tab(indent) << "} else {" << std::endl;
    else_part()->print(os, indent + 1);
  }
  os << tab(indent) << "}" << std::endl;
}

//---------------------------------------------------------------------
//  St_while::print の実装
//---------------------------------------------------------------------
void St_while::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "while ";
  if (condition())
  {
    condition()->print(os);
  }
  else
  {
    os << "UNDEF";
  }
  os << " {" << std::endl;
  if (body())
  {
    body()->print(os, indent + 1);
  }
  os << tab(indent) << "}" << std::endl;
}