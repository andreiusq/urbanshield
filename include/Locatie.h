#ifndef LOCATIE_H
#define LOCATIE_H

#include <string>
#include <ostream>

class Locatie {
    double latitudine;
    double longitudine;
    std::string adresa;

public:
    Locatie(double lat, double lon, const std::string& adr);

    double getLatitudine() const;
    double getLongitudine() const;
    const std::string& getAdresa() const;

    double distantaFata(const Locatie& alta) const;
    bool esteInZona(const Locatie& centru, double razaKm) const;

    friend std::ostream& operator<<(std::ostream& os, const Locatie& loc);
};

#endif // LOCATIE_H
