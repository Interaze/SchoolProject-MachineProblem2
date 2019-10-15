//Write few lines of comments at the beginning of your program to explain the idea of your algorithm. Indicate in your explanation whether or not this program can be implemented to run in linear time and why.

//Within My algorithm, I construct an initial array through malloc, at the length of the max size of the vertex. Yet this operation (through reallocs), allows me to read through the file one time, which starts me with O(n) time. Along with my creation of nodes that are O(1) time and my insert into the linked list, which is at worst O(2n) complexity or O(n), my initial creation of the graph rests at O(n), due to my avoidence of nesting loops or recursion. My pathing algorithm takes a similar tactic as prior, reading through the array of linked lists, it reads each node, following its value two times. This is acheived by its use of an anchor pointer, and resetting marker pointers. Due to being able to reference a vertex value and examine its row, then examining each row I'm able to path through it within O(n) time, with my free functions working in a similar manor to my prints, my cumulaive complexity should be O(n) time totaling to a linear complexity due to the reading of the adjacency list matrix.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int from;
    int to;
}link;

typedef struct nodes{
    int value;
    struct nodes* next;
} node;

typedef struct totem{
    node* right;
    node* left;
    char color;
} pole;

typedef struct linked{
    int size;
    pole** head;
} list;

list buildAdj(FILE *);
node * addNode(int);
node * insertNode(node* linkedlist, node* nodePlacer);
void printList(list);
void printThru(node *);
void printConns(list);
int findconn(node *, list);
link * createLink(int u, int v);
pole* createTotem();
link** findMax(list);
void freeRows(node *);
void freeList(list);

int main(int argc, char** argv) {
	/* Make your program do whatever you want */
    if(argc != 2){//errorchecks user submission
        printf("\nInsufficient Arguements: Enter a File Name and Type");
        exit(0);
    }

    FILE* fp = fopen(argv[1], "r");

    if(!fp){//errorchecks fopen
        printf("\nUnable to Open file %s, Try checking your file\n", argv[1]);
        exit(0);//while not conventionally advised, due to it's unimpactful nature, exit functions are used to avoid looping scanfs
    }

    list G = buildAdj(fp);
    fclose(fp);
    //closed early to avoid redundancy

    printf("\nThe inputted Graph is:\n");
    printList(G);

    printConns(G);

    freeList(G);
    printf("\n--End of Program--\n");
}

void printConns(list G){
    int size = G.size;
    pole** core = G.head;
    int a = 0;
    int b = 0;
    link ** conns = findMax(G);
    link * first = NULL;
    link * second = NULL;

    for(int i = 0; i < size && conns[i] != NULL; i++){
        if((conns[i])->to != (conns[i])->from){
            if(!first){
                first = conns[i];
            }
            else if(!second){
                second = conns[i];
                printf("\n%d -> %d", first->to, second->from);
                core[(first->to)-1]->color = 'B';
                core[(second->from)-1]->color = 'B';
                printf("\nCORES %d and %d turned to black",(first->to)-1,(second->from)-1);
            }
            else{
                first = second;
                second = conns[i];

                printf("\n%d -> %d", first->to, second->from);
                core[(first->to)-1]->color = 'B';
                core[(second->from)-1]->color = 'B';
                printf("\nCORES %d and %d turned to black",(first->to)-1,(second->from)-1);
            }
        }
    }
    if(second){
        printf("\n%d -> %d", second->to, conns[0]->from);
        core[(second->to)-1]->color = 'B';
        core[(conns[0]->from)-1]->color = 'B';
        printf("\nCORES %d and %d turned to black",(second->to)-1,(conns[0]->from)-1);
    }

    int sources[size];
    int sinks[size];

    for (int i = 0; i < size; i++) {
        if(core[i]->left == NULL){
            if(core[i]->color == 'W'){
                sources[a] = i+1;
                printf("test\n");
                a++;
            }
        }
        else if(core[i]->right == NULL || core[i]->color == 'B'){
            if(core[i]->color == 'W'){
                sinks[b] = i+1;
                printf("%s\n", "test");
                b++;
            }
        }
    }

    printf("\nArray for Sources:\n");
    for (int z = 0; z < a; z++){
        printf("%d ", sources[z]);
    }

    printf("\nArray for Sinks:\n");
    for (int z = 0; z < b; z++){
        printf("%d ", sinks[z]);
    }

    free(conns);
}

