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
//   Exp_variable::run の実装
//---------------------------------------------------------------------
int Exp_variable::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  std::map<std::string, int>::const_iterator p;
  if ((p = lvar.find(name())) != lvar.end())
    return p->second;
  else if ((p = gvar.find(name())) != gvar.end())
    return p->second;
  else
  {
    std::cerr << "undefined variable " << name() << " " << std::endl;
    exit(1);
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
//   Exp_operation1::run の実装
//---------------------------------------------------------------------
int Exp_operation1::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  int v = operand()->run(func, gvar, lvar);
  switch (operation())
  {
  case Operator_MINUS:
    return -v;
  default:
    assert(0);
    return 0;
  }
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
    os << " " << Operator_string(operation()) << " ";
    operand2()->print(os);
  }
  else
    os << "UNDEF";
  os << ")";
}

//---------------------------------------------------------------------
//   Exp_operation2:run の実装
//---------------------------------------------------------------------
int Exp_operation2::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  int v1 = operand1()->run(func, gvar, lvar);
  int v2 = operand2()->run(func, gvar, lvar);

  switch (operation())
  {
  case Operator_PLUS:
    return v1 + v2;
  case Operator_MINUS:
    return v1 - v2;
  case Operator_MUL:
    return v1 * v2;
  case Operator_DIV:
    return v1 / v2;
  case Operator_MOD:
    return v1 % v2;
  case Operator_LT:
    return v1 < v2;
  case Operator_GT:
    return v1 > v2;
  case Operator_LE:
    return v1 <= v2;
  case Operator_GE:
    return v1 >= v2;
  case Operator_NE:
    return v1 != v2;
  case Operator_EQ:
    return v1 == v2;
  default:
    assert(0);
    return 0;
  }
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
  bool first = true;
  for (std::list<Expression *>::const_iterator it = args_.begin();
       it != args_.end(); it++)
  {
    if (!first)
      os << ',';
    (*it)->print(os);
    first = false;
  }
  os << ")";
}

//---------------------------------------------------------------------
//   Exp_function::run の実装
//---------------------------------------------------------------------
int Exp_function::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  // 引数の値のリストの計算
  std::list<int> i_args;
  for (std::list<Expression *>::const_iterator it = args_.begin();
       it != args_.end(); it++)
  {
    // 引数の値を計算してリストに追加
    i_args.push_back((*it)->run(func, gvar, lvar));
  }
  // 関数の実行
  std::map<std::string, Function *>::const_iterator p;
  std::string func_name = name();
  if (func_name == "getint")
  {
    int i;
    std::cin >> i;
    return i;
  }
  else if (func_name == "getchar")
  {
    char c;
    std::cin >> c;
    return c;
  }
  else if (func_name == "putint")
  {
    int i = i_args.front(); // 引数値のリストの先頭
    std::cout << i;
    return 0;
  }
  else if (func_name == "putchar")
  {
    char i = i_args.front(); // 引数値のリストの先頭
    std::cout << i;
    return 0;
  }
  else
  {
    if ((p = func.find(name())) != func.end())
    {
      Function *f = p->second;
      return f->run(func, gvar, i_args);
    }
    else
    {
      std::cerr << "undefined function " << name() << std::endl;
      exit(1);
    }
  }
}

//---------------------------------------------------------------------
//  St_assign::print の実装
//---------------------------------------------------------------------
void St_assign::print(std::ostream &os, int indent) const
{
  os << tab(indent);  // インデント（1段につきスペース2個）をつける
  if (lhs())          // 左辺が NULL 出なければ
    lhs()->print(os); // 左辺を表示
  else
    os << "UNDEF";
  os << " = ";
  if (rhs())          // 右辺が NULL 出なければ
    rhs()->print(os); // 右辺を表示
  else
    os << "UNDEF";
  os << ";" << std::endl;
}

//---------------------------------------------------------------------
//  St_assign::run の実装
//---------------------------------------------------------------------
Return_t St_assign::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  assert(lhs());
  assert(rhs());
  int i_rhs = rhs()->run(func, gvar, lvar);
  if (lvar.find(lhs()->name()) != lvar.end())
    lvar[lhs()->name()] = i_rhs;
  else if (gvar.find(lhs()->name()) != gvar.end())
    gvar[lhs()->name()] = i_rhs;
  else
  {
    std::cerr << "undefined variable " << lhs()->name() << std::endl;
    exit(1);
  }

  return Return_t(false, 0);
}

//---------------------------------------------------------------------
//  St_list::print の実装
//---------------------------------------------------------------------
void St_list::print(std::ostream &os, int indent) const
{
  for (std::list<Statement *>::const_iterator it = statements_.begin();
       it != statements_.end(); it++)
  {
    (*it)->print(os, indent);
  }
}

//---------------------------------------------------------------------
//  St_list::run の実装
//---------------------------------------------------------------------
Return_t St_list::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  for (std::list<Statement *>::const_iterator it = statements().begin();
       it != statements().end(); it++)
  {
    assert(*it);                                // 念の為 NULL でないことを確認
    Return_t rd = (*it)->run(func, gvar, lvar); // 実行
    if (rd.val_is_returned)
    {
      return rd;
    } // return 文が実行されていたら、rd をそのまま返す
  }
  return Return_t(); // return 文が一度も実行されていない場合
};

//---------------------------------------------------------------------
//  St_if::print の実装
//---------------------------------------------------------------------
void St_if::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "if ";
  if (condition())
    condition()->print(os);
  else
    os << "UNDEF";
  os << " {" << std::endl;
  if (then_part())
    then_part()->print(os, indent + 1);
  if (else_part())
  {
    os << tab(indent) << "} else {" << std::endl;
    else_part()->print(os, indent + 1);
  }
  os << tab(indent) << "}" << std::endl;
}

