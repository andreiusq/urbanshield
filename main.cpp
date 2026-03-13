#include <iostream>
#include "include/Locatie.h"
#include "include/Resursa.h"
#include "include/EchipaInterventie.h"
#include "include/Incident.h"
#include "include/CentruComanda.h"

int main() {
    std::cout << "UrbanShield - Crisis Management Simulator\n";

    // --- 1. Creare locatii ---
    Locatie centruOras(44.4268, 26.1025, "Centrul Orasului");
    Locatie sectorul1(44.4500, 26.0800, "Sectorul 1 - Herastrau");
    Locatie sectorul2(44.4100, 26.1300, "Sectorul 2 - Colentina");
    Locatie sectorul3(44.3900, 26.0600, "Sectorul 3 - Rahova");
    Locatie bazaPompieri(44.4350, 26.0950, "Baza Pompieri Centrala");
    Locatie bazaMedical(44.4200, 26.1100, "Spitalul de Urgenta");

    std::cout << "--- Locatii create ---\n";
    std::cout << centruOras << "\n";
    std::cout << sectorul1 << "\n\n";

    // Test distanta si zona
    std::cout << "Distanta Centru -> Sectorul1: "
              << centruOras.distantaFata(sectorul1) << " km\n";
    std::cout << "Sectorul2 este in 5km fata de centru? "
              << (sectorul2.esteInZona(centruOras, 5.0) ? "DA" : "NU") << "\n\n";

    // --- 2. Creare resurse ---
    Resursa furtunApa("furtun_apa", 10, "buc");
    Resursa trusaMedicala("trusa_medicala", 5, "buc");
    Resursa combinezoane("combinezoane_hazmat", 8, "buc");
    Resursa generatoare("generator", 3, "buc");
    Resursa franga("franghii_salvare", 15, "buc");

    std::cout << "--- Resurse create ---\n";
    std::cout << furtunApa << "\n";
    std::cout << trusaMedicala << "\n\n";

    // DEBUG IN PLM Test consum resursa
    furtunApa.consuma(2);
    std::cout << "Dupa consum 2 furtunuri: " << furtunApa << "\n";
    furtunApa.reincarcare(2);
    std::cout << "Dupa reincarcare 2 furtunuri: " << furtunApa << "\n\n";

    // --- 3. Creare echipe de interventie ---
    EchipaInterventie echipa1(1, "Pompierii Rosii", "POMPIERI", bazaPompieri);
    echipa1.adaugaResursa(furtunApa);
    echipa1.adaugaResursa(generatoare);

    EchipaInterventie echipa2(2, "SMURD Alpha", "MEDICAL", bazaMedical);
    echipa2.adaugaResursa(trusaMedicala);
    echipa2.adaugaResursa(franga);

    EchipaInterventie echipa3(3, "Echipa HAZMAT", "HAZMAT", centruOras);
    echipa3.adaugaResursa(combinezoane);
    echipa3.adaugaResursa(trusaMedicala);

    EchipaInterventie echipa4(4, "Salvare Montana", "SALVARE", sectorul1);
    echipa4.adaugaResursa(franga);
    echipa4.adaugaResursa(generatoare);

    EchipaInterventie echipaRezervaPompieri(echipa1);
    echipaRezervaPompieri.adaugaResursa(Resursa("stingator", 4, "buc"));
    std::cout << "Original: " << echipa1.getNume()
              << " | resurse: " << echipa1.totalResurseDisponibile() << "\n";
    std::cout << "Copie:    " << echipaRezervaPompieri.getNume()
              << " | resurse: " << echipaRezervaPompieri.totalResurseDisponibile() << "\n\n";

    std::cout << "--- Regula Celor 3: operator= de copiere ---\n";
    EchipaInterventie echipaTemp(99, "Temp", "MEDICAL", centruOras);
    echipaTemp = echipa2; 
    std::cout << "Dupa operator=: " << echipaTemp.getNume()
              << " | disponibila: " << (echipaTemp.esteDisponibila() ? "DA" : "NU") << "\n\n";

    std::cout << "--- Detalii echipa ---\n";
    std::cout << echipa1 << "\n";

    // --- 4. Creare incidente ---
    Incident inc1(101, "INCENDIU", 4, sectorul1, "2026-03-13T08:00",
                  "Incendiu bloc 10 etaje, 50 persoane evacuate");
    Incident inc2(102, "TOXIC_SPILL", 3, sectorul2, "2026-03-13T08:15",
                  "Scurgere acid sulfuric depozit industrial");
    Incident inc3(103, "ACCIDENT", 2, sectorul3, "2026-03-13T08:30",
                  "Coliziune multipla autostrada, 5 raniti");
    Incident inc4(104, "CUTREMUR", 5, centruOras, "2026-03-13T09:00",
                  "Cutremur 6.2 Richter, multiple cladiri avariate");
    Incident inc5(105, "INUNDATIE", 2, sectorul3, "2026-03-13T09:10",
                  "Inundatii subsoluri cartier Rahova");

    std::cout << "--- Incident creat ---\n";
    std::cout << inc1 << "\n\n";
    std::cout << "Prioritate incident CUTREMUR: " << inc4.getPrioritate() << "\n";
    std::cout << "Prioritate incident ACCIDENT: " << inc3.getPrioritate() << "\n\n";

    // --- 5. Creare Centru de Comanda ---
    CentruComanda centru("UrbanShield HQ - Tibi tu chiar citești toate liniile de cod? ", centruOras);

    centru.adaugaEchipa(echipa1);
    centru.adaugaEchipa(echipa2);
    centru.adaugaEchipa(echipa3);
    centru.adaugaEchipa(echipa4);

    centru.adaugaIncident(inc1);
    centru.adaugaIncident(inc2);
    centru.adaugaIncident(inc3);
    centru.adaugaIncident(inc4);
    centru.adaugaIncident(inc5);

    // --- 6. Stare inițială ---
    std::cout << "\n--- Stare initiala centru de comanda ---\n";
    std::cout << "\nNu de alta, da' eu n-as citi atata yap de la un student\n"
    std::cout << centru;

    std::cout << "Echipe disponibile: " << centru.numarEchipeDisponibile() << "\n";
    std::cout << "Incidente active:   " << centru.numarIncidenteActive() << "\n\n";

    // --- 7. incidente prioritizate ---
    std::cout << "--- Incidente prioritizate (descrescator) ---\n";
    auto prioritizate = centru.getIncidentePrioritizate();
    for (const auto& inc : prioritizate)
        std::cout << "  [P=" << inc.getPrioritate() << "] "
                  << inc.getTip() << " ID=" << inc.getId()
                  << " Sev=" << inc.getSeveritate() << "\n";
    std::cout << "\n";

    // --- 8. Alocare echipe la incidente ---
    std::cout << "--- Alocare echipe la cele mai critice incidente ---\n";
    centru.alocaEchipaLaIncident(104);  // CUTREMUR - cel mai critic
    centru.alocaEchipaLaIncident(101);  // INCENDIU
    centru.alocaEchipaLaIncident(102);  // TOXIC_SPILL
    std::cout << "Echipe disponibile dupa alocare: " << centru.numarEchipeDisponibile() << "\n\n";

    // --- 9. simulare in 2 pasi simuleazaEvolutia(@pasi) ---
    centru.simuleazaEvolutie(2);

    // --- 10. verific areResursaSuficienta ---
    std::cout << "--- Verificare resurse echipa ---\n";
    std::cout << "Echipa1 are 5 furtunuri? "
              << (echipa1.areResursaSuficienta("furtun_apa", 5) ? "DA" : "NU") << "\n";
    std::cout << "Echipa1 are combinezoane? "
              << (echipa1.areResursaSuficienta("combinezoane_hazmat", 1) ? "DA" : "NU") << "\n\n";


    std::cout << "Simulare finalizată vărule.";

    return 0;
}
