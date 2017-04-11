#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <queue>

//Estructuras auxiliares -------------------------------------------------------

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
    size_t objeto;
};

bool operator>(Densidad const &d1, Densidad const &d2) {
    return d1.densidad > d2.densidad;
}

struct Nodo {
    std::vector<bool> sol;
    int k;
    double pesoAc, valorAc;
    double valorOpt;    //Prioridad
};

bool operator<(Nodo const &n1, Nodo const &n2) {
    return n1.valorOpt < n2.valorOpt;
}

//Algoritmos -------------------------------------------------------------------

/**
 * Resuelve el problema de la mochila con objetos fraccionables mediante un
 * algoritmo voraz.
 *
 * Coste en tiempo: O(n logn), n = numero de objetos
 * Coste en espacio: O(n)
 *
 * @todo CAMBIAR COSTE EN ESPACIO A O(1) REDEFINIENDO < DE OBJETOREAL!!!!!!!!!!!
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param pesoMochila Peso maximo que soporta la mochila.
 * @param solucion Indica cuanto se debe coger de cada objeto [0, 1].
 * @param valorSol Valor de la mochila con los objetos dados por solucion.
 */
void mochilaVoraz(std::vector<ObjetoReal> const &objetos, double pesoMochila,
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
    //@TODO comprobar que quedan objetos por mirar
    if (pesoMochila > 0) {
        solucion[d[i].objeto] = pesoMochila / objetos[d[i].objeto].peso;
        valorSol += objetos[d[i].objeto].valor * solucion[d[i].objeto];
    }
}

/**
 * Resuelve el problema de la mochila 0-1 mediante un algoritmo de
 * programacion dinamica. El peso de cada objeto y el peso maximo de la
 * mochila deben ser enteros positivos.
 *
 * Coste: O(nM) en tiempo y espacio, n = numero de objetos, M = pesoMochila
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param pesoMochila Peso maximo que soporta la mochila.
 * @param solucion Indica si se coge el objeto o no.
 * @param valorSol Valor de la mochila con los objetos dados por solucion.
 */
void mochilaProgDin(std::vector<ObjetoInt> const &objetos, int pesoMochila,
                    std::vector<bool> &solucion, double &valorSol) {
    size_t n = objetos.size();

    //Creamos e inicializamos a 0 la tabla con la que resolvemos el problema
    std::vector<std::vector<double>> mochila(n + 1, std::vector<double>
            (pesoMochila + 1, 0));

    //Rellenamos la tabla
    //objetos[i - 1] en lugar de i ya que mochila va de [1..n] y objetos va de
    // [0, n)
    for (size_t i = 1; i <= n; ++i) {
        for (int j = 1; j <= pesoMochila; ++j) {
            if (objetos[i - 1].peso > j)    //Si no cabe no lo cogemos
                mochila[i][j] = mochila[i - 1][j];
            else    //Si cabe tomamos el maximo entre cogerlo y no cogerlo
                mochila[i][j] = std::max(mochila[i - 1][j],
                                         mochila[i - 1][j -
                                                        objetos[i - 1].peso] +
                                         objetos[i - 1].valor);
        }
    }
    valorSol = mochila[n][pesoMochila];

    //Calculamos que objetos hemos cogido
    int resto = pesoMochila;
    for (size_t i = n; i >= 1; --i) {
        if (mochila[i][resto] ==
            mochila[i - 1][resto])    //No cogido el objeto i
            solucion[i - 1] = false;
        else {    //Cogido el objeto i
            solucion[i - 1] = true;
            resto -= objetos[i - 1].peso;
        }
    }
}

/**
 * Calcula las estimaciones optimista y pesimista
 *
 * Coste: @TODO
 *
 * @param objetos
 * @param pesoMochila
 * @param k
 * @param pesoAc
 * @param valorAc
 * @param opt
 * @param pes
 */
