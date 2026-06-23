#include "CentruComanda.h"
#include "ExceptiiUrbanShield.h"
#include "IncidentToxic.h"
#include "Algoritmi.h"
#include "Registru.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <utility>

CentruComanda::CentruComanda(const std::string& nume, const Locatie& loc)
    : numeCentru(nume), locatieCentru(loc), echipe(), incidente(),
      alocariIncidentEchipa(), contorOperatii(0) {}

CentruComanda::CentruComanda(const CentruComanda& other)
    : numeCentru(other.numeCentru), locatieCentru(other.locatieCentru),
      echipe(other.echipe), incidente(), alocariIncidentEchipa(other.alocariIncidentEchipa),
      contorOperatii(other.contorOperatii), observatori() {
    // Observatorii nu se copiaza: ei sunt detinuti si conectati extern,
    // iar o copie a centrului porneste fara abonati.
    incidente.reserve(other.incidente.size());
    for (const auto& inc : other.incidente)
        incidente.push_back(inc->clone());
}

CentruComanda& CentruComanda::operator=(CentruComanda other) {
    swap(*this, other);
    return *this;
}

CentruComanda::~CentruComanda() = default;

void swap(CentruComanda& first, CentruComanda& second) noexcept {
    using std::swap;
    swap(first.numeCentru, second.numeCentru);
    swap(first.locatieCentru, second.locatieCentru);
    swap(first.echipe, second.echipe);
    swap(first.incidente, second.incidente);
    swap(first.alocariIncidentEchipa, second.alocariIncidentEchipa);
    swap(first.contorOperatii, second.contorOperatii);
    swap(first.observatori, second.observatori);
}

void CentruComanda::inregistreazaObservator(ObservatorCentru* obs) {
    if (obs == nullptr)
        throw DateInvalideException("Nu se poate inregistra un observator null.");
    if (std::find(observatori.begin(), observatori.end(), obs) == observatori.end())
        observatori.push_back(obs);
}

// cppcheck-suppress unusedFunction
void CentruComanda::dezabonezaObservator(ObservatorCentru* obs) {
    observatori.erase(std::remove(observatori.begin(), observatori.end(), obs),
                      observatori.end());
}

void CentruComanda::notifica(EvenimentCentru eveniment, const Incident* incident,
                             const std::string& detalii) const {
    for (auto* obs : observatori)
        obs->laEveniment(eveniment, incident, detalii);
}

void CentruComanda::adaugaEchipa(const EchipaInterventie& e) {
    echipe.push_back(e);
}

void CentruComanda::adaugaIncident(const Incident& inc) {
    adaugaIncident(inc.clone());
}

void CentruComanda::adaugaIncident(std::unique_ptr<Incident> inc) {
    if (!inc)
        throw DateInvalideException("Nu se poate adauga un incident null.");
    const int id = inc->getId();
    const std::string tip = inc->getTip();
    incidente.push_back(std::move(inc));
    logOperatie("Incident nou inregistrat: ID=" + std::to_string(id) + " tip=" + tip);
    notifica(EvenimentCentru::IncidentInregistrat, incidente.back().get(),
             "inregistrat in centru");
}

int CentruComanda::numarEchipeDisponibile() const {
    return static_cast<int>(std::count_if(echipe.begin(), echipe.end(),
                            [](const EchipaInterventie& e) { return e.esteDisponibila(); }));
}

// cppcheck-suppress unusedFunction
int CentruComanda::numarEchipe() const {
    return static_cast<int>(echipe.size());
}

int CentruComanda::numarIncidenteActive() const {
    return static_cast<int>(std::count_if(incidente.begin(), incidente.end(),
                            [](const auto& i) { return !i->esteRezolvat(); }));
}

// cppcheck-suppress unusedFunction
const std::vector<EchipaInterventie>& CentruComanda::getEchipe() const {
    return echipe;
}

void CentruComanda::logOperatie(const std::string& mesaj) {
    ++contorOperatii;
    std::cout << "[LOG #" << contorOperatii << "] " << mesaj << "\n";
}

