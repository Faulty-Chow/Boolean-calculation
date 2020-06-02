/********************************************************************************************************************************
主要功能：
	*可以进行布尔运算；
		**可以给定任意表达式
		**会自主判断给定的表达式是否合法，并给出错误信息
		**可以理解表达式中运算符的优先级
	*可以输出表达式的真值表；
	*目前仅支持3变量；
		**仅包含P-Q-R的表达式
	*目前仅支持5种逻辑联结词；
		**否定联结词	!
		**合取联结词	&&
		**析取联结词	||
		**条件联结词	->
		**双条件联结词	<->
	*规定输入格式为：
		**<布尔表达式>,p=x,q=y,r=z
			***不包含<>,每行以enter作为终止符，可以输入多组数据
*********************************************************************************************************************************/

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>

using namespace std;
ofstream ocout;

string negation(char p)
{
	if (p == '0')
		return "1";
	else
		return "0";
}

string conjunction(char p, char q)
{
	int x = p - '0';
	int y = q - '0';
	int result = x && y;
	if (result == 0)
		return "0";
	else
		return "1";
}

string disjunction(char p, char q)
{
	int x = p - '0';
	int y = q - '0';
	int result = x || y;
	if (result == 0)
		return "0";
	else
		return "1";
}

string conditional(char p, char q)
{
	if (p == '0' && q == '0')
		return "1";
	if (p == '0' && q == '1')
		return "1";
	if (p == '1' && q == '0')
		return "0";
	else
		return "1";
}

string biconditional(char p, char q)
{
	if (p == '0' && q == '0')
		return "1";
	if (p == '0' && q == '1')
		return "0";
	if (p == '1' && q == '0')
		return "0";
	else
		return "1";
}

class Boolean_calculation
{
	friend char equivalent_substitution(Boolean_calculation* test, char p);//用于访问类内私有变量
private:
	char p, q, r, p_copy, q_copy, r_copy;
	string expression, expression_copy;
	string result;
	int bracket_cnt = 0, bracket_max_plie = 0;//用于记录当前表达式中括号的个数和层数
	struct bracket_info
	{
		int left, right;
		int plie;
	};
	bracket_info* bracket_info_tab;//用于记录当前表达式每对括号的信息
	int parse_range_left = 0, parse_range_right = 0;//用于标记当前正在解析的部分在表达式中的区域
	int number_of_parsing_layers = 0;//用于记录解析的层数
public:
	Boolean_calculation(string exp, char P, char Q, char R);//构造函数
	~Boolean_calculation();//析构函数
	int check_legality(string current_expression);//判断当前表达式是否合法，并返回错误信息
	void print_error_massage(int error_massage);//输出错误信息
	void make_bracket_tab();//生成一张记录当前表达式中每对括号信息的表
	string extract_innermost_bracket(int bracket_plie);//提取最内层括号
	char superlative_operator(string simplest_expression);//返回当前表达式中优先级最高的符号
	string calculate_simplest_expression(string simplest_expression);//计算不包含括号的表达式
	int calculate();//主成员函数
	void print_final_result();//输出表达式最终的值
	void print_truth_tab();//输出表达式真值表
};

Boolean_calculation::Boolean_calculation(string exp, char P, char Q, char R)
{
	expression = exp;
	expression_copy = exp;
	p = P;
	p_copy = P;
	q = Q;
	q_copy = Q;
	r = R;
	r_copy = R;
	printf("Object has been established ...\n");
}

