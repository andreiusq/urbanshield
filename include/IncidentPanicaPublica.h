#ifndef INCIDENT_PANICA_PUBLICA_H
#define INCIDENT_PANICA_PUBLICA_H

#include "Incident.h"

class IncidentPanicaPublica final : public Incident {
    int persoaneExpuse;

    int greutatePrioritate() const override;
    int impactOperationalSpecific() const override;
    int crestereSeveritateFaraInterventie() const override;
    int reducereSeveritateInterventie() const override;
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    IncidentPanicaPublica(int id, int severitate, const Locatie& loc,
                          const std::string& ts, const std::string& desc,
                          int persoaneExpuse);

    std::unique_ptr<Incident> clone() const override;
    std::string getTip() const override;
    std::string specializareNecesara() const override;
    std::vector<std::string> resurseCritice() const override;
};

#endif // INCIDENT_PANICA_PUBLICA_H
