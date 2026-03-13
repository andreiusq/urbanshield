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

    const std::string& getDenumire() const;
    int getCantitate() const;
    const std::string& getUnitatea() const;

    bool esteDisponibila() const;
    void consuma(int cant);
    void reincarcare(int cant);

    friend std::ostream& operator<<(std::ostream& os, const Resursa& r);
};

#endif // RESURSA_H
