#include "ObservatorCentru.h"

#include "Incident.h"

namespace {
const char* numeEveniment(EvenimentCentru ev) {
    switch (ev) {
        case EvenimentCentru::IncidentInregistrat: return "INREGISTRAT";
        case EvenimentCentru::IncidentEscaladat:   return "ESCALADAT";
        case EvenimentCentru::IncidentRezolvat:    return "REZOLVAT";
        case EvenimentCentru::EchipaAlocata:       return "ECHIPA_ALOCATA";
    }
    return "NECUNOSCUT";
}
}  // namespace

// ---------------------------- JurnalEvenimente ----------------------------

JurnalEvenimente::JurnalEvenimente(std::ostream& flux)
    : flux(flux), numarEvenimente(0) {}

void JurnalEvenimente::laEveniment(EvenimentCentru eveniment,
                                   const Incident* incident,
                                   const std::string& detalii) {
    ++numarEvenimente;
    flux << "[JURNAL] (" << numeEveniment(eveniment) << ") ";
    if (incident != nullptr)
        flux << incident->getTip() << " ID=" << incident->getId() << " ";
    flux << detalii << "\n";
}

// ---------------------------- AlertaSeveritate ----------------------------

AlertaSeveritate::AlertaSeveritate(std::ostream& flux, int pragSeveritate)
    : flux(flux), pragSeveritate(pragSeveritate), numarAlerte(0) {}

void AlertaSeveritate::laEveniment(EvenimentCentru eveniment,
                                   const Incident* incident,
                                   const std::string& /*detalii*/) {
    if (incident == nullptr)
        return;
    if (eveniment != EvenimentCentru::IncidentInregistrat &&
        eveniment != EvenimentCentru::IncidentEscaladat)
        return;

    if (incident->getSeveritate() >= pragSeveritate) {
        ++numarAlerte;
        flux << "[ALERTA] Severitate critica " << incident->getSeveritate()
             << "/5 la " << incident->getTip()
             << " ID=" << incident->getId()
             << " (prag=" << pragSeveritate << ")\n";
    }
}
