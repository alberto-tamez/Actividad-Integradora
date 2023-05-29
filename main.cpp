#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
***************************
***************************
TRAVELLING SALESMAN PROBLEM - BEGIN
***************************
***************************
*/
const int INF = 1e9; // Representa un valor infinito

vector<vector<int> > dist; // Matriz de distancias
int N;                    // Número de nodos
vector<bool> visitado;    // Vector para registrar los nodos que ya hemos visitado
vector<int> camino;       // Vector para guardar el camino actual
vector<int> caminoMin;    // Vector para guardar el camino mínimo
int costoMinimo = INF;    // Variable para guardar el costo mínimo

/**********
Objetivo: Función para calcular la cota inferior
Entradas: El camino que se ha recorrido hasta ahorita y la posicion
Salidas: La suma de las distancias visitadas hasta ahora mas la distancia del nodo actual al nodo mas cercano
Complejidad: O(n) donde n es el numero de nodos
***********/
int cotaInferior(vector<int> &camino, int pos)
{
    int suma = 0;

    // Costo acumulado hasta ahora de los nodos ya visitados.
    for (int i = 0; i < pos - 1; i++)
    {
        suma += dist[camino[i]][camino[i + 1]];
    }

    // Se calcula la suma para la cota inferior y asi podar el arbol
    // se calcula un minimo al buscar el nodo mas cercano al nodo actual. esto sirve como estimacion para saber cuanto seria el costo minimo
    // al saber el minimo, ya puedes saber un costo estimado del camino que te falta por recorrer
    int minDist = INF;
    for (int i = 0; i < N; i++)
    {
        if (!visitado[i])
        {
            minDist = min(minDist, dist[camino[pos - 1]][i]);
        }
    }
    suma += minDist;

    // Encontramos el nodo más cercano al nodo inicial
    // esto sirve porque siempre se va a acabar en el nodo inicial. Entonces se calcula cuanto al menos te va a costar llegar al nodo inicial.
    // ese costo minimo es el que se obtiene en este for.
    minDist = INF;
    for (int i = 0; i < N; i++)
    {
        if (!visitado[i])
        {
            minDist = min(minDist, dist[i][camino[0]]);
        }
    }
    suma += minDist;

    // se regresa el costo minimo estimado
    return suma;
}

/**********
Objetivo: Función recursiva para calcular el camino mínimo usando B&B
Entradas: La posicion actual
Salidas: Nada
Complejidad: O(n!) donde n es el número de nodos
***********/
void TSP(int pos)
{
    // Si hemos visitado todos los nodos, comprobamos si el camino actual tiene un costo menor que el mínimo actual
    if (pos == N)
    {
        int costoActual = 0;
        for (int i = 0; i < N; i++)
        {
            costoActual += dist[camino[i]][camino[(i + 1) % N]]; // se obtiene el costo actual
        }
        if (costoActual < costoMinimo)
        { // si el costo actual es menor al minimo, se actualizan los valores
            costoMinimo = costoActual;
            caminoMin = camino;
        }
        return;
    }

    // Si el camino actual tiene un costo menor que el mínimo actual, seguimos explorando
    if (cotaInferior(camino, pos) < costoMinimo)
    {

        for (int i = 0; i < N; i++)
        { // se recorren todos los posibles caminos
            if (!visitado[i])
            {
                camino[pos] = i;
                visitado[i] = true;

                TSP(pos + 1);

                visitado[i] = false; // para que se vuelva a considerar en el siguiente camino
            }
        }
    }
}

/*
***************************
***************************
TRAVELLING SALESMAN PROBLEM - END
***************************
***************************
*/

// Estructura para representar una arista con sus respectivos nodos y peso
struct Arista
{
    int nodoA, nodoB, peso;

    Arista(int a, int b, int w) : nodoA(a), nodoB(b), peso(w) {}
};

// Función para comparar aristas, utilizada para ordenar las aristas según su peso
bool comparaAristas(const Arista &a, const Arista &b)
{
    return a.peso < b.peso;
}