//---------------------------------------------------------------------
//  St_if::run の実装
//---------------------------------------------------------------------
Return_t St_if::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  if (condition()->run(func, gvar, lvar))
  {
    // then部分の実行
    Return_t then_result = then_part()->run(func, gvar, lvar);
    return then_result;
  }
  else
  {
    if (else_part())
    {
      // else部分の実行
      Return_t else_result = else_part()->run(func, gvar, lvar);
      return else_result;
    }
    else
      return Return_t(false, 0);
  }
}

//---------------------------------------------------------------------
//  St_while::print の実装
//---------------------------------------------------------------------
void St_while::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "while ";
  if (condition())
    condition()->print(os);
  else
    os << "UNDEF";
  os << " {" << std::endl;
  if (body())
    body()->print(os, indent + 1);
  os << tab(indent) << "}" << std::endl;
}

//---------------------------------------------------------------------
//  St_while::run の実装
//---------------------------------------------------------------------
Return_t St_while::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{

  if (condition())
  {
    while (condition()->run(func, gvar, lvar))
    {
      Return_t ret = body()->run(func, gvar, lvar);
      if (ret.val_is_returned)
        return ret;
    }
  }
  else
    return Return_t();
}

//---------------------------------------------------------------------
//  St_return::print の実装
//---------------------------------------------------------------------
void St_return::print(std::ostream &os, int indent) const
{
  os << tab(indent) << "return ";
  if (value())
    value()->print(os);
  else
    os << "UNDEF";
  os << ";" << std::endl;
}

//---------------------------------------------------------------------
//  St_return::run の実装
//---------------------------------------------------------------------
Return_t St_return::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  assert(value());
  int rv = value()->run(func, gvar, lvar);
  return Return_t(true, rv);
};

//---------------------------------------------------------------------
//  St_function::print の実装
//---------------------------------------------------------------------
void St_function::print(std::ostream &os, int indent) const
{
  os << tab(indent) << name();
  os << "(";
  for (std::list<Expression *>::const_iterator it = args().begin();
       it != args().end(); it++)
  {
    (*it)->print(os);
    if (it != --args().end())
      os << ", ";
  }
  os << ");" << std::endl;
}

//---------------------------------------------------------------------
//  St_function::print の実装
//---------------------------------------------------------------------
Return_t St_function::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::map<std::string, int> &lvar) const
{
  function_.run(func, gvar, lvar);
  return Return_t();
}

//---------------------------------------------------------------------
//  Variable::print の実装
//---------------------------------------------------------------------
void Variable::print(std::ostream &os) const
{
  os << Type_string(type()) << " " << name() << std::flush;
}

//---------------------------------------------------------------------
//  Function::print の実装
//---------------------------------------------------------------------
void Function::print(std::ostream &os) const
{
  os << Type_string(type()) << " " << name() << "(";
  for (std::list<Variable *>::const_iterator it = args().begin();
       it != args().end(); it++)
  {
    (*it)->print(os);
    if (it != --args().end())
      os << ", ";
  }
  os << ") {" << std::endl;
  for (std::list<Variable *>::const_iterator it = lvarlist().begin();
       it != lvarlist().end(); it++)
  {
    os << tab(1);
    (*it)->print(os);
    os << ";" << std::endl;
  }
  os << std::endl;
  body()->print(os, 1);
  os << "}" << std::endl;
}

//---------------------------------------------------------------------
//  Function::run の実装
//---------------------------------------------------------------------
int Function::run(
    std::map<std::string, Function *> &func,
    std::map<std::string, int> &gvar,
    std::list<int> &i_args) const
{

  std::map<std::string, int> lvar;

  // 引数の名前と値をローカル変数表に登録する
  for (std::list<Variable *>::const_iterator it = args().begin();
       it != args().end(); it++)
  {
    lvar[(*it)->name()] = i_args.front();
    i_args.pop_front();
  }

  // ローカル変数をローカル変数表に登録する
  for (std::list<Variable *>::const_iterator it = lvarlist().begin();
       it != lvarlist().end(); it++)
  {
    lvar[(*it)->name()] = 0;
  }

  // 本体を呼び出す
  Return_t rd = body()->run(func, gvar, lvar);

  // 本体の返り値を返す
  return rd.return_val;
}

//---------------------------------------------------------------------
//  Program::print の実装
//---------------------------------------------------------------------
void Program::print(std::ostream &os) const
{
  for (std::list<Variable *>::const_iterator it = varlist().begin();
       it != varlist().end(); it++)
  {
    (*it)->print(os);
    os << ";" << std::endl;
  }
  for (std::list<Function *>::const_iterator it = funclist().begin();
       it != funclist().end(); it++)
  {
    (*it)->print(os);
    os << std::endl;
  }
  main()->print(os);
}

//---------------------------------------------------------------------
//  Program::run の実装
//---------------------------------------------------------------------
int Program::run()
{
  std::map<std::string, int> gvar;
  for (std::list<Variable *>::const_iterator it = varlist().begin();
       it != varlist().end(); it++)
  {
    gvar[(*it)->name()] = 0;
  }
  std::map<std::string, Function *> func;
  for (std::list<Function *>::const_iterator it = funclist().begin();
       it != funclist().end(); it++)
  {
    func[(*it)->name()] = *it;
  }
  std::list<int> iargs;
  int v = main()->run(func, gvar, iargs);
  return v;
}