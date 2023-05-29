#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <utility>
#include <algorithm>


const int INF = 1e9; // Representa un valor infinito

// clase punto
class Point {
    private: 
        // un objeto Point solo consistira de dos variables double. 
        double x, y;

    public:
        // metodos que inicializan un objeto Point()
        Point();
        Point(double, double);
        Point(const Point&);

        // metodos para obtener los valroes de x y y
        double getX() const;
        double getY() const;

        // metodo para mostrar valores
        std::string obtenerXY() const;

        // Overload del operador de igualdad para funcion hash
        bool operator==(const Point& p) const {
            return x == p.x && y == p.y;
        }
};

// se crean sus metodos constructores y los metodos para obtener las variables
Point::Point() {
    x = y = 0.0;
}

Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

Point::Point(const Point& p) {
    x = p.x;
    y = p.y;
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

std::string Point::obtenerXY() const { 
    return std::to_string(x) + " " + std::to_string(y);
}

// Definición de la función hash para Point. Necesario para funcion hash de <Point>
namespace std {
    template<>
    struct hash<Point> {
        std::size_t operator()(const Point& p) const {
            std::size_t h1 = std::hash<double>()(p.getX());
            std::size_t h2 = std::hash<double>()(p.getY());
            return h1 ^ (h2 << 1); // Bitwise shift y XOR
        }
    };
}

// clase Arista
class Arista {
    private: 
        // un objeto Arista consistirá de dos puntos. 
        Point p1, p2;

    public:
        // metodos que inicializan un objeto Arista()
        Arista();
        Arista(const Point&, const Point&);
        Arista(const Arista&);

        // metodos para obtener los puntos
        Point getP1() const;
        Point getP2() const;

        // se sobrecarga el operador == para comparar dos aristas.
        bool operator==(const Arista& a) const {
            return ((p1 == a.p1 && p2 == a.p2) || (p1 == a.p2 && p2 == a.p1));
        }
};

Arista::Arista(): p1(), p2() {}

Arista::Arista(const Point& point1, const Point& point2) : p1(point1), p2(point2) {}

Arista::Arista(const Arista& a) : p1(a.p1), p2(a.p2) {}

Point Arista::getP1() const {
    return p1;
}
Point Arista::getP2() const {
    return p2;
}

// definicion de la funcion hash para la clase arista para crear un unordered set
namespace std {
    template <>
    struct hash<Arista> {
        size_t operator()(const Arista& a) const {
            // Obtenemos los hashes de cada punto
            size_t h1 = hash<Point>{}(a.getP1());
            size_t h2 = hash<Point>{}(a.getP2());
            
            // Los combinamos
            // Nota: Este es un método simple y no es necesariamente el mejor método para combinar hashes
            return h1 ^ h2;
        }
    };
}


class Triangulo { 
    private:
        // un objeto triangulo solo consistira de 3 puntos
        Point A, B, C;
    public: 
        // metodos para inicializar el objeto
        Triangulo();
        Triangulo(const Point&, const Point&, const Point&);
        Triangulo(const Triangulo&);

        // metodos para obtener los puntos de un triangulo
        const Point& getA() const;
        const Point& getB() const;
        const Point& getC() const;

        // metodo para obtener las aristas de un triangulo
        std::vector<Arista> getAristas() const;
        
        // metodo para obtener el cincuncentro y circunradio
        Point getCircunCentro() const;
        double getCircunRadio() const;

        // metodo para imprimir todos los puntos del triangulo
        void mostrarPuntos() const;

        // se sobreecarga el operador == para comparar si dos triangulos son iguales. 
        bool operator==(const Triangulo& t) const { 
            return (A == t.getA() && B == t.getB() && C == t.getC()) ||
                   (A == t.getA() && B == t.getC() && C == t.getB()) ||
                   (A == t.getB() && B == t.getA() && C == t.getC()) ||
                   (A == t.getB() && B == t.getC() && C == t.getA()) ||
                   (A == t.getC() && B == t.getA() && C == t.getB()) ||
                   (A == t.getC() && B == t.getB() && C == t.getA());
        }

};

// se crean sus metodos constructores y los metodos para obtener las variables
Triangulo::Triangulo() {
    A = B = C = Point();
}

Triangulo::Triangulo(const Point& A, const Point& B, const Point& C) {
    this->A = A;
    this->B = B;
    this->C = C;
}

Triangulo::Triangulo(const Triangulo& T) {
    this->A = T.A;
    this->B = T.B;
    this->C = T.C;
}

const Point& Triangulo::getA() const { 
    return A;
}

const Point& Triangulo::getB() const { 
    return B;
}

const Point& Triangulo::getC() const { 
    return C;
}

// metodo para obtener la arista
std::vector<Arista> Triangulo::getAristas() const {
    return { Arista(A, B), Arista(B, C), Arista(C, A)};
}

Point Triangulo::getCircunCentro() const{
    // formula para obtener los puntos de un circulo
    double D = 2 * (A.getX()*(B.getY() - C.getY()) + B.getX()*(C.getY() - A.getY()) + C.getX()*(A.getY() - B.getY()));
    double h = ((A.getX()*A.getX() + A.getY()*A.getY()) * (B.getY() - C.getY()) + 
                (B.getX()*B.getX() + B.getY()*B.getY()) * (C.getY() - A.getY()) + 
                (C.getX()*C.getX() + C.getY()*C.getY()) * (A.getY() - B.getY())) / D;
    double k = ((A.getX()*A.getX() + A.getY()*A.getY()) * (C.getX() - B.getX()) + 
                (B.getX()*B.getX() + B.getY()*B.getY()) * (A.getX() - C.getX()) + 
                (C.getX()*C.getX() + C.getY()*C.getY()) * (B.getX() - A.getX())) / D;
    return Point(h, k);
}
double Triangulo::getCircunRadio() const{
    double D = std::abs(2 * (A.getX()*(B.getY() - C.getY()) + B.getX()*(C.getY() - A.getY()) + C.getX()*(A.getY() - B.getY())));
    double dAB = std::pow(B.getX() - A.getX(), 2) + std::pow(B.getY() - A.getY(), 2);
    double dBC = std::pow(C.getX() - B.getX(), 2) + std::pow(C.getY() - B.getY(), 2);
    double dCA = std::pow(A.getX() - C.getX(), 2) + std::pow(A.getY() - C.getY(), 2);
    return std::sqrt(dAB * dBC * dCA) / D;
}


void Triangulo::mostrarPuntos() const {
    std::cout << "Punto A: " << A.obtenerXY() << 
    " Punto B: " << B.obtenerXY() << 
    " Punto C: " << C.obtenerXY() << std::endl;
}


int N;
std::vector<Point> puntos;

/**********
Objetivo: Función para obtener la diferencia entre dos puntos
Entradas: Dos diferentes puntos
Salidas: La diferencia entre los dos puntos en decimal
Complejidad: O(1) porque se hacen calculos simples
***********/
double distanciaDosPuntos(const Point &p1, const Point &p2) {
    double diffx = p1.getX() - p2.getX();
    double diffy = p1.getY() - p2.getY();
    return std::sqrt( (diffx * diffx) + (diffy * diffy)); // se regresa la distancia con valor decimal
}

/**********
Objetivo: Función para saber si una arista se encuentra en el vector de triangulos_invalidos
Entradas: Una arista y el vector<Triangulo> de triangulos_invalidos
Salidas: True si se encuentra la arista en triangulos invalidos, false si no se encuentra
Complejidad: O(t) donde t es el numero de triangulos
***********/
bool arista_en_triangulosInvalidos (const Arista& arista, const std::vector<Triangulo>& triangulos_invalidos) {
    // aqui se obtienen los edges del triangulo y por cada arista obtenida se va comparando si pi y pj son iguales o no
    for (const auto& triangulo : triangulos_invalidos) {
        std::vector<Arista> aristas_vector = triangulo.getAristas();
        for (const auto& a : aristas_vector) {
            if (arista == a) { // se regresa true si la arista es parte de una arista en triangulos invalidos
                return true;
            }
        }
    }
    return false; // Los puntos no forman una arista en ningún triángulo
}

/**********
Objetivo: Función para saber si una arista se encuentra en el vector de aristas_comunes
Entradas: Una arista y el vector<Arista> de aristas_comunes
Salidas: True si se encuentra la arista en triangulos invalidos, false si no se encuentra
Complejidad: O(a) donde a es el numero de aristas en aristas comunes
***********/
bool arista_en_aristasComunes (const Arista& arista, const std::vector<Arista>& aristas_comunes) {
    for (const auto& arista_comun : aristas_comunes) {
        if (arista == arista_comun) {
            return true; // si se encontro la arista en aristas comunes, regresar true de que se encontro
        }
    }
    return false; // regresar false si no se encontro nada
}



/**********
Objetivo: Función para saber si la arista es una arista de triangulos_invalidos y si esta arista no esta en aristas_comunes
Entradas: Una arista, vetor triangulos_invalidos, vector aristas_comunes
Salidas: True si se encuentra la arista en triangulos invalidos, false si no se encuentra
Complejidad: O(a + t) donde a es el numero de aristas en aristas comunes y t es el numero de triangulos en triangulos_invalidos
***********/
bool confirmacion_arista(const Arista& arista,const std::vector<Triangulo>& triangulos_invalidos, const std::vector<Arista>& aristas_comunes) { 
    if (arista_en_triangulosInvalidos(arista, triangulos_invalidos) && !arista_en_aristasComunes(arista, aristas_comunes)) {
        return true; // si se cumple la condicion, regresa true
    }
    return false; // regresa false si no se cumple la condicion
}


std::vector<Triangulo> delaunay_triangulation(std::vector<Point>& puntos) { 
    // 1. se forma el super_triangulo. El primer triangulo en DT
    int minx = INF;
    int maxx = 0;
    int miny = INF;
    int maxy = 0;

    for (const auto& elemento : puntos) {
        // se obtiene minx y maxx
        if (elemento.getX() < minx) {
            minx = elemento.getX();
        }
        if (elemento.getX() > maxx) {
            maxx = elemento.getX();
        }

        // se obtiene miny y maxy
        if (elemento.getY() < miny) {
            miny = elemento.getY();
        }
        if (elemento.getY() > maxy) {
            maxy = elemento.getY();
        }
        
    }
    int distx = maxx - minx;
    int disty = maxy - miny;

    // se crea el super triangulo
    Triangulo super_triangulo = Triangulo(
        Point(minx - (9.5*distx), miny + (0.5*disty)),
        Point(minx + (5.5*distx), miny + (9.16*disty)),
        Point(minx + (5.5*distx), miny - (8.16*disty))
    );

    // 2. Estructura de datos para triangluos creados. El primer triangulo es incluido. 
    std::vector<Triangulo> triangulos_DT;
    std::vector<Triangulo> triangulos_invalidos;

    triangulos_DT.push_back(super_triangulo);

    // se agregan todos los puntos a la queue
    // para obtener un valor de la queue se necesita getX(), getY(), o mostrarCY()
    std::queue<Point> listaPuntos;

    // se agregan todos los puntos del vecotr a la queue
    for (const auto& punto : puntos) {
        listaPuntos.push(punto);
    }

    // 5. Ahora se crea un unordered_set de puntos invalidos
    //std::cout << "ENTRO AL PASO 5" << std::endl;
    std::unordered_set<Point> puntos_invalidos_set;
    std::vector<Point> puntos_invalidos;

    //se crea un vector de aristas comunes
    // punto p1, punto p2
    std::vector<Arista> aristas_comunes;
    std::unordered_set<Arista> aristas_comunes_set; // se crea un unordered set

    // 3. Se checa si hay un punto a agregar.
    while (!listaPuntos.empty()) {
        Point punto = listaPuntos.front(); // se otbiene el punto que esta al frente de la lista
        listaPuntos.pop(); // se elimina el punto al frente de la lista
        //std::cout << "Punto: " << punto.obtenerXY() << std::endl;
        // 4. Se itera sobre triangulos_DT
        for (const auto& triangulo : triangulos_DT) {
            //std::cout << "ENTRO AL PASO 4" << std::endl;
            // 4a - 4b: Encontrar circuncentro y curcunradio y determinar la distancia entre punto y circuncentro
            double distancia_punto_circuncentro = distanciaDosPuntos(punto,triangulo.getCircunCentro()); // distancia entre centro del ciruclo y el punto
            
            //std::cout << "Triangulo: " << triangulo.getCircunCentro().obtenerXY() << std::endl;
            //std::cout << "Circuncentro: " << triangulo.getCircunCentro().obtenerXY() << std::endl;
            //std::cout << "Circunradio: " << triangulo.getCircunRadio() << std::endl;
            //std::cout << "DistanciaPuntos: " << distancia_punto_circuncentro << std::endl;

            // 4c. Si la distnacia es menor que el circunradio, se guarda triangulo actual en triangulos_invalidos
            if (distancia_punto_circuncentro < triangulo.getCircunRadio()) {
                triangulos_invalidos.push_back(triangulo); // se guarda en vector triangulos invalidos
                //std::cout << "Se hizo pushback: " << std::endl;
                //triangulos_invalidos[0].mostrarPuntos();
            }

        }

        // 6. se recorre el vector de triangulos invalidos
        for (int idx = 0; idx < triangulos_invalidos.size(); idx++) {
            //std::cout << "ENTRO AL PASO 6" << std::endl;
            //6a
            aristas_comunes.erase(aristas_comunes.begin(), aristas_comunes.end());
            // se recorre el arreglo de triangulos_invalidos y checa cuales aristas son similares.
            for (int jdx = 0; jdx < triangulos_invalidos.size(); jdx++) {

                // cuando los triangulos son diferentes
                if (idx != jdx) {
                    // se guardan en aristasT1 y aristasT2 un vector<pair<Point, Point>>. 
                    // es un vector donde se tiene un par de puntos que son las aristas
                    auto aristasT1 = triangulos_invalidos[idx].getAristas();
                    auto aristasT2 = triangulos_invalidos[jdx].getAristas();

                    // se recorren los dos vectores de las aristas y se comparan
                    for (auto& arista1 : aristasT1) {
                        for (auto& arista2 : aristasT2) {
                            // en este if se checa si se tiene una arista en comun. 
                            if (arista1 == arista2) {
                                //std::cout << "Se encontro arista en comun" << std::endl;

                                // cuando se tiene una arist en comun
                                aristas_comunes_set.insert(arista1); // se guarda la arista en aristas_comunes
                                // en la siguiente linea, se elimina triangulos_invalidos[idx] de triangulos_dt
                                triangulos_DT.erase(
                                    std::remove_if(
                                        triangulos_DT.begin(),
                                        triangulos_DT.end(),
                                        [&triangulos_invalidos, &idx](const Triangulo& t) { return t == triangulos_invalidos[idx]; }
                                    ),
                                    triangulos_DT.end()
                                );
                                
                            }
                        }
                    }


                }
            }

            //6b. se agrega a puntos_invalidos_set todo los vertices de los triangulos de triangulos_invalidos
            puntos_invalidos_set.insert(triangulos_invalidos[idx].getA()); // inserta primer vertice
            puntos_invalidos_set.insert(triangulos_invalidos[idx].getB()); // inserta segundo vertice
            puntos_invalidos_set.insert(triangulos_invalidos[idx].getC()); // inserta tercer vertice
        }

        // se recorre el unordered_set de puntos invalidos y se inserta en un vector. Esto es necesario para paso 7
        for (const auto& punto_invalido : puntos_invalidos_set) {
            puntos_invalidos.push_back(punto_invalido);
            //std::cout << punto_invalido.obtenerXY() << std::endl;
        }

        for (const auto& arista_comu : aristas_comunes_set) {
            aristas_comunes.push_back(arista_comu);
            //std::cout << arista_comu.getP1().obtenerXY() << " " << arista_comu.getP2().obtenerXY() << std::endl;
        }

        /*std::cout << "ENTRO AL PASO 7" << std::endl;
        std::cout << puntos_invalidos.size() << std::endl;
        std::cout << triangulos_invalidos.size() << std::endl;
        std::cout << aristas_comunes.size() << std::endl;*/


        //7. Se recorren los puntos invalidos
            for (int idx = 0; idx < puntos_invalidos.size()  - 1; idx++) {
                for (int jdx = idx + 1; jdx < puntos_invalidos.size(); jdx++) {
                    if (confirmacion_arista(Arista(puntos_invalidos[idx], puntos_invalidos[jdx]), triangulos_invalidos, aristas_comunes)) {
                        Triangulo nuevo_triangulo = Triangulo(puntos_invalidos[idx], puntos_invalidos[jdx], punto); // se forma el nuevo triangulo con los nuevos puntos
                        triangulos_DT.push_back(nuevo_triangulo); // se inserta el nuevo triangulo al vector de trianguos_DT
                        //std::cout << "Triangulo: ";
                        //triangulos_DT[triangulos_DT.size() - 1].mostrarPuntos();
                    }
                }
            }

            //std::cout << "ENTRO AL PASO 8" << std::endl;
            // se aseguran que las siguientes estructuras esten siendo borradas antes de empezar el siguiente ciclo
            triangulos_invalidos.erase(triangulos_invalidos.begin(), triangulos_invalidos.end());
            puntos_invalidos.erase(puntos_invalidos.begin(), puntos_invalidos.end());
            aristas_comunes.erase(aristas_comunes.begin(), aristas_comunes.end());
            aristas_comunes_set.clear();
            puntos_invalidos_set.clear();
    }
    return triangulos_DT;
}

void voronoi_diagram(std::vector<Point>& puntos) {
    //1. Encontrar triangulos_DT de delaunay
    std::vector<Triangulo> triangulos_DT = delaunay_triangulation(puntos);
    
    std::unordered_set<Point> puntos_validos;
    // se recorren todos los triangulos
    for (int idx = 0; idx < triangulos_DT.size() - 1; idx++) {
        for (int jdx = idx + 1; jdx < triangulos_DT.size(); jdx++) {

            auto aristasT1 = triangulos_DT[idx].getAristas(); // se obtienen aristas del triangulo 1
            auto aristasT2 = triangulos_DT[jdx].getAristas(); // se obtienen aristas del triangulo 2

            // se recorren los dos vectores de las aristas y se comparan
            for (auto& arista1 : aristasT1) {
                for (auto& arista2 : aristasT2) {
                    // en este if se checa si slos triangulos tienen una arista en comun.
                    // si si, significa que sus dos circuncentro deben de ser unidos
                    if (arista1 == arista2) {
                        puntos_validos.insert(triangulos_DT[idx].getCircunCentro());
                        puntos_validos.insert(triangulos_DT[jdx].getCircunCentro());
                    }
                }
            }
        }
    }
    
    for (const auto& puntos : puntos_validos) {
        std::cout << puntos.obtenerXY() << std::endl;
    }
    
}

int main() {
    std::cin >> N;
    puntos = std::vector<Point>(N); // inicializa el vector con los puntos

    for (int idx = 0; idx < N; idx++) {
        double x;
        double y;

        std::cin >> x >> y;
        puntos[idx] = Point(x,y);
    }

    voronoi_diagram(puntos);

    return 0;
}
/*
input:
10
656 607
349 228
460 380
657 378
618 187
155 99
541 570
272 649
429 98
120 339

Salida:
-4
-41
208 231
286 389
487 242
485 226
293 103
-42
279 453
383 527
559 451
557 301
487 242
286 389

caso prueba
3
1 1
2 1
2 2
*/
