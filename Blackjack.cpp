#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// karte
struct Karta {
    string boja;
    string rang;
};

// spil
vector<Karta> initializespil() {
    vector<Karta> spil;
    string bojas[] = { "Srca", "Karo", "Tref", "Pik" };
    string rangs[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

    for (const auto& boja : bojas) {
        for (const auto& rang : rangs) {
            Karta Karta;
            Karta.boja = boja;
            Karta.rang = rang;
            spil.push_back(Karta);
        }
    }

    return spil;
}

// mjesanje spila
void shufflespil(vector<Karta>& spil) {
    srand(static_cast<unsigned int>(time(0)));
    random_shuffle(spil.begin(), spil.end());
}

// dil karata
Karta dijeljenjeK(vector<Karta>& spil) {
    Karta Karta = spil.back();
    spil.pop_back();
    return Karta;
}

// vrjednost karata
int vrijednostK(const vector<Karta>& hand) {
    int vrijednost = 0;
    int numAsova = 0;

    for (const auto& Karta : hand) {
        if (Karta.rang == "A") {
            vrijednost += 11;
            numAsova++;
        }
        else if (Karta.rang == "K" || Karta.rang == "Q" || Karta.rang == "J") {
            vrijednost += 10;
        }
        else {
            vrijednost += stoi(Karta.rang);
        }
    }

    while (vrijednost > 21 && numAsova > 0) {
        vrijednost -= 10;
        numAsova--;
    }

    return vrijednost;
}

// funk za sej u tek dat
void saveGame(const vector<Karta>& karteIgraca, const vector<Karta>& karteDilera, int bodoviIgraca, int bodoviDilera) {
    ofstream file("game_data.txt");
    if (!file) {
        cout << "Pogreska pri ucitavanju." << endl;
        return;
    }

    file << bodoviIgraca << endl;
    file << bodoviDilera << endl;

    for (const auto& Karta : karteIgraca) {
        file << Karta.boja << " " << Karta.rang << endl;
    }

    file << "-" << endl;

    for (const auto& Karta : karteDilera) {
        file << Karta.boja << " " << Karta.rang << endl;
    }

    file.close();
    cout << "Igra uspijesno spremljena." << endl;
}

// funk za prikazivanje iz tek dat
bool loadGame(vector<Karta>& karteIgraca, vector<Karta>& karteDilera, int& bodoviIgraca, int& bodoviDilera) {
    ifstream file("game_data.txt");
    if (!file) {
        cout << "Nije pronadena spremljena igra." << endl;
        return false;
    }

    file >> bodoviIgraca;
    file >> bodoviDilera;

    string boja, rang;
    while (file >> boja && boja != "-") {
        file >> rang;
        Karta Karta;
        Karta.boja = boja;
        Karta.rang = rang;
        karteIgraca.push_back(Karta);
    }

    while (file >> boja >> rang) {
        Karta Karta;
        Karta.boja = boja;
        Karta.rang = rang;
        karteDilera.push_back(Karta);
    }

    file.close();
    cout << "Igra uspijesno ucitana." << endl;
    return true;
}

// funk za prikazivanje karte
void displayKarta(const Karta& Karta) {
    cout << Karta.rang << " " << Karta.boja << endl;
}

// funk za prikazivanje igracevih karata
void prikazIgrac(const vector<Karta>& karteIgraca) {
    cout << "Vase karte:" << endl;
    for (const auto& Karta : karteIgraca) {
        displayKarta(Karta);
    }
    cout << "Ukupna vrijednost: " << vrijednostK(karteIgraca) << endl;
    cout << endl;
}

// funk za prikazivanje dilerovih kartica
void prikazDiler(const vector<Karta>& karteDilera, bool showFirstKarta) {
    cout << "Katre dilera:" << endl;
    int KartaCount = karteDilera.size();
    for (int i = 0; i < KartaCount; i++) {
        if (i == 0 && !showFirstKarta) {
            cout << "Nepoznata karta" << endl;
        }
        else {
            displayKarta(karteDilera[i]);
        }
    }
    cout << endl;
}

// funk za blackjackk
void igra() {
    vector<Karta> spil = initializespil();
    shufflespil(spil);

    vector<Karta> karteIgraca;
    vector<Karta> karteDilera;

    int bodoviIgraca = 0;
    int bodoviDilera = 0;

    // provjera dal ima sejvane igre prije gg
    cout << "Zelite li ucitati spremljenu igru? (d/n): ";
    char choice;
    cin >> choice;

    if (choice == 'd' || choice == 'D') {
        if (!loadGame(karteIgraca, karteDilera, bodoviIgraca, bodoviDilera)) {
            return;
        }
    }
    else {
        karteIgraca.push_back(dijeljenjeK(spil));
        karteIgraca.push_back(dijeljenjeK(spil));
        bodoviIgraca = vrijednostK(karteIgraca);

        karteDilera.push_back(dijeljenjeK(spil));
        karteDilera.push_back(dijeljenjeK(spil));
        bodoviDilera = vrijednostK(karteDilera);
    }

    // gejm lup
    bool krajIgre = false;
    while (!krajIgre) {
        prikazIgrac(karteIgraca);
        prikazDiler(karteDilera, false);

        // provjera blackjacka dilera il igraca
        if (bodoviIgraca == 21) {
            cout << "Cestitke! Pobijedili ste" << endl;
            bodoviIgraca = 0;
            krajIgre = true;
        }
        else if (bodoviDilera == 21) {
            cout << "Diler pobijeduje sa Blackjackom!" << endl;
            bodoviDilera = 0;
            krajIgre = true;
        }
        else {
            // igracev potezz
            cout << "Zelite li novu kartu ili stati? (k/s): ";
            cin >> choice;

            if (choice == 'k' || choice == 'K') {
                karteIgraca.push_back(dijeljenjeK(spil));
                bodoviIgraca = vrijednostK(karteIgraca);

                if (bodoviIgraca > 21) {
                    cout << "Premasili ste:( Diler pobijeduje!!" << endl;
                    bodoviIgraca = 0;
                    krajIgre = true;
                }
            }
            else if (choice == 's' || choice == 'S') {
                // dilerov potezz
                while (bodoviDilera < 17) {
                    karteDilera.push_back(dijeljenjeK(spil));
                    bodoviDilera = vrijednostK(karteDilera);
                }

                prikazIgrac(karteIgraca);
                prikazDiler(karteDilera, true);

                if (bodoviDilera > 21) {
                    cout << "Diler premasuje! Vi pobjedujete:)." << endl;
                    bodoviDilera = 0;
                }
                else if (bodoviDilera > bodoviIgraca) {
                    cout << "Diler pobjeduje." << endl;
                    bodoviDilera = 0;
                }
                else if (bodoviDilera < bodoviIgraca) {
                    cout << "Vi pobjedujete!" << endl;
                    bodoviIgraca = 0;
                }
                else {
                    cout << "Izjednaceno." << endl;
                    bodoviIgraca = 0;
                }

                krajIgre = true;
            }
        }
    }

    // sejv gamea
    cout << "Zelite li spremiti igru? (d/n): ";
    cin >> choice;

    if (choice == 'd' || choice == 'D') {
        saveGame(karteIgraca, karteDilera, bodoviIgraca, bodoviDilera);
    }
}

int main() {
    igra();
    return 0;
}