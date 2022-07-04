#include <stdio.h>
#include<stdlib.h>

typedef struct Node{                                 //structure of nodes of tree
    int kind;                                        //represent leaf,two or three node
    int x,y;                                         //to store values
    struct Node*lchild,*mchild,*rchild;              //children of a node
}node;
 

node* create3node(){                                 //to create a three node with all children to null 
    node* n=(node*)malloc(sizeof(node));             //and values initialize to 0
    n->lchild=NULL;
    n->rchild=NULL;
    n->mchild=NULL;
    n->x=0;
    n->y=0;
    n->kind=3;
    return n;
}

node* create2node(){                                 //to create a two node
    node* n=(node*)malloc(sizeof(node));
    n->lchild=NULL;
    n->rchild=NULL;
    n->mchild=NULL;
    n->x=0;
    n->y=0;
    n->kind=2;
    return n;
}

node* createleafnode(){                              //to create a leaf node
    node* n=(node*)malloc(sizeof(node));
    n->lchild=NULL;
    n->rchild=NULL;
    n->mchild=NULL;
    n->x=0;
    n->y=0;
    n->kind=1;
    return n;
} 
int height(node*root) {                                 //function which returns height of a subtree rooted at root
    if(root==NULL) return 0;
    return 1+height(root->lchild);
}
int min(node*root) {                              //function which returns minimum of a subtree rooted at root
    node*n=root;
    while(n->kind!=1) n=n->lchild;
    return n->x;
}

node*MakeSingleton(int x) {                       //function which creates single node tree,i.e,
    node*n=createleafnode();                      //leaf node in our implementation
    n->x=x;
    return n;
}

typedef struct Tree {                            //structure which stores 2 nodes and min. value of 2nd node if present
    node*n1,*n2;                                 //this struct. helps in returning final nodes and value after insertion
    int m;                                       //n1,n2 act as left and right child of their parent and m is min. of n2 subtree
}tree;

tree*maketree() {                                //ends up creating a tree structure
    tree*t=(tree*)malloc(sizeof(tree));
    t->n1=NULL;
    t->n2=NULL;
    return t;
}
tree*insert1(node*,node*,node*);                 //funct. which return tree struct after insertion 
                                                 //in case when h(s1) > h(s2)

tree*insert2(node*,node*,node*);                 //funct. which return tree struct after insertion 
                                                 //in case when h(s2) > h(s1)



node* Merge(node*s1,node*s2) {                   //Main function which returns final node of merged tree
    int h1,h2,h;
    if(s1->x>s2->x) {                            //to check whether elements of set s1 are bigger from s2
        return Merge(s2,s1);                     //and if not then merges by taking s2 to be small in our implementation
    }                                            
    node*s=create2node();                        //creates additional node to store final tree if needed
    h1=height(s1);                                    //heights of trees s1 and s2 respectively
    h2=height(s2);
    if(h1==h2) {                                //to be implement if heights are equal
        s->lchild=s1;                           //s represent parent node whose left child is s1 and middle child is s2
        s->mchild=s2;
        s->x=min(s2);                           //stores min. of s2 in s as required in 2-3 tree implementation
        return s;
    }
    h=h1-h2;
    tree*t=maketree();                          //to store tree struct which will return after calling insert
    node*ptr=s1;                                //additional pointer which will point to rightmost node of s1
    if(h1>h2) {                                               //at a height of h2+1, in case h(s1)>h(s2)
        while(--(h)) {
            if(ptr->rchild!=NULL) ptr=ptr->rchild;          
            else ptr=ptr->mchild;
        }
        t=insert1(s1,s2,ptr);
    }
    h=-h;
    ptr=s2;                                     //in case h(s2)>h(s1) ptr points to left most node at height h1+1
    if(h2>h1) {
        while(--h){
            ptr=ptr->lchild;
        }
        t=insert2(s1,s2,ptr);
    }
    if(t->n2==NULL) return t->n1;               //if n2, i.e 2nd node return from insert is NULL then tree is simply 
                                                //rooted at n1 so return it only
        else {                                  //if n2 is not NULL then return their parent s
            s->lchild=t->n1;                    //whose left child is n1 and right child is n2
            s->mchild=t->n2;
            s->x=t->m;
        }
        return s;
}

