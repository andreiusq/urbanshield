#ifndef FABRICA_INCIDENTE_H
#define FABRICA_INCIDENTE_H

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Incident.h"
#include "Locatie.h"

// =====================================================================
// SABLON DE PROIECTARE: FACTORY (Factory Method / Object Factory)
// ---------------------------------------------------------------------
// FabricaIncidente centralizeaza crearea incidentelor polimorfice.
// Codul client cere un incident dupa numele tipului ("INCENDIU",
// "CUTREMUR", ...) fara sa cunoasca clasa concreta sau constructorul ei.
//
// Implementare bazata pe un registru de "creatori": fiecare tip de
// incident isi inregistreaza o functie care stie sa construiasca
// instanta din parametrii generici (ParametriIncident).
// =====================================================================

// Parametri generici de constructie. Campurile specifice (magnitudine,
// numar de raniti etc.) sunt optionale si interpretate de fiecare creator.
struct ParametriIncident {
    int id = 0;
    int severitate = 1;
    Locatie locatie{0.0, 0.0, ""};
    std::string timestamp;
    std::string descriere;

    // Campuri specifice subtipurilor (folosite selectiv de creatori).
    bool cladireInalta = false;       // INCENDIU
    std::string substanta;            // TOXIC
    bool zonaIndustriala = false;     // TOXIC
    int persoaneRanite = 0;           // ACCIDENT
    double magnitudine = 0.0;         // CUTREMUR
    int cladiriAvariate = 0;          // CUTREMUR
    int zoneAfectate = 0;             // INUNDATIE
    int persoaneExpuse = 0;           // PANICA_PUBLICA
};

class FabricaIncidente {
public:
    using Creator = std::function<std::unique_ptr<Incident>(const ParametriIncident&)>;

    // Inregistreaza un creator pentru un nume de tip. Suprascrie daca exista.
    void inregistreaza(const std::string& tip, Creator creator);

    // Creeaza un incident dupa numele tipului; arunca daca tipul e necunoscut.
    std::unique_ptr<Incident> creeaza(const std::string& tip,
                                      const ParametriIncident& p) const;

    bool cunoasteTip(const std::string& tip) const;
    std::vector<std::string> tipuriDisponibile() const;

    // Construieste o fabrica deja populata cu toate tipurile standard
    // de incidente din UrbanShield.
    static FabricaIncidente standard();

private:
    std::map<std::string, Creator> creatori;
};

#endif // FABRICA_INCIDENTE_H
