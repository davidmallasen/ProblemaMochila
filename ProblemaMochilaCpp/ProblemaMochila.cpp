#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <queue>
#include <stdlib.h>
#include <time.h>

//Estructuras auxiliares comunes------------------------------------------------

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

bool operator>(Densidad const &d1, Densidad const &d2) {
    return d1.densidad > d2.densidad;
}

//Voraz-------------------------------------------------------------------------

/**
 * Resuelve el problema de la mochila con objetos fraccionables mediante un
 * algoritmo voraz. Presuponemos que la suma de los pesos de todos los
 * objetos > M.
 *
 * Coste en tiempo: O(n logn), n = numero de objetos.
 * Coste en espacio: O(n).
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param M Peso maximo que soporta la mochila.
 * @param solucion Indica cuanto se debe coger de cada objeto [0, 1].
 * @param valorSol Valor de la mochila con los objetos dados por solucion.
 */
void mochilaVoraz(std::vector<ObjetoReal> const &objetos, double M,
                  std::vector<double> &solucion, double &valorSol) {
    size_t n = objetos.size();

    //Calculamos las densidades de cada objeto
    std::vector<Densidad> d(n);
    for (size_t i = 0; i < n; ++i) {
        d[i].densidad = objetos[i].valor / objetos[i].peso;
        d[i].obj = i;    //Para saber a que objeto corresponde
    }

    //Ordenamos de mayor a menor las densidades
    std::sort(d.begin(), d.end(), std::greater<Densidad>());

    //Cogemos los objetos mientras quepan enteros
    size_t i;
    for (i = 0; i < n && M - objetos[d[i].obj].peso >= 0; ++i) {
        valorSol += objetos[d[i].obj].valor;
        M -= objetos[d[i].obj].peso;
        solucion[d[i].obj] = 1;
    }

    //Si aun no se ha llenado la mochila completamos partiendo el objeto
    if (M > 0) {
        solucion[d[i].obj] = M / objetos[d[i].obj].peso;
        valorSol += objetos[d[i].obj].valor * solucion[d[i].obj];
    }
}

//Programacion dinamica---------------------------------------------------------

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
void mochilaProgDin(std::vector<ObjetoInt> const &objetos, int M,
                    std::vector<bool> &solucion, double &valorSol) {
    size_t n = objetos.size();

    //Creamos e inicializamos a 0 la tabla con la que resolvemos el problema
    std::vector<std::vector<double>> mochila(n + 1, std::vector<double>
            (M + 1, 0));

    //Rellenamos la tabla
    //objetos[i - 1] ya que mochila va de [1..n] y objetos va de [0, n)
    for (size_t i = 1; i <= n; ++i) {
        for (int j = 1; j <= M; ++j) {
            if (objetos[i - 1].peso > j)    //Si no cabe no lo cogemos
                mochila[i][j] = mochila[i - 1][j];
            else    //Si cabe tomamos el maximo entre cogerlo y no cogerlo
                mochila[i][j] =
                        std::max(mochila[i - 1][j],
                                 mochila[i - 1][j - objetos[i - 1].peso] +
                                 objetos[i - 1].valor);
        }
    }
    valorSol = mochila[n][M];

    //Calculamos que objetos hemos cogido
    for (size_t i = n; i >= 1; --i) {
        if (mochila[i][M] == mochila[i - 1][M]) //No cogido el objeto i
            solucion[i - 1] = false;
        else {    //Cogido el objeto i
            solucion[i - 1] = true;
            M -= objetos[i - 1].peso;
        }
    }
}

//Ramificacion y poda-----------------------------------------------------------

struct Nodo {
    std::vector<bool> sol;
    int k;
    double pesoAc, valorAc;
    double valorOpt;    //Prioridad
};

bool operator<(Nodo const &n1, Nodo const &n2) {
    return n1.valorOpt < n2.valorOpt;
}

/**
 * Calcula las estimaciones optimista y pesimista segun el estado en el que
 * nos encontremos. Presupone que los objetos estan ordenados en orden
 * decreciente de su densidad (valor/peso).
 *
 * Coste: O(n-k), n = numero de objetos, k = indice por el que vamos.
 * Coste espacio: O(1).
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param d Vector ordenado en orden creciente de las densidades de los objetos.
 * @param M Peso maximo que soporta la mochila.
 * @param k Indice del objeto por el que vamos.
 * @param pesoAc Peso acumulado en la mochila.
 * @param valorAc Valor acumulado en la mochila.
 * @param opt Cota optimista.
 * @param pes Cota pesimista.
 */
