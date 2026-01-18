#include <stdio.h>
#include <string.h>

int main() {
    char str[1000];
    strcpy(str, "Hi this is sanfinity writing my first exercise problem in the most effectent way possible");
    
    // String Literal - Crashes
    // char *str = "Hello";

    int n = strlen(str);
    for(int l = 0, r = n-1 ; l < r ; l++,r--) {
        char temp = str[l];
        str[l] = str[r]; 
        str[r] = temp;
    }
    printf("Reversed string: %s\n", str);

    return 0;
}
