/*
############################# porto ####################################
Given N containers, read the code, CNPJ (indentity code of the bussiness) and weight of the container.
After is given M selected conteiners codes, CNPJ and weight.
Check for each M container given, if the CNPJ or weight are incorrects and put it on an ordered list given priority for the CNPJ errors and second the weight above 10% of difference from the original weight in order of percentage.

So print such list showing the correct and incorrect CNPJ or the percentage of weight difference

SOLUTION:
Solution above is using Merge Sort implementation.
TIME COMPLEXITY: N LOG(N)

IN
6
QOZJ7913219 34.699.211/9365-11 13822
FCCU4584578 50.503.434/5731-28 16022
KTAJ0603546 20.500.522/6013-58 25279
ZYHU3978783 43.172.263/4442-14 24543
IKQZ7582839 51.743.446/1183-18 12160
HAAZ0273059 25.699.428/4746-79 16644
5
ZYHU3978783 43.172.263/4442-14 29765
IKQZ7582839 51.743.446/1113-18 18501
KTAJ0603546 20.500.522/6113-58 17842
QOZJ7913219 34.699.211/9365-11 16722
FCCU4584578 50.503.434/5731-28 16398

OUT
KTAJ0603546: 20.500.522/6013-58<->20.500.522/6113-58
IKQZ7582839: 51.743.446/1183-18<->51.743.446/1113-18
QOZJ7913219: 2900kg (21%)
ZYHU3978783: 5222kg (21%)

*/

#include <bits/stdc++.h>

using namespace std;

//STRUCT FOR CONTAINER
typedef struct conteiner{
    string cod, cnpj, aux;
    int id, peso, porcentagem;
    bool ehDivergente;

    bool operator>=(const conteiner& c)const{
        if(ehDivergente != c.ehDivergente) return ehDivergente;
        else{
            if(ehDivergente)
                return id < c.id;
            else{
                if(porcentagem != c.porcentagem)
                    return porcentagem > c.porcentagem;
                else return id < c.id;
            }
        }
    }

    bool operator<(const conteiner& c)const{
        return cod < c.cod;
    }
} Conteiner;

Conteiner regConteiner[100000];
Conteiner conteinerSelec[10000];

int sizeS = 0;

//FUCTION FOR SEARCH A CONTAINER IN LOG(N)
pair<pair<string, int>, int> searchFor(string cod, int n){
    int fim = n;
    int ini = 0;
    //BINARI SEARCH
    while(ini < fim){
        int meio = ini + (fim - ini)/2;
        if(cod == regConteiner[meio].cod)
            return {{regConteiner[meio].cnpj, regConteiner[meio].id}, regConteiner[meio].peso};

        if(cod < regConteiner[meio].cod)
            fim = meio;
        else ini = meio + 1;
    }

    return {{"-1", -1}, -1};
}

//MERGE FUNCTION OF MERGESORT
void intercalar(Conteiner conteiner1[],int ini, int meio, int fim, int comparador){
    int i, j, k;
    int s1 = meio - ini + 1;
    int s2 = fim - meio;
    Conteiner L[s1], R[s2];

    for(i = 0; i < s1; i++) L[i] = conteiner1[ini + i];
    for(i = 0; i < s2; i++) R[i] = conteiner1[meio + i + 1];

    i = 0;
    j = 0;
    k = ini;

    while(i < s1 && j < s2){
        if(comparador == 0){
            if(L[i] < R[j]) conteiner1[k++] = L[i++];
            else conteiner1[k++] = R[j++];
        }else{
            if(L[i] >= R[j]) conteiner1[k++] = L[i++];
            else conteiner1[k++] = R[j++];
        }
    }

    while(i < s1) conteiner1[k++] = L[i++];
    while(j < s2) conteiner1[k++] = R[j++];
}

//MERGESORT FUNCTION
void mergeSort(Conteiner conteiner1[],int ini, int fim, int comparador){
    if(ini < fim){
        int meio = ini + (fim - ini)/2;
        mergeSort(conteiner1, ini, meio, comparador);
        mergeSort(conteiner1, meio + 1, fim, comparador);
        intercalar(conteiner1, ini, meio, fim, comparador);
    }
}

//MAIN FUNCTION
int main(int argc, char* argv[]) {

    ios_base::sync_with_stdio(0);

	// READING DATA ON FILE
	ifstream in(argv[1]);
    streambuf *cinbuf = std::cin.rdbuf();
    cin.rdbuf(in.rdbuf());

    ofstream out(argv[2]);
    streambuf *coutbuf = std::cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    int n, m, peso;
	string codigo, cnpj;

    //READING IN
	cin>>n;
    for(int i = 0; i < n; i++){
        cin>>codigo>>cnpj>>peso;
        regConteiner[i] = (conteiner{codigo, cnpj, "", i, peso, 0, false});
    }

    mergeSort(regConteiner, 0, n-1, 0); // 0 significa que irá comparar com <

    cin>>m;

    int cont = 0;

    for(int i = 0; i < m; i++){
        cin>>codigo>>cnpj>>peso;
        //BINARY SEARCH
        pair<pair<string, int>,int> conteinerReg = searchFor(codigo, n);

        if(conteinerReg.second != -1){
            int aux = abs(peso - conteinerReg.second);
            double val = double(double(aux)/double(conteinerReg.second))*100;
            int v = round(val);
            if(conteinerReg.first.first != cnpj){
                conteinerSelec[sizeS++] = (conteiner{codigo, conteinerReg.first.first, cnpj, conteinerReg.first.second, aux, round(val), true});
                cont++;
            }else if(v > 10){
                conteinerSelec[sizeS++] = (conteiner{codigo, conteinerReg.first.first, cnpj,conteinerReg.first.second, aux, round(val), false});
            }
        }
    }

    mergeSort(conteinerSelec ,0, sizeS-1, 1); // 1 significa que ira comparar com >=

    for(int i = 0; i < sizeS; i++){
        cout<<conteinerSelec[i].cod;
        if(conteinerSelec[i].ehDivergente){
            cout<<": "<<conteinerSelec[i].cnpj<<"<->"<<conteinerSelec[i].aux<<"\n";
        }else{
            cout<<": "<<conteinerSelec[i].peso<<"kg ("<<conteinerSelec[i].porcentagem<<"%)\n";
        }
    }

	//END
	return 0;
}
