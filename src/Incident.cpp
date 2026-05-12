#include "Incident.h"
#include <map>
#include <stdexcept>

Incident::Incident(int id, const std::string& tip, int severitate,
                   const Locatie& loc, const std::string& ts,
                   const std::string& desc)
    : idIncident(id), tipIncident(tip), nivelSeveritate(severitate),
      locatie(loc), timestamp(ts), rezolvat(false), descriere(desc) {
    if (severitate < 1 || severitate > 5)
        throw std::invalid_argument("Severitatea trebuie sa fie intre 1 si 5.");
}

int Incident::calculeazaPrioritate() const {
    // Greutate dupa tip: incendii si scurgeri toxice au prioritate mai mare
    static const std::map<std::string, int> greutate = {
        {"INCENDIU",      30},
        {"TOXIC_SPILL",   30},
        {"CUTREMUR",      25},
        {"INUNDATIE",     20},
        {"ACCIDENT",      10}
    };
    auto it = greutate.find(tipIncident);
    int g = (it != greutate.end()) ? it->second : 10;
    return nivelSeveritate * 10 + g;
}
int Incident::getPrioritate() const { return calculeazaPrioritate(); }

void Incident::marcheazaRezolvat() {
    rezolvat = true;
}

void Incident::actualizeazaSeveritate(int nouaSeveritate) {
    if (nouaSeveritate < 1 || nouaSeveritate > 5)
        throw std::invalid_argument("Severitatea trebuie sa fie intre 1 si 5.");
    nivelSeveritate = nouaSeveritate;
}

std::ostream& operator<<(std::ostream& os, const Incident& inc) {
    os << "Incident[" << inc.idIncident << "] " << inc.tipIncident
       << " | Severitate: " << inc.nivelSeveritate
       << "/5 | Prioritate: " << inc.calculeazaPrioritate() << "\n"
       << "  Locatie:   " << inc.locatie << "\n"
       << "  Timestamp: " << inc.timestamp << "\n"
       << "  Rezolvat:  " << (inc.rezolvat ? "DA" : "NU") << "\n"
       << "  Descriere: " << inc.descriere;
    return os;
}
