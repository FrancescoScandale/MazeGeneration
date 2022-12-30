#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct cell {
    int row;
    int col;
    int walls[4];   //0=north, 1=east, 2=south, 3=west
    struct cell *father; //pointer to the rep of the disjoint set; it's the element with the smallest row (and then col) of the set
    int rank;

    //these variables are for the DFS implementation
    int index;     //index of next free position in *sons
    struct cell **sons;
} cell;

typedef struct {
    cell* representative;
} ds;

int maxRows;
int maxCols;
int found=0;

//------------------------SIGNATURES---------------------------------
void printMaze(cell** maze);
void initMazeDS(cell** maze);
void mergeDisjointSets(cell* one, cell* two);
cell* findRepresentative(cell* a);
int checkReached(cell** maze);
void findPath(cell* current, cell* end, cell* previous);

//------------------------------MAIN---------------------------------
int main(int argc, char *argv[]) {
    //initializing the variables
    //srand(time(NULL));
    int over=0,i,j,wallsDown=0;
    int row, col, wall, row2, col2, wall2;
    cell** maze;

    if(argc > 1){
        maxRows=atoi(argv[1]);
        maxCols=atoi(argv[2]);
    }
    else {
        maxRows = 3;
        maxCols = 2;
    }
    maze = (cell**) malloc(sizeof(cell)*maxRows);
    for(i=0;i<maxRows;i++){
        maze[i] = (cell*) malloc(sizeof(cell)*maxCols);
    }

    initMazeDS(maze);
    //printMaze(maze);

    //algorithm
    while(!over){
       //choose wall
        row = rand()%maxRows;
        col = rand()%maxCols;
        wall = rand()%4;

        //find the other cell
        if(wall==0){
            row2 = row-1;
            col2 = col;
            wall2 = 2;
        }
        else if(wall==1){
            row2 = row;
            col2 = col+1;
            wall2 = 3;
        }
        else if(wall==2){
            row2 = row+1;
            col2 = col;
            wall2 = 0;
        }
        else if(wall==3){
            row2 = row;
            col2 = col-1;
            wall2 = 1;
        }

        if (!((row==0&&wall==0) || (row==maxRows-1&&wall==2) || (col==0&&wall==3) || (col==maxCols-1&&wall==1))){  //not perimeter walls
            if(maze[row][col].walls[wall]==1 && (findRepresentative(&maze[row][col])!=findRepresentative(&maze[row2][col2]))){ //wall exists and cells are in disjointed sets
                wallsDown++;
                //set corresponding walls to 0
                maze[row][col].walls[wall] = 0;
                maze[row2][col2].walls[wall2] = 0;
                
                //manage the disjoint sets
                mergeDisjointSets(&maze[row][col],&maze[row2][col2]);

                //check if start and end are connected
                over = checkReached(maze);

                //print only sometimes
                //if(wallsDown%maxRows==0 || over!=0) printMaze(maze);
                printMaze(maze);
            }
        }
    }

    //DFS
    /*findPath(&maze[0][0],&maze[maxRows-1][maxCols-1],&maze[0][0]);

    if(found) printf("\nA path was found with the DFS algorithm.\n");
    else printf("Something went wrong while computing the DFS path.\n");*/
    printf("Maze algorithm ended destroying %d walls.\n",wallsDown);

    return 0;
}

//------------------------FUNCTIONS---------------------------------
void printMaze(cell** maze){
    int i,j;

    for(i=0;i<maxCols;i++){
        printf(" _");
    }
    printf("\n");

    for(i=0;i<maxRows;i++){
        for(j=0;j<maxCols;j++){
            if(maze[i][j].walls[3]==1) printf("|");
            else printf(" ");
            if(maze[i][j].walls[2]==1) printf("_");
            else printf(" ");
        }
        printf("|\n");
    }

    printf("\n\n");
}

//filling the maze
void initMazeDS(cell** maze){
    int i,j;

    for(i=0;i<maxRows;i++){
        for(j=0;j<maxCols;j++){
            maze[i][j].row = i;
            maze[i][j].col = j;
            maze[i][j].walls[0]=1;
            maze[i][j].walls[1]=1;
            maze[i][j].walls[2]=1;
            maze[i][j].walls[3]=1;
            maze[i][j].father=&maze[i][j];
            maze[i][j].rank=0;

            maze[i][j].index=0;
            maze[i][j].sons = (cell**) malloc(sizeof(cell*)*(maxCols*maxRows*1000));
        }
    }
}

//does UNION() from the Disjoint Sets theory
void mergeDisjointSets(cell* one, cell* two){
    cell* a;
    cell* b;

    //both already in a set -> UNION()
    a = (cell*) findRepresentative(one);
    b = (cell*) findRepresentative(two);
    if(a->rank > b->rank){  //the new representative is the one with the lesser index in disjointSets
        b->father = a;

        a->sons[a->index] = b;
        a->index++;
    }
    else {
        a->father = b;

        b->sons[b->index] = a;
        b->index++;
        if(a->rank == b->rank){
            b->rank++;
        }
    }
    
    return;
}

//returns the index of disjointSets where the representative stays, via recursive calls going up the forest
cell* findRepresentative(cell* a) {
    if(a->father == 0) return a;   //a is just initialised, just return yourself

    if(a != a->father) a->father = findRepresentative(a->father);
    
    return a->father;
}

int checkReached(cell** maze){
    cell* a;
    cell* b;

    a = findRepresentative(&maze[0][0]);
    b = findRepresentative(&maze[maxRows-1][maxCols-1]);

    //if representatives are different -> different sets -> not reached
    if(a!=b) return 0;
    else return 1;
}

//DFS algorithm
void findPath(cell* current, cell* end, cell* previous){
    int i;

    //printf("%d,%d\t",current->row,current->col);
    for(i=0;i<current->index && !found;i++){
        if(current->sons[i]!=previous){
            if(current->sons[i] != end) {
                findPath(current->sons[i],end,current);
            }
            else {   //reached the end
                found=1;
                return;
            }
        }
    }
    if(!found && current->father==previous){   //all sons visited, return to father
        return;
    }
    else if(!found){    //go up to the father
        findPath(current->father,end,current);
        return;
    }

    return;
}