#include "util.h"

void die(char *message)
{
    printf("Error: %s", message);
    exit(1);
}

int randomInRange(int start, int end)
{
    return start + rand() % (end + 1 - start);
}