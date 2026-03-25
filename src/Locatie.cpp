#include "Locatie.h"
#include <cmath>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Locatie::Locatie(double lat, double lon, const std::string& adr)
    : latitudine(lat), longitudine(lon), adresa(adr) {}

double Locatie::distantaFata(const Locatie& alta) const {
    // Aproximare plana: 1 grad latitudine ~ 111 km, 1 grad longitudine ~ 111 * cos(lat) km
    const double KM_PER_DEG_LAT = 111.0;
    double latMed = (latitudine + alta.latitudine) / 2.0 * M_PI / 180.0;
    double kmPerDegLon = 111.0 * std::cos(latMed);

    double dLat = (alta.latitudine - latitudine) * KM_PER_DEG_LAT;
    double dLon = (alta.longitudine - longitudine) * kmPerDegLon;
    return std::sqrt(dLat * dLat + dLon * dLon);
}

bool Locatie::esteInZona(const Locatie& centru, double razaKm) const {
    return distantaFata(centru) <= razaKm;
}

std::ostream& operator<<(std::ostream& os, const Locatie& loc) {
    os << loc.adresa
       << " (" << std::fixed << std::setprecision(4)
       << loc.latitudine << ", " << loc.longitudine << ")";
    return os;
}
