#include <Setup.h>

Talon elevator {2};

void elevatorSet(float speed) { elevator.Set(speed); }
void elevatorStop() { elevator.Set(0); }
