/*
  DATAGRAM
  A UDP based system is being created for transfer of packages in a TCP/IP web.
  Datas are bytes sequence with max of 512 bytes. 
  Given the number of packcages that is coming on the other side of the web, print the byte sequence just till the last ordered packgage
  and when a new packcage comes, check if it is ordered again and print what is missing.
  
  SOLUTION: Was implemented the HEAPSORT algortihm for such problem.
  
  IN
	6 2
	0 3 01 02 03
	1 2 04 05
	2 4 06 07 08 09
	4 2 0F 10
	3 5 0A 0B 0C 0D 0E
	5 6 11 12 13 14 15 16
	
  OUT 
	0: 01 02 03 04 05
	1: 06 07 08 09
	2: 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16

*/

#include<bits/stdc++.h>
using namespace std;

int n, q, id, m;

//HEAPIFY 
void heapify(int i, int e, pair<int, string> *pac){
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if(l < e && pac[l] > pac[largest])
        largest = l;

    if(r < e && pac[r] > pac[largest])
        largest = r;

    if(largest != i){
        swap(pac[i], pac[largest]);
        heapify(largest, e ,pac);
    }
}

//HEAPSORT FUNCTION
void heapSort(int e, pair<int, string> *pac){
    for(int i = e/2 - 1; i >= 0; i--)
        heapify(i, e, pac);

    for(int i = e-1; i >= 0; i--){
        swap(pac[0], pac[i]);
        heapify(0, i, pac);
    }
}

int visited[100001];


//PROCESSING DATA IN
void processarEntrada(pair<int, string> *pac){
    int i, cont = 0;
    for(i = q-1; i < n; i+=q){
        if(i + q >= n) i = n-1;
        heapSort(i+1, pac);
        cout<<cont++<<":";
        for(int j = 0; j <= i; j++){
            if(pac[j].first != j) break;
            if(!visited[j]){
                visited[j] = 1;
                cout<<pac[j].second;
            }
        }
        cout<<"\n";
    }
}


//MAIN FUNCTION
int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(0);

    // OPENING DATA FILE
	ifstream in(argv[1]);
    streambuf *cinbuf = std::cin.rdbuf();
    cin.rdbuf(in.rdbuf());

    ofstream out(argv[2]);
    streambuf *coutbuf = std::cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    cin>>n>>q;

    pair<int, string> pac[n+1];

    //READING
    for(int i = 0; i < n; i++){
        string sum, aux;
        cin>>id>>m;
        for(int j = 0; j < m; j++){
            cin>>aux;
            sum = sum + " " + aux;
        }
        visited[i] = 0;
        pac[i] = {id, sum};
    }

    processarEntrada(pac);

    return 0;
}
