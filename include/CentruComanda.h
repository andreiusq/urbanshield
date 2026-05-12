#ifndef CENTRU_COMANDA_H
#define CENTRU_COMANDA_H

#include <map>
#include <memory>
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
    std::vector<std::unique_ptr<Incident>> incidente;
    std::map<int, int> alocariIncidentEchipa;
    int contorOperatii;

    int gasesteEchipaOptima(const Incident& inc) const;
    Incident* gasesteIncidentActiv(int idIncident);
    const Incident* gasesteIncidentActiv(int idIncident) const;
    void elibereazaEchipa(int idEchipa);
    void logOperatie(const std::string& mesaj);

public:
    CentruComanda(const std::string& nume, const Locatie& loc);
    CentruComanda(const CentruComanda& other);
    CentruComanda& operator=(CentruComanda other);
    ~CentruComanda();

    friend void swap(CentruComanda& first, CentruComanda& second) noexcept;

    void adaugaEchipa(const EchipaInterventie& e);
    void adaugaIncident(const Incident& inc);
    void adaugaIncident(std::unique_ptr<Incident> inc);
    int numarEchipeDisponibile() const;
    int numarEchipe() const;
    int numarIncidenteActive() const;
    const std::vector<EchipaInterventie>& getEchipe() const;

    // Functii netriviale
    std::vector<const Incident*> getIncidentePrioritizate() const;
    bool alocaEchipaLaIncident(int idIncident);
    void simuleazaEvolutie(int pasi);
    void genereazaRaport() const;

    friend std::ostream& operator<<(std::ostream& os, const CentruComanda& cc);
};

#endif // CENTRU_COMANDA_H
