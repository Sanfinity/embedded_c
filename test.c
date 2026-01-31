#include <stdio.h>
#include <string.h>

int main()
{
    int a = 010;
    printf("\n a = %x",a);
    
    unsigned char k = 10; //10 * 92;
    printf
    ("\n k = %d",k);

    /* Compiler Error no % on floating point numbers*/
    // float c = 3.14;
    // int d = c%2;
    int b , c;
    a = b = c = k = 3;
    printf("\na = %d, b = %d, c = %d, k = %d", a,b,c,k);

    // int ch;
    // ch = getchar();
    // ch++;
    // putchar(ch);

    printf("\n %25.3s","Hello");

    for (int i = -10; !i; i++) {
        printf("%d",-i);
    }

    // Infinite Loop
    // int d;
    // for (;;){
    //     if (d == 5) break;
    //     printf("\t %d", d);
    //     d++;
    // }

    float a1[10];
    int a2[10];
    char str[5] = "Hello";
    printf("\n\nSize of a1: %d, a2: %d, string: %d\nfloat: %d, int: %d\n", sizeof(a1), sizeof(a2), sizeof(str), sizeof(float), sizeof(int));
    printf("\nString: %s\n", str);
    // Using scanset in scanf
    // char buf[100];
    // scanf("%[^~]",buf);
    // printf("\n%s",buf);

    // char newstr[50];
    // scanf("%10c",newstr);
    // printf("\n%s\n",newstr);

    // char s;
    // while ( (c=getchar()) != '\n') {
    //     if ('A' <= c && c <= 'Z') {
    //         putchar(c+'a'-'A');
    //     }
    //     else {
    //         putchar(c);
    //     }
    // }

    char p[] = "string";
    char t; int i,j;
    for (i = 0, j = strlen(p); i<j; i++) {
        t = p[i];
        p[i] = p[j-i];
        p[j-i] = t;
    }
    printf("\n%s",p);

    char p1[ ] = "%d\n";
    p1[1] = 'c';
    printf(p1,65);

    char a3[3][50] = {"santhosh","sanfinity","santhoshInfinity"};

    for (int i = 0; printf("\nHello1"),printf("\nHello2"),i<1;i++);

    printf("\nProgram Terminated!");
    return 0;
}