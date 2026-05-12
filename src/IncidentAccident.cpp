#include "IncidentAccident.h"
#include "ExceptiiUrbanShield.h"

IncidentAccident::IncidentAccident(int id, int severitate, const Locatie& loc,
                                   const std::string& ts, const std::string& desc,
                                   int persoaneRanite)
    : Incident(id, severitate, loc, ts, desc), persoaneRanite(persoaneRanite) {
    if (persoaneRanite < 0)
        throw DateInvalideException("Numarul de persoane ranite nu poate fi negativ.");
}

std::unique_ptr<Incident> IncidentAccident::clone() const {
    return std::make_unique<IncidentAccident>(*this);
}

std::string IncidentAccident::getTip() const {
    return "ACCIDENT";
}

std::string IncidentAccident::specializareNecesara() const {
    return "MEDICAL";
}

std::vector<std::string> IncidentAccident::resurseCritice() const {
    return {"trusa_medicala", "franghii_salvare"};
}

int IncidentAccident::greutatePrioritate() const {
    return persoaneRanite >= 5 ? 18 : 10;
}

int IncidentAccident::impactOperationalSpecific() const {
    return 3 + persoaneRanite;
}

int IncidentAccident::crestereSeveritateFaraInterventie() const {
    return persoaneRanite >= 5 ? 1 : 0;
}

int IncidentAccident::reducereSeveritateInterventie() const {
    return 2;
}

void IncidentAccident::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "\n  Persoane ranite: " << persoaneRanite;
}
