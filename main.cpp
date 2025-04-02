#include <cstring>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

// Users struct
struct users {
    string username;
    string password;
};

// Chance struct
struct chance {
    int regis_chance = 3;
};


// Function declaration
void regis(vector<users> &user_list, chance &chance);
void login(vector<users> &user_list);
void choice1(vector<users> &user_list, chance &chance);


// Register function
void regis(vector<users> &user_list, chance &chance) { // not supported if function name is register
    users new_user;
    regex usernameFormat("^[a-zA-Z0-9_]{3,20}$");

    if(chance.regis_chance == 0) {
        cout << "\nKesempatan registrasi habis! Silakan mencoba lagi!\n";
        chance.regis_chance = 3;
        choice1(user_list, chance);
    }

    cout << "\n========== Register ==========\n" << endl;
    cout << "Masukkan username: "; cin.ignore(); getline(cin, new_user.username);
    cout << "Masukkan password: "; getline(cin, new_user.password);

    for(users &user : user_list) {
        if(user.username == new_user.username) {
            chance.regis_chance -= 1;
            cout << "Username sudah terdaftar! Silakan gunakan username lain! Kesempatan registrasi " << chance.regis_chance << "X lagi!\n";
            regis(user_list, chance);
        }
    }

    // recheck username and password
    if(new_user.username.empty()) {
        chance.regis_chance -= 1;
        cout << "\nUsername harus diisi! Kesempatan registrasi " << chance.regis_chance << "x lagi!\n";
        regis(user_list, chance);
    } else if(new_user.password.empty()) {
        chance.regis_chance -= 1;
        cout << "\nPasssword harus diisi! Kesempatan registrasi " << chance.regis_chance << "x lagi!\n";
        regis(user_list, chance);
    } else if(!regex_match(new_user.username, usernameFormat)) {
        chance.regis_chance -= 1;
        cout << "\nUsername hanya boleh mengandung huruf, angka dan garis bawah! Minimal 3 karakter, Maximal 20 karakter!\n";
        cout << "Kesempatan registrasi " << chance.regis_chance << "x lagi!\n";
        regis(user_list, chance);
    } else {
        user_list.push_back(new_user); // add new user to users vector
        cout << "\nSelamat akun berhasil dibuat! Silakan Login ke dalam TravelYupi!\n";
        chance.regis_chance = 3;
        login(user_list);
    }
}


// Login function
void login(vector<users> &user_list) {
    string username, password;
    bool found = false;

    cout << "\n========== Login ==========\n" << endl;
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
        cout << "\nUsername atau password salah! Silakan coba lagi!\n";
    }
}


// First choice function
void choice1(vector<users> &user_list, chance &chance) {
    string choice;

    cout << "\n========== TravelYupi ==========\n" << endl;
    cout << "--- Selamat Datang di TravelYupi ---" << endl;
    cout << "[1]Login\n[2]Register\n[3]Exit\nSilakan pilih Login atau Register berdasarkan angka: ";cin >> choice;

    if(choice == "1") {
        login(user_list);
    } else if(choice == "2") {
        regis(user_list, chance);
    } else if(choice == "3") {
        cout << "\n Terimakasi telah menggunakan TravelYupi! Selamat Tinggal!\n" << endl;
        exit(0);
    } else {
        cout << "\nSilakan pilih berdasarkan angka yang tersedia!\n";
    }
}


// Main function
int main() {

    // Users data
    vector<users> user_list = {
        {"admin", "admin"},
        {"jaki", "jaki"},
    };
    chance chance; // Regis chance data


    choice1(user_list, chance);
}