#ifndef RESURSA_H
#define RESURSA_H

#include <string>
#include <ostream>

class Resursa {
    std::string denumire;
    int cantitate;
    std::string unitate;

public:
    Resursa(const std::string& den, int cant, const std::string& unit);

    const std::string& getDenumire() const { return denumire; }
    int getCantitate() const { return cantitate; }
    const std::string& getUnitatea() const { return unitate; }

    bool esteDisponibila() const { return cantitate > 0; }
    void consuma(int cant);
    void reincarcare(int cant);

    friend std::ostream& operator<<(std::ostream& os, const Resursa& r);
};

#endif // RESURSA_H
