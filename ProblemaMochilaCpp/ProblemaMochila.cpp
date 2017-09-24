#include <iostream>
#include <fstream>
#include <chrono>

#include "Algoritmos.h"
#include "CasosPrueba.h"

int main() {
    srand((unsigned int) time(NULL));

    auto t1 = std::chrono::steady_clock::now();

    //casoPruebaVoraz();
    //casoPruebaProgDin();
    casoPruebaRamPoda();
    //casoPruebaGenetico();

    auto t2 = std::chrono::steady_clock::now();
    auto time_span =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "Los casos de prueba han tardado " << time_span.count()
              << " segundos.\n";
    return 0;
}
