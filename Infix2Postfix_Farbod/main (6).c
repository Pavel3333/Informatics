#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct node{
    float value;
    struct node *next;
};

struct myStack{
    struct node *head;
    int size;
};

struct myStack *initStack(){
    struct myStack *st = (struct myStack*)malloc(sizeof(struct myStack));
    if(st == NULL){
        printf("stack initialization failed\n");
        return NULL;
    }
    st->head = (struct node*)malloc(sizeof(struct node));
    st->head->next = NULL;
    st->size = 0;
    return st;
}

void push(float val, struct myStack *s){
    if(s->size == 0){
        s->head->value = val;
    }
    else{
        struct node *headNode = (struct head*)malloc(sizeof(struct node));

        if(headNode == NULL){
            printf("node initialization failed\n");
            return NULL;
        }

        headNode->value = val;
        headNode->next = s->head;
        s->head = headNode;
    }
    s->size++;
}

float pop(struct myStack *s){
    if(s->size > 0){
        if(s->size == 1){
            s->size--;
            return s->head->value;
        }
        struct node tmp = *(s->head);
        free(s->head);
        s->head = tmp.next;
        s->size--;
        return tmp.value;
    }
    else{
        printf("stack has not any elements\n");
        return 0;
    }
}

void freeStack(struct myStack *s){
    if(s != NULL){
        free(s->head);
        free(s);
    }
}

void display(struct myStack s){
    if(s.size > 0){
        int i=0;
        for(i; i < s.size; i++){
            printf("  %d\n", s.head->value);
            s.head = s.head->next;
        }
    }
    else printf("stack has not any elements\n");
}

int sizeofStack(struct myStack s){
    return s.size;
}

void find(float num, struct myStack *s){
    struct node *tmp = (s->head);
    struct node *old = tmp;
    while(tmp->next != NULL && tmp->value != num){
        old = tmp;
        tmp = (tmp->next);
    }
    if(tmp->value == num){
        old->next = tmp->next;
        s->size--;
        free(tmp);
    }
    else printf("This value not found\n");
}

struct myStack *reverseStack(struct myStack *s){
    struct myStack *revs;
    revs = initStack();
    if(s->size > 0){
        int i=0;
        for(i; i < s->size; i++){
            push(pop(s), revs);
        }
        push(pop(s), revs);
        freeStack(s);
        return revs;
    }
    else{
        printf("stack has not any elements\n");
        return s;
    }
}

void colculateAll(struct myStack *st, char *operandStr){
    float first, second;
    if(sizeofStack(*st) > 1){
        int i = strlen(operandStr)-1;
        for(i; i >= 0; i--){
            switch(operandStr[i]){
            case '+':
                second = pop(st);
                first = pop(st);
                push(first + second, st);
                break;
            case '-':
                second = pop(st);
                first = pop(st);
                push(first - second, st);
                break;
            case '*':
                second = pop(st);
                first = pop(st);
                push(first * second, st);
                break;
            case '/':
                second = pop(st);
                first = pop(st);
                push(first / second, st);
                break;
            }
        }
    }
    memset(operandStr, '\0', strlen(operandStr));
}

void colculateLast(struct myStack *st, char *operandStr){
    float first, second;
    if(sizeofStack(*st) > 1){
        switch(operandStr[strlen(operandStr)-1]){
        case '+':
            second = pop(st);
            first = pop(st);
            push(first + second, st);
            break;
        case '-':
            second = pop(st);
            first = pop(st);
            push(first - second, st);
            break;
        case '*':
            second = pop(st);
            first = pop(st);
            push(first * second, st);
            break;
        case '/':
            second = pop(st);
            first = pop(st);
            push(first / second, st);
            break;
        }
        operandStr[strlen(operandStr)-1] = '\0';
    }
}

float getNum(int *cnt, char *str){
    char numStr[8]="";
    float number = 0;
    if(str[*cnt] <= '0' || str[*cnt] >= '9') (*cnt)++;
    while((str[*cnt] >= '0' && str[*cnt] <= '9') || str[*cnt] == '.'){
        numStr[strlen(numStr)] = str[*cnt];
        (*cnt)++;
    }
    (*cnt)--;
    number = atof(numStr);
    return number;
}

int hasMultDiv(int cnt, char *str){
    if(cnt == strlen(str)-1) return 0;
    else{
        while(cnt < strlen(str)-1 && str[cnt] != ')'){
            if(str[cnt] == '*' || str[cnt] == '/') return 1;
            cnt++;
        }
        return 0;
    }
}


int main()
{
    struct myStack *st;
    st = initStack();
    float number;
    int cnt=0;
    printf("Enter infix expression\n\n");
    char infix[64]="", operandStr[8]="", brackets=0;
    fgets(infix, 64, stdin);
    while(cnt < strlen(infix)-1){
        if(infix[cnt] == '(' || infix[cnt] == ')'){
            if(infix[cnt] == '(') brackets++;
            else {
                brackets--;
                colculateAll(st, operandStr);
            }
            cnt++;
        }
        if(infix[cnt] >= '0' && infix[cnt] <= '9'){
            number = getNum(&cnt, infix);
            push(number, st);
        }
        if(infix[cnt]=='+' || infix[cnt]=='-' || infix[cnt]=='*' || infix[cnt]=='/'){
            operandStr[strlen(operandStr)] = infix[cnt];
            if((infix[cnt+1]== '('))cnt++;
            else{
                number = getNum(&cnt, infix);
                push(number, st);
                if((operandStr[strlen(operandStr)-1] == '*' || operandStr[strlen(operandStr)-1] == '/')){
                    colculateLast(st, operandStr);
                }
                else if(!hasMultDiv(cnt, infix)){
                    colculateLast(st, operandStr);
                }
            }
        }
        cnt++;
    }
    colculateAll(st, operandStr);
    infix[strlen(infix)-1] = '\0';
    printf("\n%s = %g",infix, pop(st));
    freeStack(st);
    return 0;
}