void calculoEst(std::vector<ObjetoReal> const &objetos, double pesoMochila,
                int k, double pesoAc, double valorAc, double &opt,
                double &pes) {
    double hueco = pesoMochila - pesoAc;
    int n = objetos.size();
    pes = opt = valorAc;
    k++;
    while (k < n &&
           objetos[k].peso <= hueco) {  //Podemos coger el objeto k entero
        hueco -= objetos[k].peso;
        opt += objetos[k].valor;
        pes += objetos[k].valor;
        k++;  //@TODO cambiar a for??
    }
    if(k < n) { //Quedan objetos por probar y objetos[k].peso > hueco
        //Fraccionamos el objeto k (solucion voraz)
        opt += (hueco / objetos[k].peso) * objetos[k].valor;
        //Extendemos a una solucion en la version 0-1
        k++;
        while(k < n && hueco > 0) {
            if(objetos[k].peso <= hueco) {
                hueco -=  objetos[k].peso;
                pes += objetos[k].valor;
            }
            k++; //@TODO cambiar a for
        }
    }
}

/**
 * Resuelve el problema de la mochila 0-1 mediante un algoritmo de
 * ramifiacion y poda.
 *
 * Coste: @TODO
 *
 * @TODO Anyadir los comentarios de las transparencias
 * @TODO Se presupone que los objetos estan ordenados segun su densidad >
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param pesoMochila Peso maximo que soporta la mochila.
 * @param solMejor Indica si se coge el objeto o no.
 * @param valorMejor Valor de la mochila con los objetos dados por solMejor.
 */
void mochilaRamPoda(std::vector<ObjetoReal> const &objetos, double pesoMochila,
                    std::vector<bool> &solMejor, double &valorMejor) {
    Nodo X, Y;
    std::priority_queue<Nodo> C;
    size_t n = objetos.size();
    Y.k = -1;   //Empezamos en -1 para que vaya de [0, n) @TODO
    Y.pesoAc = 0;
    Y.valorAc = 0;
    calculoEst();

    C.push(Y);
    while (!C.empty() && C.top().valorOpt >= valorMejor) {
        Y = C.top();
        C.pop();
        X.k = Y.k + 1;
        X.sol = Y.sol;

        if (Y.pesoAc + objetos[X.k].peso <= pesoMochila) {
            X.sol[X.k] = true;
            X.pesoAc = Y.pesoAc + objetos[X.k].peso;
            X.valorAc = Y.valorAc + objetos[X.k].valor;
            X.valorOpt = Y.valorOpt;
            if (X.k == n) {
                solMejor = X.sol;
                valorMejor = X.valorAc;
            } else {
                C.push(X);
            }
        }

        calculoEst();
        if (X.valorOpt >= valorMejor) {
            X.sol[X.k] = false;
            X.pesoAc = Y.pesoAc;
            X.valorAc = Y.valorAc;
            if (X.k == n) {
                solMejor = X.sol;
                valorMejor = X.valorAc;
            } else {
                C.push(X);
                valorMejor = std::max(valorMejor, 0/*pes */);
            }
        }
    }
}

//Main -------------------------------------------------------------------------

int main() {
    size_t n = 5;
    std::vector<ObjetoInt> objetos(n);
    for (int i = 0; i < n; ++i)
        objetos[i].peso = (i + 1) * 10;

    objetos[0].valor = 20;
    objetos[1].valor = 30;
    objetos[2].valor = 66;
    objetos[3].valor = 40;
    objetos[4].valor = 60;

    std::vector<bool> solucion(n);
    double valorSol;

    auto t1 = std::chrono::steady_clock::now();

    mochilaProgDin(objetos, 100, solucion, valorSol);

    auto t2 = std::chrono::steady_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
            t2 - t1);

    std::cout << "ValorSol: " << valorSol << '\n';
    for (int i = 0; i < n; ++i)
        std::cout << "Peso de " << i << ": " << solucion[i] << '\n';

    std::cout << "El algoritmo ha tardado " << time_span.count() << " "
            "segundos.\n";

    return 0;
}