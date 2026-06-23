# UrbanShield
## Disaster Response & Crisis Management Simulator

UrbanShield este un simulator OOP in C++ pentru gestionarea situatiilor de urgenta intr-un oras.
Sistemul modeleaza incidente reale, aloca echipe de interventie, gestioneaza resurse si simuleaza evolutia crizei in timp.

## Tema 0

- Nume proiect: `UrbanShield`
- Descriere scurta: aplicatie C++ care prioritizeaza automat incidente urbane (incendii, inundatii, cutremure, scurgeri toxice, accidente rutiere majore), aloca unitati potrivite si estimeaza impactul operational.

## Functionalitati principale

- management incidente
- management echipe de interventie
- management resurse si consum operational
- management cladiri/zone si risc local
- prioritizare automata a incidentelor
- alocare inteligenta de unitati
- simulare a evolutiei crizei pe intervale de timp
- generare de rapoarte operationale

## Checklist momentan

### T1

- [x] minim 3-4 clase prin compunere (5 clase: Locatie, Resursa, EchipaInterventie, Incident, CentruComanda)
- [x] constructori de initializare pentru toate clasele
- [x] regula celor 3 pentru cel putin o clasa (EchipaInterventie)
- [x] `operator<<` pentru toate clasele (cu compunere de apeluri)
- [x] minim 3 functii membru netriviale (1-2 complexe): prioritizare, alocare inteligenta, simulare evolutie, raport
- [x] scenariu coerent in `main`

### T2

- [x] separare `.h/.hpp` si `.cpp`
- [x] baza + minim 3 derivate in aceeasi ierarhie
- [x] functii virtuale pure specifice domeniului
- [x] clone (`virtual constructor`)
- [x] container cu pointeri la baza si apel polimorfic
- [x] `dynamic_cast` cu sens
- [x] ierarhie proprie de exceptii (minim 3)
- [x] `static`, STL, `const`, functii de nivel inalt
- [ ] commit separat cu clasa derivata noua integrata

### T3

- [x] 2 design patterns (Factory: `FabricaIncidente`; Observer: `ObservatorCentru` + `JurnalEvenimente`/`AlertaSeveritate`)
- [x] clasa template cu minim 2 instantieri (`Registru<T>`: `Registru<EchipaInterventie>`, `Registru<const Incident*>`)
- [x] functie template cu minim 2 instantieri (`maximDupa`/`sumaDupa` pe `Incident*`, `EchipaInterventie`, `int`)
- [x] minim 80-90% cod propriu C++
- [ ] tag de git pe commit cu toate bifele (v0.3)
