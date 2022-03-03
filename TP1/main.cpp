/*O jogo do pulo consiste em um tabuleiro NxM e o objetivo é que as pessoas jogando atinjam a última posição 
do tabuleiro (nesse caso a posição N-1xM-1) antes que o resto das pessoas, e cada passo no tabuleiro deve seguir
o valor determinado na casa onde a pessoa se econtra, podendo ela se mover em qualquer sentido na vertical ou
na horizontal contanto que o número de casas andadas seja exatamente o valor contido na casa de onde partiu o movimento*/

#include <bits/stdc++.h>
#define INF 0x3f3f3f;

using namespace std;

typedef pair<int, int> vertice;
typedef pair<int, pair<int, int> > cam_min;

class Grafo{
    int tamanho; //numero de vertices do grafo
    list<vertice> *vertices; //lista dos vertices dos grafos
public:
    Grafo(int tamanho); //metodo contrutor
    void add(int v1, vertice v2); //metodo de adcionar arestas
    void imprimir(); //mDEBUG
    cam_min dijkstra(int vx, int vy, int fim, int N, int M); //metodo de caminho minimo
};

Grafo::Grafo(int tamanho){
    this-> tamanho = tamanho;
    vertices = new list<vertice>[tamanho]; //cria as listas das vizinhancas dos vertices do grafo
}

void Grafo::add(int v1, vertice v2){
    vertices[v1].push_back(v2); //adciona v2 a vizinhanca de v1
}

//funcao para imprimir o grafo (DEBUG)
void Grafo::imprimir(){
    for(int i=0; i<tamanho; i++){
        cout << i << endl;
        for(list<vertice>::iterator it = vertices[i].begin(); it != vertices[i].end(); ++it){
            vertice aux = *it;
            cout << aux.first << ' ' << aux.second << endl;
        }
    }
    cout << endl;
}

class compara{ 
    public: 
    int operator() (const vertice& u, const vertice& v) { 
        return u.first > v.first;
    } 
};

//funcao para encontrar o menor caminho de uma posicao até o objetivo
cam_min Grafo::dijkstra(int vx, int vy, int fim, int N, int M){
    int inicio = vx*M + vy; //vertice inicial
    int dist[N*M]; //vetor de distancias
    vertice antecessor[N*M]; //armazena as coordenadas do penultimo vertice do caminho
    bool visitado[N*M]; //vetor de flag de visita
    for(int i=0; i<N*M; i++){ //inicializa os vetores auxiliares
        dist[i] = INF;
        visitado[i] = 0;
    }
    priority_queue<vertice, vector<vertice>, compara> fila;
    dist[inicio] = 0; //a distancia do vertice pra ele mesmo
    fila.push(make_pair(dist[inicio], inicio)); //insere na fila (distancia percorrida, vertice atual)
    while(!fila.empty()){
        vertice topo = fila.top(); //pega o par da frente
        fila.pop(); //remove ele da fila
        int v = topo.second; //vertice do par
        if(visitado[v] == 0){ //testa se o vertice ja foi verificado 
            visitado[v] = 1; //marca como visitado
            list<vertice>::iterator it;
            for(it = vertices[v].begin(); it != vertices[v].end(); it++){
                int adj = (it->first)*M + it->second; //obtem o vertice adjacente
                if(dist[adj] > (dist[v] + 1)){
                    dist[adj] = dist[v] + 1; //atualiza a distancia percorrida
                    fila.push(make_pair(dist[adj], adj)); //insere na fila
                }
                antecessor[adj] = make_pair(vx, vy);
                vx = it->first;
                vy = it->second;
            }
        }
    }
    int aux = M*(antecessor[fim].first) + antecessor[fim].second; //indice da rodada que determinou a ordem da ultima rodada
    return make_pair(dist[fim], antecessor[aux]); //retorna distancia minima ate destino
}

//funcao pra retornar o indice do menor elemento de um array
int min(vertice* arr, int tam){
    int min = INF + 1;
    int index = 0;
    for(int i=0; i<tam; i++){
        if(arr[i].first < min){
            min = arr[i].first;
            index = i;
        }
        else if(arr[i].first == min){
            if(arr[i].second < arr[index].second){
                index = i;
            }
        }
    }
    return index; 
}

int main(int argc, char* argv[]){   
    
    int N, M; //dimensoes do tabuleiro
    int jogadores; //numero de jogadores
    cin >> N >> M >> jogadores;
    int tabuleiro[N][M];
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            cin >> tabuleiro[i][j]; 
        }
    }

    vector<vertice> init; //array de posicoes iniciais dos jogadores
    for(int i=0; i<jogadores; i++){
        int a, b;
        cin >> a >> b;
        init.push_back(make_pair(a, b)); //cria vetor de posicoes iniciais
    }

    Grafo G(N*M); //cria o grafo das posicoes do tabuleiro
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            if(tabuleiro[i][j] == 0){ //verifica se pode mover
                continue;
            }
            if((i + tabuleiro[i][j]) < N){ //checa se existe posição na direcao +x
                G.add((j + i*M), make_pair(i+tabuleiro[i][j], j)); //adciona uma aresta entre os vetices
            }
            if(i >= tabuleiro[i][j]){ //checa se existe posição na direcao -x
                G.add((j + i*M), make_pair(i-tabuleiro[i][j], j));
            }
            if((j + tabuleiro[i][j]) < M){ //checa se existe posição na direcao +y
                G.add((j + i*M), make_pair(i, j+tabuleiro[i][j]));
            }
            if(j >= tabuleiro[i][j]){ //checa se existe posição na direcao -y
                G.add((j + i*M), make_pair(i, j-tabuleiro[i][j]));
            }
        }
    }

    cam_min aux[jogadores]; //vetor com os menores caminhos entre cada jogador e o objetivo
    for(int i=0; i<jogadores; i++){
        int fim = N*M - 1;
        aux[i] = G.dijkstra(init[i].first, init[i].second, fim, N, M);
    }

    vertice caminhos[jogadores];
    for(int i=0; i<jogadores; i++){
        caminhos[i].first = aux[i].first;
        caminhos[i].second = tabuleiro[aux[i].second.first][aux[i].second.second];
    }

    int index_min = min(caminhos, jogadores);
    if(caminhos[index_min].first == 0x3f3f3f){
        cout << "SEM VENCEDORES" << endl;
    }
    else{
        cout << char(index_min + 65) << endl << caminhos[index_min].first << endl;
    }
    
    return 0;
}
