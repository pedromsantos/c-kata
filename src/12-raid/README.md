# Guild Raid kata

## First run

- Write tests to characterize the code as is

You will soon realize that it's very hard to write tests for this code due to the usage of static functions and global state. Once you gave it a good try at writing characterization tests read the section below and try again.

### Breaking dependencies using a seam

Legacy Code requires extra care than code written with tests. Here, the first goal is to put the tests in place. In this context, Michael Feathers introduced the concept of "seam". "A seam is a place where you can alter behavior in your program without editing in that place."

Types of seams:

- Pre-Processing seams: where we can inject behavior when a pre-processor runs
- Link seams: intercepting calls to external files at link time and replacing them with injectable behavior
- Object seams: when dealing with Object Oriented languages, we can use polymorphism or other techniques to change the behavior at (test) runtime

#### Using function pointers to decouple production code

In the following example, we will see one application of the seam model to some C code. We informally call this "_the mother of all refactoring_" (for legacy code), because it depicts very well the main underlying idea.

_Example:_

```c
#include <stdlib.h>

typedef struct {
    int data;
} Game;

Game* game_create(void) {
    Game* game = malloc(sizeof(Game));
    game->data = 0;
    return game;
}

void game_play(Game* game) {
    int dice_result = rand() % 6 + 1;  // Coupled to rand()
    // ... other game logic ...
}
```

In this case, the Game is coupled with the random number generator. We need to control the dice rolling to test the Game.

**Step 1:** Add a function pointer to the Game struct to encapsulate the behavior that has the Coupling issue.

```c
typedef struct {
    int data;
    int (*roll_fn)(void);
} Game;

static int default_roll(void) {
    return rand() % 6 + 1;
}

Game* game_create(void) {
    Game* game = malloc(sizeof(Game));
    game->data = 0;
    game->roll_fn = default_roll;
    return game;
}

void game_play(Game* game) {
    int dice_result = game->roll_fn();
    // ... other game logic ...
}
```

**Step 2:** In your _test code_, create a test function and inject it into the Game.

```c
static int test_roll_result = 0;

static int test_roll(void) {
    return test_roll_result;
}

void test_game_with_six(void) {
    Game* game = game_create();
    game->roll_fn = test_roll;
    test_roll_result = 6;

    game_play(game);

    // ASSERT
}
```

**Step 3:** Write tests using the injected function.

This technique can also work when using global variables or static functions. The main advantage is that it can be performed systematically. That reduces the manual changes to production code and the risk of introducing bugs.

With good test coverage, you can refactor the Game. You can completely remove the need for a seam, by injecting the dependency in the Game constructor, for example.

Here, in terms of design, the seam makes things worse. But the goal is to have tests. And once we have them, we can then proceed with the refactoring of the design and make things better!

## Second run

- Refactor the code to remove design issues
