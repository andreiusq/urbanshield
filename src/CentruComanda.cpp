#include "CentruComanda.h"
#include "ExceptiiUrbanShield.h"
#include "IncidentToxic.h"
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
      contorOperatii(other.contorOperatii) {
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
}

int CentruComanda::numarEchipeDisponibile() const {
    return static_cast<int>(std::count_if(echipe.begin(), echipe.end(),
                            [](const EchipaInterventie& e) { return e.esteDisponibila(); }));
}

int CentruComanda::numarEchipe() const {
    return static_cast<int>(echipe.size());
}

int CentruComanda::numarIncidenteActive() const {
    return static_cast<int>(std::count_if(incidente.begin(), incidente.end(),
                            [](const auto& i) { return !i->esteRezolvat(); }));
}

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
            if (e.areResursaSuficienta(toxic->resursaCritica(), toxic->cantitateMinimaIzolare()))
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
                }
                continue;
            }

            inc.escaladeazaFaraInterventie();
            if (inc.getSeveritate() != sev) {
                logOperatie("Incident ID=" + std::to_string(inc.getId())
                            + " s-a agravat: severitate " + std::to_string(sev)
                            + " -> " + std::to_string(inc.getSeveritate()));
            }

            if (inc.necesitaInterventieUrgenta())
                alocaEchipaLaIncident(inc.getId());
        }

        for (int idIncident : incidenteRezolvate)
            alocariIncidentEchipa.erase(idIncident);
    }
}

void CentruComanda::genereazaRaport() const {
    std::cout << *this;
    std::cout << "Echipe disponibile: " << numarEchipeDisponibile() << "\n"
              << "Incidente active: " << numarIncidenteActive() << "\n";
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
