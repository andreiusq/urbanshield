#ifndef INCIDENT_TOXIC_H
#define INCIDENT_TOXIC_H

#include "Incident.h"

class IncidentToxic final : public Incident {
    std::string substanta;
    bool zonaIndustriala;

    int greutatePrioritate() const override;
    int impactOperationalSpecific() const override;
    int crestereSeveritateFaraInterventie() const override;
    int reducereSeveritateInterventie() const override;
    void afiseazaDetaliiSpecifice(std::ostream& os) const override;

public:
    IncidentToxic(int id, int severitate, const Locatie& loc,
                  const std::string& ts, const std::string& desc,
                  const std::string& substanta, bool zonaIndustriala);

    std::unique_ptr<Incident> clone() const override;
    std::string getTip() const override;
    std::string specializareNecesara() const override;
    std::vector<std::string> resurseCritice() const override;

    const std::string& resursaCritica() const;
    int cantitateMinimaIzolare() const;
};

#endif // INCIDENT_TOXIC_H