tree*insert1(node*s1,node*s2,node*ptr) {        //recursively insert1
    tree*t=maketree();
                                               //here s1 is pointer which will traverse
    if(s1==ptr) {                              //Base case 
        if(s1->kind==2) {                      //if s1 is 2node then insert s2 by making s1 to be 3node
            s1->kind=3;                        //and s2 be its rightmost child
            s1->rchild=s2;             
            s1->y=min(s2);                     //stores min. of s2 in s1
            t->n1=s1;
            t->n2=NULL;
            return t;                          //return tree struct as explained earlier
        }
        else {                                 //if s1 is 3node so split s1 to two 2nodes 
            node*new=create2node();            //one is s1 and other is new,here elements value of s1<new
            new->x=min(s2);                    //stores min of s2 as required
            new->lchild=s1->rchild;
            new->mchild=s2;
            s1->kind=2;
            s1->rchild=NULL;
            t->n1=s1;                          //now n1 points to s1
            t->n2=new;                         //n2 points to new
            t->m=s1->y;                        //m stores min. of n2
            return t;
        }
    }
    if(s1->kind==2) {                          //if not base case and it is 2node
        t=insert1(s1->mchild,s2,ptr);          //recursive call to insert1
        if(t->n2==NULL) {                      //if after insertion n2 is still null
            s1->mchild=t->n1;                  //then add only n1 to its parent
            tree*t1=maketree();
            t1->n1=s1;
            t1->n2=NULL;
            return t1;
        }
        else {                                //if i=n2 is not null and s1(parent) is 2node
            s1->kind=3;                       //so convert s1 to 3node and add n1 and n2
            s1->mchild=t->n1;
            s1->rchild=t->n2;
            s1->y=t->m;                       //and now also add min. of n2(rightmost child)
            tree*t1=maketree();
            t1->n1=s1;
            t1->n2=NULL;
            return t1;
        }
    }
    if(s1->kind==3) {                        //if it is not base case and 3node
        t=insert1(s1->rchild,s2,ptr);        //recursive call
        if(t->n2!=NULL) {                    //if n2 is not null and it is 3node
            node*n=create2node();            //so split it into two 2nodes one is s1 and other n
            n->lchild=t->n1;                 //s1 with original child except rchild and n with n1 and n2
            n->mchild=t->n2;
            n->x=t->m;                       //min. of mchild of n is as return by t i.e m
            s1->rchild=NULL;                 //as done splitting so rchild of s1 now present with n
            s1->kind=2;
            tree*t1=maketree();
            t1->n1=s1;
            t1->n2=n;
            t1->m=s1->y;                     //min. of whole n is same as min of rchild of s1 as no modification is done 
            return t1;                       //with rchild
        }
        else {                               //if n2 is null then simply add n1 to s1 only
            s1->rchild=t->n1;                //min. of n1 doesn't change as all elements that are inserted are bigger than rchild
            tree*t1=maketree();
            t1->n1=s1;
            t1->n2=NULL;
            return t1;
        }
    }
}                                            //no return statement as all cases are already taken into account


tree*insert2(node*s1,node*s2,node*ptr) {     //recursively insert2
    tree*t=maketree();                       //here s2 is pointer which will traverse

    if(s2==ptr) {                            //Base case
        if(s2->kind==2) {                    //if s2 is 2node then insert s1 by making s2 to be 3node
            s2->kind=3;
            s2->rchild=s2->mchild;          //as s1 should be added as left child so simply shifting mchild to rchild 
            s2->mchild=s2->lchild;          //lchild to mchild
            s2->lchild=s1;                  //adding s1 as lchild
            s2->y=s2->x;                    //as a consequence of shifting min. of rchild should store at y
            s2->x=min(s2->mchild);          //storing min. of mchild as s1->x
            t->n1=s2;
            t->n2=NULL;
            return t;
        }
        else {                              //if s2 is 3node so splitting s2 to two 2nodes
            node*new=create2node();         //one is s2 and other new
            new->x=min(s2->lchild);         //with new elements value< s2
            new->lchild=s1;                 //hence making lchild of s2 as mchild of new
            new->mchild=s2->lchild;
            s2->kind=2;
            s2->lchild=s2->mchild;          //swapping as a consequence of above
            s2->mchild=s2->rchild;          
            int a=s2->x;                    //a stores min. of current lchild( initial mchild)
            s2->rchild=NULL;                //as it is min. of modified s2
            s2->x=s2->y;
            t->n1=new;                 
            t->n2=s2;
            t->m=a;                         //which should be returned
            return t;
        }
    }
    if(s2->kind==2) {                      //if not base case and 2node
        t=insert2(s1,s2->lchild,ptr);      //recursive call
        if(t->n2==NULL) {                  //if n2 is null only add n1 as lchild
            s2->lchild=t->n1;              
            tree*t1=maketree();
            t1->n1=s2;
            t1->n2=NULL;
            return t1;
        }
        else {                             //if n2 not null
            s2->kind=3;                    //convert s2 be 3node
            s2->rchild=s2->mchild;         //swapping so as to add n1 and n2
            s2->mchild=t->n2;              //as l and m child
            s2->lchild=t->n1;
            s2->y=s2->x;                   //as a consequence of swapping
            s2->x=t->m;
            tree*t1=maketree();
            t1->n1=s2;
            t1->n2=NULL;
            return t1;
        }
    }  
    if(s2->kind==3) {                      //if not base case and 3node
        t=insert2(s1,s2->lchild,ptr);      //recursive call
        if(t->n2!=NULL) {                  //if n2 not null split s2 to two 2node s2 and n
            node*n=create2node();
            n->lchild=t->n1;               //with elements val of n < s2
            n->mchild=t->n2;
            n->x=t->m;
            s2->lchild=s2->mchild;         //swapping so as to convert to 2 node
            s2->mchild=s2->rchild;
            s2->rchild=NULL;
            s2->kind=2;                   
            int a=s2->x;                   //storing so as to return min of modified s2 as m in t struct
            s2->x=s2->y;
            tree*t1=maketree();
            t1->n1=n;
            t1->n2=s2;
            t1->m=a;                      //min value
            return t1;
        }
        else {                           //if n2 is null so only adding n1 as lchild
            s2->lchild=t->n1;
            tree*t1=maketree();
            t1->n1=s2;
            t1->n2=NULL;
            return t1;
        }
    }
}

