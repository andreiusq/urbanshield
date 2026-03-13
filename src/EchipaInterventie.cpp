#include "EchipaInterventie.h"
#include <algorithm>
#include <numeric>

EchipaInterventie::EchipaInterventie(int id, const std::string& nume,
                                     const std::string& spec, const Locatie& loc)
    : idEchipa(id), numeEchipa(nume), specializare(spec),
      locatieCurenta(loc), resurse(), disponibila(true) {}

// Regula celor 3 - constructor de copiere
EchipaInterventie::EchipaInterventie(const EchipaInterventie& other)
    : idEchipa(other.idEchipa), numeEchipa(other.numeEchipa),
      specializare(other.specializare), locatieCurenta(other.locatieCurenta),
      resurse(other.resurse), disponibila(other.disponibila) {}

// Regula celor 3 - operator= de copiere
EchipaInterventie& EchipaInterventie::operator=(const EchipaInterventie& other) {
    if (this != &other) {
        idEchipa = other.idEchipa;
        numeEchipa = other.numeEchipa;
        specializare = other.specializare;
        locatieCurenta = other.locatieCurenta;
        resurse = other.resurse;
        disponibila = other.disponibila;
    }
    return *this;
}

// Regula celor 3 - destructor
EchipaInterventie::~EchipaInterventie() {
    resurse.clear();
}

bool EchipaInterventie::areResurseMinime() const {
    return std::any_of(resurse.begin(), resurse.end(),
                       [](const Resursa& r) { return r.esteDisponibila(); });
}

int EchipaInterventie::getId() const { return idEchipa; }
const std::string& EchipaInterventie::getNume() const { return numeEchipa; }
const std::string& EchipaInterventie::getSpecializare() const { return specializare; }
const Locatie& EchipaInterventie::getLocatie() const { return locatieCurenta; }
bool EchipaInterventie::esteDisponibila() const { return disponibila; }

void EchipaInterventie::adaugaResursa(const Resursa& r) {
    resurse.push_back(r);
}

void EchipaInterventie::seteazaDisponibila(bool status) {
    disponibila = status;
}

void EchipaInterventie::actualizeazaLocatie(const Locatie& novaLoc) {
    locatieCurenta = novaLoc;
}

int EchipaInterventie::totalResurseDisponibile() const {
    return std::accumulate(resurse.begin(), resurse.end(), 0,
                           [](int sum, const Resursa& r) {
                               return sum + r.getCantitate();
                           });
}

bool EchipaInterventie::areResursaSuficienta(const std::string& tip, int cantNecesara) const {
    for (const auto& r : resurse) {
        if (r.getDenumire() == tip && r.getCantitate() >= cantNecesara)
            return true;
    }
    return false;
}

void EchipaInterventie::alocaResurse(const std::string& tip, int cant) {
    for (auto& r : resurse) {
        if (r.getDenumire() == tip) {
            r.consuma(cant);
            if (!areResurseMinime())
                disponibila = false;
            return;
        }
    }
    throw std::runtime_error("Resursa '" + tip + "' nu exista in echipa " + numeEchipa);
}

std::ostream& operator<<(std::ostream& os, const EchipaInterventie& e) {
    os << "EchipaInterventie[" << e.idEchipa << "]: " << e.numeEchipa << "\n"
       << "  Specializare: " << e.specializare << "\n"
       << "  Disponibila:  " << (e.disponibila ? "DA" : "NU") << "\n"
       << "  Locatie:      " << e.locatieCurenta << "\n"
       << "  Resurse (" << e.resurse.size() << "):\n";
    for (const auto& r : e.resurse)
        os << "    - " << r << "\n";
    return os;
}
