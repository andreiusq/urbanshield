#include "Incident.h"
#include "ExceptiiUrbanShield.h"
#include <algorithm>

int Incident::totalIncidenteCreate = 0;

Incident::Incident(int id, int severitate, const Locatie& loc,
                   const std::string& ts, const std::string& desc)
    : idIncident(id), nivelSeveritate(severitate), locatie(loc),
      timestamp(ts), rezolvat(false), descriere(desc) {
    if (!esteSeveritateValida(severitate))
        throw DateInvalideException("Severitatea trebuie sa fie intre 1 si 5.");
    ++totalIncidenteCreate;
}

Incident::Incident(const Incident& other)
    : idIncident(other.idIncident), nivelSeveritate(other.nivelSeveritate),
      locatie(other.locatie), timestamp(other.timestamp),
      rezolvat(other.rezolvat), descriere(other.descriere) {
    ++totalIncidenteCreate;
}

int Incident::getPrioritate() const {
    return nivelSeveritate * 10 + greutatePrioritate() + calculeazaImpactOperational();
}

int Incident::calculeazaImpactOperational() const {
    return nivelSeveritate * impactOperationalSpecific();
}

void Incident::marcheazaRezolvat() {
    rezolvat = true;
}

void Incident::actualizeazaSeveritate(int nouaSeveritate) {
    if (!esteSeveritateValida(nouaSeveritate))
        throw DateInvalideException("Severitatea trebuie sa fie intre 1 si 5.");
    nivelSeveritate = nouaSeveritate;
}

void Incident::escaladeazaFaraInterventie() {
    nivelSeveritate = std::clamp(
        nivelSeveritate + crestereSeveritateFaraInterventie(), 1, 5);
}

void Incident::aplicaInterventie() {
    nivelSeveritate = std::clamp(
        nivelSeveritate - reducereSeveritateInterventie(), 1, 5);
    if (nivelSeveritate == 1)
        marcheazaRezolvat();
}

bool Incident::necesitaInterventieUrgenta() const {
    return !rezolvat && nivelSeveritate >= 4;
}

void Incident::afiseaza(std::ostream& os) const {
    os << "Incident[" << idIncident << "] " << getTip()
       << " | Severitate: " << nivelSeveritate
       << "/5 | Prioritate: " << getPrioritate()
       << " | Impact operational: " << calculeazaImpactOperational() << "\n"
       << "  Locatie:      " << locatie << "\n"
       << "  Timestamp:    " << timestamp << "\n"
       << "  Rezolvat:     " << (rezolvat ? "DA" : "NU") << "\n"
       << "  Specializare: " << specializareNecesara() << "\n"
       << "  Descriere:    " << descriere;
    afiseazaDetaliiSpecifice(os);
}

bool Incident::esteSeveritateValida(int severitate) {
    return severitate >= 1 && severitate <= 5;
}

int Incident::getTotalIncidenteCreate() {
    return totalIncidenteCreate;
}

std::ostream& operator<<(std::ostream& os, const Incident& inc) {
    inc.afiseaza(os);
    return os;
}
