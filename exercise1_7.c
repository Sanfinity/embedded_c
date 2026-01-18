#include <stdio.h>
#include <string.h>

int main() {

    char str[] =
        "This   is    a     sample      string   with   many    blanks.";

    int n = strlen(str);
    int i = 1, j = 1;
    while (i<n) {
        if (str[i] == ' ' && str[i] == str[i-1]){
            i++;
            continue;
        }
        str[j++] = str[i++];
    }
    str[j] = '\0';

    printf("Modified: %s\n", str);

    return 0;
}
