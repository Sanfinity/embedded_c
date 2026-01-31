#include<stdio.h>

int main() {
    printf("\n############# START of Program #############\n\n");

    /*
        input : n
        output : 
           1   
          2 2  
         3 3 3 
    */
    int n;
    printf("Enter the value for n: ");
    scanf("%d",&n);

    for (int i = 1; i <= n; i++) {
        for (int j = n-i; j > 0; j--) {
            printf(" ");
        }
        for (int j = 1; j <= i; j++) {
            printf("%d ",i);
        }
        printf("\n");
    }

    printf("\n############## END of Program ##############\n\n");
    return 0;
}