#include "CasosPrueba.h"

#include <fstream>
#include <chrono>

void generaCasoPruebaMochilaReal(std::string nombreFichero, int nObjetos,
                                 double maxPesoObjeto, double maxValorObjeto,
                                 double M) {
    double peso, valor;
    std::ofstream out;
    out.open(nombreFichero);

    out << M << ' ' << nObjetos << '\n';
    for (int i = 0; i < nObjetos; ++i) {
        peso = ((double) rand() / (double) RAND_MAX) * maxPesoObjeto;
        valor = ((double) rand() / (double) RAND_MAX) * maxValorObjeto;

        out << peso << ' ' << valor << '\n';
    }

    out.close();
}

void generaCasoPruebaMochilaInt(std::string nombreFichero, int nObjetos,
                                int maxPesoObjeto, double maxValorObjeto,
                                int M) {
    double valor;
    int peso;
    std::ofstream out;
    out.open(nombreFichero);

    out << M << ' ' << nObjetos << '\n';
    for (int i = 0; i < nObjetos; ++i) {
        peso = rand() % maxPesoObjeto;
        valor = ((double) rand() / (double) RAND_MAX) * maxValorObjeto;

        out << peso << ' ' << valor << '\n';
    }

    out.close();
}

void leeCasoPruebaMochilaReal(std::string nombreFichero, double &M,
                              std::vector<ObjetoReal> &objetos) {
    std::ifstream in;
    in.open(nombreFichero);
    if (!in.is_open()) {
        std::cout << "ERROR. No se ha podido abrir el fichero.\n";
        return;
    }

    int nObjetos;
    in >> M >> nObjetos;
    objetos.resize(nObjetos);

    for (int i = 0; i < nObjetos; ++i) {
        in >> objetos[i].peso >> objetos[i].valor;
    }

    in.close();
}

void leeCasoPruebaMochilaInt(std::string nombreFichero, int &M,
                             std::vector<ObjetoInt> &objetos) {
    std::ifstream in;
    in.open(nombreFichero);
    if (!in.is_open()) {
        std::cout << "ERROR. No se ha podido abrir el fichero.\n";
        return;
    }

    int nObjetos;
    in >> M >> nObjetos;
    objetos.resize(nObjetos);

    for (int i = 0; i < nObjetos; ++i) {
        in >> objetos[i].peso >> objetos[i].valor;
    }

    in.close();
}

void casoPruebaVoraz() {
    std::cout << "-----CASO PRUEBA VORAZ-----\n";
/*
    casoPruebaVoraz("Caso Prueba 1000A", "CasoPruebaReal1000A.txt",
                    N_IT_CASO_PEQ);
    casoPruebaVoraz("Caso Prueba 1000B", "CasoPruebaReal1000B.txt",
                    N_IT_CASO_PEQ);
    casoPruebaVoraz("Caso Prueba 1000C", "CasoPruebaReal1000C.txt",
                    N_IT_CASO_PEQ);

    casoPruebaVoraz("Caso Prueba 100000A", "CasoPruebaReal100000A.txt",
                    N_IT_CASO_PEQ);
    casoPruebaVoraz("Caso Prueba 100000B", "CasoPruebaReal100000B.txt",
                    N_IT_CASO_PEQ);
    casoPruebaVoraz("Caso Prueba 100000C", "CasoPruebaReal100000C.txt",
                    N_IT_CASO_PEQ);

    casoPruebaVoraz("Caso Prueba 1000000A", "CasoPruebaReal1000000A.txt",
                    N_IT_CASO_PEQ);
    casoPruebaVoraz("Caso Prueba 1000000B", "CasoPruebaReal1000000B.txt",
                    N_IT_CASO_PEQ);
    casoPruebaVoraz("Caso Prueba 1000000C", "CasoPruebaReal1000000C.txt",
                    N_IT_CASO_PEQ);
*/

    casoPruebaVoraz("Caso Prueba 10000000A", "CasoPruebaReal10000000A.txt",
                    N_IT_CASO_GR);
    casoPruebaVoraz("Caso Prueba 10000000B", "CasoPruebaReal10000000B.txt",
                    N_IT_CASO_GR);
    casoPruebaVoraz("Caso Prueba 10000000C", "CasoPruebaReal10000000C.txt",
                    N_IT_CASO_GR);

    casoPruebaVoraz("Caso Prueba 100000000A", "CasoPruebaReal100000000A.txt",
                    N_IT_CASO_GR);
    casoPruebaVoraz("Caso Prueba 100000000B", "CasoPruebaReal100000000B.txt",
                    N_IT_CASO_GR);
    casoPruebaVoraz("Caso Prueba 100000000C", "CasoPruebaReal100000000C.txt",
                    N_IT_CASO_GR);
}

