# g2_wypozyczalnia
Repozytorium projektu grupy 2 PPwC IwIK 2025/2026.
Temat prejektu: Wypożyczalnia samochodowa
System Zarządzania Flotą Pojazdów (Wypożyczalnia)

Celem projektu jest stworzenie systemu do zarządzania flotą pojazdów w wypożyczalni. 

Hierarchia Klas 
Podstawą systemu jest klasa bazowa Pojazd, z której dziedziczą wyspecjalizowane typy pojazdów:
- Osobowy
- Dostawczy
- Motocykl

Kluczowe Funkcjonalności (MVP):
Zarządzanie flotą: Funckja umożliżliwająca dodawanie nowych pojazdów do systemu oraz definiowanie ich parametrów. Dodatkowo samochody mogą być wypożyczone użytkownikom, którzy mają konta w systemie (trzeba założyć)
Monitorowanie stanu : System śledzi status każdego pojazdu (np. dostępny, wypożyczony, serwisowany).
Ewidencja przebiegu: aktualizacja oraz zmiana stanu licznika samochodu, zapis przed i po wypożyczeniu.
Proces wypożyczenia i zwrotu: rejestracja momentu wydania pojazdu klientowi, jego stanu licznika oraz jego powrotu do bazy.
Historia eksploatacji: Podgląd pełnej historii wypożyczeń dla konkretnego pojazdu.
Zarządzanie przeglądami: logika blokująca możliwość wypożyczenia pojazdu, który wymaga serwisu lub przekroczył limit przebiegu. Dodatkowe opłaty za krzekroczenie danego limitu przebiegu przy oddawaniu samochodu.


Każdy obiekt w systemie przechowuje zestaw niezbędnych danych:
Dane pojazdu: ID, marka, model, rok produkcji.
Dane eksploatacyjne: Bieżący przebieg, aktualny status techniczny.
Dane transakcyjne: Historia wypożyczeń (kto, kiedy, na jak długo), naliczone opłaty.
