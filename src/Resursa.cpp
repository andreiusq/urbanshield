#include "Resursa.h"
#include "ExceptiiUrbanShield.h"

Resursa::Resursa(const std::string& den, int cant, const std::string& unit)
    : denumire(den), cantitate(cant), unitate(unit) {
    if (cant < 0)
        throw DateInvalideException("Cantitatea nu poate fi negativa.");
}

void Resursa::consuma(int cant) {
    if (cant <= 0)
        throw DateInvalideException("Cantitatea consumata trebuie sa fie pozitiva.");
    if (cant > cantitate)
        throw ResursaInsuficientaException("Resursa insuficienta: " + denumire);
    cantitate -= cant;
}

void Resursa::reincarcare(int cant) {
    if (cant <= 0)
        throw DateInvalideException("Cantitatea de reincarcare trebuie sa fie pozitiva.");
    cantitate += cant;
}

std::ostream& operator<<(std::ostream& os, const Resursa& r) {
    os << r.denumire << ": " << r.cantitate << " " << r.unitate;
    return os;
}
