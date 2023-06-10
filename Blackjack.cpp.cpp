#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Karta structure
struct Karta {
    string boja;
    string rang;
};

// Function to initialize the spil
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

// Function to shuffle the spil
void shufflespil(vector<Karta>& spil) {
    srand(static_cast<unsigned int>(time(0)));
    random_shuffle(spil.begin(), spil.end());
}

// Function to deal a Karta
Karta dijeljenjeK(vector<Karta>& spil) {
    Karta Karta = spil.back();
    spil.pop_back();
    return Karta;
}

// Function to calculate the vrijednost of a hand
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

// Function to save game data to a textual file
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

// Function to load game data from a textual file
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

// Function to display a Karta
void displayKarta(const Karta& Karta) {
    cout << Karta.rang << " " << Karta.boja << endl;
}

// Function to display the player's hand
void prikazIgrac(const vector<Karta>& karteIgraca) {
    cout << "Vase karte:" << endl;
    for (const auto& Karta : karteIgraca) {
        displayKarta(Karta);
    }
    cout << "Ukupna vrijednost: " << vrijednostK(karteIgraca) << endl;
    cout << endl;
}

// Function to display the dealer's hand
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

// Function to play the blackjack game
void igra() {
    vector<Karta> spil = initializespil();
    shufflespil(spil);

    vector<Karta> karteIgraca;
    vector<Karta> karteDilera;

    int bodoviIgraca = 0;
    int bodoviDilera = 0;

    // Check if there is a saved game
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

    // Game loop
    bool krajIgre = false;
    while (!krajIgre) {
        prikazIgrac(karteIgraca);
        prikazDiler(karteDilera, false);

        // Check if player or dealer has blackjack
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
            // Player's turn
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
                // Dealer's turn
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

    // Save game
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