#include<stdio.h>
#include<string.h>

int main() {
    printf("\n############# START of Program #############\n\n");   
    
    char input[100],output[100] = "",delete[50],w[50];
    printf("Enter the sentence: ");
    scanf("%[^\n]",input);
    printf("\nEnter the word to be deleted: ");
    scanf("%s",delete);
    int i = 0;
    while(input[i] != '\0') {
        int j = 0;
        while(1) {
            if (input[i] != ' ' && input[i] != '\0')
                w[j++] = input[i++];
            else
                break;
        }
        w[j] = '\0';
        if (strcmp(w,delete) != 0) {
            strcat(output,w);
            strcat(output," ");
        }
        i++;
    }
    printf("\nOutput after deletion is %s\n", output);
    printf("\n############## END of Program ##############\n\n");
    return 0;
}