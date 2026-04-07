# Stack Kata

## Requirements

Implement a Stack with the following public functions:

- `stack_push(Stack* stack, int value)` - Adds a value to the top of the stack
- `int stack_pop(Stack* stack)` - Removes and returns the top value from the stack
- The stack should handle the case when attempting to pop from an empty stack (return error code or special value)

## Function Signatures

```c
typedef struct Stack Stack;

Stack* stack_create(void);
void stack_destroy(Stack* stack);
void stack_push(Stack* stack, int value);
int stack_pop(Stack* stack);  // Returns -1 on error, or use error parameter
bool stack_is_empty(Stack* stack);
```

## Test Cases

| Input (push) | Expected Output (pop) |
| ------------ | --------------------- |
| Empty Stack  | error/-1              |
| push(1)      | 1                     |
| push(2)      | 2                     |
| push(1,2)    | 2, then 1             |
| push(1,2,3)  | 3, then 2, then 1     |

### Example Usage

```c
#include "kata.h"
#include <stdio.h>

int main(void) {
    Stack* stack = stack_create();

    stack_push(stack, 1);
    stack_push(stack, 2);
    stack_push(stack, 3);

    printf("%d\n", stack_pop(stack)); // 3
    printf("%d\n", stack_pop(stack)); // 2
    printf("%d\n", stack_pop(stack)); // 1

    if (stack_is_empty(stack)) {
        printf("Stack is empty\n");
    }

    stack_destroy(stack);
    return 0;
}
```

## TDD Rules

1. ✅ Write production code only to pass a failing unit test
2. ✅ Write only enough of a unit test to make it fail
3. ✅ Write only enough production code to make the failing test pass

## Additional Guidelines

- Follow the "Red-Green-Refactor" cycle
- Keep tests and production code as simple as possible
- Commit after each successful test

## Resources

- [TestDesiderata by Kent Beck](https://kentbeck.github.io/TestDesiderata)