/*
检查表达式合法性，并输出错误列表；
	0	表达式合法
	1	命题变量错误
	2	非法字符
	3	语法错误：‘！’
	4	语法错误：‘&&’
	5	语法错误：‘||’
	6	语法错误：‘->’
	7	语法错误：‘<->’
	8	括号不匹配
	9	括号内为空
*/
int Boolean_calculation::check_legality(string current_expression)
{
	int length = current_expression.length();
	int i = 0;
	stack<unsigned char> stack_bracket;
	while (i < length)
	{
		switch (current_expression[i])
		{
		case '0':
		{
			if (i != length - 1)
				if (current_expression[i + 1] == 'p' || current_expression[i + 1] == 'q' || current_expression[i + 1] == 'r' || current_expression[i + 1] == '0' || current_expression[i + 1] == '1' || current_expression[i + 1] == '(')
					return 1;
			if (i != 0)
				if (current_expression[i - 1] == 'p' || current_expression[i - 1] == 'q' || current_expression[i - 1] == 'r' || current_expression[i - 1] == '0' || current_expression[i - 1] == '1' || current_expression[i - 1] == ')')
					return 1;
			i++;
			break;
		}
		case '1':
		{
			if (i != length - 1)
				if (current_expression[i + 1] == 'p' || current_expression[i + 1] == 'q' || current_expression[i + 1] == 'r' || current_expression[i + 1] == '0' || current_expression[i + 1] == '1' || current_expression[i + 1] == '(')
					return 1;
			if (i != 0)
				if (current_expression[i - 1] == 'p' || current_expression[i - 1] == 'q' || current_expression[i - 1] == 'r' || current_expression[i - 1] == '0' || current_expression[i - 1] == '1' || current_expression[i - 1] == ')')
					return 1;
			i++;
			break;
		}
		case 'p':
		{
			if (i != length - 1)
				if (current_expression[i + 1] == 'p' || current_expression[i + 1] == 'q' || current_expression[i + 1] == 'r' || current_expression[i + 1] == '0' || current_expression[i + 1] == '1' || current_expression[i + 1] == '(')
					return 1;
			if (i != 0)
				if (current_expression[i - 1] == 'p' || current_expression[i - 1] == 'q' || current_expression[i - 1] == 'r' || current_expression[i - 1] == '0' || current_expression[i - 1] == '1' || current_expression[i - 1] == ')')
					return 1;
			i++;
			break;
		}
		case 'q':
		{
			if (i != length - 1)
				if (current_expression[i + 1] == 'p' || current_expression[i + 1] == 'q' || current_expression[i + 1] == 'r' || current_expression[i + 1] == '0' || current_expression[i + 1] == '1' || current_expression[i + 1] == '(')
					return 1;
			if (i != 0)
				if (current_expression[i - 1] == 'p' || current_expression[i - 1] == 'q' || current_expression[i - 1] == 'r' || current_expression[i - 1] == '0' || current_expression[i - 1] == '1' || current_expression[i - 1] == ')')
					return 1;
			i++;
			break;
		}
		case 'r':
		{
			if (i != length - 1)
				if (current_expression[i + 1] == 'p' || current_expression[i + 1] == 'q' || current_expression[i + 1] == 'r' || current_expression[i + 1] == '0' || current_expression[i + 1] == '1' || current_expression[i + 1] == '(')
					return 1;
			if (i != 0)
				if (current_expression[i - 1] == 'p' || current_expression[i - 1] == 'q' || current_expression[i - 1] == 'r' || current_expression[i - 1] == '0' || current_expression[i - 1] == '1' || current_expression[i - 1] == ')')
					return 1;
			i++;
			break;
		}
		case '!':
		{
			if (i == length - 1)
				return 3;
			else if (current_expression[i + 1] != 'p' && current_expression[i + 1] != 'q' && current_expression[i + 1] != 'r' && current_expression[i + 1] != '0' && current_expression[i + 1] != '1' && current_expression[i + 1] != '(')
				return 3;
			if (i != 0)
				if (current_expression[i - 1] == 'p' || current_expression[i - 1] == 'q' || current_expression[i - 1] == 'r' || current_expression[i - 1] == '0' || current_expression[i - 1] == '1' || current_expression[i - 1] == ')')
					return 3;
			i++;
			break;
		}
		case '&':
		{
			if (current_expression[i + 1] != '&')
				return 2;
			if (i == 0)
				return 4;
			else if (current_expression[i - 1] != 'p' && current_expression[i - 1] != 'q' && current_expression[i - 1] != 'r' && current_expression[i - 1] != '0' && current_expression[i - 1] != '1' && current_expression[i - 1] != ')')
				return 4;
			if (i + 1 == length - 1)
				return 4;
			else if (current_expression[i + 2] != 'p' && current_expression[i + 2] != 'q' && current_expression[i + 2] != 'r' && current_expression[i + 2] != '0' && current_expression[i + 2] != '1' && current_expression[i + 2] != '(' && current_expression[i + 2] != '!')
				return 4;
			i += 2;
			break;
		}
		case '|':
		{
			if (current_expression[i + 1] != '|')
				return 2;
			if (i == 0)
				return 5;
			else if (current_expression[i - 1] != 'p' && current_expression[i - 1] != 'q' && current_expression[i - 1] != 'r' && current_expression[i - 1] != '0' && current_expression[i - 1] != '1' && current_expression[i - 1] != ')')
				return 5;
			if (i + 1 == length - 1)
				return 5;
			else if (current_expression[i + 2] != 'p' && current_expression[i + 2] != 'q' && current_expression[i + 2] != 'r' && current_expression[i + 2] != '0' && current_expression[i + 2] != '1' && current_expression[i + 2] != '(' && current_expression[i + 2] != '!')
				return 5;
			i += 2;
			break;
		}
		case '-':
		{
			if (current_expression[i + 1] != '>')
				return 2;
			if (i == 0)
				return 6;
			else if (current_expression[i - 1] != 'p' && current_expression[i - 1] != 'q' && current_expression[i - 1] != 'r' && current_expression[i - 1] != '0' && current_expression[i - 1] != '1' && current_expression[i - 1] != ')')
				return 6;
			if (i + 1 == length - 1)
				return 6;
			else if (current_expression[i + 2] != 'p' && current_expression[i + 2] != 'q' && current_expression[i + 2] != 'r' && current_expression[i + 2] != '0' && current_expression[i + 2] != '1' && current_expression[i + 2] != '(' && current_expression[i + 2] != '!')
				return 6;
			i += 2;
			break;
		}
		case '<':
		{
			if (current_expression[i + 1] != '-' || current_expression[i + 2] != '>')
				return 2;
			if (i == 0)
				return 7;
			else if (current_expression[i - 1] != 'p' && current_expression[i - 1] != 'q' && current_expression[i - 1] != 'r' && current_expression[i - 1] != '0' && current_expression[i - 1] != '1' && current_expression[i - 1] != ')')
				return 7;
			if (i + 2 == length - 1)
				return 7;
			else if (current_expression[i + 3] != 'p' && current_expression[i + 3] != 'q' && current_expression[i + 3] != 'r' && current_expression[i + 3] != '0' && current_expression[i + 3] != '1' && current_expression[i + 3] != '(' && current_expression[i + 3] != '!')
				return 7;
			i += 3;
			break;
		}
		case '(':
		{
			if (current_expression[i + 1] == ')')
				return 9;
			stack_bracket.push(current_expression[i]);
			bracket_cnt++;
			i++;
			break;
		}
		case ')':
		{
			if (current_expression[i + 1] == '(')
				return 1;
			if (stack_bracket.empty())
				return 8;
			else
				stack_bracket.pop();
			i++;
			break;
		}
		default:
			return 2;
		}
	}
	if (stack_bracket.empty())
	{
		//cout << current_expression << endl;
		//printf("bracket_cnt=%d\n", bracket_cnt);
		return 0;
	}
	else
		return 8;
}

