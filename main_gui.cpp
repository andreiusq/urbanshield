#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "include/Locatie.h"
#include "include/Resursa.h"
#include "include/EchipaInterventie.h"
#include "include/Incident.h"
#include "include/IncidentAccident.h"
#include "include/IncidentCutremur.h"
#include "include/IncidentIncendiu.h"
#include "include/IncidentInundatie.h"
#include "include/IncidentPanicaPublica.h"
#include "include/IncidentToxic.h"
#include "include/CentruComanda.h"

// ─── Utilitare UI ────────────────────────────────────────────────────────────

static ImVec4 culoareSeveritate(int sev) {
    switch (sev) {
        case 1: return {0.2f, 0.8f, 0.2f, 1.f};   // verde
        case 2: return {0.6f, 0.9f, 0.2f, 1.f};   // galben-verde
        case 3: return {1.0f, 0.8f, 0.0f, 1.f};   // galben
        case 4: return {1.0f, 0.4f, 0.0f, 1.f};   // portocaliu
        case 5: return {1.0f, 0.1f, 0.1f, 1.f};   // rosu
        default: return {1.f, 1.f, 1.f, 1.f};
    }
}

static ImVec4 culoareTip(const std::string& tip) {
    if (tip == "INCENDIU")    return {1.0f, 0.3f, 0.1f, 1.f};
    if (tip == "TOXIC_SPILL") return {0.6f, 0.0f, 0.8f, 1.f};
    if (tip == "CUTREMUR")    return {0.9f, 0.6f, 0.1f, 1.f};
    if (tip == "INUNDATIE")   return {0.2f, 0.5f, 1.0f, 1.f};
    if (tip == "ACCIDENT")    return {0.8f, 0.8f, 0.2f, 1.f};
    if (tip == "PANICA_PUBLICA") return {0.9f, 0.2f, 0.5f, 1.f};
    return {0.8f, 0.8f, 0.8f, 1.f};
}

// Captura operator<< intr-un string
template<typename T>
static std::string toString(const T& obj) {
    std::ostringstream oss;
    oss << obj;
    return oss.str();
}

// ─── Starea aplicatiei ───────────────────────────────────────────────────────

struct AppState {
    CentruComanda centru;
    std::vector<std::string> logMessages;
    int selectedIncidentId = -1;
    int simPasi = 1;
    bool showAbout = false;

    AppState() : centru("UrbanShield HQ", Locatie(44.4268, 26.1025, "Centrul Orasului")) {
        initDateDemo();
    }

    void initDateDemo() {
        Locatie sectorul1(44.4500, 26.0800, "Sectorul 1 - Herastrau");
        Locatie sectorul2(44.4100, 26.1300, "Sectorul 2 - Colentina");
        Locatie sectorul3(44.3900, 26.0600, "Sectorul 3 - Rahova");
        Locatie bazaPompieri(44.4350, 26.0950, "Baza Pompieri Centrala");
        Locatie bazaMedical(44.4200, 26.1100, "Spitalul de Urgenta");
        Locatie centruOras(44.4268, 26.1025, "Centrul Orasului");

        EchipaInterventie e1(1, "Pompierii Rosii", "POMPIERI", bazaPompieri);
        e1.adaugaResursa(Resursa("furtun_apa", 10, "buc"));
        e1.adaugaResursa(Resursa("generator", 3, "buc"));

        EchipaInterventie e2(2, "SMURD Alpha", "MEDICAL", bazaMedical);
        e2.adaugaResursa(Resursa("trusa_medicala", 5, "buc"));
        e2.adaugaResursa(Resursa("franghii_salvare", 15, "buc"));

        EchipaInterventie e3(3, "Echipa HAZMAT", "HAZMAT", centruOras);
        e3.adaugaResursa(Resursa("combinezoane_hazmat", 8, "buc"));
        e3.adaugaResursa(Resursa("trusa_medicala", 3, "buc"));

        EchipaInterventie e4(4, "Salvare Montana", "SALVARE", sectorul1);
        e4.adaugaResursa(Resursa("franghii_salvare", 15, "buc"));
        e4.adaugaResursa(Resursa("generator", 2, "buc"));

        centru.adaugaEchipa(e1);
        centru.adaugaEchipa(e2);
        centru.adaugaEchipa(e3);
        centru.adaugaEchipa(e4);

        centru.adaugaIncident(IncidentIncendiu(101, 4, sectorul1,
            "2026-03-13T08:00", "Incendiu bloc 10 etaje", true));
        centru.adaugaIncident(IncidentToxic(102, 3, sectorul2,
            "2026-03-13T08:15", "Scurgere acid sulfuric", "acid sulfuric", true));
        centru.adaugaIncident(IncidentAccident(103, 2, sectorul3,
            "2026-03-13T08:30", "Coliziune multipla, 5 raniti", 5));
        centru.adaugaIncident(IncidentCutremur(104, 5, centruOras,
            "2026-03-13T09:00", "Cutremur 6.2 Richter", 6.2, 12));
        centru.adaugaIncident(IncidentInundatie(105, 2, sectorul3,
            "2026-03-13T09:10", "Inundatii subsoluri Rahova", 3));
        centru.adaugaIncident(IncidentPanicaPublica(106, 3, centruOras,
            "2026-03-13T09:20", "Panica dupa replica seismica", 120));

        log("Sistem initializat cu date demo.");
    }

