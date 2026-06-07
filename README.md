<div align="center">
  <h1>🚘 Wypożyczalnia - System Zarządzania Flotą</h1>

  [![Język](https://img.shields.io/badge/Język-C++-blue.svg)](https://isocpp.org/)
  [![Status](https://img.shields.io/badge/Status-W_trakcie_rozwoju-brightgreen.svg)]()

  *Repozytorium projektu grupy 2 w ramach przedmiotu **PPwC IwIK 2025/2026**.*
</div>

---

# 📌 Spis treści
- [O projekcie](#-o-projekcie)
- [Kluczowe funkcjonalności (MVP)](#-kluczowe-funkcjonalności-mvp)
- [Struktura danych i klasy](#-struktura-danych-i-klasy)
- [Struktura plików w repozytorium](#-struktura-plików-w-repozytorium)
- [Wymagania i uruchomienie](#-wymagania-i-uruchomienie)

---

# 💡 O projekcie

Aplikacja to konsolowy system do kompleksowego zarządzania flotą pojazdów, pozwalający na symulację działania wypożyczalni samochodów. Obsługuje zarządzanie różnymi typami pojazdów, kontami użytkowników oraz historią transakcji i wypożyczeń. Projekt został zrealizowany z wykorzystaniem **paradygmatu programowania obiektowego** w języku C++.

---

# ⚙️ Kluczowe funkcjonalności (MVP)

*   **Zarządzanie flotą:** Dodawanie nowych pojazdów do systemu oraz definiowanie ich parametrów (marka, model, rok produkcji).
*   **Zarządzanie użytkownikami:** Wypożyczanie aut dostępne tylko dla zarejestrowanych klientów.
*   **Monitorowanie stanu:** Bieżące śledzenie statusu każdego pojazdu (dostępny, wypożyczony, serwisowany).
*   **Ewidencja przebiegu:** Zapis licznika przed i po wypożyczeniu oraz aktualizacja danych.
*   **Wypożyczenia i zwroty:** Rejestracja wydania pojazdu oraz weryfikacja stanu przy powrocie do bazy.
*   **Historia i finanse:** Pełna historia wypożyczeń.
*   **Zarządzanie przeglądami:** Blokada wypożyczenia aut wymagających serwisu; kary za przekroczenie limitu kilometrów.

---

# 📦 Struktura danych i klasy

<--- Należy zrobić update -----> 

Każdy obiekt w systemie agreguje:
*   **Dane pojazdu:** ID, marka, model, rok produkcji.
*   **Dane eksploatacyjne:** Bieżący przebieg, status techniczny.
*   **Dane transakcyjne:** Historia wypożyczeń, powiązanie z `User`, opłaty (`Transaction`).

---

# 📂 Struktura plików w repozytorium

| Plik / Katalog | Opis |
| :--- | :--- |
| **`main.cpp`** | Główny plik programu, punkt startowy aplikacji. |
| **`RentalSystem`** `(.h/.cpp)` | Zarządzanie logiką wypożyczalni, flotą i klientami. |
| **`Vehicle`** `(.h/.cpp)` | Klasa bazowa pojazdu i jej klasy pochodne. |
| **`User`** `(.h/.cpp)` | Reprezentacja klienta wypożyczalni. |
| **`Transaction`** `(.h/.cpp)` | Obsługa kosztów i dat wypożyczeń. |
| **`menus`** `(.h/.cpp)` | Wyświetlanie różnych menu. |
| **`data/`** | Katalog na pliki z danymi, które są zapisane w .csv. |
| **`diagrams`** | Diagramy klas i architektury (pliki `.png`). |

---

# 💻 Wymagania i uruchomienie

Do uruchomienia projektu wymagany jest kompilator wspierający standard **C++11** (lub nowszy), np. `g++`.

### Instrukcja kompilacji (`g++`):

1. **Pobierz repozytorium:**
   ```bash
   git clone https://github.com/zaKrystian/g2_wypozyczalnia.git
   ```
2. **Przejdź do folderu:**
   ```bash
   cd g2_wypozyczalnia
   ```
3. **Skompiluj pliki:**
   ```bash
   g++ *.cpp -o wypozyczalnia
   ```
4. **Uruchom:**
   ```bash
   # Windows
   wypozyczalnia.exe
   
   # Linux/Mac
   ./wypozyczalnia
   ```
