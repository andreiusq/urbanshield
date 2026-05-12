#include "IncidentCutremur.h"
#include "ExceptiiUrbanShield.h"

IncidentCutremur::IncidentCutremur(int id, int severitate, const Locatie& loc,
                                   const std::string& ts, const std::string& desc,
                                   double magnitudine, int cladiriAvariate)
    : Incident(id, severitate, loc, ts, desc),
      magnitudine(magnitudine), cladiriAvariate(cladiriAvariate) {
    if (magnitudine <= 0.0 || cladiriAvariate < 0)
        throw DateInvalideException("Date invalide pentru incidentul de cutremur.");
}

std::unique_ptr<Incident> IncidentCutremur::clone() const {
    return std::make_unique<IncidentCutremur>(*this);
}

std::string IncidentCutremur::getTip() const {
    return "CUTREMUR";
}

std::string IncidentCutremur::specializareNecesara() const {
    return "SALVARE";
}

std::vector<std::string> IncidentCutremur::resurseCritice() const {
    return {"franghii_salvare", "generator"};
}

int IncidentCutremur::greutatePrioritate() const {
    return magnitudine >= 6.0 ? 35 : 25;
}

int IncidentCutremur::impactOperationalSpecific() const {
    return 7 + cladiriAvariate / 5;
}

int IncidentCutremur::crestereSeveritateFaraInterventie() const {
    return 1;
}

int IncidentCutremur::reducereSeveritateInterventie() const {
    return 1;
}

void IncidentCutremur::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "\n  Magnitudine: " << magnitudine
       << "\n  Cladiri avariate: " << cladiriAvariate;
}
