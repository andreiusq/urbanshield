#ifndef INCIDENT_H
#define INCIDENT_H

#include <memory>
#include <string>
#include <ostream>
#include <vector>
#include "Locatie.h"

class Incident {
    int idIncident;
    int nivelSeveritate;
    Locatie locatie;
    std::string timestamp;
    bool rezolvat;
    std::string descriere;

    static int totalIncidenteCreate;

    virtual int greutatePrioritate() const = 0;
    virtual int impactOperationalSpecific() const = 0;
    virtual int crestereSeveritateFaraInterventie() const = 0;
    virtual int reducereSeveritateInterventie() const = 0;
    virtual void afiseazaDetaliiSpecifice(std::ostream& os) const = 0;

protected:
    Incident(int id, int severitate, const Locatie& loc, const std::string& ts,
             const std::string& desc = "");
    Incident(const Incident& other);

public:
    virtual ~Incident() = default;

    Incident& operator=(const Incident& other) = default;

    virtual std::unique_ptr<Incident> clone() const = 0;
    virtual std::string getTip() const = 0;
    virtual std::string specializareNecesara() const = 0;
    virtual std::vector<std::string> resurseCritice() const = 0;

    int getId() const { return idIncident; }
    int getSeveritate() const { return nivelSeveritate; }
    const Locatie& getLocatie() const { return locatie; }
    const std::string& getTimestamp() const { return timestamp; }
    bool esteRezolvat() const { return rezolvat; }
    int getPrioritate() const;
    int calculeazaImpactOperational() const;
    const std::string& getDescriere() const { return descriere; }

    void marcheazaRezolvat();
    void actualizeazaSeveritate(int nouaSeveritate);
    void escaladeazaFaraInterventie();
    void aplicaInterventie();
    bool necesitaInterventieUrgenta() const;
    void afiseaza(std::ostream& os) const;

    static bool esteSeveritateValida(int severitate);
    static int getTotalIncidenteCreate();

    friend std::ostream& operator<<(std::ostream& os, const Incident& inc);
};

#endif // INCIDENT_H
