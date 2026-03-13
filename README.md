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

- [ ] separare `.h/.hpp` si `.cpp`
- [ ] baza + minim 3 derivate in aceeasi ierarhie
- [ ] functii virtuale pure specifice domeniului
- [ ] clone (`virtual constructor`)
- [ ] container cu pointeri la baza si apel polimorfic
- [ ] `dynamic_cast` cu sens
- [ ] ierarhie proprie de exceptii (minim 3)
- [ ] `static`, STL, `const`, functii de nivel inalt
- [ ] commit separat cu clasa derivata noua integrata

### T3

- [ ] 2 design patterns
- [ ] clasa template cu minim 2 instantieri
- [ ] functie template cu minim 2 instantieri
- [ ] integrare minim 2 biblioteci externe (in afara stocarii)