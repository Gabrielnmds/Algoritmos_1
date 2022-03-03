#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    
    int k; //quantidade de diamantes
    cin >> k;
    int S = 0;
    vector<int> D(k); //vetor dos pesos das pedras
    for(int i=0; i<k; i++){
        cin >> D[i];
        S += D[i];
    }
    int C = S/2;
    int M[k+1][C+1];

    for(int i=0; i<k+1; i++){
        M[i][0] = 0;
    }
    for(int j=0; j<C+1; j++){
        M[0][j] = 0;
    }
    
    for(int i=1; i<k+1; i++){
        for(int j=1; j<C+1; j++){
            if(D[i-1] <= j){
                M[i][j] = max(M[i-1][j], M[i-1][j-D[i-1]] + D[i-1]);
            }
            else{
                M[i][j] = M[i-1][j];
            }
        }
    }

    cout << S - 2*M[k][C] << endl;
}