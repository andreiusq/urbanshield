#include <exception>
#include <iostream>

#include "include/CentruComanda.h"
#include "include/EchipaInterventie.h"
#include "include/ExceptiiUrbanShield.h"
#include "include/IncidentAccident.h"
#include "include/IncidentCutremur.h"
#include "include/IncidentIncendiu.h"
#include "include/IncidentInundatie.h"
#include "include/IncidentPanicaPublica.h"
#include "include/IncidentToxic.h"
#include "include/Locatie.h"
#include "include/Resursa.h"

static int ruleazaDemoUrbanShield() {
    std::cout << "UrbanShield - Crisis Management Simulator\n";

    Locatie centruOras(44.4268, 26.1025, "Centrul Orasului");
    Locatie sectorul1(44.4500, 26.0800, "Sectorul 1 - Herastrau");
    Locatie sectorul2(44.4100, 26.1300, "Sectorul 2 - Colentina");
    Locatie sectorul3(44.3900, 26.0600, "Sectorul 3 - Rahova");
    Locatie bazaPompieri(44.4350, 26.0950, "Baza Pompieri Centrala");
    Locatie bazaMedical(44.4200, 26.1100, "Spitalul de Urgenta");

    std::cout << "--- Locatii create ---\n";
    std::cout << centruOras << "\n";
    std::cout << sectorul1 << "\n\n";
    std::cout << "Distanta Centru -> Sectorul1: "
              << centruOras.distantaFata(sectorul1) << " km\n";
    std::cout << "Sectorul2 este in 5km fata de centru? "
              << (sectorul2.esteInZona(centruOras, 5.0) ? "DA" : "NU") << "\n\n";

    Resursa furtunApa("furtun_apa", 10, "buc");
    Resursa trusaMedicala("trusa_medicala", 5, "buc");
    Resursa combinezoane("combinezoane_hazmat", 8, "buc");
    Resursa generatoare("generator", 3, "buc");
    Resursa franga("franghii_salvare", 15, "buc");

    std::cout << "--- Resurse create ---\n";
    std::cout << furtunApa << "\n";
    std::cout << trusaMedicala << "\n\n";

    furtunApa.consuma(2);
    std::cout << "Dupa consum 2 furtunuri: " << furtunApa << "\n";
    furtunApa.reincarcare(2);
    std::cout << "Dupa reincarcare 2 furtunuri: " << furtunApa << "\n\n";

    EchipaInterventie echipa1(1, "Pompierii Rosii", "POMPIERI", bazaPompieri);
    echipa1.adaugaResursa(furtunApa);
    echipa1.adaugaResursa(generatoare);

    EchipaInterventie echipa2(2, "SMURD Alpha", "MEDICAL", bazaMedical);
    echipa2.adaugaResursa(trusaMedicala);
    echipa2.adaugaResursa(franga);

    EchipaInterventie echipa3(3, "Echipa HAZMAT", "HAZMAT", centruOras);
    echipa3.adaugaResursa(combinezoane);
    echipa3.adaugaResursa(trusaMedicala);

    EchipaInterventie echipa4(4, "Salvare Urbana", "SALVARE", sectorul1);
    echipa4.adaugaResursa(franga);
    echipa4.adaugaResursa(generatoare);

    EchipaInterventie echipaRezervaPompieri(echipa1);
    echipaRezervaPompieri.adaugaResursa(Resursa("stingator", 4, "buc"));
    std::cout << "Original: " << echipa1.getNume()
              << " | resurse: " << echipa1.totalResurseDisponibile() << "\n";
    std::cout << "Copie:    " << echipaRezervaPompieri.getNume()
              << " | resurse: " << echipaRezervaPompieri.totalResurseDisponibile() << "\n\n";

    EchipaInterventie echipaTemp(99, "Temp", "MEDICAL", centruOras);
    echipaTemp = echipa2;
    std::cout << "Dupa operator= EchipaInterventie: " << echipaTemp.getNume()
              << " | disponibila: " << (echipaTemp.esteDisponibila() ? "DA" : "NU") << "\n\n";

    IncidentIncendiu inc1(101, 4, sectorul1, "2026-03-13T08:00",
                          "Incendiu bloc 10 etaje, 50 persoane evacuate", true);
    IncidentToxic inc2(102, 3, sectorul2, "2026-03-13T08:15",
                       "Scurgere acid sulfuric depozit industrial", "acid sulfuric", true);
    IncidentAccident inc3(103, 2, sectorul3, "2026-03-13T08:30",
                          "Coliziune multipla, 5 raniti", 5);
    IncidentCutremur inc4(104, 5, centruOras, "2026-03-13T09:00",
                          "Cutremur 6.2 Richter, multiple cladiri avariate", 6.2, 12);
    IncidentInundatie inc5(105, 2, sectorul3, "2026-03-13T09:10",
                           "Inundatii subsoluri cartier Rahova", 3);
    IncidentPanicaPublica inc6(106, 3, centruOras, "2026-03-13T09:20",
                               "Panica in zona centrala dupa replica seismica", 120);

    std::cout << "--- Incident creat prin ierarhie polimorfica ---\n";
    std::cout << inc1 << "\n\n";
    std::cout << "Prioritate incident CUTREMUR: " << inc4.getPrioritate() << "\n";
    std::cout << "Prioritate incident ACCIDENT: " << inc3.getPrioritate() << "\n\n";

    CentruComanda centru("UrbanShield HQ", centruOras);

    centru.adaugaEchipa(echipa1);
    centru.adaugaEchipa(echipa2);
    centru.adaugaEchipa(echipa3);
    centru.adaugaEchipa(echipa4);

    centru.adaugaIncident(inc1);
    centru.adaugaIncident(inc2);
    centru.adaugaIncident(inc3);
    centru.adaugaIncident(inc4);
    centru.adaugaIncident(inc5);
    centru.adaugaIncident(inc6);

    CentruComanda copieCentru("Copie temporara", centruOras);
    copieCentru = centru;
    std::cout << "Copie CentruComanda prin copy-and-swap: "
              << copieCentru.numarIncidenteActive() << " incidente active\n\n";

    std::cout << "--- Stare initiala centru de comanda ---\n";
    std::cout << centru;
    std::cout << "Echipe disponibile: " << centru.numarEchipeDisponibile() << "\n";
    std::cout << "Incidente active:   " << centru.numarIncidenteActive() << "\n\n";

    std::cout << "--- Incidente prioritizate (descrescator) ---\n";
    auto prioritizate = centru.getIncidentePrioritizate();
    for (const auto* inc : prioritizate)
        std::cout << "  [P=" << inc->getPrioritate() << "] "
                  << inc->getTip() << " ID=" << inc->getId()
                  << " Sev=" << inc->getSeveritate()
                  << " Spec=" << inc->specializareNecesara() << "\n";
    std::cout << "\n";

    std::cout << "--- Alocare echipe la cele mai critice incidente ---\n";
    centru.alocaEchipaLaIncident(104);
    centru.alocaEchipaLaIncident(101);
    centru.alocaEchipaLaIncident(102);
    std::cout << "Echipe disponibile dupa alocare: "
              << centru.numarEchipeDisponibile() << "\n\n";

    centru.simuleazaEvolutie(2);

    std::cout << "--- Verificare resurse echipa ---\n";
    std::cout << "Echipa1 are 5 furtunuri? "
              << (echipa1.areResursaSuficienta("furtun_apa", 5) ? "DA" : "NU") << "\n";
    std::cout << "Echipa1 are combinezoane? "
              << (echipa1.areResursaSuficienta("combinezoane_hazmat", 1) ? "DA" : "NU") << "\n\n";

    try {
        Resursa apa("apa", 1, "buc");
        apa.consuma(5);
    } catch (const UrbanShieldException& ex) {
        std::cout << "Exceptie UrbanShield prinsa in main: " << ex.what() << "\n";
    }

    std::cout << "Incidente create/clonate in sesiune: "
              << Incident::getTotalIncidenteCreate() << "\n";
    std::cout << "Simulare finalizata.\n";

    return 0;
}

int main() {
    try {
        return ruleazaDemoUrbanShield();
    } catch (const UrbanShieldException& ex) {
        std::cerr << "Eroare UrbanShield: " << ex.what() << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Eroare neasteptata: " << ex.what() << "\n";
    }
    return 1;
}