void casoPruebaProgDin() {
    std::cout << "-----CASO PRUEBA PROG DIN-----\n";
/*
    casoPruebaProgDin("Caso Prueba 1000A", "CasoPruebaInt1000A.txt",
                      N_IT_CASO_PEQ);
    casoPruebaProgDin("Caso Prueba 1000B", "CasoPruebaInt1000B.txt",
                      N_IT_CASO_PEQ);
    casoPruebaProgDin("Caso Prueba 1000C", "CasoPruebaInt1000C.txt",
                      N_IT_CASO_PEQ);

    casoPruebaProgDin("Caso Prueba 100000A", "CasoPruebaInt100000A.txt",
                      N_IT_CASO_PEQ);
    casoPruebaProgDin("Caso Prueba 100000B", "CasoPruebaInt100000B.txt",
                      N_IT_CASO_PEQ);
    casoPruebaProgDin("Caso Prueba 100000C", "CasoPruebaInt100000C.txt",
                      N_IT_CASO_PEQ);
*/
    //SOLO CON 1 MILLON DE OBJETOS Y CON EL TAMANYO DE LA MOCHILA A 1000
    casoPruebaProgDin("Caso Prueba 1000000A", "CasoPruebaInt1000000A.txt",
                      N_IT_CASO_GR);
    casoPruebaProgDin("Caso Prueba 1000000B", "CasoPruebaInt1000000B.txt",
                      N_IT_CASO_GR);
    casoPruebaProgDin("Caso Prueba 1000000C", "CasoPruebaInt1000000C.txt",
                      N_IT_CASO_GR);
/*
    casoPruebaProgDin("Caso Prueba 10000000A", "CasoPruebaInt10000000A.txt",
                      N_IT_CASO_GR);
    casoPruebaProgDin("Caso Prueba 10000000B", "CasoPruebaInt10000000B.txt",
                      N_IT_CASO_GR);
    casoPruebaProgDin("Caso Prueba 10000000C", "CasoPruebaInt10000000C.txt",
                      N_IT_CASO_GR);

    casoPruebaProgDin("Caso Prueba 100000000A",
                      "CasoPruebaInt100000000A.txt",
                      N_IT_CASO_GR);*/
}

void casoPruebaRamPoda() {
    std::cout << "-----CASO PRUEBA RAM PODA-----\n";
/*
    casoPruebaRamPoda("Caso Prueba Real 1000A", "CasoPruebaReal1000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Real 1000B", "CasoPruebaReal1000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Real 1000C", "CasoPruebaReal1000C.txt",
                      N_IT_CASO_GR);

    casoPruebaRamPoda("Caso Prueba Real 100000A",
                      "CasoPruebaReal100000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Real 100000B",
                      "CasoPruebaReal100000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Real 100000C",
                      "CasoPruebaReal100000C.txt",
                      N_IT_CASO_GR);

    casoPruebaRamPoda("Caso Prueba Int 1000A", "CasoPruebaInt1000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Int 1000B", "CasoPruebaInt1000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Int 1000C", "CasoPruebaInt1000C.txt",
                      N_IT_CASO_GR);

    casoPruebaRamPoda("Caso Prueba Int 100000A", "CasoPruebaInt100000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Int 100000B", "CasoPruebaInt100000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba Int 100000C", "CasoPruebaInt100000C.txt",
                      N_IT_CASO_GR);
*/
//    std::cout << "==========CASO REAL===================\n";
/*
    casoPruebaRamPoda("Caso Prueba 1000000A", "CasoPruebaReal1000000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 1000000B", "CasoPruebaReal1000000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 1000000C", "CasoPruebaReal1000000C.txt",
                      N_IT_CASO_GR);

    casoPruebaRamPoda("Caso Prueba 10000000A", "CasoPruebaReal10000000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 10000000B", "CasoPruebaReal10000000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 10000000C", "CasoPruebaReal10000000C.txt",
                      N_IT_CASO_GR);

    casoPruebaRamPoda("Caso Prueba 100000000A",
                      "CasoPruebaReal100000000A.txt",
                      N_IT_CASO_GR);*/

    std::cout << "==========CASO INT===================\n";

    casoPruebaRamPoda("Caso Prueba 1000000A", "CasoPruebaInt1000000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 1000000B", "CasoPruebaInt1000000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 1000000C", "CasoPruebaInt1000000C.txt",
                      N_IT_CASO_GR);
/*
    casoPruebaRamPoda("Caso Prueba 10000000A", "CasoPruebaInt10000000A.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 10000000B", "CasoPruebaInt10000000B.txt",
                      N_IT_CASO_GR);
    casoPruebaRamPoda("Caso Prueba 10000000C", "CasoPruebaInt10000000C.txt",
                      N_IT_CASO_GR);

    casoPruebaRamPoda("Caso Prueba 100000000A",
                      "CasoPruebaInt100000000A.txt",
                      N_IT_CASO_GR);*/
}

