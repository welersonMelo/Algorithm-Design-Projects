/*
	Given a hexa sequence of data such data has to be compressed using RLE and Huffman algorithms. 
	Print the best of them and the percentage of compression followed by the compressed data in hexa.
	 
	IN
	4
	5 0xAA 0xAA 0xAA 0xAA 0xAA
	7 0x10 0x20 0x30 0x40 0x50 0x60 0x70
	9 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
	4 0xFA 0xFA 0xC1 0xC1
	
	OUT 
	0: [HUF 20.00%] 0x00
	1: [HUF 42.86%] 0x9C 0x6B 0x50
	2: [HUF 22.22%] 0x00 0x00
	2: [RLE 22.22%] 0x09 0xFF
	3: [HUF 25.00%] 0xC0
	 
	
 */

#include<bits/stdc++.h>

using namespace std;

const int INF = 1000000007;

FILE *in, *out;

struct Sequencia{
    int siz;
    short int val[10001];
    int rleSiz = 0;
    short int rle[30001];
    int hufSiz = 0;
    short int huf[20001];
};

struct No{
    int freq;
    short int simbId;
    No* dir;
    No* esq;
};

struct MinHeap{
    int siz;    // Current size of min heap
    int capacity;   // capacity of min heap
    struct No **arr;  // Attay of minheap node pointers
};

struct Histograma{
    int freq;
    short int simb;
};

string tabela[300];
int arr[300];
Sequencia sequencia[4123];

void leiaAte(int n){
    int a;
    for(int i = 0; i < n; i++){
        fscanf(in, "%d", &a);
        sequencia[i].siz = a;

        for(int j = 0; j < a; j++)
            fscanf(in, "%x", &sequencia[i].val[j]);
    }
    return;
}



 int binaryToDecimal(string s){
    int n = stoi(s);

    int output = 0;

    for(int i=0; n > 0; i++) {

        if(n % 10 == 1) {
            output += (1 << i);
        }
        n /= 10;
    }

    return output;
}

string hexToBinary(string sHex){
    string sReturn = "";
    for (int i = 0; i < sHex.length (); ++i)
    {
        switch (sHex [i])
        {
            case '0': sReturn.append ("0000"); break;
            case '1': sReturn.append ("0001"); break;
            case '2': sReturn.append ("0010"); break;
            case '3': sReturn.append ("0011"); break;
            case '4': sReturn.append ("0100"); break;
            case '5': sReturn.append ("0101"); break;
            case '6': sReturn.append ("0110"); break;
            case '7': sReturn.append ("0111"); break;
            case '8': sReturn.append ("1000"); break;
            case '9': sReturn.append ("1001"); break;
            case 'a': sReturn.append ("1010"); break;
            case 'b': sReturn.append ("1011"); break;
            case 'c': sReturn.append ("1100"); break;
            case 'd': sReturn.append ("1101"); break;
            case 'e': sReturn.append ("1110"); break;
            case 'f': sReturn.append ("1111"); break;
        }
    }
    return sReturn;
}

