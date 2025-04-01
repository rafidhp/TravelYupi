#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Users struct
struct users {
    string username;
    string password;
};


// Login function
void login(vector<users> &user_list) {
    string username, password;
    bool found = false;

    cout << "\n========== ========== ==========\n" << endl;
    cout << "Masukkan username: "; cin >> username;
    cout << "Masukkan password: "; cin >> password;

    for(users &user : user_list) {
        if(user.username == username && user.password == password) {
            found = true;
            break;
        }
    }

    if(found) {
        cout << "\nLogin berhasil! Selamat Datang " << username << " di TravelYupi!\n";
    } else {
        cout << "Username atau password salah! Silakan coba lagi!";
    }
}


// First choice function
void choice1(vector<users> &user_list) {
    string choice;

    cout << "\n========== ========== ==========\n" << endl;
    cout << "--- Selamat Datang di TravelYupi ---" << endl;
    cout << "[1]Login\n[2]Register\n[3]Exit\nSilakan pilih Login atau Register berdasarkan angka: ";cin >> choice;

    if(choice == "1") {
        login(user_list);
    } else if(choice == "2") {
        cout << "Register";
    } else if(choice == "3") {
        exit(0);
    } else {
        cout << "Silakan pilih berdasarkan angka yang tersedia!";
    }
}


// Main function
int main() {

    // Users data
    vector<users> user_list = {
        {"admin", "admin"},
        {"jaki", "jaki"},
    };


    choice1(user_list);
}