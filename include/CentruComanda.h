#ifndef CENTRU_COMANDA_H
#define CENTRU_COMANDA_H

#include <string>
#include <vector>
#include <ostream>
#include "Locatie.h"
#include "EchipaInterventie.h"
#include "Incident.h"

class CentruComanda {
    std::string numeCentru;
    Locatie locatieCentru;
    std::vector<EchipaInterventie> echipe;
    std::vector<Incident> incidente;
    int contorOperatii;

    int gasesteEchipaOptima(const Incident& inc) const;
    void logOperatie(const std::string& mesaj);

public:
    CentruComanda(const std::string& nume, const Locatie& loc);

    void adaugaEchipa(const EchipaInterventie& e);
    void adaugaIncident(const Incident& inc);
    int numarEchipeDisponibile() const;
    int numarIncidenteActive() const;

    // Functii netriviale
    std::vector<Incident> getIncidentePrioritizate() const;
    bool alocaEchipaLaIncident(int idIncident);
    void simuleazaEvolutie(int pasi);
    void genereazaRaport() const;

    friend std::ostream& operator<<(std::ostream& os, const CentruComanda& cc);
};

#endif // CENTRU_COMANDA_H