int hexToDecimal(string hex){
    unsigned long result = 0;
    for (int i=0; i < hex.length(); i++) {
        if (hex[i]>=48 && hex[i]<=57)
        {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

void compRLE(int atual){
    int siz = sequencia[atual].siz;

    string compact = "";

    sequencia[atual].val[siz] = -1;

    short int ant = sequencia[atual].val[0];
    int contRep = 1, cont = 0;

    for(int i = 1; i < siz+1; i++){
        short int curr = sequencia[atual].val[i];
        if(curr != ant){
            sequencia[atual].rle[cont++] = contRep;
            sequencia[atual].rle[cont++] = ant;
            contRep = 1;
        }else contRep++;
        ant = curr;
    }
    sequencia[atual].rleSiz = cont;
}

MinHeap *createHeap(int capacidade){
    MinHeap *minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->siz = 0;
    minHeap->capacity = capacidade;
    minHeap->arr = (No**) malloc(minHeap->capacity * sizeof(No*));
    return minHeap;
}

void swapNo(No** a, No** b){
    No* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx){
    int menor = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    //esq
    if (esq < minHeap->siz && minHeap->arr[esq]->freq < minHeap->arr[menor]->freq)
        menor = esq;
    //else if(esq < minHeap->siz && minHeap->arr[esq]->freq == minHeap->arr[menor]->freq && minHeap->arr[esq]->simbId < minHeap->arr[menor]->simbId)
    //    menor = esq;

    //dir
    if (dir < minHeap->siz && minHeap->arr[dir]->freq < minHeap->arr[menor]->freq)
        menor = dir;
    //else if (dir < minHeap->siz && minHeap->arr[dir]->freq == minHeap->arr[menor]->freq && minHeap->arr[dir]->simbId < minHeap->arr[menor]->simbId)
    //    menor = dir;

    if (menor != idx){
        swapNo(&minHeap->arr[menor], &minHeap->arr[idx]);
        minHeapify(minHeap, menor);
    }
}

No* extractMin(MinHeap* minHeap) {
    No* temp = minHeap->arr[0];
    minHeap->arr[0] = minHeap->arr[minHeap->siz - 1];
    --minHeap->siz;
    minHeapify(minHeap, 0);
    return temp;
}

void buildMinHeap(MinHeap* minHeap){
    int n = minHeap->siz - 1;
    for(int i = (n - 1) / 2; i >= 0; i--)
        minHeapify(minHeap, i);
}

void insertMinHeap(MinHeap* minHeap, int freq, int data,  No* esq, No* dir){
    struct No* temp = (No*) malloc(sizeof (struct No));
    ++minHeap->siz;

    temp->esq = esq;
    temp->dir = dir;
    temp->simbId = data;
    temp->freq = freq;

    int i = minHeap->siz - 1;
    minHeap->arr[i] = temp;
    buildMinHeap(minHeap);
}

No* huffmanTree(int data[], int freq[], int siz){
    struct No *left, *right, *top;
    struct MinHeap* minHeap = createHeap(siz);

    for(int i = 0; i < 257; i++)
        if(freq[i] > 0)
            insertMinHeap(minHeap, freq[i], data[i], NULL, NULL);

    minHeap->siz = siz;

    while (minHeap->siz > 1){
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        insertMinHeap(minHeap, left->freq + right->freq, -1, left, right);
    }
    return extractMin(minHeap);
}


void gereCodigo(int simbId, int n){
    string aux = "";
    for(int i = 0; i < n; i++){
        if(arr[i] == 0) aux.append("0");
        else aux.append("1");
    }
    if(aux == "") aux = "0";
    //cout<<hex<<simbId<<": "<<aux<<endl;
    tabela[simbId] = aux;
}

void codes(No* root, int top){
    if (root->esq){
        arr[top] = 0;
        codes(root->esq, top + 1);
    }
    if (root->dir){
        arr[top] = 1;
        codes(root->dir, top + 1);
    }

    if(!(root->dir) && !(root->dir)){
        gereCodigo(root->simbId, top);
    }
}

Histograma hist[257];

int gereHistograma(int atual){
    for(int i = 0; i < 257; i++) hist[i].freq = 0, hist[i].simb = i;

    int siz = sequencia[atual].siz, cont = 0;

    for(int i = 0; i < siz; i++){
        short int aux = sequencia[atual].val[i];
        if(hist[aux].freq == 0) cont++;
        hist[aux].freq++;
        hist[aux].simb = aux;
    }
    return cont;
}

string compacted, compactedHex;

void compHUF(int atual){
    //string str = sequencia[atual].val;
    int histSimbId[257];
    int histFreq[257];

    int tam = gereHistograma(atual);

    for(int i = 0; i < 257; i++){
        histSimbId[i] = hist[i].simb;
        histFreq[i] = hist[i].freq;
    }
    int top = 0;
    No* root = huffmanTree(histSimbId, histFreq, tam);

    memset(arr, 0, sizeof arr);

    codes(root, top);

    compacted = "";
    int siz = sequencia[atual].siz;
    for(int i = 0; i < siz; i++){
        //string aux = str.substr(i, 2);
        short int aux = sequencia[atual].val[i];
        compacted.append(tabela[aux]);
    }

    int resto = 8 - (compacted.size() % 8), sz = compacted.size();

    while(resto--) compacted.append("0");

    compactedHex = "";
    string aux;
    int cont = 0;
    for(int i = 0; i < sz; i+=8){
        aux = compacted.substr(i, 8);
        sequencia[atual].huf[cont++] = binaryToDecimal(aux);
    }
    sequencia[atual].hufSiz = cont;
}

void processeAte(int n){
    for(int i = 0; i < n; i++){
		//calling compression algorithms
        compRLE(i);
        compHUF(i);

        double base = sequencia[i].siz;
        double rle = sequencia[i].rleSiz/base;
        double huf = sequencia[i].hufSiz/base;
		
		//Comparing them and printing the results
		
        if(huf <= rle){
            huf += (1/INF);
            fprintf(out, "%d: [HUF %.2f%] ", i, huf*100);
            int siz = sequencia[i].hufSiz;
            for(int j = 0; j < siz; j++){
                fprintf(out, "%s%X", sequencia[i].huf[j] < 16? "0x0": "0x", sequencia[i].huf[j]);

                if(!(j + 1 == siz)) fputs(" ", out);
                else fputs("\n", out);
            }
        }

        if(rle <= huf){
            rle += (1/INF);
            fprintf(out, "%d: [RLE %.2f%] ", i, rle*100);
            int siz = sequencia[i].rleSiz;
            for(int j = 0; j < siz; j++){
                fprintf(out, "%s%X", sequencia[i].rle[j] < 16? "0x0": "0x", sequencia[i].rle[j]);

                if(!(j + 1 == siz)) fputs(" ", out);
                else fputs("\n", out);
            }
        }
    }
}

int main(int argc, char* argv[]){

	in = fopen(argv[1], "r");
    out = fopen(argv[2], "w+");

    int n;

    fscanf(in, "%d", &n); // reading N
    leiaAte(n); //reading other values

    processeAte(n);

    return 0;
}
