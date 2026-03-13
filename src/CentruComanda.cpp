#include "CentruComanda.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

CentruComanda::CentruComanda(const std::string& nume, const Locatie& loc)
    : numeCentru(nume), locatieCentru(loc), echipe(), incidente(), contorOperatii(0) {}

void CentruComanda::adaugaEchipa(const EchipaInterventie& e) {
    echipe.push_back(e);
}

void CentruComanda::adaugaIncident(const Incident& inc) {
    incidente.push_back(inc);
    logOperatie("Incident nou inregistrat: ID=" + std::to_string(inc.getId())
                + " tip=" + inc.getTip());
}

int CentruComanda::numarEchipeDisponibile() const {
    return static_cast<int>(std::count_if(echipe.begin(), echipe.end(),
                            [](const EchipaInterventie& e) { return e.esteDisponibila(); }));
}

int CentruComanda::numarIncidenteActive() const {
    return static_cast<int>(std::count_if(incidente.begin(), incidente.end(),
                            [](const Incident& i) { return !i.esteRezolvat(); }));
}

void CentruComanda::logOperatie(const std::string& mesaj) {
    ++contorOperatii;
    std::cout << "[LOG #" << contorOperatii << "] " << mesaj << "\n";
}

// scor pt echipa = (specializare potrivita ? 100 : 0) + resurse_disponibile - distanta*2
int CentruComanda::gasesteEchipaOptima(const Incident& inc) const {
    int bestIdx = -1;
    double bestScor = -1.0;

    for (int i = 0; i < static_cast<int>(echipe.size()); ++i) {
        const auto& e = echipe[i];
        if (!e.esteDisponibila()) continue;

        double scor = 0.0;

        // Potrivire specializare -> incident tip
        static const std::map<std::string, std::string> specMap = {
            {"INCENDIU",   "POMPIERI"},
            {"INUNDATIE",  "POMPIERI"},
            {"TOXIC_SPILL","HAZMAT"},
            {"CUTREMUR",   "SALVARE"},
            {"ACCIDENT",   "MEDICAL"}
        };
        auto it = specMap.find(inc.getTip());
        if (it != specMap.end() && e.getSpecializare() == it->second)
            scor += 100.0;

        // Resurse disponibile (mai multe = mai bine)
        scor += e.totalResurseDisponibile();

        // Proximitate fata de incident (distanta mica = scor mai mare)
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
std::vector<Incident> CentruComanda::getIncidentePrioritizate() const {
    std::vector<Incident> active;
    for (const auto& inc : incidente) {
        if (!inc.esteRezolvat())
            active.push_back(inc);
    }
    std::sort(active.begin(), active.end(),
              [](const Incident& a, const Incident& b) {
                  return a.getPrioritate() > b.getPrioritate();
              });
    return active;
}

// alocare inteligenta (dar PROASTA) de echipa la incident
bool CentruComanda::alocaEchipaLaIncident(int idIncident) {
    // Cauta incidentul
    Incident* incPtr = nullptr;
    for (auto& inc : incidente) {
        if (inc.getId() == idIncident && !inc.esteRezolvat()) {
            incPtr = &inc;
            break;
        }
    }
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
    logOperatie("Echipa '" + echipe[idx].getNume() + "' alocata la incident ID="
                + std::to_string(idIncident));
    return true;
}

// simularea evolutiei crizei pe mai multi pasi
// La fiecare pas:
//   - incidentele nerezolvate fara echipa alocata escaladeaza severitatea cu 1
//   - incidentele cu severitate maxima (5) si inca nerezolvate raman la 5
//   - se incearca realocarea de echipe pentru incidentele critice (sev >= 4)
//   - echipele care nu au mai fost alocate in acest pas sunt eliberate (simuleaza terminarea interventiei)
void CentruComanda::simuleazaEvolutie(int pasi) {
    for (int pas = 1; pas <= pasi; ++pas) {
        std::cout << "\n=== Pas simulare #" << pas << " ===\n";

        for (auto& e : echipe)
            e.seteazaDisponibila(true);

        for (auto& inc : incidente) {
            if (inc.esteRezolvat()) continue;

            int sev = inc.getSeveritate();

            // incidentele care nu au pe nimeni cresc in severitate
            if (sev < 5) {
                inc.actualizeazaSeveritate(sev + 1);
                logOperatie("Incident ID=" + std::to_string(inc.getId())
                            + " s-a agravat: severitate " + std::to_string(sev)
                            + " -> " + std::to_string(sev + 1));
            }

            // incidentele cu severitate mica (1) dupa escaladare = rezolvate automat
            if (inc.getSeveritate() == 1) {
                inc.marcheazaRezolvat();
                logOperatie("Incident ID=" + std::to_string(inc.getId()) + " rezolvat automat.");
            }

            // incidentele critice (sev >= 4): realoc urgenta
            if (inc.getSeveritate() >= 4 && !inc.esteRezolvat()) {
                alocaEchipaLaIncident(inc.getId());
            }
        }
    }
}


std::ostream& operator<<(std::ostream& os, const CentruComanda& cc) {
    os << "=== CentruComanda: " << cc.numeCentru << " ===\n"
       << "Locatie centru: " << cc.locatieCentru << "\n"
       << "Echipe inregistrate: " << cc.echipe.size() << "\n";
    for (const auto& e : cc.echipe)
        os << e << "\n";
    os << "Incidente inregistrate: " << cc.incidente.size() << "\n";
    for (const auto& inc : cc.incidente)
        os << inc << "\n\n";
    return os;
}
