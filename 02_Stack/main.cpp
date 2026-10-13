#include <iostream>

class Stack
{

private:
    int arr[100];
    int topIndex;

public:
    Stack()
    {
        topIndex = -1;
    }
    void push(int value)
    {
        // 직접 구현
        if (size() >= 100)
            return;
        arr[++topIndex] = value;
    }

    void pop()
    {
        if (empty())
            return;
        topIndex--;
        // 직접 구현
    }

    int top() const
    {
        if (empty())
            return -1;
        return arr[topIndex];
        // 직접 구현
    }

    bool empty() const
    {
        return topIndex == -1;
        // 직접 구현
    }

    int size() const
    {
        return topIndex + 1;
        // 직접 구현
    }
};

int main()
{
    Stack s;

    // 1. 빈 스택 테스트
    std::cout << s.empty() << "\n"; // 1
    std::cout << s.size() << "\n";  // 0
    s.pop();                        // 오류 없이 넘어가야 함

    // 2. 가득 찬 스택 테스트
    for (int i = 0; i < 100; i++)
    {
        s.push(i);
    }

    std::cout << s.size() << "\n"; // 100
    std::cout << s.top() << "\n";  // 99

    // 3. 용량 초과 테스트
    s.push(999);

    std::cout << s.size() << "\n"; // 100
    std::cout << s.top() << "\n";  // 99
}