Incident* CentruComanda::gasesteIncidentActiv(int idIncident) {
    for (const auto& inc : incidente) {
        if (inc->getId() == idIncident && !inc->esteRezolvat())
            return inc.get();
    }
    return nullptr;
}

const Incident* CentruComanda::gasesteIncidentActiv(int idIncident) const {
    for (const auto& inc : incidente) {
        if (inc->getId() == idIncident && !inc->esteRezolvat())
            return inc.get();
    }
    return nullptr;
}

void CentruComanda::elibereazaEchipa(int idEchipa) {
    for (auto& e : echipe) {
        if (e.getId() == idEchipa) {
            e.seteazaDisponibila(true);
            return;
        }
    }
}

// scor = potrivire specializare + resurse critice + proximitate
int CentruComanda::gasesteEchipaOptima(const Incident& inc) const {
    int bestIdx = -1;
    double bestScor = -1.0;

    for (int i = 0; i < static_cast<int>(echipe.size()); ++i) {
        const auto& e = echipe[i];
        if (!e.esteDisponibila()) continue;

        double scor = 0.0;

        // Apel virtual prin pointer/referinta la baza Incident.
        if (e.getSpecializare() == inc.specializareNecesara())
            scor += 100.0;

        for (const auto& resursa : inc.resurseCritice()) {
            if (e.areResursaSuficienta(resursa, 1))
                scor += 15.0;
        }

        if (const auto* toxic = dynamic_cast<const IncidentToxic*>(&inc)) {
            if (e.areResursaSuficienta(IncidentToxic::resursaCritica(),
                                       toxic->cantitateMinimaIzolare()))
                scor += 30.0;
            else
                scor -= 45.0;
        }

        scor += e.totalResurseDisponibile();

        double dist = e.getLocatie().distantaFata(inc.getLocatie());
        scor -= dist * 2.0;

        if (scor > bestScor) {
            bestScor = scor;
            bestIdx = i;
        }
    }
    return bestIdx;
}

// prioritizare automata a incidentelor nerezolvate
std::vector<const Incident*> CentruComanda::getIncidentePrioritizate() const {
    std::vector<const Incident*> active;
    for (const auto& inc : incidente) {
        if (!inc->esteRezolvat())
            active.push_back(inc.get());
    }
    std::sort(active.begin(), active.end(),
              [](const Incident* a, const Incident* b) {
                  return a->getPrioritate() > b->getPrioritate();
              });
    return active;
}

// Foloseste functia template maximDupa pe un vector de Incident*.
// (a doua instantiere a sablonului maximDupa, vezi si genereazaRaport)
const Incident* CentruComanda::incidentCelMaiPrioritar() const {
    auto active = getIncidentePrioritizate();
    const Incident* const* gasit =
        maximDupa(active.begin(), active.end(),
                  [](const Incident* inc) { return inc->getPrioritate(); });
    return gasit != nullptr ? *gasit : nullptr;
}

// alocare inteligenta de echipa la incident
bool CentruComanda::alocaEchipaLaIncident(int idIncident) {
    if (alocariIncidentEchipa.find(idIncident) != alocariIncidentEchipa.end()) {
        logOperatie("Incident ID=" + std::to_string(idIncident) + " are deja echipa alocata.");
        return true;
    }

    const Incident* incPtr = gasesteIncidentActiv(idIncident);
    if (!incPtr) {
        logOperatie("Incident ID=" + std::to_string(idIncident) + " nu a fost gasit sau e rezolvat.");
        return false;
    }

    int idx = gasesteEchipaOptima(*incPtr);
    if (idx == -1) {
        logOperatie("Nicio echipa disponibila pentru incident ID=" + std::to_string(idIncident));
        return false;
    }

    echipe[idx].seteazaDisponibila(false);
    echipe[idx].actualizeazaLocatie(incPtr->getLocatie());
    alocariIncidentEchipa[idIncident] = echipe[idx].getId();
    logOperatie("Echipa '" + echipe[idx].getNume() + "' alocata la incident ID="
                + std::to_string(idIncident));
    notifica(EvenimentCentru::EchipaAlocata, incPtr,
             "echipa '" + echipe[idx].getNume() + "'");
    return true;
}

