/*
	Compressing data with 32bits, using binary generator G with private key of 32bits with M = 1103515245 and D = 12345
	Using Diffie-Hellman for key exchange and ASCII for comunication
	
	IN
 A [a]
 B [b]
 DH [p] [g]
 #[NM]
 [M1]
 . . .
 [Mn] 
	
	A 12
	B 45
	DH 101 16
	2
	abCdEa
	!@

	OUT
	A->B: 25
	B->A: 84
	A->B: 37 181 39 62 104 241
	B->A: 63 185

*/

#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

const long long int2a32 = 4294967296; // 2^32 

// FAST MODULAR EXPONENTIATION
ll modpow(ll x, ll n, ll m){
    if(n == 0) return 1%m;
    ll u = modpow(x, n/2, m);
    u = (u*u) % m;
    if(n%2 == 1) u = (u*x) % m;
    return u;
}

int main(int argc, char* argv[]) {

    ios_base::sync_with_stdio(0);

	// Abrindo arquivos
	ifstream in(argv[1]);
    streambuf *cinbuf = std::cin.rdbuf();
    cin.rdbuf(in.rdbuf());

    ofstream out(argv[2]);
    streambuf *coutbuf = std::cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    ll M = 1103515245, D = 12345;
    ll a, b, A, B, p, g;
    unsigned long long key, firstKey;
    string entrada;
    int n;
	
	// READING DATA
    cin>>entrada>>a>>entrada>>b;
    cin>>entrada>>p>>g;
    cin>>n;
	
	//GENERATING THE KEYS FOR Diffie-Hellman
    A = modpow(g, a, p);
    B = modpow(g, b, p);
    key = modpow(A, b, p);
	
    cout<<"A->B: "<<A<<"\n";
    cout<<"B->A: "<<B<<"\n";

    firstKey = key;

    //READING MESSAGES
    unsigned int k = 0;
    for(int i = 0; i < n; i++){
        string mensagem;
        cin>>mensagem;
        int siz = mensagem.size();

        //PRINTING CRIPTED MESSAGE
        if(i%2 == 0)cout<<"A->B:";
        else    cout<<"B->A:";

        for(int j = 0; j < siz; j++){
            if(k == 0) key = (M * key + D) % int2a32;
            unsigned char ch = mensagem[j];
            unsigned int z = (key >> k) & 0xFF;
            k = (k + 8) % 32;
            cout<<" "<<(unsigned int) (ch xor z);
        }
        cout<<"\n";
    }

    return 0;
}
