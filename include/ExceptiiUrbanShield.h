#ifndef EXCEPTII_URBAN_SHIELD_H
#define EXCEPTII_URBAN_SHIELD_H

#include <stdexcept>
#include <string>

class UrbanShieldException : public std::runtime_error {
public:
    explicit UrbanShieldException(const std::string& mesaj);
};

class DateInvalideException : public UrbanShieldException {
public:
    explicit DateInvalideException(const std::string& mesaj);
};

class ResursaInsuficientaException : public UrbanShieldException {
public:
    explicit ResursaInsuficientaException(const std::string& mesaj);
};

class EntitateNegasitaException : public UrbanShieldException {
public:
    explicit EntitateNegasitaException(const std::string& mesaj);
};

#endif // EXCEPTII_URBAN_SHIELD_H