void Boolean_calculation::print_error_massage(int error_massage)
{
	switch (error_massage)
	{
	case 0:
		printf("Valid Expressions...\n");
		break;
	case 1:
		ocout << "Parse Error : propositional variable !" << endl;
		break;
	case 2:
		ocout << "Unknow Character !" << endl;
		break;
	case 3:
		ocout << "Parse Error : Negation connective !" << endl;
		break;
	case 4:
		ocout << "Parse Error : Conjunction connective !" << endl;
		break;
	case 5:
		ocout << "Parse Error : Disjunction connective !" << endl;
		break;
	case 6:
		ocout << "Parse Error : Conditional connective !" << endl;
		break;
	case 7:
		ocout << "Parse Error : Biconditional connective !" << endl;
		break;
	case 8:
		ocout << "Bracket can't be Paired !" << endl;
		break;
	case 9:
		ocout << "Expect value in the Bracket !" << endl;
		break;
	}
}

void Boolean_calculation::make_bracket_tab()
{
	bracket_info_tab = new bracket_info[bracket_cnt];
	int length = expression.length();
	int i = 0, cnt = 0, nearest;
	stack<unsigned char> stack_bracket;
	while (i < length)
	{
		if (expression[i] == '(')
		{
			stack_bracket.push(expression[i]);
			bracket_info_tab[cnt].left = i;
			bracket_info_tab[cnt].right = 0;
			cnt++;
		}
		if (expression[i] == ')')
		{
			nearest = cnt - 1;
			while (bracket_info_tab[nearest].right != 0)
				nearest--;
			bracket_info_tab[nearest].right = i;
			bracket_info_tab[nearest].plie = stack_bracket.size();
			if (bracket_max_plie < bracket_info_tab[nearest].plie)
				bracket_max_plie = bracket_info_tab[nearest].plie;
			stack_bracket.pop();
		}
		i++;
	}
	/*	for (int j = 0; j < bracket_cnt; j++)
			printf("%d %d %d\n", bracket_info_tab[j].left, bracket_info_tab[j].right, bracket_info_tab[j].plie);*/
}

