//Write few lines of comments at the beginning of your program to explain the idea of your algorithm.

//Within My algorithm, I initialize the linked list to hold front and back values, with its creation, I develop a maximal edge list and proceed to link the edges together. Once completed, I attach any non-listed vertices to their appropriate place depending on wether they're a source or sink, printing out their respective result

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
link * createLink(int u, int v);
pole* createTotem();
node * insertNode(node* linkedlist, node* nodePlacer);
void printConns(list);
link** findMax(list);
int findconn(node *, list);
void printList(list);
void printThru(node *);
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
    int g = 0;
    int sources[size];
    int sinks[size];
    link ** conns = findMax(G);
    link * first = NULL;
    link * second = NULL;

    printf("\nEdges Required for Insert:");

    for(int i = 0; i < size && conns[i] != NULL; i++){
        if((conns[i])->to != (conns[i])->from){
            if(!first){
                first = conns[i];
            }
            else if(!second){//goes through list of connections linking the head of the first to the tail of a second
                second = conns[i];
                printf("\n%d %d", first->to, second->from);
                core[(first->to)-1]->color = 'B';//avoid overlinking
                core[(second->from)-1]->color = 'B';
            }
            else{
                first = second;
                second = conns[i];//moves up markers
                printf("\n%d %d", first->to, second->from);
                core[(first->to)-1]->color = 'B';//avoids overlinking
                core[(second->from)-1]->color = 'B';
            }
        }
    }
    if(second){//displays final connection of last left element back to top right element
        printf("\n%d %d", second->to, conns[0]->from);
        core[(second->to)-1]->color = 'B';
        core[(conns[0]->from)-1]->color = 'B';
    }

    for (int i = 0; i < size; i++) {//determines if node is sink or source, once determined, the loop assigns it into either array
        if(core[i]->left == NULL){
            if(core[i]->color == 'W'){//checks to ensure it's not been used to generate a node yet and assigns if true to sink array
                sources[a] = i+1;
                a++;
            }
        }
        else if(core[i]->right == NULL){//checks to ensure it's not been used to generate a node yet and assigns if true to source array
            if(core[i]->color == 'W'){
                sinks[b] = i+1;
                b++;
            }
        }
    }

    for(g = 0; g < (a) && g < (b); g++){//displays outlying, unlinked nodes to an appropriate sources by connective each sink to a source where they exist
        printf("\n%d %d", sinks[g], sources[g]);
    }
    for(; g < (a); g++){
        printf("\n%d %d", (conns[0]->to), sources[g]);
    }
    for(;g < b; g++){
        printf("\n%d %d", sinks[g], (conns[0]->to));
    }

    link** persue = conns;
    while(*persue){
        free(*persue);
        persue = persue + 1;
    }
    free(conns);
}

//printConns is a function that initially collects the generated adjacency list matrix of the graph, then loads the array of edgesprinting the edges determined to be needed and then freeing the conns array

int findconn(node * head, list linkedList){
    pole** core = linkedList.head;

    if(!head){
        return 0;
    }
    while(head){
        if((core[(head->value)-1])->color == 'W'){
            (core[(head->value)-1]->color) = 'B';//prevents overrunning
            return head->value;
        }
        head = head->next;
    }

    return 0;
}

//find conn takes a pointer to a linked list and will find a node that hasn't been linked, returning the vertex if it's a suitable connection, returning 0 if it fails

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
            if(c){//if there was a value discovered, creates with the c value
                linkarr[surge] = createLink(p,c);
                surge++;
            }
            else{//if not sets equal to itself
                linkarr[surge] = createLink(p,p);
                surge++;
            }
        }
    }

    return linkarr;
}

//findMax takes a linkedList, scanning through and collecting the maximal edges for each vertext, if it doesn't exist, has the Link be the same to and from value

link * createLink(int u, int v){
    link * temp = malloc(sizeof(link));
    if(!temp){
        return NULL;
    }
    temp->from = u;
    temp->to = v;
    return temp;
}

//createLink takes in two values of a vertice connection, and attaches them into a malloc'd struct, returning the result as a Li

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

//freelist is the overarching method for freeing the linked lists, as the structs are called through main, we don't have to free those, yet we need to deconstruct our struct list, and path down the linked lists, stopping by each row to individually free it, which is performed through a method call to freeRows. Afterwhich, we end by freeing the final malloc, used to hold the entire linkedlist together, referred to as the node double pointer also frees the elements in the core

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

//printList taks the list struct, made in the header, and prints the size of the storage array, then calles the printThru method to proceed printing each node respectively forwards and backwards

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
                }
                j++;
            }
        }
        linkedlist[u-1]->right = insertNode(linkedlist[(u-1)]->right, nodePlacer);//calls for the insertion of the new node in its respective row
        linkedlist[v-1]->left = insertNode(linkedlist[(v-1)]->left, flipSide);
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
            free(nodePlacer);
            return head;
        }
        linkedlist = linkedlist->next;
    }
    if(linkedlist->value != val){//avoids repeats on the final value
        linkedlist->next = nodePlacer;
    }
    else{
        free(nodePlacer);
        return head;
    }
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

//createTotem mallocs out a pole struct and assigns default values, returning the malloced pointer
