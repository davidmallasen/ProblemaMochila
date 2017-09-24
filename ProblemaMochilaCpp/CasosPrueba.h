#ifndef PROBLEMAMOCHILACPP_CASOSPRUEBA_H
#define PROBLEMAMOCHILACPP_CASOSPRUEBA_H

#include "Algoritmos.h"

#include <string>

static const std::string NOMBRE_FICHERO = "pruebaReal2.txt";
static const int N_OBJETOS = 1000;
static const double MAX_PESO_OBJETO = 100;
static const double MAX_VALOR_OBJETO = 100;
static const double MAX_MOCHILA = 2500;

static const int N_IT_CASO_PEQ = 3;
static const int N_IT_CASO_GR = 1;
static const int N_IT_GEN = 5;

/**
 * Genera un fichero de nombre nombreFichero con el tamanyo de la mochila
 * M y el numero de objetos nObjetos en la primera linea separados por un
 * espacio. Produce nObjetos aleatorios con la restriccion de que el peso y
 * el valor esten en (0, maxPesoObjeto) o (0, maxValorObjeto).
 * Escribe los objetos a partir de la segunda linea. Cada linea de
 * objeto son dos numeros, peso y valor, separados por un espacio.
 *
 * @param nombreFichero Nombre del fichero donde guardar los datos.
 * @param nObjetos Numero de objetos
 * a generar.
 * @param maxPesoObjeto Peso maximo de cada objeto.
 * @param maxValorObjeto Valor maximo de cada objeto.
 * @param M Peso maximo soportado por la mochila.
 */
void generaCasoPruebaMochilaReal(std::string nombreFichero, int nObjetos,
                                 double maxPesoObjeto, double maxValorObjeto,
                                 double M);

void generaCasoPruebaMochilaInt(std::string nombreFichero, int nObjetos,
                                int maxPesoObjeto, double maxValorObjeto,
                                int M);

/**
 * Construye el vector de objetos a partir de los datos de nombreFichero,
 * se presupone estructura coherente con el metodo generaCasoPruebaMochila.
 * Devuelve tambien el tamanyo maximo de la mochila M.
 *
 * @param nombreFichero Nombre del fichero del cual leer.
 * @param M Tamanyo maximo de la mochila.
 * @param objetos Vector de objetos. Se presupone vacio.
 */
void leeCasoPruebaMochilaReal(std::string nombreFichero, double &M,
                              std::vector<ObjetoReal> &objetos);

void leeCasoPruebaMochilaInt(std::string nombreFichero, int &M,
                             std::vector<ObjetoInt> &objetos);

void casoPruebaVoraz();
void casoPruebaVoraz(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt);

void casoPruebaProgDin();
void casoPruebaProgDin(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt);

void casoPruebaRamPoda();
void casoPruebaRamPoda(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt);

void casoPruebaGenetico();
void casoPruebaGenetico(std::string const &casoPrueba, std::string const &
nombreFichero, const int nIt);

#endif //PROBLEMAMOCHILACPP_CASOSPRUEBA_H
