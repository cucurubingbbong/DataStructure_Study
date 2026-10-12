# 단일 연결 리스트 (Singly Linked List) — C++

## 목차

- [단일 연결 리스트 (Singly Linked List) — C++](#단일-연결-리스트-singly-linked-list--c)
  - [목차](#목차)
- [1. 연결 리스트란?](#1-연결-리스트란)
  - [배열·vector와 다른 점](#배열vector와-다른-점)
- [2. Node 구조와 포인터](#2-node-구조와-포인터)
  - [`Node*& head`를 쓰는 이유](#node-head를-쓰는-이유)
- [3. 노드 생성·연결·순회](#3-노드-생성연결순회)
- [4. 맨 앞 삽입 — pushFront](#4-맨-앞-삽입--pushfront)
- [5. 맨 뒤 삽입 — pushBack](#5-맨-뒤-삽입--pushback)
  - [핵심: 반복문 조건](#핵심-반복문-조건)
  - [빈 리스트 예외 처리](#빈-리스트-예외-처리)
- [6. 값을 찾아 삭제 — removeValue](#6-값을-찾아-삭제--removevalue)
  - [맨 앞 삭제](#맨-앞-삭제)
  - [중간 노드 삭제](#중간-노드-삭제)
  - [처리 가능한 경우](#처리-가능한-경우)
- [7. 연결 리스트 뒤집기 — reverseList](#7-연결-리스트-뒤집기--reverselist)
  - [직접 작성한 정답 코드](#직접-작성한-정답-코드)
  - [포인터 이동을 손으로 추적하기](#포인터-이동을-손으로-추적하기)
  - [왜 순서가 중요한가?](#왜-순서가-중요한가)
  - [빈 리스트·노드 하나도 정상 처리](#빈-리스트노드-하나도-정상-처리)
- [8. 전체 출력과 메모리 해제](#8-전체-출력과-메모리-해제)
  - [출력 함수](#출력-함수)
  - [해제 함수](#해제-함수)
- [9. 종합 코드](#9-종합-코드)
- [10. 시간·공간 복잡도](#10-시간공간-복잡도)
- [11. 자주 헷갈리는 부분](#11-자주-헷갈리는-부분)
  - [이번 구현에서 배운 안전 규칙](#이번-구현에서-배운-안전-규칙)
- [12. 복습 문제와 체크리스트](#12-복습-문제와-체크리스트)
  - [구현 체크리스트](#구현-체크리스트)
  - [스스로 답해 볼 질문](#스스로-답해-볼-질문)
  - [추가 실습 (아직 구현했다고 표시하지 않음)](#추가-실습-아직-구현했다고-표시하지-않음)
- [13. 최종 정리](#13-최종-정리)

---

# 1. 연결 리스트란?

연결 리스트(Linked List)는 **데이터를 가진 노드(Node)를 포인터로 연결**한 자료구조다.

```text
head
 ↓
[10 | next] → [20 | next] → [30 | nullptr]
```

각 노드는 두 정보를 가진다.

- `data`: 노드에 저장할 값
- `next`: 다음 노드의 주소

마지막 노드는 다음 노드가 없기 때문에 `next == nullptr`이다.

## 배열·vector와 다른 점

```text
배열 / vector (논리적 모습)
[10][20][30]    ← 원소들이 연속적으로 저장됨

단일 연결 리스트
[10 | *] --→ [20 | *] --→ [30 | null]
   ↑             ↑             ↑
각 노드는 서로 다른 메모리 위치에 있을 수 있음
```

| 비교 | `vector` | 단일 연결 리스트 |
| --- | --- | --- |
| 저장 구조 | 원소가 연속된 메모리 | 노드들을 포인터로 연결 |
| 인덱스 접근 | `v[i]`: `O(1)` | `i`번째 노드까지 이동: `O(n)` |
| 맨 앞 삽입 | `O(n)` | `head`가 있으면 `O(1)` |
| 맨 뒤 삽입 | 상환 `O(1)` | `tail`이 없다면 `O(n)` |
| 특정 값 검색 | `O(n)` (정렬·별도 색인 없음) | `O(n)` |
| 메모리 특성 | 연속 공간, 캐시 효율이 대체로 좋음 | 노드마다 포인터 저장과 동적 할당 비용 발생 |

> **주의:** 연결 리스트에서 삽입·삭제가 언제나 `O(1)`인 것은 아니다. 수정할 위치를 이미 알고 있을 때 연결을 바꾸는 작업이 `O(1)`이고, **위치를 찾는 과정**은 보통 `O(n)`이다.

---

# 2. Node 구조와 포인터

```cpp
struct Node
{
    int data;
    Node* next;
};
```

예를 들어 아래 코드는 값이 `10`인 노드 하나를 **동적으로 생성**한다.

```cpp
Node* head = new Node{10, nullptr};
```

```text
head (Node* 변수)
  │
  └──→ [data: 10 | next: nullptr]
```

- `Node* head`: `Node`를 가리키는 포인터 변수
- `new Node{10, nullptr}`: 노드를 생성하고 주소 반환
- `head->data`: 가리키는 노드의 `data` 접근
- `head->next`: 가리키는 노드의 다음 노드 주소 접근
- `delete head`: `new`로 만든 노드 해제

## `Node*& head`를 쓰는 이유

```cpp
void pushFront(Node*& head, int value);
```

`Node*&`는 **Node 포인터에 대한 참조**다.

```cpp
void badExample(Node* head) // 포인터 값이 복사됨
{
    head = new Node{5, head};
}
```

이 함수는 호출한 쪽의 `head`를 바꾸지 못한다. 새 노드의 주소도 반환하지 않으므로 누수가 발생한다.

반면:

```cpp
void pushFront(Node*& head, int value)
{
    head = new Node{value, head};
}
```

호출한 쪽의 포인터 변수 자체가 바뀐다.

```text
Node* head    → 노드 주소를 담는 변수
Node*& head   → 그 포인터 변수의 참조
```

> 포인터를 읽기만 한다면 `Node*` 전달로 충분하지만, **호출자의 head 값을 변경**하려면 `Node*&` 또는 `Node**` 등이 필요하다.

---

# 3. 노드 생성·연결·순회

처음 직접 구현한 형태:

```cpp
Node* head = new Node{10, nullptr};
Node* second = new Node{20, nullptr};
Node* third = new Node{30, nullptr};

head->next = second;
second->next = third;
```

결과:

```text
head
 ↓
10 → 20 → 30 → nullptr
```

순회할 때는 `head`를 직접 이동시키지 않고 **별도의 이동 포인터 `curr`**를 사용한다.

```cpp
Node* curr = head;

while (curr != nullptr)
{
    std::cout << curr->data << ' ';
    curr = curr->next;
}
```

출력:

```text
10 20 30
```

**왜 `curr`를 쓰는가?**

`head`를 옮겨 버리면 리스트의 시작 위치를 잃을 수 있기 때문이다. `curr`는 순회용이고 `head`는 시작 노드를 가리키도록 유지한다.

노드가 `n`개일 때 전체 순회는 `O(n)`, 추가 공간은 `O(1)`이다.

---

# 4. 맨 앞 삽입 — pushFront

목표:

```text
삽입 전: head → 10 → 20 → 30 → null
삽입 후: head →  5 → 10 → 20 → 30 → null
```

처음에는 빈 리스트를 `if`로 분리해 구현했지만, 실제로는 더 간단하게 만들 수 있다.

```cpp
void pushFront(Node*& head, int value)
{
    Node* newHead = new Node{value, head};
    head = newHead;
}
```

실행 순서:

1. 새 노드의 `next`를 **기존 head**로 지정한다.
2. `head`를 **새 노드 주소**로 변경한다.

```text
기존:       head → [10] → [20]
새 노드:    [5] ─────────→ [10]
변경 후:    head → [5] → [10] → [20]
```

`head == nullptr`이라도 `new Node{value, head}`의 `next`가 `nullptr`이 되므로 정상 작동한다.

**시간 복잡도: `O(1)` / 추가 공간: 새 노드 `O(1)`**

---

# 5. 맨 뒤 삽입 — pushBack

목표:

```text
삽입 전: head → 10 → 20 → 30 → null
삽입 후: head → 10 → 20 → 30 → 40 → null
```

```cpp
void pushBack(Node*& head, int value)
{
    if (head == nullptr)
    {
        head = new Node{value, nullptr};
        return;
    }

    Node* curr = head;

    while (curr->next != nullptr)
    {
        curr = curr->next;
    }

    curr->next = new Node{value, nullptr};
}
```

## 핵심: 반복문 조건

```cpp
while (curr->next != nullptr)
```

`curr->next`가 `nullptr`이 되면 **현재 `curr`가 마지막 노드**다.

반면 `while (curr != nullptr)`로 끝까지 이동하면 `curr` 자체가 `nullptr`이 되어, 그대로 `curr->next = ...`를 실행할 수 없다.

## 빈 리스트 예외 처리

처음부터 `head == nullptr`이라면 `curr->next`를 사용할 수 없다. 그래서 **빈 리스트를 먼저 처리**한다.

**시간 복잡도: `O(n)`** (`tail` 포인터를 별도로 유지하지 않는 경우).

> `tail`이 마지막 노드를 가리키도록 유지하면 맨 뒤 삽입도 `O(1)`로 만들 수 있다. 대신 삽입·삭제마다 `tail` 갱신이 올바른지 관리해야 한다.

---

# 6. 값을 찾아 삭제 — removeValue

목표:

```text
삭제 전: 10 → 20 → 30 → 40 → null
removeValue(head, 30)
삭제 후: 10 → 20 ─────→ 40 → null
```

직접 구현한 코드를 변수 이름만 통일해 정리하면 다음과 같다.

```cpp
void removeValue(Node*& head, int value)
{
    // 1. 빈 리스트
    if (head == nullptr)
    {
        return;
    }

    // 2. 첫 번째 노드 삭제
    if (head->data == value)
    {
        Node* nextHead = head->next;
        delete head;
        head = nextHead;
        return;
    }

    // 3. 중간 / 마지막 노드 탐색 및 삭제
    Node* prev = nullptr;
    Node* curr = head;

    while (curr != nullptr)
    {
        if (curr->data == value)
        {
            prev->next = curr->next;
            delete curr;
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}
```

## 맨 앞 삭제

```cpp
Node* nextHead = head->next; // 다음 노드 기억
delete head;                  // 기존 head 해제
head = nextHead;              // 새 head 지정
```

노드가 하나뿐이어도 `nextHead == nullptr`이 되므로 문제없다.

## 중간 노드 삭제

```text
prev       curr
 ↓          ↓
20 ──────→ 30 ──────→ 40
```

```cpp
prev->next = curr->next;
delete curr;
```

1. `20`의 `next`를 `40`의 주소로 바꾼다.
2. 연결에서 빠진 `30` 노드를 해제한다.
3. 더 이상 `curr`를 역참조하지 않는다.

> `prev`가 `nullptr`이면 위험하지 않을까? 위 구현에서는 **head 삭제를 먼저 처리**했기 때문에, 이후 탐색 중 첫 노드가 아닌 일치 노드를 찾았다면 `prev`가 반드시 존재한다.

## 처리 가능한 경우

- 빈 리스트: 변경 없음
- 첫 번째 노드: `head` 갱신
- 중간 노드: `prev->next` 재연결
- 마지막 노드: 이전 노드의 `next`가 `nullptr`이 됨
- 없는 값: 끝까지 탐색하고 변경 없음
- 중복 값: **먼저 만난 노드 하나만** 삭제 (`return`)

**시간 복잡도:** 첫 노드 삭제 `O(1)`, 값 탐색이 필요하면 최악 `O(n)` / 추가 공간 `O(1)`.

---

# 7. 연결 리스트 뒤집기 — reverseList

이번 학습에서 가장 까다로운 포인터 조작이다. **새 노드를 만들지 않고 `next` 방향만 반대로 바꾼다.**

```text
뒤집기 전
head → 10 → 20 → 30 → 40 → null

뒤집기 후
head → 40 → 30 → 20 → 10 → null
```

## 직접 작성한 정답 코드

```cpp
void reverseList(Node*& head)
{
    Node* prev = nullptr;
    Node* curr = head;
    Node* nextNode = nullptr;

    while (curr != nullptr)
    {
        nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }

    head = prev;
}
```

핵심은 아래 **4줄의 순서**다.

```cpp
nextNode = curr->next; // 1. 원래 다음 노드의 주소를 보존
curr->next = prev;     // 2. 현재 노드의 화살표를 반대로
prev = curr;           // 3. 뒤집힌 구간의 맨 앞으로 이동
curr = nextNode;       // 4. 아직 안 뒤집은 다음 노드로 이동
```

## 포인터 이동을 손으로 추적하기

초기 상태:

```text
prev = null
curr ↓
       10 → 20 → 30 → null
```

**1회전: 10을 뒤집음**

```text
뒤집힌 부분: 10 → null  (prev = 10)
남은 부분:   20 → 30 → null  (curr = 20)
```

**2회전: 20을 뒤집음**

```text
뒤집힌 부분: 20 → 10 → null  (prev = 20)
남은 부분:   30 → null       (curr = 30)
```

**3회전: 30을 뒤집음**

```text
뒤집힌 부분: 30 → 20 → 10 → null  (prev = 30)
남은 부분:   없음                    (curr = null)
```

반복문 종료 후:

```cpp
head = prev;
```

이제 `head`는 `30`을 가리킨다.

## 왜 순서가 중요한가?

**잘못된 순서:**

```cpp
curr->next = prev;
// nextNode = curr->next; // 이미 방향이 바뀌어서 원래 다음 노드를 잃음
```

`curr->next`를 덮어쓴 뒤에는 원래 다음 노드 주소를 복구할 수 없으므로, **반드시 먼저 `nextNode`에 저장**해야 한다.

## 빈 리스트·노드 하나도 정상 처리

- `head == nullptr`: `while`문을 건너뛰고 `head = nullptr`
- 노드 1개: `next`가 `nullptr`인 상태를 유지하고 자기 자신이 새 head가 됨
- 일반 리스트: 모든 노드를 한 번씩 방문

**시간 복잡도: `O(n)` / 추가 공간 복잡도: `O(1)`**. `new`나 새 노드 생성은 필요 없다.

---

# 8. 전체 출력과 메모리 해제

## 출력 함수

```cpp
void printList(const Node* head)
{
    const Node* curr = head;

    while (curr != nullptr)
    {
        std::cout << curr->data << ' ';
        curr = curr->next;
    }

    std::cout << '\n';
}
```

출력만 하므로 `const Node*`를 사용할 수 있다.

## 해제 함수

```cpp
void clearList(Node*& head)
{
    Node* curr = head;

    while (curr != nullptr)
    {
        Node* nextNode = curr->next; // 삭제 전에 다음 노드 보존
        delete curr;
        curr = nextNode;
    }

    head = nullptr;
}
```

**안전한 순서:**

```text
1. curr->next 저장
2. curr 삭제
3. 저장해 둔 nextNode로 이동
```

`delete curr`를 실행한 다음 `curr->next`를 읽으면 **해제된 메모리 접근(Undefined Behavior)** 문제가 생긴다.

`curr = nullptr`로 바꾸는 것만으로는 노드 메모리가 해제되지 않는다. 그리고 `clearList` 후 `head = nullptr`로 초기화해 두면 호출자가 해제된 첫 노드를 잘못 사용할 위험을 줄일 수 있다.

---

# 9. 종합 코드

아래는 지금까지 직접 구현한 기능들을 하나로 정리한 **실행 가능한 C++ 예제**다. 출력·해제만 함수로 분리했고, 함수 이름은 일반적인 소문자 시작 스타일로 통일했다.

```cpp
#include <iostream>

struct Node
{
    int data;
    Node* next;
};

void pushFront(Node*& head, int value)
{
    head = new Node{value, head};
}

void pushBack(Node*& head, int value)
{
    if (head == nullptr)
    {
        head = new Node{value, nullptr};
        return;
    }

    Node* curr = head;
    while (curr->next != nullptr)
    {
        curr = curr->next;
    }

    curr->next = new Node{value, nullptr};
}

void removeValue(Node*& head, int value)
{
    if (head == nullptr)
    {
        return;
    }

    if (head->data == value)
    {
        Node* nextHead = head->next;
        delete head;
        head = nextHead;
        return;
    }

    Node* prev = nullptr;
    Node* curr = head;

    while (curr != nullptr)
    {
        if (curr->data == value)
        {
            prev->next = curr->next;
            delete curr;
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}

void reverseList(Node*& head)
{
    Node* prev = nullptr;
    Node* curr = head;
    Node* nextNode = nullptr;

    while (curr != nullptr)
    {
        nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }

    head = prev;
}

void printList(const Node* head)
{
    const Node* curr = head;

    while (curr != nullptr)
    {
        std::cout << curr->data << ' ';
        curr = curr->next;
    }

    std::cout << '\n';
}

void clearList(Node*& head)
{
    Node* curr = head;

    while (curr != nullptr)
    {
        Node* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }

    head = nullptr;
}

int main()
{
    Node* head = nullptr;

    pushFront(head, 20);
    pushFront(head, 10);
    pushBack(head, 30);
    pushBack(head, 40);
    printList(head);             // 10 20 30 40

    removeValue(head, 10);      // 맨 앞 삭제
    removeValue(head, 30);      // 중간 삭제
    removeValue(head, 99);      // 없는 값은 그대로
    printList(head);             // 20 40

    reverseList(head);
    printList(head);             // 40 20

    clearList(head);
    printList(head);             // 빈 줄

    return 0;
}
```

예상 출력:

```text
10 20 30 40
20 40
40 20

```

컴파일 예시:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
./main
```

메모리 오류 점검이 가능하다면 AddressSanitizer 사용:

```bash
g++ -std=c++17 -g -fsanitize=address,undefined -fno-omit-frame-pointer main.cpp -o main
./main
```

---

# 10. 시간·공간 복잡도

| 연산 | 시간 복잡도 | 추가 공간 복잡도 | 비고 |
| --- | --- | --- | --- |
| 맨 앞 노드 접근 | `O(1)` | `O(1)` | `head` 사용 |
| `pushFront` | `O(1)` | `O(1)` | 새 노드 1개 할당 |
| `pushBack` | `O(n)` | `O(1)` | `tail` 포인터 없는 구현 |
| 특정 값 탐색 | `O(n)` | `O(1)` | 최악에 모든 노드 확인 |
| `removeValue` | `O(n)` | `O(1)` | 맨 앞 삭제는 `O(1)` |
| `reverseList` | `O(n)` | `O(1)` | 새 노드 생성하지 않음 |
| `printList` | `O(n)` | `O(1)` | 전체 방문 |
| `clearList` | `O(n)` | `O(1)` | 전체 해제 |

> 표의 **추가 공간 복잡도**에서 `pushFront`·`pushBack`의 새 노드 1개는 `O(1)`이며, 리스트 전체에 저장한 `n`개 노드의 공간은 총 `O(n)`이다.

---

# 11. 자주 헷갈리는 부분

| 헷갈리기 쉬운 생각 | 올바른 이해 |
| --- | --- |
| `head`를 순회용으로 옮겨도 된다 | 시작 주소를 잃을 수 있으니 별도 `curr` 사용 |
| `head == nullptr`이어도 `head->next`를 검사한다 | `nullptr`은 역참조 불가능하므로 먼저 분기 |
| `Node* head` 매개변수를 수정하면 호출자의 head도 바뀐다 | 포인터가 값으로 복사됨. head 변경이 필요하면 `Node*&` 사용 |
| `delete curr` 후 `curr->next`를 읽을 수 있다 | 이미 객체 수명이 끝났으므로 잘못된 접근 |
| `curr = nullptr`이면 메모리도 해제된다 | 포인터 변수만 변경. `delete`가 필요 |
| 연결 리스트의 모든 삽입·삭제는 `O(1)`이다 | 위치 탐색까지 필요하면 `O(n)` |
| 뒤집을 때 바로 `curr->next = prev`를 실행해도 된다 | 원래 다음 노드 주소를 먼저 저장해야 함 |
| 값이 중복되면 `removeValue`가 전부 삭제한다 | 이 구현은 `return` 때문에 **첫 번째 일치 노드 하나만 삭제** |
| `delete` 이후 모든 별칭 포인터가 `nullptr`이 된다 | 다른 포인터는 무효한 주소를 계속 가질 수 있음 |

## 이번 구현에서 배운 안전 규칙

1. 역참조(`->`)하기 전에 포인터가 유효한지 확인한다.
2. 링크를 덮어쓰기 전에 필요한 **기존 주소부터 저장**한다.
3. `delete`한 노드를 다시 역참조하지 않는다.
4. 모든 노드를 해제했다면 `head`를 `nullptr`로 초기화한다.
5. 동적 할당한 노드를 누가 소유하고 해제하는지 명확하게 한다.
6. 포인터 3개(`prev`, `curr`, `nextNode`)의 역할을 종이에 그려 본다.

---

# 12. 복습 문제와 체크리스트

## 구현 체크리스트

- [x] `Node` 구조체 정의
- [x] 노드 3개를 직접 연결
- [x] `while`문으로 전체 노드 순회
- [x] `pushFront(Node*& head, int value)` 구현
- [x] `pushBack(Node*& head, int value)` 구현
- [x] 빈 리스트에서 삽입 처리
- [x] `removeValue(Node*& head, int value)` 구현
- [x] 첫 노드 / 중간 노드 / 없는 값 처리
- [x] `reverseList(Node*& head)` 구현
- [x] 반복문으로 메모리 전부 해제

## 스스로 답해 볼 질문

1. 왜 `pushFront`는 빈 리스트에서도 별도 `if`문이 필요 없을까?
2. `pushBack`에서 `while (curr->next != nullptr)`을 사용하는 이유는?
3. `removeValue`에서 `prev->next = curr->next`를 먼저 실행하는 이유는?
4. `reverseList`에서 `curr->next = prev`보다 `nextNode = curr->next`가 먼저인 이유는?
5. `new`로 생성한 노드를 해제한 뒤 `head`만 `nullptr`로 바꾸면 다른 포인터도 안전해질까?
6. `tail` 포인터를 유지하면 맨 뒤 삽입의 시간 복잡도는 어떻게 달라질까?

## 추가 실습 (아직 구현했다고 표시하지 않음)

- [ ] `findValue(head, value)` : 값이 있는지 탐색하기
- [ ] `sizeList(head)` : 전체 노드 개수 세기
- [ ] `insertAfter(node, value)` : 특정 노드 뒤에 삽입하기
- [ ] `removeAll(head, value)` : 같은 값을 가진 모든 노드 삭제하기
- [ ] 빈 리스트 / 노드 1개 / 중복 데이터로 모든 함수 테스트하기

다음 학습 주제: **스택(Stack)**. 배열과 `std::stack`을 비교하고, 직접 구현한 뒤 `push`, `pop`, `top` 동작을 익힌다.

---

# 13. 최종 정리

```text
단일 연결 리스트
└─ Node = data + next
└─ head = 첫 번째 노드 주소
└─ 마지막 노드의 next = nullptr

pushFront
└─ 새 노드가 기존 head를 가리킴
└─ head를 새 노드로 변경
└─ O(1)

pushBack
└─ 마지막 노드까지 curr 이동
└─ curr->next에 새 노드 연결
└─ O(n) (tail 없는 경우)

removeValue
└─ 맨 앞 삭제는 head 변경
└─ 중간 삭제는 prev->next를 curr->next로 연결
└─ 삭제한 노드는 delete
└─ 탐색 포함 최악 O(n)

reverseList
└─ nextNode = curr->next  // 다음 노드 기억
└─ curr->next = prev      // 연결 방향 변경
└─ prev = curr            // prev 이동
└─ curr = nextNode        // curr 이동
└─ head = prev            // 새 시작점 설정
└─ O(n) 시간 / O(1) 추가 공간

메모리 안전
└─ delete 전에 다음 주소 기억
└─ delete한 노드 재사용 금지
└─ 전체 해제 후 head = nullptr
```

**가장 중요한 한 줄:** 연결 리스트는 값을 움직이는 것보다 **노드 사이의 주소 연결을 정확한 순서로 수정하는 것**이 핵심이다.
