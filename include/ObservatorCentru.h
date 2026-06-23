#ifndef OBSERVATOR_CENTRU_H
#define OBSERVATOR_CENTRU_H

#include <ostream>
#include <string>

class Incident;

// =====================================================================
// SABLON DE PROIECTARE: OBSERVER
// ---------------------------------------------------------------------
// Subiectul observat este CentruComanda. Cand se petrec evenimente
// operationale (incident inregistrat, escaladat, rezolvat, echipa
// alocata), centrul notifica toti observatorii inregistrati.
//
// ObservatorCentru este interfata abstracta (Observer). Implementarile
// concrete reactioneaza diferit: jurnalizare, alerte de severitate etc.
// =====================================================================

enum class EvenimentCentru {
    IncidentInregistrat,
    IncidentEscaladat,
    IncidentRezolvat,
    EchipaAlocata
};

class ObservatorCentru {
public:
    virtual ~ObservatorCentru() = default;

    // Notificare cu referinta la incidentul implicat (poate fi nullptr
    // pentru evenimente fara incident asociat).
    virtual void laEveniment(EvenimentCentru eveniment,
                             const Incident* incident,
                             const std::string& detalii) = 0;
};

// ---------------------------------------------------------------------
// Observator concret 1: jurnalizeaza fiecare eveniment intr-un ostream.
// ---------------------------------------------------------------------
class JurnalEvenimente : public ObservatorCentru {
    std::ostream& flux;
    int numarEvenimente;

public:
    explicit JurnalEvenimente(std::ostream& flux);

    void laEveniment(EvenimentCentru eveniment,
                     const Incident* incident,
                     const std::string& detalii) override;

    int totalEvenimente() const { return numarEvenimente; }
};

// ---------------------------------------------------------------------
// Observator concret 2: numara escaladarile critice si emite alerte
// cand un incident depaseste un prag de severitate.
// ---------------------------------------------------------------------
class AlertaSeveritate : public ObservatorCentru {
    std::ostream& flux;
    int pragSeveritate;
    int numarAlerte;

public:
    AlertaSeveritate(std::ostream& flux, int pragSeveritate);

    void laEveniment(EvenimentCentru eveniment,
                     const Incident* incident,
                     const std::string& detalii) override;

    int totalAlerte() const { return numarAlerte; }
};

#endif // OBSERVATOR_CENTRU_H
