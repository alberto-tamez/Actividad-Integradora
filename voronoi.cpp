#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <utility>

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
        std::string mostrarXY() const;
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

std::string Point::mostrarXY() const { 
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
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
        
        // metodo para obtener el cincuncentro y circunradio
        Point getCircunCentro();
        double getCircunRadio();

        // metodo para imprimir todos los puntos del triangulo
        void mostrarPuntos() const;
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

Point Triangulo::getCircunCentro(){
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
double Triangulo::getCircunRadio(){
    double D = std::abs(2 * (A.getX()*(B.getY() - C.getY()) + B.getX()*(C.getY() - A.getY()) + C.getX()*(A.getY() - B.getY())));
    double dAB = std::pow(B.getX() - A.getX(), 2) + std::pow(B.getY() - A.getY(), 2);
    double dBC = std::pow(C.getX() - B.getX(), 2) + std::pow(C.getY() - B.getY(), 2);
    double dCA = std::pow(A.getX() - C.getX(), 2) + std::pow(A.getY() - C.getY(), 2);
    return std::sqrt(dAB * dBC * dCA) / D;
}


void Triangulo::mostrarPuntos() const {
    std::cout << "Punto A: " << A.mostrarXY()<< std::endl;
    std::cout << "Punto B: " << B.mostrarXY() << std::endl;
    std::cout << "Punto C: " << C.mostrarXY() << std::endl;
}


int N;
std::vector<Point> puntos;

/**********
Objetivo: Función para obtener la diferencia entre dos puntos
Entradas: Dos diferentes puntos
Salidas: La diferencia entre los dos puntos
Complejidad: O(1) porque se hacen calculos simples
***********/
double distanciaDosPuntos(const Point &p1, const Point &p2) {
    double diffx = p1.getX() - p2.getX();
    double diffy = p1.getY() - p2.getY();
    return std::sqrt( (diffx * diffx) + (diffy * diffy));
}

void delaunay_triangulation(std::vector<Point>& puntos) { 
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
    std::queue<Point> listaPuntos;
    for (const auto& punto : puntos) {
        listaPuntos.push(punto);
    }

    // 3. Se checa si hay un punto a agregar. 
    while (!listaPuntos.empty()) {
        Point puntoTemporal = listaPuntos.front();

        // 4a - 4b: Encontrar circuncentro y curcunradio y determinar la distancia entre punto y circuncentro
        // se recorre el vector completo de triangulos_DT
        
        for (auto& triangulo : triangulos_DT) {
            int distanciaPT = distanciaDosPuntos(puntoTemporal,triangulo.getCircunCentro()); // distancia entre centro del ciruclo y el punto

            // 4c, distancia menor que el circunradio y por lo tanto esta en el circulo
            if (distanciaPT < triangulo.getCircunRadio()) {
                triangulos_invalidos.push_back(triangulo);
            }

        }
        std::vector<Point> puntosInvalidos;
        for (auto triangulo: triangulos_invalidos) {
            
        }
        


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


    delaunay_triangulation(puntos);


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
*/