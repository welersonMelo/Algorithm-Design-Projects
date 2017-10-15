/*
  Given C matrix N X M with walls(1), free spaces(0) and a start space (X) find the path to get out of the matrix and print it
   
  Solution: Using a backtracking algortihm based on flood fill graph algorithm
  
  IN:
  C
  N M
  Matrix ...
  
    2
	5 4
	1 1 1 1 1
	1 0 0 0 1
	1 0 X 0 1
	1 1 0 1 1
	3 4
	1 1 1
	1 X 1
	1 0 1 
  
  OUT 
  OBS: (INICIO = START, SAIDA = END, D = RIGHT, F = FRONT, E = LEFT, T = BACK, BT = BACKTRACKING, SEM SAIDA = NO ESCAPE)
	
	L0: 
	INICIO [2,2]
	D [2,2]->[2,3]
	F [2,3]->[1,3]
	E [1,3]->[1,2]
	E [1,2]->[1,1]
	T [1,1]->[2,1]
	BT [1,1]<-[2,1]
	BT [1,2]<-[1,1]
	BT [1,3]<-[1,2]
	BT [2,3]<-[1,3]
	BT [2,2]<-[2,3]
	T [2,2]->[3,2]
	SAIDA [3,2]
	L1:
	INICIO [1,1]
	T [1,1]->[2,1]
	BT [1,1]<-[2,1]
	SEM SAIDA
	 
	
*/

#include<bits/stdc++.h>
using namespace std;
char a[102][102];
int begI = 0, begJ = 0, n, m;
bool encontrou;

FILE *in, *out;

//IF IS OUT OF BOUND OF THE MATRIX
bool isOut(int i, int j){
    return (i < 0 || i >= n || j < 0 || j >= m);
}

int dir1[] = {0, -1, 0,  1};
int dir2[] = {1, 0 , -1, 0};
char dir[] = {'D', 'F', 'E', 'T'};

//FLOOD FILL ALGORITHM FOR BACKTRACKING IN MATRIX AND PRINT THE PATH
void flood(int i, int j){
    for(int k = 0; k < 4; k++){
        if(!isOut(i + dir1[k], j + dir2[k])){
            if(a[i + dir1[k]][j + dir2[k]] != '1'){
                if(!encontrou)
                    fprintf(out, "%c [%d,%d]->[%d,%d]\n", dir[k] ,i, j, i + dir1[k], j + dir2[k]);
                a[i + dir1[k]][j + dir2[k]] = '1';
                flood(i + dir1[k], j + dir2[k]);
                if(!encontrou)
                    fprintf(out, "BT [%d,%d]<-[%d,%d]\n", i, j, i + dir1[k], j + dir2[k]);
            }
        }else{
            encontrou = true;
            fprintf(out, "SAIDA [%d,%d]\n",i, j);
            return;
        }
    }
}

//MAIN FUNCTION
int main(int argc, char* argv[]){
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w+");

    int c, caso = 0;
    char aux;
    fscanf(in, "%d", &c);
    //READING C MATRIX
    while(c--){
        fscanf(in, "%d%d", &m, &n);
        fscanf(in, "%c", &aux);

        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                fscanf(in, "%c ", &a[i][j]);
                if(a[i][j] == 'X'){
                    begI = i;
                    begJ = j;
                    a[i][j] = '1';
                }
            }
        }
        encontrou = false;
        fprintf(out, "L%d:\n", caso++);
        fprintf(out, "INICIO [%d,%d]\n", begI, begJ);

        flood(begI, begJ);

        if(!encontrou)
            fprintf(out, "SEM SAIDA\n");
    }
    return 0;
}