string Boolean_calculation::extract_innermost_bracket(int bracket_plie)
{
	for (int i = 0; i < bracket_cnt; i++)
		if (bracket_info_tab[i].plie == bracket_max_plie)
		{
			parse_range_left = bracket_info_tab[i].left;
			parse_range_right = bracket_info_tab[i].right;
			return expression.substr(bracket_info_tab[i].left + 1, bracket_info_tab[i].right - bracket_info_tab[i].left - 1);
		}
}

Boolean_calculation::~Boolean_calculation()
{
	printf("The object has been destroyed...\n");
}

char Boolean_calculation::superlative_operator(string simplest_expression)
{
	int length = simplest_expression.length();
	int i = 0, superlative = 0;
	while (i < length)
	{
		switch (simplest_expression[i])
		{
		case '!':
			return '!';
		case '&':
		{
			superlative = 2;
			i += 2;
			break;
		}
		case '|':
		{
			if (superlative > 3 || superlative == 0)
				superlative = 3;
			i += 2;
			break;
		}
		case '-':
		{
			if (superlative > 4 || superlative == 0)
				superlative = 4;
			i += 2;
			break;
		}
		case '<':
		{
			if (superlative == 0)
				superlative = 5;
			i += 3;
			break;
		}
		default:
			i++;
		}
	}
	switch (superlative)
	{
	case 1:
		return '!';
	case 2:
		return '&';
	case 3:
		return '|';
	case 4:
		return '-';
	case 5:
		return '<';
	}
}

string Boolean_calculation::calculate_simplest_expression(string simplest_expression)
{
	int length = simplest_expression.length();
	if (length == 1)
		return simplest_expression;
	else
	{
		string temporary_storage = "";
		char superlative = superlative_operator(simplest_expression);
		int i = simplest_expression.find(superlative, 0);
		switch (superlative)
		{
		case '!':
		{
			simplest_expression[i + 1] = equivalent_substitution(this, simplest_expression[i + 1]);
			temporary_storage = ::negation(simplest_expression[i + 1]);
			simplest_expression = simplest_expression.replace(i, 2, temporary_storage);
			number_of_parsing_layers++;
			temporary_storage = "";
			break;
		}
		case '&':
		{
			simplest_expression[i - 1] = equivalent_substitution(this, simplest_expression[i - 1]);
			simplest_expression[i + 2] = equivalent_substitution(this, simplest_expression[i + 2]);
			temporary_storage = ::conjunction(simplest_expression[i - 1], simplest_expression[i + 2]);
			simplest_expression = simplest_expression.replace(i - 1, 4, temporary_storage);
			number_of_parsing_layers++;
			temporary_storage = "";
			break;
		}
		case '|':
		{
			simplest_expression[i - 1] = equivalent_substitution(this, simplest_expression[i - 1]);
			simplest_expression[i + 2] = equivalent_substitution(this, simplest_expression[i + 2]);
			temporary_storage = ::disjunction(simplest_expression[i - 1], simplest_expression[i + 2]);
			simplest_expression = simplest_expression.replace(i - 1, 4, temporary_storage);
			number_of_parsing_layers++;
			temporary_storage = "";
			break;
		}
		case '-':
		{
			simplest_expression[i - 1] = equivalent_substitution(this, simplest_expression[i - 1]);
			simplest_expression[i + 2] = equivalent_substitution(this, simplest_expression[i + 2]);
			temporary_storage = ::conditional(simplest_expression[i - 1], simplest_expression[i + 2]);
			simplest_expression = simplest_expression.replace(i - 1, 4, temporary_storage);
			number_of_parsing_layers++;
			temporary_storage = "";
			break;
		}
		case '<':
		{
			simplest_expression[i - 1] = equivalent_substitution(this, simplest_expression[i - 1]);
			simplest_expression[i + 3] = equivalent_substitution(this, simplest_expression[i + 3]);
			temporary_storage = ::biconditional(simplest_expression[i - 1], simplest_expression[i + 3]);
			simplest_expression = simplest_expression.replace(i - 1, 5, temporary_storage);
			number_of_parsing_layers++;
			temporary_storage = "";
			break;
		}
		}
		int massage = check_legality(simplest_expression);
		if (massage != 0)
		{
			printf("An error occurred when parsing layer %d !\n", number_of_parsing_layers);
			expression = expression.replace(parse_range_left, parse_range_right - parse_range_left + 1, simplest_expression);
			cout << "The current expression is : " << expression << endl;
			return "error";
		}
		else
			simplest_expression = calculate_simplest_expression(simplest_expression);
	}
	return simplest_expression;
}