void calculoEst(std::vector<ObjetoReal> const &objetos, std::vector<Densidad>
const &d, double M, int k, double pesoAc, double valorAc, double &opt,
                double &pes) {
    double hueco = M - pesoAc;
    size_t n = objetos.size();
    pes = opt = valorAc;
    k++;
    for (k; k < n && objetos[d[k].obj].peso <= hueco; ++k) {
        //Cogemos el objeto k entero
        hueco -= objetos[d[k].obj].peso;
        opt += objetos[d[k].obj].valor;
        pes += objetos[d[k].obj].valor;
    }
    if (k < n) { //Quedan objetos por probar y objetos[k].peso > hueco
        //Fraccionamos el objeto k (solucion voraz)
        opt += (hueco / objetos[d[k].obj].peso) * objetos[d[k].obj].valor;
        //Extendemos a una solucion en la version 0-1
        k++;
        for (k; k < n && hueco > 0; ++k) {
            if (objetos[d[k].obj].peso <= hueco) {
                hueco -= objetos[d[k].obj].peso;
                pes += objetos[d[k].obj].valor;
            }
        }
    }
}

/**
 * Resuelve el problema de la mochila 0-1 mediante un algoritmo de
 * ramifiacion y poda.
 *
 * Coste: O(n 2^n), n = numero de objetos.
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param M Peso maximo que soporta la mochila.
 * @param solMejor Indica si se coge el objeto o no.
 * @param valorMejor Valor de la mochila con los objetos dados por solMejor.
 */
void mochilaRamPoda(std::vector<ObjetoReal> const &objetos, double M,
                    std::vector<bool> &solMejor, double &valorMejor) {
    Nodo X, Y;
    std::priority_queue<Nodo> C;
    size_t n = objetos.size();
    double pes;

    //Calculamos las densidades de cada objeto
    std::vector<Densidad> d(n);
    for (size_t i = 0; i < n; ++i) {
        d[i].densidad = objetos[i].valor / objetos[i].peso;
        d[i].obj = i;    //Para saber a que objeto corresponde
    }

    //Ordenamos de mayor a menor las densidades
    std::sort(d.begin(), d.end(), std::greater<Densidad>());

    //Generamos la raiz
    Y.k = -1;   //Empezamos en -1 para que vaya de [0, n)
    Y.pesoAc = 0;
    Y.valorAc = 0;
    Y.sol.resize(n, false);
    calculoEst(objetos, d, M, Y.k, Y.pesoAc, Y.valorAc, Y.valorOpt, valorMejor);

    C.push(Y);
    while (!C.empty() && C.top().valorOpt >= valorMejor) {
        Y = C.top();
        C.pop();
        X.k = Y.k + 1;
        X.sol = Y.sol;

        //Si cabe probamos a meter el objeto en la mochila
        if (Y.pesoAc + objetos[d[X.k].obj].peso <= M) {
            X.sol[d[X.k].obj] = true;
            X.pesoAc = Y.pesoAc + objetos[d[X.k].obj].peso;
            X.valorAc = Y.valorAc + objetos[d[X.k].obj].valor;
            X.valorOpt = Y.valorOpt;
            if (X.k == n) {
                solMejor = X.sol;
                valorMejor = X.valorAc;
            } else {
                C.push(X);
            }
        }

        //Probamos a no meter el objeto en la mochila
        calculoEst(objetos, d, M, X.k, Y.pesoAc, Y.valorAc, X.valorOpt, pes);
        if (X.valorOpt >= valorMejor) {
            X.sol[d[X.k].obj] = false;
            X.pesoAc = Y.pesoAc;
            X.valorAc = Y.valorAc;
            if (X.k == n) {
                solMejor = X.sol;
                valorMejor = X.valorAc;
            } else {
                C.push(X);
                valorMejor = std::max(valorMejor, pes);
            }
        }
    }
}

//Algoritmo genetico------------------------------------------------------------

struct Cromosoma {
    std::vector<bool> crom;
    double valor;
};

