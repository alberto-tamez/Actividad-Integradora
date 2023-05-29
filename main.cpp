#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

/*
***************************
***************************
TRAVELLING SALESMAN PROBLEM - BEGIN
***************************
***************************
*/
const int INF = 1e9; // Representa un valor infinito

vector<vector<int>> dist; // Matriz de distancias
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

/*
Algoritmo de voronoi --------------------------------
*/
double puntoLineaDist(const pair<int, int> &punto, const pair<int, int> &inicio, const pair<int, int> &fin)
{
    double a = (double)fin.second - inicio.second;
    double b = (double)inicio.first - fin.first;
    double c = (double)a * inicio.first + b * inicio.second;

    double dist = abs(a * punto.first + b * punto.second - c) / sqrt(a * a + b * b);

    return dist;
}

void ramerDouglasPeucker(const vector<pair<int, int>> &puntos, double epsilon, vector<pair<int, int>> &resultado)
{
    if (puntos.size() < 2)
    {
        return;
    }

    auto maxI = 0;
    double maxDist = 0.0;

    for (auto i = 1; i < puntos.size() - 1; ++i)
    {
        double dist = puntoLineaDist(puntos[i], puntos.front(), puntos.back());
        if (dist > maxDist)
        {
            maxI = i;
            maxDist = dist;
        }
    }

    if (maxDist > epsilon)
    {
        vector<pair<int, int>> antes(maxI + 1);
        copy(puntos.begin(), puntos.begin() + maxI + 1, antes.begin());
        vector<pair<int, int>> despues(puntos.size() - maxI);
        copy(puntos.begin() + maxI, puntos.end(), despues.begin());

        ramerDouglasPeucker(antes, epsilon, resultado);
        resultado.pop_back();
        ramerDouglasPeucker(despues, epsilon, resultado);
    }
    else
    {
        resultado.push_back(puntos.front());
        resultado.push_back(puntos.back());
    }
}
double distancia(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

// Función para encontrar la central más cercana dado un punto (x, y)
int centralMasCercana(int x, int y, const vector<pair<int, int>> &centrales)
{
    double minDist = 1e9;
    int idx = -1;
    for (int i = 0; i < centrales.size(); ++i)
    {
        double dist = distancia(x, y, centrales[i].first, centrales[i].second);
        if (dist < minDist)
        {
            minDist = dist;
            idx = i;
        }
    }
    return idx;
}

// Función para generar e imprimir los polígonos de Voronoi que cumplan con los requisitos
void voronoi(const vector<pair<int, int>> &centrales, int xmin, int ymin, int xmax, int ymax)
{

    // Generar la malla con las coordenadas x e y del rectángulo delimitado
    vector<vector<int>> malla(xmax - xmin + 1, vector<int>(ymax - ymin + 1, -1));

    int incremento = 10; // Puedes ajustar este valor según el tamaño del área

    for (int x = xmin; x <= xmax; x += incremento)
    {
        for (int y = ymin; y <= ymax; y += incremento)
        {
            malla[x - xmin][y - ymin] = centralMasCercana(x, y, centrales);
        }
    }

    // Generar polígonos de Voronoi para cada central según la malla
    vector<vector<pair<int, int>>> poligonos(centrales.size());
    for (int x = 0; x < malla.size(); ++x)
    {
        for (int y = 0; y < malla[0].size(); ++y)
        {
            int central_idx = malla[x][y];
            poligonos[central_idx].push_back({x + xmin, y + ymin});
        }
    }
    vector<vector<pair<int, int>>> poligonosFiltrados(centrales.size());

    for (int i = 0; i < poligonos.size(); ++i)
    {
        for (const auto &punto : poligonos[i])
        {
            if (punto.first >= xmin && punto.first <= xmax && punto.second >= ymin && punto.second <= ymax)
            {
                poligonosFiltrados[i].push_back(punto);
            }
        }
    }

    // Imprimir los polígonos de Voronoi, en el orden ascendente de la menor coordenada x de cada polígono
    cout << -4 << endl;
    for (int i = 0; i < poligonosFiltrados.size(); ++i)
    {
        auto comparaCentrales = [&centrales](int i, int j)
        {
            return centrales[i].first < centrales[j].first;
        };
        vector<int> indice(centrales.size());
        iota(indice.begin(), indice.end(), 0);
        sort(indice.begin(), indice.end(), comparaCentrales);

        int central_idx = indice[i];

        // Ordenar puntos del polígono en sentido horario
        auto puntoCentral = centrales[central_idx];
        auto comparaPuntos = [&puntoCentral](pair<int, int> a, pair<int, int> b)
        {
            int axdiff = a.first - puntoCentral.first;
            int aydiff = a.second - puntoCentral.second;
            int bxdiff = b.first - puntoCentral.first;
            int bydiff = b.second - puntoCentral.second;

            if (axdiff * bydiff - aydiff * bxdiff > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        };

        sort(poligonosFiltrados[central_idx].begin(), poligonosFiltrados[central_idx].end(), comparaPuntos);

        // Imprimir polígono de Voronoi
        double epsilon = 30.0; // El valor de epsilon se puede ajustar según la cantidad de simplificación deseada
        vector<pair<int, int>> poligonoSimplificado;
        ramerDouglasPeucker(poligonosFiltrados[central_idx], epsilon, poligonoSimplificado);

        // Imprimir polígono de Voronoi simplificado
        cout << -4 << i + 1 << endl;
        for (const auto &punto : poligonoSimplificado)
        {
            cout << punto.first << " " << punto.second << endl;
        }
    }
}

// ------------------------------------------------------------------------------------------------
int main()
{

    cout << -1 << endl;
    int N;
    cin >> N;

    vector<Arista> aristas;

    // Leer matriz de adyacencia de distancias
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            int distancia;
            cin >> distancia;

            // Añadir aristas no repetidas con peso de la distancia
            if (i < j)
            {
                aristas.emplace_back(i, j, distancia);
            }
        }
    }
    // Voronoi  ----------------------------------------------------------------
    int nCentrales = N;
    vector<pair<int, int>> centrales(nCentrales);
    for (int i = 0; i < nCentrales; ++i)
    {
        cin >> centrales[i].first >> centrales[i].second;
    }
    // ----------------------------------------------------------------

    // Encontrar conexiones óptimas de colonias con fibra óptica
    vector<Arista> mst = kruskal(N, aristas);

    // Imprimir resultado
    vector<int> pesos;
    for (Arista &arista : mst)
    {
        pesos.push_back(arista.peso);
    }

    // Ordenar los pesos en orden ascendente
    sort(pesos.begin(), pesos.end());

    for (int peso : pesos)
    {
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

    int xmin = INT_MAX, xmax = INT_MIN, ymin = INT_MAX, ymax = INT_MIN;

    for (const auto &central : centrales)
    {
        xmin = min(xmin, central.first);
        xmax = max(xmax, central.first);
        ymin = min(ymin, central.second);
        ymax = max(ymax, central.second);
    }

    voronoi(centrales, xmin, ymin, xmax, ymax);

    return 0;
}
