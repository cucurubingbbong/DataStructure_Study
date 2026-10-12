#include <iostream>

struct Node
{
    int data;
    Node *next;
};

void PushFront(Node *&head, int value)
{
    // 직접 구현
    // Node *newHead = new Node{value, nullptr};
    // if (head == nullptr)
    // {
    //     head = newHead;
    //     return;
    // }

    Node *newHead = new Node{value, head};
    head = newHead;
}

void PushBack(Node *&head, int value)
{
    // 직접 구현
    if (head == nullptr)
    {
        head = new Node{value, nullptr};
        return;
    }
    Node *curr = head;
    while (curr->next != nullptr)
    {
        curr = curr->next;
    }
    curr->next = new Node{value, nullptr};
}

void RemoveValue(Node *&head, int value)
{
    // 직접 구현
    if (head == nullptr)
        return;

    if (head->data == value)
    {
        Node *nextHead = head->next;
        delete head;
        head = nextHead;
        return;
    }
    Node *pre = nullptr;
    Node *crr = head;
    while (crr != nullptr)
    {
        if (crr->data == value)
        {
            pre->next = crr->next;
            delete crr;
            return;
        }
        pre = crr;
        crr = crr->next;
    }
}

void ReverseList(Node *&head)
{
    Node *prev = nullptr;
    Node *curr = head;
    Node *nextNode = nullptr;

    while(curr != nullptr)
    {
        nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr =  nextNode;
    }
    head = prev;
}

int main()
{
    Node *head = nullptr;

    PushFront(head, 20);
    PushFront(head, 10);

    PushBack(head, 30);
    PushBack(head, 40);

    // 10 20 30 40

    RemoveValue(head, 10); // 맨 앞 삭제
    RemoveValue(head, 30); // 중간 삭제
    RemoveValue(head, 99); // 없는 값 삭제

    ReverseList(head);

    Node *curr = head;
    // 전체 노드 출력
    while (curr != nullptr)
    {
        std::cout << curr->data << "\n";
        curr = curr->next;
    }
    curr = head;

    // 전체 노드 메모리 해제
    while (curr != nullptr)
    {
        Node *nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    return 0;
}