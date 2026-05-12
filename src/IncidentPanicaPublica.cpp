#include "IncidentPanicaPublica.h"
#include "ExceptiiUrbanShield.h"

IncidentPanicaPublica::IncidentPanicaPublica(int id, int severitate, const Locatie& loc,
                                             const std::string& ts, const std::string& desc,
                                             int persoaneExpuse)
    : Incident(id, severitate, loc, ts, desc), persoaneExpuse(persoaneExpuse) {
    if (persoaneExpuse < 0)
        throw DateInvalideException("Numarul de persoane expuse nu poate fi negativ.");
}

std::unique_ptr<Incident> IncidentPanicaPublica::clone() const {
    return std::make_unique<IncidentPanicaPublica>(*this);
}

std::string IncidentPanicaPublica::getTip() const {
    return "PANICA_PUBLICA";
}

std::string IncidentPanicaPublica::specializareNecesara() const {
    return "MEDICAL";
}

std::vector<std::string> IncidentPanicaPublica::resurseCritice() const {
    return {"trusa_medicala"};
}

int IncidentPanicaPublica::greutatePrioritate() const {
    return persoaneExpuse >= 100 ? 22 : 14;
}

int IncidentPanicaPublica::impactOperationalSpecific() const {
    return 2 + persoaneExpuse / 50;
}

int IncidentPanicaPublica::crestereSeveritateFaraInterventie() const {
    return persoaneExpuse >= 100 ? 1 : 0;
}

int IncidentPanicaPublica::reducereSeveritateInterventie() const {
    return 2;
}

void IncidentPanicaPublica::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "\n  Persoane expuse: " << persoaneExpuse;
}
