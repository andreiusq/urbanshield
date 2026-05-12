#include "ExceptiiUrbanShield.h"

UrbanShieldException::UrbanShieldException(const std::string& mesaj)
    : std::runtime_error(mesaj) {}

DateInvalideException::DateInvalideException(const std::string& mesaj)
    : UrbanShieldException(mesaj) {}

ResursaInsuficientaException::ResursaInsuficientaException(const std::string& mesaj)
    : UrbanShieldException(mesaj) {}

EntitateNegasitaException::EntitateNegasitaException(const std::string& mesaj)
    : UrbanShieldException(mesaj) {}
