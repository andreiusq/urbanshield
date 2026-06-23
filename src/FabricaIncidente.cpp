#include "FabricaIncidente.h"

#include "ExceptiiUrbanShield.h"
#include "IncidentAccident.h"
#include "IncidentCutremur.h"
#include "IncidentIncendiu.h"
#include "IncidentInundatie.h"
#include "IncidentPanicaPublica.h"
#include "IncidentToxic.h"

void FabricaIncidente::inregistreaza(const std::string& tip, Creator creator) {
    if (!creator)
        throw DateInvalideException("FabricaIncidente: creator null pentru tipul " + tip);
    creatori[tip] = std::move(creator);
}

std::unique_ptr<Incident> FabricaIncidente::creeaza(const std::string& tip,
                                                    const ParametriIncident& p) const {
    auto it = creatori.find(tip);
    if (it == creatori.end())
        throw EntitateNegasitaException("FabricaIncidente: tip necunoscut '" + tip + "'");
    return (it->second)(p);
}

bool FabricaIncidente::cunoasteTip(const std::string& tip) const {
    return creatori.find(tip) != creatori.end();
}

std::vector<std::string> FabricaIncidente::tipuriDisponibile() const {
    std::vector<std::string> tipuri;
    tipuri.reserve(creatori.size());
    for (const auto& [tip, creator] : creatori)
        tipuri.push_back(tip);
    return tipuri;
}

FabricaIncidente FabricaIncidente::standard() {
    FabricaIncidente fabrica;

    fabrica.inregistreaza("INCENDIU", [](const ParametriIncident& p) {
        return std::make_unique<IncidentIncendiu>(
            p.id, p.severitate, p.locatie, p.timestamp, p.descriere, p.cladireInalta);
    });

    fabrica.inregistreaza("TOXIC", [](const ParametriIncident& p) {
        return std::make_unique<IncidentToxic>(
            p.id, p.severitate, p.locatie, p.timestamp, p.descriere,
            p.substanta, p.zonaIndustriala);
    });

    fabrica.inregistreaza("ACCIDENT", [](const ParametriIncident& p) {
        return std::make_unique<IncidentAccident>(
            p.id, p.severitate, p.locatie, p.timestamp, p.descriere, p.persoaneRanite);
    });

    fabrica.inregistreaza("CUTREMUR", [](const ParametriIncident& p) {
        return std::make_unique<IncidentCutremur>(
            p.id, p.severitate, p.locatie, p.timestamp, p.descriere,
            p.magnitudine, p.cladiriAvariate);
    });

    fabrica.inregistreaza("INUNDATIE", [](const ParametriIncident& p) {
        return std::make_unique<IncidentInundatie>(
            p.id, p.severitate, p.locatie, p.timestamp, p.descriere, p.zoneAfectate);
    });

    fabrica.inregistreaza("PANICA_PUBLICA", [](const ParametriIncident& p) {
        return std::make_unique<IncidentPanicaPublica>(
            p.id, p.severitate, p.locatie, p.timestamp, p.descriere, p.persoaneExpuse);
    });

    return fabrica;
}