const int MAX_GENERACIONES = 50;
const int TAM_ULT = 5;
const double PROB_MUTACION = 0.001;
const double PROB_CRUCE = 0.85;

/**
 * Calcula la aptitud de un cromosoma. Tomamos la aptitud de cada cromosoma como
 * el valor de los objetos que tiene. Si sobrepasa el limite de peso se quitan
 * objetos aleatoriamente hasta que el cromosoma sea valido.
 *
 * Coste: O(n), n = numero de objetos.
 *
 * @param c Cromosoma a evaluar.
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param M Peso maximo que soporta la mochila.
 */
void funcAptitud(Cromosoma &c, std::vector<ObjetoReal> const &objetos,
                 double M) {
    double pesoAc, valorAc;
    pesoAc = valorAc = 0;

    //Calculamos lo que tenemos en la mochila
    for (int i = 0; i < c.crom.size(); ++i) {
        if (c.crom[i]) {
            pesoAc += objetos[i].peso;
            valorAc += objetos[i].valor;
        }
    }

    //Si no cabe en la mochila, vamos descartando aleatoriamene hasta que quepa
    size_t r = rand() % c.crom.size();
    while (pesoAc > M) {
        if (c.crom[r]) {
            c.crom[r] = false;
            pesoAc -= objetos[r].peso;
            valorAc -= objetos[r].valor;
        }
        r = (r + 1) % c.crom.size();
    }

    c.valor = valorAc;
}

/**
 * Inicializa la poblacion de forma aleatoria.
 *
 * Coste: O(nm), n = numero de objetos, m = tamanyo de la poblacion.
 *
 * @param poblacion Conjunto de cromosomas.
 * @param objetos Conjunto de objetos que tenemos disponibles.
 */
void iniPoblacion(std::vector<Cromosoma> &poblacion,
                  std::vector<ObjetoReal> const &objetos) {
    for (Cromosoma &c : poblacion)
        for (int i = 0; i < objetos.size(); ++i)
            c.crom.push_back(rand() % 2);
}

/**
 * Suponemos que seleccionados ya esta creado con el mismo tamanyo que poblacion
 * Suponemos que los cromosomas ya tienen su valor calculado
 * Usamos elitismo
 * @TODO
 * Coste: n², @TODO reducir a n logn ordenando y luego escogiendo segun
 * cierta probabilidad
 * @param poblacion
 * @param seleccionados
 */
void funcSeleccion(std::vector<Cromosoma> const &poblacion,
                   std::vector<Cromosoma> &seleccionados) {
    //Calculamos la suma de todos los valores
    double valorAc = 0;
    for (Cromosoma const &c : poblacion)
        valorAc += c.valor;

    //Elegimos el elemento cuyo valor acumulado supere un numero aleatorio
    // entre 0 y la suma de todos los valores
    for (int i = 0; i < poblacion.size(); ++i) {
        double r = valorAc * ((double) rand() / (double) RAND_MAX);

        double valorAux = 0;
        int j;
        for (j = 0; valorAux < r; ++j)
            valorAux += poblacion[j].valor;

        if (j > 0) j--;
        seleccionados[i] = poblacion[j];
    }
}

/**
 * @TODO
 * @param seleccionados
 */
void funcCruce(std::vector<Cromosoma> &seleccionados, std::vector<ObjetoReal>
const &objetos, double M) {
    for (size_t i = 1; i < seleccionados.size(); i += 2) {
        double r = (double) rand() / (double) RAND_MAX;
        if (r <= PROB_CRUCE) {
            size_t k = rand() % seleccionados[i].crom.size();
            bool aux;
            for (int j = 0; j < k; ++j) {
                aux = seleccionados[i].crom[j];
                seleccionados[i].crom[j] = seleccionados[i - 1].crom[j];
                seleccionados[i - 1].crom[j] = aux;
            }
        }
    }
}

/**
 * Muta de 1 a 3 elementos de cada cromosoma con probabilidad PROB_MUTACION
 * @TODO
 * @param seleccionados
 */
void funcMutacion(std::vector<Cromosoma> &seleccionados, std::vector<ObjetoReal>
const &objetos, double M) {
    for (Cromosoma &c : seleccionados) {
        double r = (double) rand() / (double) RAND_MAX;
        if (r <= PROB_MUTACION) {
            int numMut = (rand() % 3) + 1;
            for (int i = 0; i < numMut; ++i) {
                size_t j = rand() % c.crom.size();
                c.crom[j] = !c.crom[j];
            }
        }
    }
}

