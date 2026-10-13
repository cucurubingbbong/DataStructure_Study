# Stack (스택) — 직접 구현과 C++ STL

> C++ 자료구조 공부 **Day 2** 정리.  

---

# 1. Stack이란?

스택(Stack)은 **가장 나중에 들어온 데이터가 가장 먼저 나가는 LIFO(Last In, First Out)** 자료구조이다.

```text
push(10)      push(20)      push(30)      pop()

┌────┐        ┌────┐        ┌────┐        ┌────┐
│ 10 │        │ 20 │ ← top  │ 30 │ ← top  │ 20 │ ← top
└────┘        ├────┤        ├────┤        ├────┤
              │ 10 │        │ 20 │        │ 10 │
              └────┘        ├────┤        └────┘
                            │ 10 │
                            └────┘
```

- **push:** 스택 맨 위에 데이터를 넣는다.
- **pop:** 스택 맨 위 데이터를 제거한다.
- **top:** 스택 맨 위 데이터를 확인한다.
- **empty:** 스택이 비어 있는지 확인한다.
- **size:** 스택에 저장된 데이터 개수를 확인한다.

스택은 **중간 위치의 원소를 자유롭게 접근하는 구조가 아니다.** 맨 위(`top`)를 기준으로 작업한다.

---

# 2. 배열로 Stack 구현하기

## topIndex의 역할

크기 100인 배열과 `topIndex` 하나로 스택을 구현할 수 있다.

```cpp
int arr[100];
int topIndex = -1;
```

`topIndex`는 **현재 스택의 마지막(맨 위) 원소가 들어 있는 배열 인덱스**이다.

```text
초기 상태
arr = [ ?, ?, ?, ... ]
topIndex = -1    // 유효한 원소가 없음

push(10)
arr = [ 10, ?, ?, ... ]
topIndex = 0

push(20)
arr = [ 10, 20, ?, ... ]
topIndex = 1

pop()
arr = [ 10, 20, ?, ... ]
topIndex = 0     // 20이 남아 보여도 유효한 원소는 10뿐
```

중요한 관계:

```text
스택이 비어 있음 ⇔ topIndex == -1
스택의 원소 개수 = topIndex + 1
현재 맨 위 원소 = arr[topIndex] (비어 있지 않을 때만)
스택이 가득 참 ⇔ topIndex == 99 (용량이 100일 때)
```

## 직접 구현한 Stack 클래스

학습 중 직접 작성한 코드에서 주석만 정리한 버전이다.

```cpp
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
        if (size() >= 100) return;
        arr[++topIndex] = value;
    }

    void pop()
    {
        if (empty()) return;
        topIndex--;
    }

    int top() const
    {
        // 전제: 스택이 비어 있지 않아야 한다.
        return arr[topIndex];
    }

    bool empty() const
    {
        return topIndex == -1;
    }

    int size() const
    {
        return topIndex + 1;
    }
};
```

이 구현은 **용량이 고정된 100개짜리 스택**이다. `push()`는 가득 차면 추가하지 않고, `pop()`은 비어 있으면 아무 동작도 하지 않는다.

> **주의:** 위 `top()`에는 빈 스택 검사가 없다. `topIndex == -1`일 때 `arr[-1]`에 접근하면 **정의되지 않은 동작(Undefined Behavior)** 이다. 실제 사용 전에 비어 있지 않은지 확인해야 한다.

---

# 3. 각 멤버 함수가 동작하는 원리

## 생성자 — 초기 상태 설정

```cpp
Stack()
{
    topIndex = -1;
}
```

- 배열에 데이터가 있든 없든 **스택의 유효한 원소가 없다는 상태**를 표현한다.
- 멤버 선언부에서 `int topIndex = -1;`로 초기화하는 방법도 있다.

## push() — 맨 위에 추가

```cpp
void push(int value)
{
    if (size() >= 100) return;
    arr[++topIndex] = value;
}
```

`++topIndex`는 **먼저 인덱스를 증가시킨 후** 해당 위치에 값을 저장한다.

```text
topIndex = -1
push(10)
  1. ++topIndex → 0
  2. arr[0] = 10
```

`arr[topIndex++] = value;`라고 쓰면 현재 인덱스에 먼저 접근하므로, 초기 인덱스 `-1`에서 잘못된 접근이 일어난다. **전위 증가와 후위 증가의 차이**를 기억한다.

## pop() — 맨 위에서 제거

```cpp
void pop()
{
    if (empty()) return;
    topIndex--;
}
```

배열 칸의 값을 실제로 `0`으로 바꿀 필요는 없다. `topIndex`가 감소하면 그 뒤의 데이터는 더 이상 **스택의 유효한 원소가 아니다.** 이후 `push()`로 해당 칸에 다른 값이 들어갈 수 있다.

## top() — 맨 위 값 확인

```cpp
int top() const
{
    return arr[topIndex];
}
```