// Función que encuentra el padre de un nodo en un conjunto disjunto
// Complejidad de tiempo: O(log N), donde N es el número de nodos
// Complejidad de espacio: O(1), ya que no usa espacio adicional
int encontrarPadre(vector<int> &padre, int nodo)
{
    if (padre[nodo] == nodo)
        return nodo;
    return padre[nodo] = encontrarPadre(padre, padre[nodo]);
}

// Función para unir dos conjuntos en un conjunto disjunto
// Complejidad de tiempo: O(log N), donde N es el número de nodos
// Complejidad de espacio: O(1), ya que no usa espacio adicional
void unirConjuntos(vector<int> &padre, int nodoA, int nodoB)
{
    int padreA = encontrarPadre(padre, nodoA);
    int padreB = encontrarPadre(padre, nodoB);
    padre[padreA] = padreB;
}

// Algoritmo de Kruskal
// Complejidad de tiempo: O(E * log(E)), donde E es el número de aristas
// Complejidad de espacio: O(V + E), donde V es el número de nodos
vector<Arista> kruskal(int N, vector<Arista> aristas)
{
    int numNodos = N;
    vector<int> padre(N);
    vector<Arista> mst; // Árbol de expansión mínima

    // Inicializar conjunto disjunto
    for (int i = 0; i < N; ++i)
    {
        padre[i] = i;
    }

    // Ordenar aristas por peso
    sort(aristas.begin(), aristas.end(), comparaAristas);

    for (Arista &arista : aristas)
    {
        int nodoA = arista.nodoA;
        int nodoB = arista.nodoB;

        // Verificar que los nodos no estén ya conectados
        if (encontrarPadre(padre, nodoA) != encontrarPadre(padre, nodoB))
        {
            mst.push_back(arista);
            unirConjuntos(padre, nodoA, nodoB);
            numNodos--;

            // Todas las colonias conectadas
            if (numNodos == 1)
            {
                break;
            }
        }
    }

    return mst;
}

int main()
{

    int N;
    cin >> N;

    vector<Arista> aristas;

    // Leer matriz de adyacencia de distancias
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int distancia;
            cin >> distancia;

            // Añadir aristas no repetidas con peso de la distancia
            if (i < j) {
                aristas.emplace_back(i, j, distancia);
            }
        }
    }

    // Encontrar conexiones óptimas de colonias con fibra óptica
    vector<Arista> mst = kruskal(N, aristas);

    // Imprimir resultado
    vector<int> pesos;
    for (Arista &arista : mst) {
        pesos.push_back(arista.peso);
    }
    
    // Ordenar los pesos en orden ascendente
    sort(pesos.begin(), pesos.end());

    for (int peso : pesos) {
        cout << peso << endl;
    }

    /*
    ***************************
    ***************************
    TRAVELLING SALESMAN PROBLEM - BEGIN
    ***************************
    ***************************
    */
    // primer valor es el tamaño de la matriz, el segundo valor es el valor default de cada posiscion
    dist = vector<vector<int>>(N, vector<int>(N)); // Inicializa la matriz de distancias
    visitado = vector<bool>(N, false);             // Inicializa el vector de visitados
    camino = vector<int>(N);                       // Inicializa el vector de camino
    caminoMin = vector<int>(N);                    // Inicializa el vector del camino mínimo

    // Recibe la matriz de distancias
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> dist[i][j];
        }
    }

    camino[0] = 0; // El primer nodo visitado es el 0
    visitado[0] = true;

    TSP(1); // Comenzamos desde el nodo 1

    // Imprime el costo mínimo y el camino mínimo
    cout << -2 << endl;
    for (int i = 0; i < N; i++)
    {
        cout << caminoMin[i] << " "; // Imprime el camino mínimo en letras
    }
    cout << caminoMin[0] << endl; // Imprime el camino mínimo en letras
    cout << costoMinimo << "\n";
    /*
    ***************************
    ***************************
    TRAVELLING SALESMAN PROBLEM - END
    ***************************
    ***************************
    */

    return 0;
}
