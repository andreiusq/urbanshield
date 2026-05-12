#ifndef INCIDENT_INUNDATIE_H
#define INCIDENT_INUNDATIE_H

#include "Incident.h"

class IncidentInundatie final : public Incident {
    int zoneAfectate;

    int greutatePrioritate() const override;
    int impactOperationalSpecific() const override;
    int crestereSeveritateFaraInterventie() const override;
    int reducereSeveritateInterventie() const override;
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    IncidentInundatie(int id, int severitate, const Locatie& loc,
                      const std::string& ts, const std::string& desc,
                      int zoneAfectate);

    std::unique_ptr<Incident> clone() const override;
    std::string getTip() const override;
    std::string specializareNecesara() const override;
    std::vector<std::string> resurseCritice() const override;
};

#endif // INCIDENT_INUNDATIE_H
