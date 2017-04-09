#include <iostream>
#include <vector>
#include <algorithm>

struct Objeto {
    double peso;
    double valor;
};

struct Densidad {
    double densidad;
    size_t objeto;
};

bool operator>(Densidad const &d1, Densidad const &d2) {
    return d1.densidad > d2.densidad;
}

void mochilaVoraz(std::vector<Objeto> const &objetos, double pesoMochila,
                  std::vector<double> &solucion, double &valorSol) {
    size_t n = objetos.size();

    //Calculamos las densidades de cada objeto
    std::vector<Densidad> d(n);
    for (size_t i = 0; i < n; ++i) {
        d[i].densidad = objetos[i].valor / objetos[i].peso;
        d[i].objeto = i;    //Para saber a que objeto corresponde
    }

    //Ordenamos de mayor a menor las densidades
    std::sort(d.begin(), d.end(), std::greater<Densidad>());

    //Cogemos los objetos mientras quepan enteros
    size_t i;
    for (i = 0; i < n && pesoMochila - objetos[d[i].objeto].peso >= 0; ++i) {
        valorSol += objetos[d[i].objeto].valor;
        pesoMochila -= objetos[d[i].objeto].peso;
        solucion[d[i].objeto] = 1;
    }

    //Si aun no se ha llenado la mochila completamos partiendo el objeto
    if (pesoMochila > 0) {
        solucion[d[i].objeto] = pesoMochila / objetos[d[i].objeto].peso;
        valorSol += objetos[d[i].objeto].valor * solucion[d[i].objeto];
    }
}


int main() {
    size_t n = 5;
    std::vector<Objeto> objetos(n);
    for (int i = 0; i < n; ++i)
        objetos[i].peso = (i + 1) * 10;

    objetos[0].valor = 20;
    objetos[1].valor = 30;
    objetos[2].valor = 66;
    objetos[3].valor = 40;
    objetos[4].valor = 60;

    std::vector<double> solucion(n);
    double valorSol;

    mochilaVoraz(objetos, 100, solucion, valorSol);

    std::cout << "ValorSol: " << valorSol << '\n';
    for (int i = 0; i < n; ++i)
        std::cout << "Peso de " << i << ": " << solucion[i] << '\n';

    return 0;
}