// simularea evolutiei crizei pe mai multi pasi
void CentruComanda::simuleazaEvolutie(int pasi) {
    if (pasi <= 0)
        throw DateInvalideException("Numarul de pasi de simulare trebuie sa fie pozitiv.");

    for (int pas = 1; pas <= pasi; ++pas) {
        std::cout << "\n=== Pas simulare #" << pas << " ===\n";

        std::vector<int> incidenteRezolvate;

        for (auto& incPtr : incidente) {
            Incident& inc = *incPtr;
            if (inc.esteRezolvat()) continue;

            int sev = inc.getSeveritate();
            auto alocare = alocariIncidentEchipa.find(inc.getId());

            if (alocare != alocariIncidentEchipa.end()) {
                inc.aplicaInterventie();
                logOperatie("Incident ID=" + std::to_string(inc.getId())
                            + " stabilizat de echipa alocata: severitate "
                            + std::to_string(sev) + " -> "
                            + std::to_string(inc.getSeveritate()));
                if (inc.esteRezolvat()) {
                    elibereazaEchipa(alocare->second);
                    incidenteRezolvate.push_back(inc.getId());
                    logOperatie("Incident ID=" + std::to_string(inc.getId()) + " rezolvat.");
                    notifica(EvenimentCentru::IncidentRezolvat, &inc, "stabilizat complet");
                }
                continue;
            }

            inc.escaladeazaFaraInterventie();
            if (inc.getSeveritate() != sev) {
                logOperatie("Incident ID=" + std::to_string(inc.getId())
                            + " s-a agravat: severitate " + std::to_string(sev)
                            + " -> " + std::to_string(inc.getSeveritate()));
                notifica(EvenimentCentru::IncidentEscaladat, &inc,
                         "severitate " + std::to_string(sev) + " -> "
                         + std::to_string(inc.getSeveritate()));
            }

            if (inc.necesitaInterventieUrgenta())
                alocaEchipaLaIncident(inc.getId());
        }

        for (int idIncident : incidenteRezolvate)
            alocariIncidentEchipa.erase(idIncident);
    }
}

// cppcheck-suppress unusedFunction
void CentruComanda::genereazaRaport() const {
    std::cout << *this;
    std::cout << "Echipe disponibile: " << numarEchipeDisponibile() << "\n"
              << "Incidente active: " << numarIncidenteActive() << "\n";

    // Instantiere Registru<EchipaInterventie>: index al echipelor dupa id,
    // folosit pentru interogari rapide in raport.
    Registru<EchipaInterventie> registruEchipe;
    for (const auto& e : echipe)
        registruEchipe.adauga(e);

    // Functie membru template Registru::filtreaza + functie template sumaDupa.
    auto disponibile = registruEchipe.filtreaza(
        [](const EchipaInterventie& e) { return e.esteDisponibila(); });
    int resurseDisponibile = sumaDupa(
        disponibile.begin(), disponibile.end(),
        [](const EchipaInterventie& e) { return e.totalResurseDisponibile(); });

    std::cout << "Resurse totale in echipe disponibile: "
              << resurseDisponibile << "\n";

    const Incident* top = incidentCelMaiPrioritar();
    if (top != nullptr)
        std::cout << "Cel mai prioritar incident activ: " << top->getTip()
                  << " ID=" << top->getId()
                  << " (P=" << top->getPrioritate() << ")\n";
}

std::ostream& operator<<(std::ostream& os, const CentruComanda& cc) {
    os << "=== CentruComanda: " << cc.numeCentru << " ===\n"
       << "Locatie centru: " << cc.locatieCentru << "\n"
       << "Echipe inregistrate: " << cc.echipe.size() << "\n";
    for (const auto& e : cc.echipe)
        os << e << "\n";
    os << "Incidente inregistrate: " << cc.incidente.size() << "\n";
    for (const auto& inc : cc.incidente)
        os << *inc << "\n\n";
    os << "Alocari active: " << cc.alocariIncidentEchipa.size() << "\n";
    return os;
}