`topIndex`가 유효할 때 배열의 해당 인덱스를 **즉시 접근**한다. 다만 **빈 스택에서 호출하면 안 된다.**

## empty(), size() — 상태 확인

```cpp
bool empty() const
{
    return topIndex == -1;
}

int size() const
{
    return topIndex + 1;
}
```

`const`는 이 함수들이 객체의 멤버 상태를 변경하지 않는다는 의미이다.

---

# 4. 경계 조건과 안전성

## 직접 확인한 테스트

| 테스트 | 확인 내용 | 결과 |
| --- | --- | --- |
| 빈 스택 | `empty()`는 `true`, `size()`는 `0` | 통과 |
| 빈 스택의 `pop()` | 아무 동작 없이 반환 | 통과 |
| 원소 100개 삽입 | `size()`는 `100`, `top()`은 마지막 값 | 통과 |
| 가득 찬 스택에 추가 삽입 | 101번째 원소는 추가되지 않음 | 통과 |
| 용량 초과 후 상태 | `size()`와 `top()`이 유지됨 | 통과 |

이 테스트들은 **정상 동작을 확인**했지만, 모든 입력과 모든 사용 패턴에 대해 안전성을 증명하는 것은 아니다.

## 빈 스택에서 top() 호출하는 문제

```cpp
int top() const
{
    return arr[topIndex]; // topIndex == -1이라면 잘못된 접근
}
```

직접 만드는 스택에서는 안전성을 위해 예외를 던지도록 개선할 수 있다.

```cpp
#include <stdexcept>

int top() const
{
    if (empty())
    {
        throw std::out_of_range("Stack is empty");
    }
    return arr[topIndex];
}
```

이것은 **선택 가능한 설계**이다. 예외 대신 오류 상태를 반환하는 인터페이스를 설계할 수도 있다. 중요한 것은 잘못된 접근을 피하는 것이다.

## 용량 초과 처리

현재 구현:

```cpp
if (size() >= 100) return;
```

학습용으로는 충분하지만, 실제 코드에서는 **삽입 실패를 호출자에게 알릴 방법**을 고려한다. 현재는 `push()`를 호출해도 삽입되지 않았다는 사실을 호출자가 바로 알 수 없다.

---

# 5. 시간 복잡도(Time Complexity)

| 연산 | 시간 복잡도 | 이유 |
| --- | --- | --- |
| `push()` | `O(1)` | `topIndex` 증가 후 해당 배열 위치에 저장 |
| `pop()` | `O(1)` | `topIndex` 하나 감소 |
| `top()` | `O(1)` | `arr[topIndex]`로 직접 접근 |
| `empty()` | `O(1)` | 정수 한 번 비교 |
| `size()` | `O(1)` | `topIndex + 1` 계산 |

**핵심 이유:** `topIndex`를 이용해 맨 위 원소의 위치를 **이미 알고 있기 때문**에, 스택의 원소 수가 증가해도 전체를 순회할 필요가 없다.

### 연결 리스트 `pushBack()`과 비교

```text
단일 연결 리스트 pushBack() (tail 포인터 없음)
  head에서 시작 → 마지막 노드까지 탐색 → 연결
  O(n)

배열 기반 Stack push()
  topIndex가 위치를 알고 있음 → 바로 저장
  O(1)
```

단, 연결 리스트도 **`tail` 포인터를 별도로 관리한다면** 맨 뒤 삽입을 `O(1)`로 만들 수 있다.

### 공간 복잡도

- 이번 배열 기반 스택은 `int arr[100]`을 내부에 고정으로 보관하므로 **전체 저장 공간은 O(1)** (100칸 고정)이다.
- 동적으로 커지는 일반적인 스택은 저장한 원소 수를 `n`이라 할 때 **O(n)** 저장 공간이 필요하다.

---

# 6. C++ STL — std::stack

C++에는 `<stack>` 헤더에서 제공하는 `std::stack`이 있다.

```cpp
#include <stack>

std::stack<int> s;
s.push(10);
s.push(20);

int value = s.top();  // 20 (비어 있지 않을 때)
s.pop();              // 20 제거

bool isEmpty = s.empty(); // false
std::size_t count = s.size(); // 1
```

| 기능 | 직접 만든 Stack | STL `std::stack` |
| --- | --- | --- |
| 값 추가 | `push(x)` | `push(x)` |
| 맨 위 제거 | `pop()` | `pop()` |
| 맨 위 확인 | `top()` | `top()` |
| 비었는지 확인 | `empty()` | `empty()` |
| 개수 확인 | `size()` | `size()` |
| 기본 용량 | 100개 고정 | 기본적으로 동적으로 확장 |
| 내부 저장 구조 | `int[100]` | 기본 내부 컨테이너는 `std::deque` |

`std::stack`은 **컨테이너 어댑터(Container Adapter)** 이다. 내부 컨테이너의 기능 중 스택에 필요한 인터페이스만 외부에 제공한다.