    void log(const std::string& msg) {
        logMessages.push_back(msg);
        if (logMessages.size() > 200)
            logMessages.erase(logMessages.begin());
    }
};

// ─── Panouri ImGui ───────────────────────────────────────────────────────────

static void drawIncidente(AppState& app) {
    ImGui::Begin("Incidente", nullptr, ImGuiWindowFlags_NoCollapse);

    auto prioritizate = app.centru.getIncidentePrioritizate();

    ImGui::Text("Active: %d", app.centru.numarIncidenteActive());
    ImGui::Separator();

    ImGui::BeginTable("tbl_inc", 5,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
        ImVec2(0, 280));
    ImGui::TableSetupColumn("ID",         ImGuiTableColumnFlags_WidthFixed, 40);
    ImGui::TableSetupColumn("Tip",        ImGuiTableColumnFlags_WidthFixed, 100);
    ImGui::TableSetupColumn("Sev",        ImGuiTableColumnFlags_WidthFixed, 40);
    ImGui::TableSetupColumn("Prioritate", ImGuiTableColumnFlags_WidthFixed, 80);
    ImGui::TableSetupColumn("Locatie",    ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    for (const auto* inc : prioritizate) {
        const std::string tip = inc->getTip();
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("%d", inc->getId());

        ImGui::TableNextColumn();
        ImGui::TextColored(culoareTip(tip), "%s", tip.c_str());

        ImGui::TableNextColumn();
        ImGui::TextColored(culoareSeveritate(inc->getSeveritate()),
                           "%d", inc->getSeveritate());

        ImGui::TableNextColumn();
        ImGui::Text("%d", inc->getPrioritate());

        ImGui::TableNextColumn();
        ImGui::TextUnformatted(inc->getLocatie().getAdresa().c_str());
    }
    ImGui::EndTable();

    ImGui::Separator();
    ImGui::Text("Aloca echipa la incident ID:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    ImGui::InputInt("##incid", &app.selectedIncidentId);
    ImGui::SameLine();
    if (ImGui::Button("Aloca")) {
        bool ok = app.centru.alocaEchipaLaIncident(app.selectedIncidentId);
        app.log(ok
            ? "Echipa alocata la incident " + std::to_string(app.selectedIncidentId)
            : "Alocare esuata pt incident " + std::to_string(app.selectedIncidentId));
    }

    ImGui::End();
}

static void drawEchipe(AppState& app) {
    ImGui::Begin("Echipe de Interventie", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Disponibile: %d", app.centru.numarEchipeDisponibile());
    ImGui::Separator();

    // Afisam prin operator<< fiecare echipa intr-un TreeNode
    // Folosim getIncidentePrioritizate pentru a afla ce avem; echipele le accesam
    // prin CentruComanda::operator<< capturat in string si parsat simplu
    // Mai elegant: afisam direct din obiectele stocate (facem CentruComanda sa
    // expuna echipele prin getter adaugat on-the-fly in header).
    // Pentru a nu modifica clasele, capturam toString(centru) si il afisam intr-un
    // scroll area — dar e mai frumos cu panoul structurat de mai jos.

    // Deoarece echipele sunt private in CentruComanda, afisam raportul complet
    // intr-un child scrollabil.
    ImGui::BeginChild("echipe_scroll", ImVec2(0, 0), true);
    for (const auto& echipa : app.centru.getEchipe()) {
        std::string detalii = toString(echipa);
        ImGui::TextUnformatted(detalii.c_str());
        ImGui::Separator();
    }
    ImGui::EndChild();

    ImGui::End();
}

static void drawSimulare(AppState& app) {
    ImGui::Begin("Simulare Criza", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Pasii de simulat:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80);
    ImGui::InputInt("##pasi", &app.simPasi);
    if (app.simPasi < 1) app.simPasi = 1;
    if (app.simPasi > 10) app.simPasi = 10;

    if (ImGui::Button("Simuleaza evolutie")) {
        app.log("=== Simulare " + std::to_string(app.simPasi) + " pasi ===");
        app.centru.simuleazaEvolutie(app.simPasi);
        app.log("Simulare finalizata.");
    }

    ImGui::SameLine();
    if (ImGui::Button("Aloca toate incidentele critice")) {
        auto prioritizate = app.centru.getIncidentePrioritizate();
        int alocate = 0;
        for (const auto* inc : prioritizate) {
            if (inc->getSeveritate() >= 4) {
                if (app.centru.alocaEchipaLaIncident(inc->getId()))
                    ++alocate;
            }
        }
        app.log("Alocate " + std::to_string(alocate) + " echipe la incidente critice.");
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset demo")) {
        app = AppState();
    }

    ImGui::Separator();

    // Bara progres severitate medie
    auto prioritizate = app.centru.getIncidentePrioritizate();
    if (!prioritizate.empty()) {
        float sumSev = 0.f;
        for (const auto* i : prioritizate) sumSev += static_cast<float>(i->getSeveritate());
        float avgSev = sumSev / static_cast<float>(prioritizate.size());
        char buf[32];
        snprintf(buf, sizeof(buf), "Sev medie: %.1f / 5", avgSev);
        ImGui::ProgressBar(avgSev / 5.f, ImVec2(-1, 0), buf);
    }

    // Bara echipe disponibile
    int totalEchipe = app.centru.numarEchipe();
    int disponibile = app.centru.numarEchipeDisponibile();
    char buf2[32];
    snprintf(buf2, sizeof(buf2), "Echipe: %d / %d", disponibile, totalEchipe);
    ImGui::ProgressBar(
        totalEchipe > 0 ? static_cast<float>(disponibile) / static_cast<float>(totalEchipe) : 0.f,
        ImVec2(-1, 0), buf2);

    ImGui::End();
}

static void drawLog(AppState& app) {
    ImGui::Begin("Log Operational", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::BeginChild("log_scroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& msg : app.logMessages)
        ImGui::TextUnformatted(msg.c_str());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();
    ImGui::End();
}

static void drawStatistici(AppState& app) {
    ImGui::Begin("Statistici", nullptr, ImGuiWindowFlags_NoCollapse);

    auto prioritizate = app.centru.getIncidentePrioritizate();

    // Distributie pe tip - bare colorate
    std::map<std::string, int> perTip;
    for (const auto* inc : prioritizate)
        perTip[inc->getTip()]++;

    ImGui::Text("Distributie incidente active pe tip:");
    for (const auto& [tip, cnt] : perTip) {
        ImGui::TextColored(culoareTip(tip), "%-15s", tip.c_str());
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, culoareTip(tip));
        char label[32];
        snprintf(label, sizeof(label), "%d##%s", cnt, tip.c_str());
        float val = static_cast<float>(cnt);
        ImGui::ProgressBar(val / 5.f, ImVec2(120, 0), label);
        ImGui::PopStyleColor();
    }

    ImGui::Separator();
    ImGui::Text("Incidente active: %d", app.centru.numarIncidenteActive());
    ImGui::Text("Echipe disponibile: %d", app.centru.numarEchipeDisponibile());

    ImGui::End();
}

static void drawAbout(AppState& app) {
    if (!app.showAbout) return;
    ImGui::Begin("Despre UrbanShield", &app.showAbout, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("UrbanShield - Crisis Management Simulator");
    ImGui::Text("GUI: ImGui + SFML");
    ImGui::Text("Clase: Locatie, Resursa, EchipaInterventie, Incident, CentruComanda");
    ImGui::End();
}

// ─── Main ────────────────────────────────────────────────────────────────────

static void runHeadless() {
    std::cout << "Mod headless (fara display detectat).\n";
    AppState app;
    auto prioritizate = app.centru.getIncidentePrioritizate();
    std::cout << "Incidente prioritizate:\n";
    for (const auto* inc : prioritizate)
        std::cout << "  " << *inc << "\n\n";
    app.centru.alocaEchipaLaIncident(104);
    app.centru.alocaEchipaLaIncident(101);
    app.centru.simuleazaEvolutie(2);
    std::cout << app.centru;
}

static bool hasDisplay() {
#if defined(__linux__) || defined(__unix__)
    const char* display = std::getenv("DISPLAY");
    const char* wayland = std::getenv("WAYLAND_DISPLAY");
    return (display != nullptr && display[0] != '\0') ||
           (wayland != nullptr && wayland[0] != '\0');
#else
    return true; // Windows / macOS au mereu display
#endif
}

int main() {
    if (!hasDisplay()) {
        runHeadless();
        return 0;
    }

    sf::RenderWindow window(
        sf::VideoMode(1280, 800),
        "UrbanShield - Crisis Management",
        sf::Style::Default
    );

    if (!window.isOpen()) {
        std::cerr << "Nu s-a putut deschide fereastra. Rulare headless.\n";
        runHeadless();
        return 0;
    }

    window.setFramerateLimit(60);
    (void)ImGui::SFML::Init(window);

    // Tema inchisa
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.f;
    style.FrameRounding  = 4.f;
    style.GrabRounding   = 4.f;
    style.WindowBorderSize = 1.f;

    // Culori accent portocaliu-rosu (tema urgenta)
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_TitleBgActive]  = ImVec4(0.7f, 0.15f, 0.05f, 1.f);
    colors[ImGuiCol_Header]         = ImVec4(0.6f, 0.12f, 0.04f, 0.8f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.8f, 0.2f,  0.05f, 0.9f);
    colors[ImGuiCol_Button]         = ImVec4(0.6f, 0.12f, 0.04f, 0.8f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.8f, 0.2f,  0.05f, 1.f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(1.0f, 0.3f,  0.1f,  1.f);
    colors[ImGuiCol_CheckMark]      = ImVec4(1.0f, 0.4f,  0.1f,  1.f);
    colors[ImGuiCol_SliderGrab]     = ImVec4(0.8f, 0.2f,  0.05f, 1.f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.15f, 0.15f, 0.18f, 1.f);
    colors[ImGuiCol_WindowBg]       = ImVec4(0.08f, 0.08f, 0.10f, 0.97f);

    AppState app;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, clock.restart());

        // ── Meniu principal ──────────────────────────────────────────
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("UrbanShield")) {
                if (ImGui::MenuItem("Reset demo")) app = AppState();
                ImGui::Separator();
                if (ImGui::MenuItem("Iesire")) window.close();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Ajutor")) {
                if (ImGui::MenuItem("Despre")) app.showAbout = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // ── Layout: setam pozitiile ferestrelor la primul frame ───────
        const float menuH   = ImGui::GetFrameHeight();
        const float W       = static_cast<float>(window.getSize().x);
        const float H       = static_cast<float>(window.getSize().y) - menuH;
        const float leftW   = W * 0.55f;
        const float rightW  = W - leftW;
        const float topH    = H * 0.55f;
        const float botH    = H - topH;

        ImGui::SetNextWindowPos( {0,        menuH},         ImGuiCond_Always);
        ImGui::SetNextWindowSize({leftW,    topH},          ImGuiCond_Always);
        drawIncidente(app);

        ImGui::SetNextWindowPos( {0,        menuH + topH},  ImGuiCond_Always);
        ImGui::SetNextWindowSize({leftW,    botH},          ImGuiCond_Always);
        drawSimulare(app);

        ImGui::SetNextWindowPos( {leftW,    menuH},         ImGuiCond_Always);
        ImGui::SetNextWindowSize({rightW,   topH * 0.6f},   ImGuiCond_Always);
        drawEchipe(app);

        ImGui::SetNextWindowPos( {leftW,    menuH + topH * 0.6f},  ImGuiCond_Always);
        ImGui::SetNextWindowSize({rightW,   topH * 0.4f},          ImGuiCond_Always);
        drawStatistici(app);

        ImGui::SetNextWindowPos( {leftW,    menuH + topH},  ImGuiCond_Always);
        ImGui::SetNextWindowSize({rightW,   botH},          ImGuiCond_Always);
        drawLog(app);

        drawAbout(app);

        // ── Render ───────────────────────────────────────────────────
        window.clear(sf::Color(18, 18, 24));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
