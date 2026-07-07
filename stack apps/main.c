#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
typedef struct node
{
    float data;
    struct node*link;
}node;
typedef struct
{
   node*top;
}stack;
stack*init()
{
    stack*s=malloc(sizeof(stack));
    if(!s)return NULL;
   s->top=NULL;
    return s;
}
int isempty(stack*s)
{
    return s->top==NULL;
}
void push(stack*s,float x)
{
    node*temp=malloc(sizeof(node));
    temp->link=s->top;
    temp->data=x;
    s->top=temp;
}
float pop(stack*s)
{
    if(isempty(s))
    {
        printf("stack is empty");
        return -1;
    }
    node*temp=s->top;
    float x=temp->data;
    s->top=s->top->link;
    free(temp);
    return x;
}
float peek(stack*s)
{
    return s->top->data;
}
int priority(char op)
{
    if(op=='^')return 4;
    if(op=='%')return 3;
    if(op=='*')return 3;
    if(op=='/')return 3;
    if(op=='+')return 2;
    if(op=='-')return 2;
    if(op=='(')return 0;
}
float operation(char op,float num1,float num2)
{
    if(op=='^')return pow(num2,num1);
    if(op=='*')return num1*num2;
    if(op=='/')return num2*1.0/num1;
    if(op=='%')return fmod(num2,num1);
    if(op=='+')return num1+num2;
    if(op=='-')return num2-num1;
}
int isminus(char*exp)
{
    return exp[0]=='-'&&exp[1]=='\0';
}
int isoperator(char c)
{return c=='*'||c=='/'||c=='+'||c=='%';}
char* infixtopostfix(char*str)
{
    char*post=malloc(strlen(str)*2+1);
    stack*s=init();
    char*token=strtok(str," ");
    int j=0;
    while(token)
    {
        if(token[0]=='^')
            push(s,token[0]);
        else if(isoperator(token[0]))
        {
            while(!isempty(s)&&(priority((char)peek(s))>=priority(token[0]))&&(char)peek(s)!='(')
            {
                post[j++]=(char)pop(s);
                post[j++]=' ';
            }
            push(s,token[0]);
        }
        else if(isminus(token))
        {
            while(!isempty(s)&&(priority((char)peek(s))>=priority(token[0]))&&(char)peek(s)!='(')
            {
                post[j++]=(char)pop(s);
                post[j++]=' ';
            }
            push(s,token[0]);
        }
        else if(token[0]=='(')
                    push(s,token[0]);
        else if(token[0]==')')
        {
            while(!isempty(s)&&(char)peek(s)!='(')
            {
                post[j++]=(char)pop(s);
                post[j++]=' ';
            }
            pop(s);
        }
        else
        {
            int i;
            for(i=0;token[i];i++)
                post[j++]=token[i];
            post[j++]=' ';
        }
        token=strtok(NULL," ");
    }
    while(!isempty(s))
    {post[j++]=(char)pop(s);
    post[j++]=' ';}
    post[j]='\0';
    printf("%s\n",post);
    return post;

}
float evaluatepostfix(char*str)
{
    stack*s=init();
    char*token=strtok(str," ");
    while(token)
    {
        if(isoperator(token[0])||token[0]=='^')
        {
            float num1=pop(s);
            float num2=pop(s);
            float res=operation(token[0],num1,num2);
            push(s,res);
        }
        else if(isminus(token))
        {
            float num1=pop(s);
            float num2=pop(s);
            float res=operation(token[0],num1,num2);
            push(s,res);
        }
        else
        {
            push(s,atof(token));
        }
        token=strtok(NULL," ");

    }
    return pop(s);
}
int main()
{
    char s[100];
    printf("Enter space delimitted infix expression\n");
    gets(s);
    char*post=infixtopostfix(s);
    printf("%f",evaluatepostfix(post));
    return 0;
}
