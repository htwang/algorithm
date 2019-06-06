#include <stack>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <unordered_set>

int prec(char op)
{
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

bool isop(char c)
{
    static std::unordered_set<char> ops{'+', '-', '*', '/', '^'};
    return ops.find(c) != ops.end();
}

std::string infix_to_postfix(const std::string& expr)
{
    std::ostringstream oss;
    std::stack<char> s;

    for (char c : expr)
    {
        if (std::isspace(c)) continue;

        if (c == '$' || std::isalnum(c))
        {
            oss << c;
        }
        else if ( c == '(')
        {
            s.push(c); 
        }
        else if ( c == ')')
        {
            while (!s.empty() && s.top() != '(')
            {
                oss << s.top(); 
                s.pop();
            }
            if (!s.empty()) s.pop();
        }
        else if (isop(c))
        {
            while (!s.empty() && prec(s.top())>=prec(c))
            {
                oss << s.top();
                s.pop();
            }
            s.push(c);
        }
        else
        {
            throw std::runtime_error("unrecognized char " + c);
        }
    }
    while (!s.empty())
    {
        oss << s.top(); s.pop();
    }

    return oss.str();
}

int main()
{
    std::string infix = "($aa+$b*($c^$d-$e)^($f+$g*$h))-$i";
    std::cout << infix_to_postfix(infix) << std::endl;
}
