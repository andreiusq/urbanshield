#ifndef REGISTRU_H
#define REGISTRU_H

#include <algorithm>
#include <functional>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "ExceptiiUrbanShield.h"

// =====================================================================
// CLASA TEMPLATE: Registru<T>
// ---------------------------------------------------------------------
// Container generic indexat dupa o cheie intreaga (int), folosit pentru
// a stoca si interoga colectii de entitati din UrbanShield.
//
// Este instantiata in proiect pentru cel putin doua tipuri distincte:
//   - Registru<EchipaInterventie>  (entitati stocate prin valoare)
//   - Registru<const Incident*>    (entitati polimorfice prin pointer)
//
// Cheia este extrasa din element printr-un functor configurabil, deci
// clasa nu impune o interfata fixa pe T. Implicit, foloseste membrul
// .getId(), care exista pe toate entitatile relevante.
// =====================================================================
template <typename T>
class Registru {
    std::vector<T> elemente;
    std::function<int(const T&)> extragereCheie;

public:
    using value_type = T;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Implicit: cheia este rezultatul lui element.getId().
    Registru()
        : elemente(),
          extragereCheie([](const T& el) { return el.getId(); }) {}

    // Permite o strategie de extragere a cheii personalizata (ex. pointeri).
    explicit Registru(std::function<int(const T&)> cheie)
        : elemente(), extragereCheie(std::move(cheie)) {}

    void adauga(const T& element) {
        const int cheie = extragereCheie(element);
        if (contineCheie(cheie))
            throw DateInvalideException(
                "Registru: cheie duplicata = " + std::to_string(cheie));
        elemente.push_back(element);
    }

    bool contineCheie(int cheie) const {
        return std::any_of(elemente.begin(), elemente.end(),
                           [&](const T& el) { return extragereCheie(el) == cheie; });
    }

    // Cauta dupa cheie; arunca daca nu exista (acces sigur).
    const T& gaseste(int cheie) const {
        auto it = std::find_if(elemente.begin(), elemente.end(),
                              [&](const T& el) { return extragereCheie(el) == cheie; });
        if (it == elemente.end())
            throw EntitateNegasitaException(
                "Registru: nu exista element cu cheia " + std::to_string(cheie));
        return *it;
    }

    T& gaseste(int cheie) {
        auto it = std::find_if(elemente.begin(), elemente.end(),
                              [&](const T& el) { return extragereCheie(el) == cheie; });
        if (it == elemente.end())
            throw EntitateNegasitaException(
                "Registru: nu exista element cu cheia " + std::to_string(cheie));
        return *it;
    }

    std::size_t dimensiune() const { return elemente.size(); }
    bool esteGol() const { return elemente.empty(); }

    const_iterator begin() const { return elemente.begin(); }
    const_iterator end() const { return elemente.end(); }

    // FUNCTIE MEMBRU TEMPLATE: returneaza submultimea care satisface un predicat.
    // Predicat este orice tip apelabil (lambda, functor, pointer la functie).
    template <typename Predicat>
    std::vector<T> filtreaza(Predicat predicat) const {
        std::vector<T> rezultat;
        std::copy_if(elemente.begin(), elemente.end(),
                     std::back_inserter(rezultat), predicat);
        return rezultat;
    }

    friend std::ostream& operator<<(std::ostream& os, const Registru<T>& reg) {
        os << "Registru (" << reg.elemente.size() << " elemente):\n";
        for (const auto& el : reg.elemente)
            os << "  - cheie=" << reg.extragereCheie(el) << "\n";
        return os;
    }
};

#endif // REGISTRU_H