## STL stack의 특징

- `s[0]`처럼 인덱스로 접근할 수 없다.
- 일반적인 `begin()`, `end()` 반복자를 제공하지 않는다.
- `top()`은 **맨 위 원소에 대한 참조**를 반환한다.
- `pop()`은 **제거만 수행하며 반환값이 없다.**
- `top()` 또는 `pop()`을 호출하기 전에 스택이 비어 있지 않은지 확인해야 한다.

예를 들어 **제거할 값을 먼저 사용해야 한다면 `top()`으로 확인한 뒤 `pop()`** 한다.

```cpp
if (!s.empty())
{
    int last = s.top();
    s.pop();
}
```

`std::stack`의 연산 복잡도는 내부 컨테이너에 의존한다. 기본 `std::deque`를 사용할 때 `push`, `pop`, `top`은 상수 시간 연산이다.

---

# 7. 스택과 배열, 연결 리스트 비교

| 구분 | 일반 배열 / vector | 배열 기반 스택 | 연결 리스트 |
| --- | --- | --- | --- |
| 접근 방식 | 인덱스로 임의 접근 | 맨 위 중심 | 포인터를 따라 순차 접근 |
| 맨 뒤 추가 | `vector` 상환 O(1) | O(1) (용량 내) | tail 없으면 O(n) |
| 맨 앞 추가 | `vector` O(n) | 직접 제공하지 않음 | head가 있으면 O(1) |
| 탐색 | O(n) | 인터페이스에 없음 | O(n) |
| 용도 | 일반적인 순차 데이터 | 되돌리기, 중첩 처리, 후입선출 | 삽입·삭제용 노드 연결 |

스택은 반드시 배열로 구현해야 하는 것은 아니다. **연결 리스트로도 스택을 구현할 수 있다.** 핵심은 어떤 자료구조를 내부에 쓰느냐보다 **LIFO 규칙과 제공하는 연산**이다.

---

# 8. 자주 헷갈리는 부분

| 착각하기 쉬운 내용 | 정확한 이해 |
| --- | --- |
| `pop()`을 하면 배열 값이 반드시 지워진다 | `topIndex`만 변경해도 논리적으로 제거됨 |
| `topIndex`는 원소 개수다 | **마지막 유효 인덱스**이며 원소 개수는 `topIndex + 1` |
| `arr[topIndex++]`와 `arr[++topIndex]`는 같다 | 전자는 접근 후 증가, 후자는 증가 후 접근 |
| `top()`은 언제나 안전하다 | 빈 스택에서 호출하면 잘못된 접근 발생 |
| STL `pop()`이 삭제한 값을 반환한다 | `std::stack::pop()`은 `void` |
| `std::stack`은 `vector`처럼 순회할 수 있다 | `begin()`, `end()`, `operator[]` 없음 |
| 스택의 모든 작업은 O(1)이다 | **기본 5개 연산**에 대한 설명이며 전체 원소 탐색 같은 작업은 별개 |
| 배열 스택은 무한히 커질 수 있다 | 이번 구현은 **최대 100개** |

---

# 9. Day 2 학습 기록

| 학습 항목 | 상태 |
| --- | --- |
| 배열로 `Stack` 클래스 직접 구현 | 완료 |
| `push`, `pop`, `top`, `empty`, `size` 구현 | 완료 |
| 빈 스택과 가득 찬 스택 테스트 | 완료 |
| 각 연산의 O(1) 이유 설명 | 완료 |
| `std::stack` 기본 사용법 | 완료 |
| STL 스택으로 문자열 역순 출력 | 직접 구현 완료 |
| 괄호 검사 / 백준 10828 | 참고용 코드 확인 (직접 풀이로 기록하지 않음) |

> 문제 풀이 소스코드는 이 노트에 포함하지 않았다. 학습 개념과 자료구조 구현 코드만 남긴다.

---

# 10. 최종 정리

```text
Stack
└─ LIFO: 마지막에 넣은 데이터를 가장 먼저 꺼냄

배열 기반 구현
├─ topIndex = -1 : 빈 스택
├─ push() : ++topIndex 후 저장
├─ pop()  : topIndex 감소
├─ top()  : arr[topIndex] 접근
├─ empty(): topIndex == -1
└─ size() : topIndex + 1

시간 복잡도
└─ push / pop / top / empty / size 모두 O(1)

주의할 점
├─ 빈 스택에서 top() 호출 금지
├─ 용량 초과 처리 필요
└─ pop()은 값을 실제로 0으로 만들 필요 없음

STL std::stack
├─ #include <stack>
├─ 기본 내부 컨테이너: deque
├─ LIFO 기능만 제공
└─ 인덱스 접근, 일반 반복자 순회 불가
```

**기억할 한 문장:** 배열 기반 스택은 `topIndex`로 맨 위 위치를 알고 있기 때문에 기본 연산을 전체 탐색 없이 `O(1)`에 수행한다.
