#pragma once
 
#include <limits.h>
#include <string.h>
#include <stdio.h>

#define NO_OF_CHARS 256

int max(int a, int b);
void badCharHeuristic(char * str, int size, int badchar[NO_OF_CHARS]);
bool search(char * txt, char * pat);