#ifndef PROBLEMAMOCHILACPP_ALGORITMOS_H
#define PROBLEMAMOCHILACPP_ALGORITMOS_H

#include <vector>
#include <iostream>

struct ObjetoReal {
    double peso;
    double valor;
};

struct ObjetoInt {
    int peso;
    double valor;
};
struct Densidad {
    double densidad;
    size_t obj;
};

inline bool operator>(Densidad const &d1, Densidad const &d2) {
    return d1.densidad > d2.densidad;
}

/**
 * Resuelve el problema de la mochila con objetos fraccionables mediante un
 * algoritmo voraz. Presuponemos que la suma de los pesos de todos los
 * objetos > M.
 *
 * Coste en tiempo: O(n logn), n = numero de objetos.
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param M Peso maximo que soporta la mochila.
 * @param solucion Indica cuanto se debe coger de cada objeto [0, 1].
 * @param valorSol Valor de la mochila con los objetos dados por solucion.
 */
void mochilaVoraz(std::vector<ObjetoReal> const &objetos, double M,
                  std::vector<double> &solucion, double &valorSol);

/**
 * Resuelve el problema de la mochila 0-1 mediante un algoritmo de
 * programacion dinamica. El peso de cada objeto y el peso maximo de la
 * mochila deben ser enteros positivos.
 *
 * Coste: O(nM) en tiempo y espacio, n = numero de objetos, M = peso que
 * soporta la mochila.
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param M Peso maximo que soporta la mochila.
 * @param solucion Indica si se coge el objeto o no.
 * @param valorSol Valor de la mochila con los objetos dados por solucion.
 */
void mochilaProgDin(std::vector<ObjetoInt> const &objetos, unsigned int M,
                    std::vector<bool> &solucion, double &valorSol);

struct Nodo {
    std::vector<bool> sol;
    int k;
    double pesoAc, valorAc;
    double valorOpt;    //Prioridad
};

inline bool operator<(Nodo const &n1, Nodo const &n2) {
    return n1.valorOpt < n2.valorOpt;
}

/**
 * Resuelve el problema de la mochila 0-1 mediante un algoritmo de
 * ramifiacion y poda.
 *
 * Coste: O(n 2^n) en tiempo y espacio, n = numero de objetos.
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param M Peso maximo que soporta la mochila.
 * @param solMejor Indica si se coge el objeto o no.
 * @param valorMejor Valor de la mochila con los objetos dados por solMejor.
 */
void mochilaRamPoda(std::vector<ObjetoReal> const &objetos, double M,
                    std::vector<bool> &solMejor, double &valorMejor,
                    long long int &nodosVisitados);

struct Cromosoma {
    std::vector<bool> crom;
    double valor;
};

inline bool operator<(Cromosoma const &c1, Cromosoma const &c2) {
    return c1.valor < c2.valor;
}

struct IndCompValor {
    std::vector<Cromosoma> *poblacion;

    IndCompValor(std::vector<Cromosoma> *poblacion) {
        this->poblacion = poblacion;
    }

    bool operator()(int i1, int i2) {
        return poblacion->at(i1).valor > poblacion->at(i2).valor;
    }
};

static int MAX_GENERACIONES = 1000;
static int TAM_POBL = 100;
static int TAM_ULT = 10;
static double PROB_MUTACION = 0.05;
static double PORC_MUTACION = 0.01;
static double PROB_CRUCE = 0.85;
static double PROB_ELITISMO = 0.1;
static double PROB_1CUARTIL = 0.5;
static double PROB_2CUARTIL = 0.8;
static double PROB_3CUARTIL = 0.95;

/**
 * Resuelve el problema de la mochila 0-1 mediante un algoritmo genetico. No
 * se asegura la solucion optima. Se suele obtener una solucion buena en un
 * tiempo razonable.
 *
 * Coste tiempo: O(nm * MAX_GENERACIONES), n = numero de objetos, m = tamanyo
 *                  de la poblacion.
 * Coste espacio: O(m)
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param M Peso maximo que soporta la mochila.
 * @param solMejor Indica si se coge el objeto o no.
 * @param valorMejor Valor de la mochila con los objetos dados por solMejor.
 */
void mochilaGenetico(std::vector<ObjetoReal> const &objetos, double M,
                     std::vector<bool> &solMejor, double &valorMejor);

#endif //PROBLEMAMOCHILACPP_ALGORITMOS_H
