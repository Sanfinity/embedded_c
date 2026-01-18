#include <stdio.h>

int lower(int c);

int main() {

    char input[] =
        "Hello WORLD! This Is A C PROGRAM 123.";
    /*
        Traverse the string character by character
        and convert each character using lower().
    */
    int i = 0;
    while (input[i] != '\0') {
        input[i] = lower(input[i]);
        i++;
    }

    printf("Output: %s\n", input);

    return 0;
}

/*
    lower(c)
    --------
    Returns:
    - lowercase version of c if c is an uppercase letter
    - otherwise returns c unchanged
*/
int lower(int c) {
    /*
        Your logic here.

        Hint:
        -----
        ASCII:
            'A' to 'Z'
            'a' to 'z'
    */
    if (c <= 'Z' && c >= 'A') {
        return c - 'A' + 'a' ;
    }
    return c;
}
