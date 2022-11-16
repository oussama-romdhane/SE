#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *removeWhitespace(const char *str) {
  int i, j;
  char *newString;

  newString = (char *)malloc(50);

  i = 0;
  j = 0;

  while (str[i] != '\0') {
    if (str[i] == ' ') {
      newString[j] = ' ';
      j++;

      while (str[i] == ' ')
        i++;
    }

    newString[j] = str[i];

    i++;
    j++;
  }

  newString[j] = '\0';

  return newString;
}