#include <iostream>
#include <stack>
#include <string>

int main()
{
    // std::stack<int> s;

    // s.push(10); // 추가
    // s.push(20);
    // s.top();   // 20
    // s.pop();   // 20 제거
    // s.empty(); // false
    // s.size();  // 1

    std::string str;
    std::cin >> str;

    std::stack<char> s;

    for (char ch : str)
    {
        s.push(ch);
    }

    while (!s.empty())
    {
        std::cout << s.top();
        s.pop();
    }
}
