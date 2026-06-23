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
#include "ObservatorCentru.h"

class CentruComanda {
    std::string numeCentru;
    Locatie locatieCentru;
    std::vector<EchipaInterventie> echipe;
    std::vector<std::unique_ptr<Incident>> incidente;
    std::map<int, int> alocariIncidentEchipa;
    int contorOperatii;
    // Subiect Observer: observatorii sunt detinuti extern, nu copiati.
    std::vector<ObservatorCentru*> observatori;

    int gasesteEchipaOptima(const Incident& inc) const;
    Incident* gasesteIncidentActiv(int idIncident);
    const Incident* gasesteIncidentActiv(int idIncident) const;
    void elibereazaEchipa(int idEchipa);
    void logOperatie(const std::string& mesaj);
    void notifica(EvenimentCentru eveniment, const Incident* incident,
                  const std::string& detalii) const;

public:
    CentruComanda(const std::string& nume, const Locatie& loc);
    CentruComanda(const CentruComanda& other);
    CentruComanda& operator=(CentruComanda other);
    ~CentruComanda();

    friend void swap(CentruComanda& first, CentruComanda& second) noexcept;

    // Observer: inregistrare/dezabonare observatori (subiectul nu ii detine).
    void inregistreazaObservator(ObservatorCentru* obs);
    void dezabonezaObservator(ObservatorCentru* obs);

    void adaugaEchipa(const EchipaInterventie& e);
    void adaugaIncident(const Incident& inc);
    void adaugaIncident(std::unique_ptr<Incident> inc);
    int numarEchipeDisponibile() const;
    int numarEchipe() const;
    int numarIncidenteActive() const;
    const std::vector<EchipaInterventie>& getEchipe() const;

    // Functii netriviale
    std::vector<const Incident*> getIncidentePrioritizate() const;
    const Incident* incidentCelMaiPrioritar() const;
    bool alocaEchipaLaIncident(int idIncident);
    void simuleazaEvolutie(int pasi);
    void genereazaRaport() const;

    friend std::ostream& operator<<(std::ostream& os, const CentruComanda& cc);
};

#endif // CENTRU_COMANDA_H
