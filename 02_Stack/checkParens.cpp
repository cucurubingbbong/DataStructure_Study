#include <iostream>
#include <stack>
#include <string>

int main()
{
    std::string str;
    std::cin >> str;

    std::stack<char> s;
    bool valid = true;

    for (char ch : str)
    {
        if (ch == '(')
        {
            s.push(ch);
        }
        else if (ch == ')')
        {
            if (s.empty())
            {
                valid = false;
                break;
            }

            s.pop();
        }
    }

    if (!s.empty())
    {
        valid = false;
    }

    std::cout << (valid ? "YES" : "NO") << '\n';

    return 0;
}