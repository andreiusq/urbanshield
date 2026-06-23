#include <exception>
#include <iostream>
#include <vector>

#include "include/Algoritmi.h"
#include "include/CentruComanda.h"
#include "include/EchipaInterventie.h"
#include "include/ExceptiiUrbanShield.h"
#include "include/FabricaIncidente.h"
#include "include/Incident.h"
#include "include/Locatie.h"
#include "include/ObservatorCentru.h"
#include "include/Registru.h"
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

    // === SABLON FACTORY ===
    // Incidentele nu mai sunt construite direct cu "new"/constructor, ci
    // printr-o fabrica ce stie sa creeze orice tip inregistrat dupa nume.
    std::cout << "--- FACTORY: creare incidente prin FabricaIncidente ---\n";
    FabricaIncidente fabrica = FabricaIncidente::standard();
    std::cout << "Tipuri inregistrate in fabrica: ";
    for (const auto& tip : fabrica.tipuriDisponibile())
        std::cout << tip << " ";
    std::cout << "\n";
    std::cout << "Fabrica cunoaste tipul 'CUTREMUR'? "
              << (fabrica.cunoasteTip("CUTREMUR") ? "DA" : "NU") << "\n";
    std::cout << "Fabrica cunoaste tipul 'TORNADA'? "
              << (fabrica.cunoasteTip("TORNADA") ? "DA" : "NU") << "\n\n";

    ParametriIncident pIncendiu;
    pIncendiu.id = 101; pIncendiu.severitate = 4; pIncendiu.locatie = sectorul1;
    pIncendiu.timestamp = "2026-03-13T08:00";
    pIncendiu.descriere = "Incendiu bloc 10 etaje, 50 persoane evacuate";
    pIncendiu.cladireInalta = true;

    ParametriIncident pToxic;
    pToxic.id = 102; pToxic.severitate = 3; pToxic.locatie = sectorul2;
    pToxic.timestamp = "2026-03-13T08:15";
    pToxic.descriere = "Scurgere acid sulfuric depozit industrial";
    pToxic.substanta = "acid sulfuric"; pToxic.zonaIndustriala = true;

    ParametriIncident pAccident;
    pAccident.id = 103; pAccident.severitate = 2; pAccident.locatie = sectorul3;
    pAccident.timestamp = "2026-03-13T08:30";
    pAccident.descriere = "Coliziune multipla, 5 raniti";
    pAccident.persoaneRanite = 5;

    ParametriIncident pCutremur;
    pCutremur.id = 104; pCutremur.severitate = 5; pCutremur.locatie = centruOras;
    pCutremur.timestamp = "2026-03-13T09:00";
    pCutremur.descriere = "Cutremur 6.2 Richter, multiple cladiri avariate";
    pCutremur.magnitudine = 6.2; pCutremur.cladiriAvariate = 12;

    ParametriIncident pInundatie;
    pInundatie.id = 105; pInundatie.severitate = 2; pInundatie.locatie = sectorul3;
    pInundatie.timestamp = "2026-03-13T09:10";
    pInundatie.descriere = "Inundatii subsoluri cartier Rahova";
    pInundatie.zoneAfectate = 3;

    ParametriIncident pPanica;
    pPanica.id = 106; pPanica.severitate = 3; pPanica.locatie = centruOras;
    pPanica.timestamp = "2026-03-13T09:20";
    pPanica.descriere = "Panica in zona centrala dupa replica seismica";
    pPanica.persoaneExpuse = 120;

    auto inc1 = fabrica.creeaza("INCENDIU", pIncendiu);
    std::cout << "--- Incident creat prin FACTORY ---\n";
    std::cout << *inc1 << "\n\n";
    std::cout << "Prioritate incident creat prin fabrica 'CUTREMUR': "
              << fabrica.creeaza("CUTREMUR", pCutremur)->getPrioritate() << "\n\n";

    // === SABLON OBSERVER ===
    // Doi observatori concreti se aboneaza la evenimentele centrului.
    JurnalEvenimente jurnal(std::cout);
    AlertaSeveritate alerta(std::cout, /*pragSeveritate=*/4);

    CentruComanda centru("UrbanShield HQ", centruOras);
    centru.inregistreazaObservator(&jurnal);
    centru.inregistreazaObservator(&alerta);

    centru.adaugaEchipa(echipa1);
    centru.adaugaEchipa(echipa2);
    centru.adaugaEchipa(echipa3);
    centru.adaugaEchipa(echipa4);

    std::cout << "--- OBSERVER: adaugarea incidentelor declanseaza notificari ---\n";
    centru.adaugaIncident(std::move(inc1));
    centru.adaugaIncident(fabrica.creeaza("TOXIC", pToxic));
    centru.adaugaIncident(fabrica.creeaza("ACCIDENT", pAccident));
    centru.adaugaIncident(fabrica.creeaza("CUTREMUR", pCutremur));
    centru.adaugaIncident(fabrica.creeaza("INUNDATIE", pInundatie));
    centru.adaugaIncident(fabrica.creeaza("PANICA_PUBLICA", pPanica));
    std::cout << "\n";

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

    // === CLASA TEMPLATE Registru<T> - a doua instantiere: Registru<const Incident*> ===
    // Index polimorfic al incidentelor active dupa id, cu cheie extrasa
    // dintr-un pointer (strategie de cheie personalizata).
    std::cout << "--- Registru<const Incident*>: interogari polimorfice ---\n";
    Registru<const Incident*> registruIncidente(
        [](const Incident* const& inc) { return inc->getId(); });
    for (const auto* inc : prioritizate)
        registruIncidente.adauga(inc);

    std::cout << "Incidente in registru: " << registruIncidente.dimensiune() << "\n";
    const Incident* cutremurDinRegistru = registruIncidente.gaseste(104);
    std::cout << "Cautare dupa cheie 104 -> " << cutremurDinRegistru->getTip()
              << " (Sev=" << cutremurDinRegistru->getSeveritate() << ")\n";

    // Functie membru template Registru::filtreaza pe incidente critice.
    auto critice = registruIncidente.filtreaza(
        [](const Incident* const& inc) { return inc->getSeveritate() >= 4; });
    std::cout << "Incidente critice (Sev>=4): " << critice.size() << "\n";

    // === FUNCTIE TEMPLATE maximDupa - a doua instantiere (pe Incident*) ===
    const Incident* const* celMaiSever =
        maximDupa(prioritizate.begin(), prioritizate.end(),
                  [](const Incident* inc) { return inc->getSeveritate(); });
    if (celMaiSever != nullptr)
        std::cout << "Cel mai sever incident (maximDupa): " << (*celMaiSever)->getTip()
                  << " ID=" << (*celMaiSever)->getId() << "\n\n";

    std::cout << "--- Alocare echipe la cele mai critice incidente ---\n";
    centru.alocaEchipaLaIncident(104);
    centru.alocaEchipaLaIncident(101);
    centru.alocaEchipaLaIncident(102);
    std::cout << "Echipe disponibile dupa alocare: "
              << centru.numarEchipeDisponibile() << "\n\n";

    centru.simuleazaEvolutie(2);

    std::cout << "\n--- Raport operational (Registru<EchipaInterventie> + sumaDupa) ---\n";
    centru.genereazaRaport();
    std::cout << "\n";

    std::cout << "--- Verificare resurse echipa ---\n";
    std::cout << "Echipa1 are 5 furtunuri? "
              << (echipa1.areResursaSuficienta("furtun_apa", 5) ? "DA" : "NU") << "\n";
    std::cout << "Echipa1 are combinezoane? "
              << (echipa1.areResursaSuficienta("combinezoane_hazmat", 1) ? "DA" : "NU") << "\n\n";

    // FUNCTIE TEMPLATE maximDupa / sumaDupa - instantieri pe tipuri numerice simple.
    std::vector<int> severitatiInitiale = {4, 3, 2, 5, 2, 3};
    const int* maxSev = maximDupa(severitatiInitiale.begin(), severitatiInitiale.end(),
                                  [](int s) { return s; });
    int totalSev = sumaDupa(severitatiInitiale.begin(), severitatiInitiale.end(),
                            [](int s) { return s; });
    std::cout << "Severitate maxima initiala (maximDupa<int>): "
              << (maxSev != nullptr ? *maxSev : 0)
              << " | suma severitati (sumaDupa<int>): " << totalSev << "\n\n";

    try {
        Resursa apa("apa", 1, "buc");
        apa.consuma(5);
    } catch (const UrbanShieldException& ex) {
        std::cout << "Exceptie UrbanShield prinsa in main: " << ex.what() << "\n";
    }

    try {
        fabrica.creeaza("TIP_INEXISTENT", pAccident);
    } catch (const UrbanShieldException& ex) {
        std::cout << "Exceptie Factory prinsa in main: " << ex.what() << "\n";
    }

    std::cout << "Evenimente jurnalizate de observator: " << jurnal.totalEvenimente() << "\n";
    std::cout << "Alerte de severitate emise: " << alerta.totalAlerte() << "\n";
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