int findconn(node * head, list linkedList){
    pole** core = linkedList.head;

    if(!head){
        return 0;
    }
    while(head){
        if((core[(head->value)-1])->color == 'W'){
            (core[(head->value)-1]->color) = 'B';
            return head->value;
        }
        head = head->next;
    }

    return 0;
}

link** findMax(list linkedList){
    int c;
    int size = linkedList.size;
    pole** head = linkedList.head;

    link** linkarr = malloc(sizeof(link*)*size);

    for(int d = 0; d < size; d++){
        linkarr[d] = NULL;
    }

    int surge = 0;
    for(int p = 1; p <= size; p++){
        if(head[p-1]->color == 'W'){
            c = findconn((head[p-1])->right, linkedList);
            if(c){
                linkarr[surge] = createLink(p,c);
                surge++;
            }
            else{
                linkarr[surge] = createLink(p,p);
                surge++;
            }
        }
    }

    int path = 0;
    while(path<surge){
        printf("\nto %d from %d at %d", linkarr[path]->to,linkarr[path]->from, path);
        path++;
    }

    return linkarr;

}

link * createLink(int u, int v){
    link * temp = malloc(sizeof(link));
    if(!temp){
        return NULL;
    }
    temp->from = u;
    temp->to = v;
    return temp;
}

void freeList(list linkedList){
    int size = linkedList.size;
    pole** head = linkedList.head;

    for (int i = 1; i <= size; i++) {//calls for freeing the individual nodes mallocced prior
        freeRows(head[i-1]->right);
        freeRows(head[i-1]->left);
        free(head[i-1]);
    }
    free(head);
}

//freelist is the overarching method for freeing the linked lists, as the structs are called through main, we don't have to free those, yet we need to deconstruct our struct list, and path down the linked lists, stopping by each row to individually free it, which is performed through a method call to freeRows. Afterwhich, we end by freeing the final malloc, used to hold the entire linkedlist together, referred to as the node double pointer

void freeRows(node * head){
    node* newHead;

    if(head == NULL){//returns to avoid overfreeing
        return;
    }
    while(head->next != NULL){//paths through the end of the linked list, deleting each node
        newHead = head->next;//ensures we have the next struct prior to deletion
        free(head);
        head = newHead;
    }
    free(head);
}

//freeRows is a method initially designed to be used in tandem, yet not limited to the freeList method. It takes a node struct, freeing each node connected within its *next datatype

void printList(list linkedList){
    int size = linkedList.size;
    pole** head = linkedList.head;

    printf("Size of %d", size);

    for (int i = 1; i <= size; i++) {//loops through all rows
        printf("\n");
        printThru(head[i-1]->left);
        printf(" %d: ", i);
        printThru(head[i-1]->right);//corrects offset

    }
}

//printList taks the list struct, made in the header, and prints the size of the storage array, then calles the printThru method to proceed printing each node respectively

void printThru(node * head){
    if(head == NULL){
        printf("empty");
        return;
    }
    while(head->next != NULL){//loops through the list, printing each vertex and rests at last node
        printf("%d ", head->value);
        head = head->next;
    }
    printf("%d ", head->value);//prints last node
}

//printThru recieves the head pointer for a linked list of node structs, verifying it's not empty, then prints all elements connected by the next node pointer

