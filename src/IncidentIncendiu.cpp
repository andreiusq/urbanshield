#include "IncidentIncendiu.h"

IncidentIncendiu::IncidentIncendiu(int id, int severitate, const Locatie& loc,
                                   const std::string& ts, const std::string& desc,
                                   bool cladireInalta)
    : Incident(id, severitate, loc, ts, desc), cladireInalta(cladireInalta) {}

std::unique_ptr<Incident> IncidentIncendiu::clone() const {
    return std::make_unique<IncidentIncendiu>(*this);
}

std::string IncidentIncendiu::getTip() const {
    return "INCENDIU";
}

std::string IncidentIncendiu::specializareNecesara() const {
    return "POMPIERI";
}

std::vector<std::string> IncidentIncendiu::resurseCritice() const {
    return {"furtun_apa", "generator"};
}

int IncidentIncendiu::greutatePrioritate() const {
    return cladireInalta ? 38 : 30;
}

int IncidentIncendiu::impactOperationalSpecific() const {
    return cladireInalta ? 8 : 5;
}

int IncidentIncendiu::crestereSeveritateFaraInterventie() const {
    return 1;
}

int IncidentIncendiu::reducereSeveritateInterventie() const {
    return cladireInalta ? 1 : 2;
}

void IncidentIncendiu::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "\n  Cladire inalta: " << (cladireInalta ? "DA" : "NU");
}
