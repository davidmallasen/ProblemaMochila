#include "Algoritmos.h"

#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>

void mochilaVoraz(std::vector<ObjetoReal> const &objetos, double M,
                  std::vector<double> &solucion, double &valorSol) {
    const size_t n = objetos.size();

    //Calculamos las densidades de cada objeto
    std::vector<Densidad> d(n);
    for (size_t i = 0; i < n; ++i) {
        d[i].densidad = objetos[i].valor / objetos[i].peso;
        d[i].obj = i;    //Para saber a que objeto corresponde
    }

    //Ordenamos de mayor a menor las densidades
    std::sort(d.begin(), d.end(), std::greater<Densidad>());

    //Cogemos los objetos mientras quepan enteros
    valorSol = 0;
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

void mochilaProgDin(std::vector<ObjetoInt> const &objetos, unsigned int M,
                    std::vector<bool> &solucion, double &valorSol) {
    const size_t n = objetos.size();

    //Creamos e inicializamos a 0 la tabla con la que resolvemos el problema
    std::vector<std::vector<double>> mochila(n + 1,
                                             std::vector<double>(M + 1, 0));

    //Rellenamos la tabla
    //objetos[i - 1] ya que la tabla va de [1..n] y objetos va de [0, n)
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

/**
 * Calcula las estimaciones optimista y pesimista segun el estado en el que
 * nos encontremos. Presupone que los objetos estan ordenados en orden
 * decreciente de su densidad (valor/peso).
 *
 * Coste: O(n-k), n = numero de objetos, k = indice por el que vamos.
 *
 * @param objetos Conjunto de objetos que tenemos disponibles.
 * @param d Vector ordenado en orden decreciente de las densidades.
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
    const size_t n = objetos.size();
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

void mochilaRamPoda(std::vector<ObjetoReal> const &objetos, double M,
                    std::vector<bool> &solMejor, double &valorMejor,
                    long long int &nodosVisitados) {
    Nodo X, Y;
    std::priority_queue<Nodo> C;
    const size_t n = objetos.size();
    double pes;
    nodosVisitados = 0;

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
    calculoEst(objetos, d, M, Y.k, Y.pesoAc, Y.valorAc, Y.valorOpt,
               valorMejor);

    C.push(Y);
    while (!C.empty() && C.top().valorOpt >= valorMejor) {
        Y = C.top();
        nodosVisitados++;
        C.pop();
        X.k = Y.k + 1;
        X.sol = Y.sol;

        //Si cabe probamos a meter el objeto en la mochila
        if (Y.pesoAc + objetos[d[X.k].obj].peso <= M) {
            X.sol[d[X.k].obj] = true;
            X.pesoAc = Y.pesoAc + objetos[d[X.k].obj].peso;
            X.valorAc = Y.valorAc + objetos[d[X.k].obj].valor;
            X.valorOpt = Y.valorOpt;
            if (X.k == n - 1) {
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
            if (X.k == n - 1) {
                solMejor = X.sol;
                valorMejor = X.valorAc;
            } else {
                C.push(X);
                valorMejor = std::max(valorMejor, pes);
            }
        }
    }
}

/**
 * Calcula la aptitud de un cromosoma. Tomamos la aptitud de cada cromosoma
 * como el valor de los objetos que tiene. Si sobrepasa el limite de peso
 * se quitan objetos aleatoriamente hasta que el cromosoma sea valido.
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

    //Si no cabe en la mochila, descartamos aleatoriamente hasta que quepa
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
 * Coste: O(mn), n = numero de objetos, m = tamanyo de la poblacion.
 *
 * @param poblacion Conjunto de cromosomas.
 * @param nObjetos Numero de objetos que tenemos disponibles.
 */
void iniPoblacion(std::vector<Cromosoma> &poblacion,
                  size_t nObjetos) {
    for (Cromosoma &c : poblacion)
        for (int i = 0; i < nObjetos; ++i)
            c.crom.push_back(rand() % 2);
}

/**
 * Selecciona los individuos que formaran parte de la siguiente generacion.
 * Escoge un porcentaje (PROB_ELITISMO) de los mejores cromosomas de la
 * generacion anterior y los mantiene. El resto se seleccionan segun su
 * aptitud dando mas posibilidades (PROB_xCUARTIL) a los mejores cromosomas.
 *
 * Coste: O(n logn), n = numero de objetos.
 *
 * @param poblacion Conjunto de cromosomas con las aptitudes ya calculadas.
 * @param seleccionados Vector donde almacenaremos los individuos
 * seleccionados a formar parte de la siguiente generacion. Presuponemos que
 * ya esta creado con el mismo tamanyo que poblacion.
 */
void funcSeleccion(std::vector<Cromosoma> &poblacion,
                   std::vector<Cromosoma> &seleccionados) {
    std::vector<int> ind(poblacion.size());
    for (int i = 0; i < ind.size(); ++i)
        ind[i] = i;
    IndCompValor comp(&poblacion);
    const size_t n = seleccionados.size();

    //Ordena la poblacion usando una estructura de indices auxiliar
    sort(ind.begin(), ind.end(), comp);

    //Selecciona primero a los mejores para preservarlos (elitismo)
    int nElit = (int) ceil(PROB_ELITISMO * n);
    int i = 0;
    for (i; i < nElit; ++i)
        seleccionados[i] = poblacion[ind[i]];

    //Completa seleccionando con mayor probabilidad los cromosomas mas aptos
    for (i; i < n; ++i) {
        double r = (double) rand() / (double) RAND_MAX; //Entre 0 y 1
        size_t j = rand() % n / 4;  //Posicion inferior al primer cuartil
        if (r > PROB_3CUARTIL) {    //Desplazamos la posicion a entre el
            j += (3 * n) / 4;        // tercer cuartil y el final
        } else if (r > PROB_2CUARTIL) { //Entre el 2 y el 3
            j += n / 2;
        } else if (r > PROB_1CUARTIL) { //Entre el 1 y el 2
            j += n / 4;
        }
        seleccionados[i] = poblacion[ind[j]];
    }
}

/**
 * Cruza los elementos de la poblacion usando cruce simple. Solo cruza un
 * porcentage de los elementos (PROB_CRUCE), el resto no los modifica.
 *
 * Coste: O(nm), n = numero de objetos, m = tamanyo de la poblacion.
 *
 * @param seleccionados Cromosomas seleccionados para cruzarse.
 */
void funcCruce(std::vector<Cromosoma> &seleccionados) {
    //Cogemos los elementos de dos en dos
    for (size_t i = 1; i < seleccionados.size(); i += 2) {

        double r = (double) rand() / (double) RAND_MAX;
        if (r <= PROB_CRUCE) {  //Si se deben cruzar

            //Elegimos el punto de cruce simple
            size_t k = rand() % seleccionados[i].crom.size();

            for (int j = 0; j < k; ++j)   //Cruzamos el intervalo
                std::swap(seleccionados[i].crom[j],
                          seleccionados[i - 1].crom[j]);
        }
    }
}

/**
 * Muta un porcentaje PORC_MUTACION de los elementos de cada cromosoma con
 * probabilidad PROB_MUTACION.
 *
 * Coste: O(n), n = numero de objetos.
 *
 * @param seleccionados Cromosomas seleccionados para mutar.
 */
void funcMutacion(std::vector<Cromosoma> &seleccionados) {
    for (Cromosoma &c : seleccionados) {
        double r = (double) rand() / (double) RAND_MAX;
        if (r <= PROB_MUTACION) { //Si se debe mutar

            int numMut = rand() % (int) std::ceil(c.crom.size() *
                                                  PORC_MUTACION);
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
 * @param ultMedias Contiene las ultimas TAM_ULT mejores medias.
 * @param ultMejores Contiene los ultimos TAM_ULT mejores valores.
 * @param generacionAct Generacion por la que vamos.
 * @return True si se cumple alguna condicion de terminacion, false en caso
 * contrario.
 */
bool condTerminacion(std::deque<double> &ultMedias,
                     std::deque<double> &ultMejores, int generacionAct) {
    //Nunca terminamos en las primeras generaciones
    if (generacionAct < TAM_ULT)
        return false;

    //Si se ha superado el maximo de generaciones
    if (generacionAct >= MAX_GENERACIONES)
        return true;

    //Comprobamos si se ha mejorado la media o el valor mejor ultimamente
    bool mejora = false;
    for (int i = 0; i < TAM_ULT - 1 && !mejora; ++i) {
        if (ultMedias[i] > ultMedias.back() ||
            ultMejores[i] > ultMejores.back())
            mejora = true;
    }

    return !mejora;
}

/**
 * Calcula el mejor valor, junto con su solucion, y la media de esta
 * generacion. Actualiza los valores de las ultimas generaciones y los
 * valores mejores hasta el momento.
 *
 * Coste: O(n), n = numero de objetos.
 *
 * @param poblacion Conjunto de cromosomas con las aptitudes ya calculadas.
 * @param ultMedias Contiene las TAM_ULT ultimas medias.
 * @param ultMejores Contiene los TAM_ULT ultimos mejores valores.
 * @param solMejor Mejor solucion hasta el momento.
 * @param valorMejor Valor de la mejor solucion hasta el momento.
 */
void calcMejores(std::vector<Cromosoma> const &poblacion, std::deque<double>
&ultMedias, std::deque<double> &ultMejores, std::vector<bool> &solMejor,
                 double &valorMejor) {

    ultMedias.pop_back();
    ultMejores.pop_back();

    //Calculamos los parametros de esta generacion
    double sumaVal = 0, mejor = -1;
    std::vector<bool> solMejorAux(solMejor.size());
    for (Cromosoma const &c : poblacion) {
        sumaVal += c.valor;
        if (c.valor > mejor) {
            mejor = c.valor;
            solMejorAux = c.crom;
        }
    }

    //Actualizamos
    ultMejores.push_front(mejor);
    ultMedias.push_front(sumaVal / poblacion.size());
    if (mejor > valorMejor) {
        valorMejor = mejor;
        solMejor = solMejorAux;
    }
}

void mochilaGenetico(std::vector<ObjetoReal> const &objetos, double M,
                     std::vector<bool> &solMejor, double &valorMejor) {

    //Inicializamos las estructuras
    std::vector<Cromosoma> poblacion(TAM_POBL);
    std::vector<Cromosoma> seleccionados(TAM_POBL);
    std::deque<double> ultMedias(TAM_ULT);
    std::deque<double> ultMejores(TAM_ULT);
    valorMejor = -1;
    for (Cromosoma &c : seleccionados)
        c.crom.resize(objetos.size());

    //Generamos la poblacion inicial y calculamos sus aptitudes
    iniPoblacion(poblacion, objetos.size());
    for (Cromosoma &c : poblacion)
        funcAptitud(c, objetos, M);
    calcMejores(poblacion, ultMedias, ultMejores, solMejor, valorMejor);

    //Mientras que no se cumpla la condicion de terminacion vamos
    // evolucionando las sucesivas generaciones
    for (int generacionAct = 0;
         !condTerminacion(ultMedias, ultMejores, generacionAct);
         generacionAct++) {

        funcSeleccion(poblacion, seleccionados);
        funcCruce(seleccionados);
        funcMutacion(seleccionados);
        poblacion = seleccionados;
        for (Cromosoma &c : poblacion)
            funcAptitud(c, objetos, M);
        calcMejores(poblacion, ultMedias, ultMejores, solMejor, valorMejor);
    }
}