typedef struct nod {                    //linked list representaion of queue to store nodes of trees
   node *data;                          //actually address of nodes (so as to access them)
   struct nod *next;                    //for level wise traverasl required in Extract implementation
} node_t;

void enqueue(node_t **head, node *val) {                        //functionality of queue ADT
   node_t *new_node = (node_t *)malloc(sizeof(node_t));
   if (!new_node) return;                                       //if new_node is NULL

   new_node->data = val;
   new_node->next = *head;

   *head = new_node;                                           //actually adds new_node at start of list
}

node *dequeue(node_t **head) {                                 //functionality of queue ADT
   node_t *current, *prev = NULL;
   node *retval;                                               //value which is going to be returned
   retval=NULL;                                                //initializing

   if (*head == NULL) return NULL;              

   current = *head;
   while (current->next != NULL) {                            //to find end node of list
      prev = current;
      current = current->next;
   }

   retval = current->data;                                    //storing end node data
   free(current);                                             //required in level wise traversal(addresses of nodes of tree)

   if (prev!=NULL)
      prev->next = NULL;                                      //updating list,i.e deleting last node of list
   else
      *head = NULL;                                           //if prev is NULL, then no element is present in list

   return retval;
}


void Extract(node *T){                                        //function which is call to print all elements present in merged set
    node_t **head= (node_t **)malloc(sizeof(node_t *));
    *head=NULL;
    enqueue(head,T);
    while(*head!=NULL){                                      //level wise traversal
        node *x= dequeue(head);
        if(x->kind==1)                                       //and printing only when it is leaf node
            printf("%d\n",x->x);
        if(x->lchild!=NULL)                                  //storing various chlidren of node in queue
            enqueue(head,x->lchild);
        if(x->mchild!=NULL)
            enqueue(head,x->mchild);
        if(x->rchild!=NULL)
            enqueue(head,x->rchild);
    }
}

void Test(){                                                //function which simultaneously builds tree and also call 
                                                            //Extract to print all elements of merged set
    node *T = MakeSingleton(1);                             //first tree with single node
    int i;
    for(i=2; i<=500;i++)                                      //building up complete first tree
        T= Merge(T,MakeSingleton(i));
    node *U = MakeSingleton(777);                           //2nd tree with single node
    for(i=778;i<=1000;i++)                                  //building up complete 2nd tree 
        U= Merge(U,MakeSingleton(i));  //  T>U;
    node *V=Merge(T,U);                                     //final merged tree
    Extract(V);                                             //called to print all elements
}

int main(){
    Test();
    // node*T,*T1;
    // T=MakeSingleton(1);                                    //we used this sample for testing
    // for(int i=1;i<=10;i++) {
    //     T=Merge(T,MakeSingleton(i+1));
    // }
    // T1=MakeSingleton(100);
    // for(int i=100;i>=90;i--) {
    //     T1=Merge(MakeSingleton(i-1),T1);
    // }
    // node_t **head= (node_t **)malloc(sizeof(node_t *));
    // *head=NULL;
    // enqueue(head,T);                                   //We also tested the queue operations by creating a list in main 
    // node*s=dequeue(head);                              //and calling the enqueue and dequeue operations.
    // printf("%d",s->x);
    // Extract(Merge(T,T1));
}