void casoPruebaGenetico() {
    std::cout << "-----CASO PRUEBA GENETICO-----\n";
/*
    casoPruebaGenetico("Caso Prueba 1000A", "CasoPruebaReal1000A.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 1000B", "CasoPruebaReal1000B.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 1000C", "CasoPruebaReal1000C.txt",
                       N_IT_GEN);
*/
    std::cout << "=======PARAM A===========\n";
    casoPruebaGenetico("Caso Prueba 100000A", "CasoPruebaReal100000A.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 100000B", "CasoPruebaReal100000B.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 100000C", "CasoPruebaReal100000C.txt",
                       N_IT_GEN);
/*
    casoPruebaGenetico("Caso Prueba 1000000A", "CasoPruebaReal1000000A.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 1000000B", "CasoPruebaReal1000000B.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 1000000C", "CasoPruebaReal1000000C.txt",
                       N_IT_GEN);*/

    MAX_GENERACIONES = 2000;
    TAM_ULT = 30;
    PROB_CRUCE = 0.95;
    PROB_ELITISMO = 0.05;

    std::cout << "=======PARAM B===========\n";
    casoPruebaGenetico("Caso Prueba 100000A", "CasoPruebaReal100000A.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 100000B", "CasoPruebaReal100000B.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 100000C", "CasoPruebaReal100000C.txt",
                       N_IT_GEN);
/*
    casoPruebaGenetico("Caso Prueba 1000000A", "CasoPruebaReal1000000A.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 1000000B", "CasoPruebaReal1000000B.txt",
                       N_IT_GEN);
    casoPruebaGenetico("Caso Prueba 1000000C", "CasoPruebaReal1000000C.txt",
                       N_IT_GEN);*/
}

/**
 * Ejecuta el algoritmo voraz sobre el caso de prueba casoPrueba contenido en
 * el fichero nombreFichero un numero de veces dado por nIt. Muestra por
 * pantalla el valor de la solucion obtenida por el algoritmo y el tiempo
 * que ha tardado.
 *
 * @param casoPrueba Nombre del caso de prueba.
 * @param nombreFichero Fichero de donde cargar el caso de prueba.
 * @param nIt Numero de veces a ejecutar el algoritmo.
 */
void casoPruebaVoraz(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt) {
    std::vector<ObjetoReal> objetos;
    double M;
    double valorSol;

    std::cout << casoPrueba << "\n";
    for (int i = 1; i <= nIt; ++i) {

        leeCasoPruebaMochilaReal(nombreFichero, M, objetos);

        std::vector<double> solucion(objetos.size());

        auto t1 = std::chrono::steady_clock::now();

        mochilaVoraz(objetos, M, solucion, valorSol);

        auto t2 = std::chrono::steady_clock::now();
        auto time_span =
                std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

        std::cout << "Vuelta: " << i << " ValorSol: " << valorSol << '\n';
        std::cout << "El algoritmo ha tardado " << time_span.count()
                  << " segundos.\n";
    }
    std::cout << "-------------------------------\n";
}

void casoPruebaProgDin(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt) {
    std::vector<ObjetoInt> objetos;
    int M;
    double valorSol;

    std::cout << casoPrueba << "\n";
    for (int i = 1; i <= nIt; ++i) {

        leeCasoPruebaMochilaInt(nombreFichero, M, objetos);

        std::vector<bool> solucion(objetos.size());

        auto t1 = std::chrono::steady_clock::now();

        mochilaProgDin(objetos, M, solucion, valorSol);

        auto t2 = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
                t2 - t1);

        std::cout << "Vuelta: " << i << " ValorSol: " << valorSol << '\n';
        std::cout << "El algoritmo ha tardado " << time_span.count()
                  << " segundos.\n";
    }
    std::cout << "-------------------------------\n";
}

void casoPruebaRamPoda(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt) {
    std::vector<ObjetoReal> objetos;
    double M;
    double valorSol;
    long long int nodosVisitados;

    std::cout << casoPrueba << "\n";
    for (int i = 1; i <= nIt; ++i) {

        leeCasoPruebaMochilaReal(nombreFichero, M, objetos);

        std::vector<bool> solucion(objetos.size());

        auto t1 = std::chrono::steady_clock::now();

        mochilaRamPoda(objetos, M, solucion, valorSol, nodosVisitados);

        auto t2 = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
                t2 - t1);

        std::cout << "Vuelta: " << i << " ValorSol: " << valorSol << '\n';
        std::cout << "El algoritmo ha tardado " << time_span.count()
                  << " segundos y ha atravesado " << nodosVisitados <<
                  " nodos.\n";
    }
    std::cout << "-------------------------------\n";
}

void casoPruebaGenetico(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt) {
    std::vector<ObjetoReal> objetos;
    double M;
    double valorSol;

    std::cout << casoPrueba << "\n";
    for (int i = 1; i <= nIt; ++i) {

        leeCasoPruebaMochilaReal(nombreFichero, M, objetos);

        std::vector<bool> solucion(objetos.size());

        auto t1 = std::chrono::steady_clock::now();

        mochilaGenetico(objetos, M, solucion, valorSol);

        auto t2 = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
                t2 - t1);

        std::cout << "Vuelta: " << i << " ValorSol: " << valorSol << '\n';
        std::cout << "El algoritmo ha tardado " << time_span.count()
                  << " segundos.\n";
    }
    std::cout << "-------------------------------\n";
}