/**
 * Comprueba si se cumple alguna condicion de terminacion. Si se ha superado
 * el maximo de generaciones o si no se ha mejorado ni la media ni la mejor
 * solucion en las ultimas TAM_ULT generaciones devuelve true.
 *
 * Coste: O(1).
 *
 * @param ultMedias Vector que contiene las ultimas TAM_ULT mejores medias.
 * @param ultMejores Vector que contiene los ultimos TAM_ULT mejores valores.
 * @param generacionAct Generacion por la que vamos.
 * @return True si se cumple alguna condicion de terminacion, false en caso
 * contrario.
 */
bool condTerminacion(std::vector<double> &ultMedias,
                     std::vector<double> &ultMejores, int generacionAct) {
    if (generacionAct < TAM_ULT)
        return false;

    if (generacionAct >= MAX_GENERACIONES)
        return true;

    bool mejora = false;
    double mediaAnt = ultMedias[0], mejorAnt = ultMejores[0];

    for (int i = 1; i < TAM_ULT && !mejora; ++i) {
        if (ultMedias[i] > mediaAnt || ultMejores[i] > mejorAnt)
            mejora = true;
    }

    return !mejora;
}

/**
 * @TODO
 * @param poblacion
 * @param ultMedias
 * @param ultMejores
 * @param solMejor
 * @param valorMejor
 */
void calcMejores(std::vector<Cromosoma> const &poblacion, std::vector<double>
&ultMedias, std::vector<double> &ultMejores, std::vector<bool> &solMejor,
                 double &valorMejor) {
    for (int i = TAM_ULT - 1; i > 0; --i) {
        ultMedias[i] = ultMedias[i - 1];
        ultMejores[i] = ultMejores[i - 1];
    }

    double sumaVal = 0, mejor = -1;
    std::vector<bool> solMejorAux(solMejor.size());
    for (Cromosoma const &c : poblacion) {
        sumaVal += c.valor;
        if (c.valor > mejor) {
            mejor = c.valor;
            solMejorAux = c.crom;
        }
    }

    ultMejores[0] = mejor;
    ultMedias[0] = sumaVal / poblacion.size();
    if (mejor > valorMejor) {
        valorMejor = mejor;
        solMejor = solMejorAux;
    }
}

void mochilaGenetico(std::vector<ObjetoReal> const &objetos, double M,
                     std::vector<bool> &solMejor, double &valorMejor) {
    std::vector<Cromosoma> poblacion(objetos.size()); //@TODO hacer constante
    std::vector<Cromosoma> seleccionados(objetos.size());
    std::vector<double> ultMedias(TAM_ULT);
    std::vector<double> ultMejores(TAM_ULT);

    valorMejor = -1;
    for (Cromosoma &c : seleccionados)
        c.crom.resize(objetos.size());      //@TODO usar el constante de arriba

    iniPoblacion(poblacion, objetos);
    for (Cromosoma &c : poblacion)
        funcAptitud(c, objetos, M);
    calcMejores(poblacion, ultMedias, ultMejores, solMejor, valorMejor);
    for (int generacionAct = 0; !condTerminacion(ultMedias, ultMejores,
                                                 generacionAct); generacionAct++) {
        funcSeleccion(poblacion, seleccionados);
        funcCruce(seleccionados, objetos, M);
        funcMutacion(seleccionados, objetos, M);
        poblacion = seleccionados;
        for (Cromosoma &c : poblacion)
            funcAptitud(c, objetos, M);
        calcMejores(poblacion, ultMedias, ultMejores, solMejor, valorMejor);
    }
}

//Main--------------------------------------------------------------------------

int main() {

    srand(time(NULL));

    size_t n = 5;
    std::vector<ObjetoReal> objetos(n);
    for (int i = 0; i < n; ++i)
        objetos[i].peso = (i + 1) * 10;

    objetos[0].valor = 20;
    objetos[1].valor = 30;
    objetos[2].valor = 66;
    objetos[3].valor = 40;
    objetos[4].valor = 70;

    std::vector<bool> solucion(n);
    double valorSol;

    auto t1 = std::chrono::steady_clock::now();

    mochilaGenetico(objetos, 100, solucion, valorSol);

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