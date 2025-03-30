#include <iostream>
#include <string>

using namespace std;

void choice1() {
    int choice;

    cout << "\n"; cout << "========== ========== ==========" << "\n" << endl;
    cout << "--- Selamat Datang di TravelYupi ---" << endl;
    cout << "[1]Login\n[2]Register\n[3]Exit\nSilakan pilih Login atau Register berdasarkan angka: ";cin >> choice;

    if(choice == 1) {
        cout << "Login";
    } else if(choice == 2) {
        cout << "Register";
    } else if(choice == 3) {
        exit(0);
    } else {
        cout << "Silakan pilih berdasarkan angka yang tersedia!";
    }
}

int main() {
    choice1();
    cout << "Hello World!";
}