#include "IncidentToxic.h"

IncidentToxic::IncidentToxic(int id, int severitate, const Locatie& loc,
                             const std::string& ts, const std::string& desc,
                             const std::string& substanta, bool zonaIndustriala)
    : Incident(id, severitate, loc, ts, desc),
      substanta(substanta), zonaIndustriala(zonaIndustriala) {}

std::unique_ptr<Incident> IncidentToxic::clone() const {
    return std::make_unique<IncidentToxic>(*this);
}

std::string IncidentToxic::getTip() const {
    return "TOXIC_SPILL";
}

std::string IncidentToxic::specializareNecesara() const {
    return "HAZMAT";
}

std::vector<std::string> IncidentToxic::resurseCritice() const {
    return {resursaCritica()};
}

const std::string& IncidentToxic::resursaCritica() const {
    static const std::string resursa = "combinezoane_hazmat";
    return resursa;
}

int IncidentToxic::cantitateMinimaIzolare() const {
    return zonaIndustriala ? 4 : 2;
}

int IncidentToxic::greutatePrioritate() const {
    return zonaIndustriala ? 36 : 30;
}

int IncidentToxic::impactOperationalSpecific() const {
    return zonaIndustriala ? 9 : 6;
}

int IncidentToxic::crestereSeveritateFaraInterventie() const {
    return zonaIndustriala ? 2 : 1;
}

int IncidentToxic::reducereSeveritateInterventie() const {
    return 1;
}

void IncidentToxic::afiseazaDetaliiSpecifice(std::ostream& os) const {
    os << "\n  Substanta: " << substanta
       << "\n  Zona industriala: " << (zonaIndustriala ? "DA" : "NU")
       << "\n  Combinezoane minime: " << cantitateMinimaIzolare();
}
