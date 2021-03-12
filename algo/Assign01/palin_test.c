#include <stdio.h>
#include <string.h>

int isleftpalin(int k, char str[]){//회문인가
    int l = strlen(str);
    int a;
    for(int i = k;i<l/2;i++){
        if(str[i] == str[l-2-i]) a=2;
        else
        {
            a = 3;
            break;
        }
    }
    return a;
}

int isrightpalin(int k, char str[]){//회문인가
    int l = strlen(str);
    int a;
    for(int i = k;i<l/2;i++){
        if(str[i+1] == str[l-1-i]) a=2;
        else
        {
            a = 3;
            break;
        }
    }
    return a;
}

#define MAX_LENGTH 100000 // Define maximum length of string

int main(){
       
    FILE *in=fopen("4.inp","r");
    FILE *out=fopen("4.out","w");
    
    int N;
    
    char input_str[MAX_LENGTH+1];
    
    fscanf(in,"%d",&N);
    
    for(int i=0; i<N; i++) {
        
        fscanf(in,"%s",input_str);
        
        int result = 0;
        int length = strlen(input_str);
        
        for(int j=0;j<length/2;j++){
            if(input_str[j] != input_str[length -1-j]){//1차 회문 아닐때 판별
                int leftPalinType = isleftpalin(j, input_str);
                int rightPalinType = isrightpalin(j, input_str);
                result = leftPalinType < rightPalinType ? leftPalinType : rightPalinType;
                break;
            }

            else{
                result = 1;//회문 아닌거 빼고 나머지는 회문
            }
        }
        fprintf(out,"%d\n",result);
    }

    fclose(in);
    fclose(out);
    
    return 0;
}