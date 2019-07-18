#include <stdio.h>

__attribute__((used))
int main() {
  const char* str = "Hello world\n";
  for (int i = 0; str[i]; ++i) {
    putchar(str[i]);
  }
  // puts(str);
  return 0;
}
