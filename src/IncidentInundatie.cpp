#include "IncidentInundatie.h"
#include "ExceptiiUrbanShield.h"

IncidentInundatie::IncidentInundatie(int id, int severitate, const Locatie& loc,
                                     const std::string& ts, const std::string& desc,
                                     int zoneAfectate)
    : Incident(id, severitate, loc, ts, desc), zoneAfectate(zoneAfectate) {
    if (zoneAfectate < 0)
        throw DateInvalideException("Numarul de zone afectate nu poate fi negativ.");
}

std::unique_ptr<Incident> IncidentInundatie::clone() const {
    return std::make_unique<IncidentInundatie>(*this);
}

std::string IncidentInundatie::getTip() const {
    return "INUNDATIE";
}

std::string IncidentInundatie::specializareNecesara() const {
    return "POMPIERI";
}

std::vector<std::string> IncidentInundatie::resurseCritice() const {
    return {"generator", "franghii_salvare"};
}

int IncidentInundatie::greutatePrioritate() const {
    return zoneAfectate >= 3 ? 28 : 20;
}

int IncidentInundatie::impactOperationalSpecific() const {
    return 4 + zoneAfectate * 2;
}

int IncidentInundatie::crestereSeveritateFaraInterventie() const {
    return zoneAfectate >= 3 ? 1 : 0;
}

int IncidentInundatie::reducereSeveritateInterventie() const {
    return 1;
}

void IncidentInundatie::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "\n  Zone afectate: " << zoneAfectate;
}
