#ifndef INCIDENT_ACCIDENT_H
#define INCIDENT_ACCIDENT_H

#include "Incident.h"

class IncidentAccident final : public Incident {
    int persoaneRanite;

    int greutatePrioritate() const override;
    int impactOperationalSpecific() const override;
    int crestereSeveritateFaraInterventie() const override;
    int reducereSeveritateInterventie() const override;
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    IncidentAccident(int id, int severitate, const Locatie& loc,
                     const std::string& ts, const std::string& desc,
                     int persoaneRanite);

    std::unique_ptr<Incident> clone() const override;
    std::string getTip() const override;
    std::string specializareNecesara() const override;
    std::vector<std::string> resurseCritice() const override;
};

#endif // INCIDENT_ACCIDENT_H
