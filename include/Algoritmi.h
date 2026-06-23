#ifndef ALGORITMI_H
#define ALGORITMI_H

#include <iterator>
#include <ostream>

// =====================================================================
// FUNCTII TEMPLATE libere, refolosibile pe orice colectie iterabila.
// ---------------------------------------------------------------------
// Instantiate in proiect pentru tipuri diferite (Incident*, EchipaInterventie,
// int, double etc.), deci satisfac cerinta de minim 2 instantieri.
// =====================================================================

// Returneaza un pointer catre elementul care maximizeaza o proiectie numerica.
// `Proiectie` este orice apelabil care primeste un element si intoarce o valoare
// comparabila (ex: prioritatea unui incident, resursele totale ale unei echipe).
// Returneaza nullptr daca intervalul este gol.
template <typename Iterator, typename Proiectie>
auto maximDupa(Iterator inceput, Iterator sfarsit, Proiectie proiectie)
    -> typename std::iterator_traits<Iterator>::pointer {
    using Pointer = typename std::iterator_traits<Iterator>::pointer;
    if (inceput == sfarsit)
        return Pointer{nullptr};

    Iterator cel_mai_bun = inceput;
    auto scor_maxim = proiectie(*inceput);
    for (Iterator it = std::next(inceput); it != sfarsit; ++it) {
        auto scor = proiectie(*it);
        if (scor > scor_maxim) {
            scor_maxim = scor;
            cel_mai_bun = it;
        }
    }
    return &(*cel_mai_bun);
}

// Aduna proiectiile numerice ale tuturor elementelor dintr-un interval.
// Folosita pentru a agrega statistici (ex: total severitati, total resurse).
template <typename Iterator, typename Proiectie>
auto sumaDupa(Iterator inceput, Iterator sfarsit, Proiectie proiectie)
    -> decltype(proiectie(*inceput)) {
    decltype(proiectie(*inceput)) total{};
    for (Iterator it = inceput; it != sfarsit; ++it)
        total += proiectie(*it);
    return total;
}

// Afiseaza toate elementele dintr-un interval, unul pe linie, cu un prefix.
// Functioneaza pentru orice T care are operator<< definit.
template <typename Iterator>
void afiseazaToate(std::ostream& os, Iterator inceput, Iterator sfarsit,
                   const char* prefix = "  ") {
    for (Iterator it = inceput; it != sfarsit; ++it)
        os << prefix << *it << "\n";
}

#endif // ALGORITMI_H
