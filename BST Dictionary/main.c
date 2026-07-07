#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node
{
    char*word;
    struct node*left,*right;
    int height;
}node;
node*newnode(char*s)
{
    node*t=malloc(sizeof(node));
    t->word=malloc(strlen(s)+1);
    strcpy(t->word,s);
    t->height=1;
    t->left=t->right=NULL;
    return t;
}
int max(int x,int y)
{
    if(x>y)return x;
    else return y;
}
int getheight(node*root)
{
    if(!root)return 0;
    return root->height;
}
node*rightrotate(node*y)
{
    node*x=y->left;
    node*t2=x->right;
    x->right=y;
    y->left=t2;
    y->height=1+max(getheight(y->left),getheight(y->right));
    x->height=1+max(getheight(x->right),getheight(x->left));
    return x;
}
node*leftrotate(node*x)
{
    node*y=x->right;
    node*t2=y->left;
    y->left=x;
    x->right=t2;
    x->height=1+max(getheight(x->left),getheight(x->right));
    y->height=1+max(getheight(y->right),getheight(y->left));
    return y;
}
int getbalancefactor(node*root)
{
    if(!root)return 0;
    return getheight(root->left)-getheight(root->right);
}
node*insert(node*root,char*s)
{
    if(!root)
        return newnode(s);
    int cmp=strcasecmp(s,root->word);
    if(cmp>0) // goes to right subtree
    {
        root->right=insert(root->right,s);
    }
    else if(cmp<0) // goes to left subtree
    {
        root->left=insert(root->left,s);
    }
    else //duplicate
    {
        printf("Duplicates not allowed\n");
        return root;
    }
    root->height=1+max(getheight(root->left),getheight(root->right));
    int balance=getbalancefactor(root);
    if(balance<-1&&strcasecmp(s,root->right->word)>0) // RR needs left rotation
    {
        return leftrotate(root);
    }
    else if(balance>1&&strcasecmp(s,root->left->word)<0) //LL needs right rotation
    {
        return rightrotate(root);
    }
    else if(balance>1&&strcasecmp(s,root->left->word)>0) //LR needs left rotation for left child then right for root
    {
        root->left=leftrotate(root->left);
        return rightrotate(root);
    }
    else if(balance<-1&&strcasecmp(s,root->right->word)<0) //RL needs right rotation at right child then left for root
    {
        root->right=rightrotate(root->right);
        return leftrotate(root);
    }
    return root;
}
node*loadDictionary(int*size)
{
    FILE*f=fopen("Dictionary.txt","r");
    if(!f)
    {
        printf("Error opening file\n");
        exit(1);
    }
    node*root=NULL;
    char s[100];
    while(fgets(s,100,f))
    {
        (*size)++;
        s[strcspn(s,"\n")]='\0';
        root=insert(root,s);
    }
    fclose(f);
    return root;
}
node*findmin(node*root)
{
    if(!root)return NULL;
    if(!root->left)return root;
    return findmin(root->left);
}
node*findmax(node*root)
{
    if(!root)return NULL;
    if(!root->right)return root;
    return findmax(root->right);
}
node*predecessor(node*root,node*x)
{
    if(x->left)return findmax(x->left);
    node*pred=NULL;
    node*curr=root;
    while(curr!=x)
    {
        if(strcasecmp(x->word,curr->word)>0)
        {
            pred=curr;
            curr=curr->right;
        }
        else
            curr=curr->left;
    }
    return pred;

}
node*successor(node*root,node*x)
{
    if(x->right)return findmin(x->right);
    node*succ=NULL;
    node*curr=root;
    while(curr!=x)
    {
        if(strcasecmp(x->word,curr->word)<0)
        {
            succ=curr;
            curr=curr->left;
        }
        else curr=curr->right;
    }
    return succ;
}
void search(node*root,char*s)
{
    char*token=strtok(s," !?,.-");
    while(token)
    {
        node*curr=root;
        node*prev=NULL;
        int found=0;
        while(curr)
        {
            prev=curr;
             if(strcasecmp(token,curr->word)>0) // lies in right subtree
        {
            curr=curr->right;
        }
        else if(strcasecmp(token,curr->word)<0) //lies in left subtree
        {
            curr=curr->left;
         }
         else //found word
         {
             found=1;
             printf("%s -CORRECT\n",token);
             break;
         }

        }
        if(!found)
         {
             node*pred=predecessor(root,prev);
             node*succ=successor(root,prev);
             printf("%s -NOT CORRECT\n",token);
             printf("Suggestion A: %s\n",prev? prev->word:"(none)");
             printf("Suggestion B: %s\n",pred? pred->word :"(none)");
             printf("Suggestion C: %s\n",succ? succ->word:"(none)");

         }
        token=strtok(NULL," !?,.-");

    }
}
int main()
{
    int size=0;
    node*root=loadDictionary(&size);
    printf("DICTIONARY LOADED SUCCESSFLY................\n");
    printf("Height of tree is %d\n",root->height);
    printf("Size of tree is %d\n",size);
    printf("Enter sentence:\n");
    char s[1000];
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0';
    search(root,s);
    return 0;
}
