#ifndef INCIDENT_CUTREMUR_H
#define INCIDENT_CUTREMUR_H

#include "Incident.h"

class IncidentCutremur final : public Incident {
    double magnitudine;
    int cladiriAvariate;

    int greutatePrioritate() const override;
    int impactOperationalSpecific() const override;
    int crestereSeveritateFaraInterventie() const override;
    int reducereSeveritateInterventie() const override;
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    IncidentCutremur(int id, int severitate, const Locatie& loc,
                     const std::string& ts, const std::string& desc,
                     double magnitudine, int cladiriAvariate);

    std::unique_ptr<Incident> clone() const override;
    std::string getTip() const override;
    std::string specializareNecesara() const override;
    std::vector<std::string> resurseCritice() const override;
};

#endif // INCIDENT_CUTREMUR_H