list buildAdj(FILE *fp){
    int u, v, i, j;
    int oldmax = 1;
    int max = 1;
    node* nodePlacer;//establishes variables that will be in constant use
    node* flipSide;

    pole** linkedlist = malloc(sizeof(pole*));// creates our array of pointers that will store our linked lists
    if(!linkedlist){//errorchecks malloc
        printf("\nmalloc failed\n");
        list badList;
        badList.head = NULL;
        badList.size = -1;
        return badList;
    }
    linkedlist[oldmax] = createTotem();//erases any garbage data that could accomodate the newly alloced space

    while(fscanf(fp,"%d %d", &u, &v) != EOF){//loops until the end of the file is acheived

        i = 0, j = 0;

        nodePlacer = addNode(v);//calls the addNode method and seeds vertex value
        flipSide = addNode(u);

        if(!nodePlacer || !flipSide){//errorchecks Node
            printf("\nmalloc of %d %d failed, integrity lost\n", u, v);
            break;
        }

        if(u >= v && u > max){//determine if either u, v, or max has the largest value, if its u or v then we adjust our array to accomodate the new array
            oldmax = max;
            max = u;
            linkedlist = realloc(linkedlist, sizeof(node *) * (max));
            for(; oldmax <= max; oldmax++){
                if(i != 0 || oldmax == 1){
                    linkedlist[oldmax-1] = createTotem();
                    //printf("\n%d oldmax", oldmax-1);
                }
                i++;
            }
        }
        else if(v > u && v > max){
            oldmax = max;
            max = v;
            linkedlist = realloc(linkedlist, sizeof(node *) * (max));
            for(; oldmax <= max; oldmax++){
                if(j != 0 || oldmax == 1){
                linkedlist[oldmax-1] = createTotem();
                //printf("\n%d oldmax", oldmax-1);
                }
                j++;
            }
        }
        linkedlist[u-1]->right = insertNode(linkedlist[(u-1)]->right, nodePlacer);//calls for the insertion of the new node in its respective row
        linkedlist[v-1]->left = insertNode(linkedlist[(v-1)]->left, flipSide);
        //printf("%d", flipSide->value);
    }

    list secretSauce;//develops the list struct and returns it to the user
    secretSauce.size = max;
    secretSauce.head = linkedlist;

    return secretSauce;
}

//list Adj takes a FILE pointer, reading through each point and asseses the values recieved, if the integers being received are larger than any former datatype, a new maximun value will be assigned. This will then be used as a reference to the new ammount we have to realloc, to accomodate the influx of vertexes. With the properhousing, then we call the createNode and insertNode methods to seed in its respective data to the appropriate area, with v being the row location (offset down one for arrays) and u being the data stored in the node as the vertex

node * insertNode(node* linkedlist, node* nodePlacer){
    int val = nodePlacer->value;
    node * head = linkedlist;
    if(linkedlist == NULL){//returns the nodePlacer as the new head of list
        //printf("\nfirst\n %d", nodePlacer->value);
        //head = nodePlacer;
        return nodePlacer;
    }
    while(linkedlist->next != NULL){//persues throught the list until it reaches the last node, then adds the node after if it's unique
        if (linkedlist->value == val) {
            //printf("\nsame\n %d", nodePlacer->value);
            free(nodePlacer);
            return head;
        }
        linkedlist = linkedlist->next;
    }
    if(linkedlist->value != val){//avoids repeats on the final value
        linkedlist->next = nodePlacer;
    }
    else{
        //printf("\nfreed\n %d", nodePlacer->value);
        free(nodePlacer);
        return head;
    }
    //printf("\nend\n %d", nodePlacer->value);
    return head;
}

//insertNode's method is twofold, as it's primary objective is to input a new nodePlacer (pointer to a node) into a passed row of the linked list, it then returns said pointer incase if we have to assign an initial pointer to the row. It's secondary objective is to ensure the input is unique, to avoid potential duplicates within the system, as two of the same directed edges would be redundant for this lab

node * addNode(int v){
    node * tempNode = malloc(sizeof(node));
    if (!tempNode) { //checks malloc
        return NULL;
    }
    tempNode->value = v;
    tempNode->next = NULL;
    return tempNode;
}

//method addNode, it takes an integer value and creates a node struct that represents a vertex and inputs the value of the vertex into the struct, nullifying the pointer to another struct and setting the value of color to a Char of W for white, while ultimately not used, I reserved it within this method for future implementation.

pole * createTotem(){
    pole * a= malloc(sizeof(pole));
    if(a){
        a->right = NULL;
        a->left = NULL;
        a->color = 'W';
        return a;
    }
    else{
        printf("FAILED");
        return NULL;
    }
}
