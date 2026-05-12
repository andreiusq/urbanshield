#ifndef INCIDENT_INCENDIU_H
#define INCIDENT_INCENDIU_H

#include "Incident.h"

class IncidentIncendiu final : public Incident {
    bool cladireInalta;

    int greutatePrioritate() const override;
    int impactOperationalSpecific() const override;
    int crestereSeveritateFaraInterventie() const override;
    int reducereSeveritateInterventie() const override;
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    IncidentIncendiu(int id, int severitate, const Locatie& loc,
                     const std::string& ts, const std::string& desc,
                     bool cladireInalta);

    std::unique_ptr<Incident> clone() const override;
    std::string getTip() const override;
    std::string specializareNecesara() const override;
    std::vector<std::string> resurseCritice() const override;
};

#endif // INCIDENT_INCENDIU_H
