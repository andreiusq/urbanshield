#ifndef ECHIPA_INTERVENTIE_H
#define ECHIPA_INTERVENTIE_H

#include <string>
#include <vector>
#include <ostream>
#include "Locatie.h"
#include "Resursa.h"

class EchipaInterventie {
    int idEchipa;
    std::string numeEchipa;
    std::string specializare;
    Locatie locatieCurenta;
    std::vector<Resursa> resurse;
    bool disponibila;

    bool areResurseMinime() const;

public:
    EchipaInterventie(int id, const std::string& nume, const std::string& spec, const Locatie& loc);

    EchipaInterventie(const EchipaInterventie& other);
    EchipaInterventie& operator=(const EchipaInterventie& other);
    ~EchipaInterventie();

    int getId() const;
    const std::string& getNume() const;
    const std::string& getSpecializare() const;
    const Locatie& getLocatie() const;
    bool esteDisponibila() const;

    void adaugaResursa(const Resursa& r);
    void seteazaDisponibila(bool status);
    void actualizeazaLocatie(const Locatie& novaLoc) { locatieCurenta = novaLoc; }
    int totalResurseDisponibile() const;
    bool areResursaSuficienta(const std::string& tip, int cantNecesara) const;
    void alocaResurse(const std::string& tip, int cant);

    friend std::ostream& operator<<(std::ostream& os, const EchipaInterventie& e);
};

#endif