int Boolean_calculation::calculate()
{
	int massage = check_legality(expression);
	if (massage == 0)
	{
		if (expression.length() != 1)
		{
			string ans = "";
			if (bracket_cnt != 0)
			{
				make_bracket_tab();
				ans = extract_innermost_bracket(bracket_max_plie);
				ans = calculate_simplest_expression(ans);
				if (ans != "error")
				{
					expression = expression.replace(parse_range_left, parse_range_right - parse_range_left + 1, ans);
					bracket_cnt = 0;
					bracket_max_plie = 0;
					delete[]bracket_info_tab;
				}
				else
				{
					delete[]bracket_info_tab;
					return 0;
				}
			}
			else
			{
				parse_range_left = 0;
				parse_range_right = expression.length() - 1;
				ans = calculate_simplest_expression(expression);
				if (ans != "error")
				{
					result = ans;
					return 1;
				}
				else
					return 0;
			}
			calculate();
		}
		else
		{
			result = equivalent_substitution(this, expression[0]);
			return 1;
		}
	}
	else
	{
		ocout << "Expression : " << expression_copy << endl;
		print_error_massage(massage);
		return 0;
	}
}

void Boolean_calculation::print_final_result()
{
	ocout << "Parse successfully !" << endl;
	ocout << "Expression : " << expression_copy << ",p=" << p << ",q=" << q << ",r=" << r << endl;
	ocout << "The layer number of parsing is " << number_of_parsing_layers << endl;
	ocout << "The value of expression is " << result << " ..." << endl;
}

void Boolean_calculation::print_truth_tab()
{
	ocout << "P\tQ\tR\tF" << endl;
	for (char i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				expression = expression_copy;
				p = i + '0';
				q = j + '0';
				r = k + '0';
				calculate();
				ocout << p << "\t" << q << "\t" << r << "\t" << result << endl;
			}
	ocout << endl;
}

char equivalent_substitution(Boolean_calculation* test, char p)
{
	switch (p)
	{
	case 'p':
		return test->p;
		break;
	case 'q':
		return test->q;
		break;
	case 'r':
		return test->r;
		break;
	case '0':
		return p;
	case '1':
		return p;
	}
}

int main()
{
	string str, expression;
	char p, q, r;
	int cnt = 0;

	ifstream inf;
	inf.open("expression.txt", ifstream::in);
	ocout.open("result.txt");

	printf("Print truth tab ? (Y/N)\n");
	char a;
	cin >> a;

	while (getline(inf, str, '\n'))
	{
		string str_copy = str;
		cout << endl;
		cnt++;
		int i = str.find(',', 0);
		int length = str.length();
		expression = str.substr(0, i);
		str = str.substr(i + 1, length - i - 1);
		if (str.length() != 11)
		{
			ocout << "Input : The %d line is not formatted correctly !" << endl;
			ocout << "Your input : " << str_copy << endl;
			ocout << "Expect : expression,p=?,q=?,r=?" << endl;
			ocout << endl;
			continue;
		}
		else
		{
			Boolean_calculation test(expression, str[2], str[6], str[10]);
			int result = test.calculate();
			if (result == 0)
			{
				ocout << "Error in parsing ..." << endl;
				ocout << endl;
			}
			else
			{
				test.print_final_result();
				if (a == 'Y' || a == 'y')
					test.print_truth_tab();
				else
				{
					ocout << endl;
					continue;
				}
			}

		}
	}

	/*Boolean_calculation test("!r!p",'1','1','1');
	int result = test.calculate();
	if (result == 0)
		printf("Error in parsing ...\n");
	else
	{
		test.print_final_result();
		printf("Print truth tab ? (Y/N)\n");
		char a;
		cin >> a;
		if (a == 'Y' || a == 'y')
			test.print_truth_tab();
	}*/

	inf.close();
	ocout.close();

	cout << endl << "All expressions have been parsed !" << endl;
	return 0;
}