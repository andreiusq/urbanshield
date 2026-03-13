#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>
#include <ostream>
#include "Locatie.h"

class Incident {
    int idIncident;
    std::string tipIncident;
    int nivelSeveritate;
    Locatie locatie;
    std::string timestamp;
    bool rezolvat;
    std::string descriere;

    int calculeazaPrioritate() const;

public:
    Incident(int id, const std::string& tip, int severitate,
             const Locatie& loc, const std::string& ts,
             const std::string& desc = "");

    int getId() const;
    const std::string& getTip() const;
    int getSeveritate() const;
    const Locatie& getLocatie() const;
    const std::string& getTimestamp() const;
    bool esteRezolvat() const;
    int getPrioritate() const;
    const std::string& getDescriere() const;

    void marcheazaRezolvat();
    void actualizeazaSeveritate(int nouaSeveritate);

    friend std::ostream& operator<<(std::ostream& os, const Incident& inc);
};

#endif // INCIDENT_H
