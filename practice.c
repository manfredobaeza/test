#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100
typedef int numtype;
typedef char chtype;

//创建一个栈来存储重复次数

typedef struct numstack{
    numtype *data;
    int ntop;
}NumStack;

//创建一个栈来存储字符串

typedef struct charstack{
    chtype *data[MAX_SIZE];//存储字符串
    int ctop;
}CharStack;

NumStack *init_Stack1(){
    NumStack *np = (NumStack *)malloc(sizeof(NumStack));
    if(np == NULL){//检查内存分配是否成功
        return NULL;
    }
    np -> data = (numtype *)malloc(MAX_SIZE * sizeof(numtype));
    if(np -> data == NULL){
        free(np);
        return NULL;
    }
    np -> ntop = -1;
    return np;
}

CharStack *init_Stack2(){
    CharStack *cp = (CharStack *)malloc(sizeof(CharStack));
    if(cp == NULL){
        return NULL;
    }
    cp -> ctop = -1;
    return cp;
}


int pushNum(NumStack *ns, numtype num){
    if(ns -> ntop == MAX_SIZE - 1){ //栈满
        printf("nStack is full!\n");
        return 0;
    }
    ns -> ntop++;
    ns -> data[ns -> ntop] = num;
    return 1;
}

int pushStr(CharStack *cs, const char *str){
    if(cs -> ctop == MAX_SIZE - 1){
        printf("cStack is full!\n");
        return 0;
    }
    cs-> ctop++;
    // 复制字符串入栈,strdup()根据传入字符串s的长度动态分配一块内存（使用 malloc）
    //将字符串内容完整复制到新内存中，返回这个新字符串的首地址。
    cs-> data[cs -> ctop] = strdup(str); 
    return 1;
}

int popNum(NumStack *ns, numtype *num){//num用于存储弹出的倍数
    if(ns -> ntop == -1){
        printf("nStack is empty!\n");
        return 0;
    }
    *num = ns -> data[ns -> ntop];
    ns -> ntop--;
    return 1;
}

int popStr(CharStack *cs, chtype **ch){//ch用于存储弹出的字符串
    if(cs -> ctop == -1){
        printf("cStack is empty!\n");
        return 0;
    }
    *ch = cs -> data[cs -> ctop];
    cs -> ctop--;
    return 1;
}

int stringprocess(NumStack *ns, CharStack *cs, char s[]){//处理字符串
    int num1 = 0;
    char ch1[MAX_SIZE] = ""; //存储当前的字符串

    for(int i = 0; s[i] != '\0'; i++){
        if(isdigit(s[i])){//如果s[i]是数字，先构造数字，然后将数字入栈
            num1 = num1 * 10 + (s[i] - '0');
        }
        else if(s[i] == '['){
            pushNum(ns, num1);//倍数入栈
            pushStr(cs, ch1);//当前字符串入栈
            num1 = 0;//重置
            ch1[0] = '\0';
        }
        else if(isalpha(s[i])){ //如果s[i]是字母，将字母入栈
            int len = strlen(ch1);
            ch1[len] = s[i];
            ch1[len + 1] = '\0';
        }
        else if(s[i] == ']'){
            numtype repeat;//repeat用于存储弹出的倍数
            chtype *prev;
            popNum(ns, &repeat);//弹出倍数
            popStr(cs, &prev);

            char temp[MAX_SIZE] = "";//用于存储重复后的字符串
            for(int j = 0; j < repeat; j++){
                strncat(temp, ch1, sizeof(temp) - strlen(temp) - 1);
            }

            char newStr[MAX_SIZE] = "";
            snprintf(newStr, sizeof(newStr), "%s%s", prev, temp);

            strncpy(ch1, newStr, sizeof(ch1) - 1);
            ch1[sizeof(ch1) - 1] = '\0';
            free(prev); // strdup() 分配的内存要释放
        }
    }
    printf("%s\n", ch1);
    return 1;
}

int main(){
    char s[] = "3[ae2[c]]"; //应该输出accaccacc
    NumStack *ns = init_Stack1();
    CharStack *cs = init_Stack2();
    if(!ns || !cs){
        fprintf(stderr, "init stack failed\n");
        return 1;
    }
    stringprocess(ns, cs, s);

    // 释放分配的栈数组
    free(ns->data);
    free(ns);
    free(cs);
    return 0;

    return 0;
}