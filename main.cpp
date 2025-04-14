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

// User login struct
struct auth {
    string user_login;
};



// Function declaration
void feature_choice(auth &auth);
void regis(vector<users> &user_list, chance &chance, auth &auth);
void login(vector<users> &user_list, auth &auth);
void choice1(vector<users> &user_list, chance &chance, auth &auth);


// Feature choice function
void feature_choice(auth &auth) {
    string choice;

    cout << "\n========== Feature Choice ==========\n" << endl;
    cout << "[1]Tiket Kereta\n[2]Tiket Pesawat\n[3]Lihat Semua Tiket\n[4]Logout\n[5]Exit\nSilakan pilih fitur berdasarkan angka: "; cin >> choice;

    if(choice == "1") {
        cout << "\n========== Tiket Kereta ==========\n" << endl;
        cout << "Pilihan Fitur untuk tiket kereta";
    } else if(choice == "2") {
        cout << "\n========== Tiket Pesawat ==========\n" << endl;
        cout << "Pilihan Fitur untuk tiket pesawat";
    } else if(choice == "3") {
        cout << "\n========== Daftar Tiket ==========\n" << endl;
        cout << "List semua tiket, tiket pesawat dan kereta di kelompokkan sesuai jenisnya";
    } else if(choice == "4") {
        cout << "\nLogout berhasil! Sampai jumpa lagi " << auth.user_login << "!";
    } else if(choice == "5") {
        cout << "\nTerimakasi telah menggunakan TravelYupi!";
        exit(0);
    } else {
        cout << "\nSilakan pilih sesuai angka yang tersedia!";
        feature_choice(auth);
    }
}


// Register function
void regis(vector<users> &user_list, chance &chance, auth &auth) { // not supported if function name is register
    users new_user;
    regex usernameFormat("^[a-zA-Z0-9_]{3,20}$");

    if(chance.regis_chance == 0) {
        cout << "\nKesempatan registrasi habis! Silakan mencoba lagi!\n";
        chance.regis_chance = 3;
        choice1(user_list, chance, auth);
    }

    cout << "\n========== Register ==========\n" << endl;
    cout << "Masukkan username: "; cin.ignore(); getline(cin, new_user.username);
    cout << "Masukkan password: "; getline(cin, new_user.password);

    for(users &user : user_list) {
        if(user.username == new_user.username) {
            chance.regis_chance -= 1;
            cout << "Username sudah terdaftar! Silakan gunakan username lain! Kesempatan registrasi " << chance.regis_chance << "X lagi!\n";
            regis(user_list, chance, auth);
        }
    }

    // recheck username and password
    if(new_user.username.empty()) {
        chance.regis_chance -= 1;
        cout << "\nUsername harus diisi! Kesempatan registrasi " << chance.regis_chance << "x lagi!\n";
        regis(user_list, chance, auth);
    } else if(new_user.password.empty()) {
        chance.regis_chance -= 1;
        cout << "\nPasssword harus diisi! Kesempatan registrasi " << chance.regis_chance << "x lagi!\n";
        regis(user_list, chance, auth);
    } else if(!regex_match(new_user.username, usernameFormat)) {
        chance.regis_chance -= 1;
        cout << "\nUsername hanya boleh mengandung huruf, angka dan garis bawah! Minimal 3 karakter, Maximal 20 karakter!\n";
        cout << "Kesempatan registrasi " << chance.regis_chance << "x lagi!\n";
        regis(user_list, chance, auth);
    } else {
        user_list.push_back(new_user); // add new user to users vector
        cout << "\nSelamat akun berhasil dibuat! Silakan Login ke dalam TravelYupi!\n";
        chance.regis_chance = 3;
        login(user_list, auth);
    }
}


// Login function
void login(vector<users> &user_list, auth &auth) {
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
        auth.user_login = username;
        feature_choice(auth);
    } else {
        cout << "\nUsername atau password salah! Silakan coba lagi!\n";
    }
}


// First choice function
void choice1(vector<users> &user_list, chance &chance, auth &auth) {
    string choice;

    cout << "\n========== TravelYupi ==========\n" << endl;
    cout << "--- Selamat Datang di TravelYupi ---" << endl;
    cout << "[1]Login\n[2]Register\n[3]Exit\nSilakan pilih Login atau Register berdasarkan angka: ";cin >> choice;

    if(choice == "1") {
        login(user_list, auth);
    } else if(choice == "2") {
        regis(user_list, chance, auth);
    } else if(choice == "3") {
        cout << "\n Terimakasi telah menggunakan TravelYupi! Selamat Tinggal!\n" << endl;
        exit(0);
    } else {
        cout << "\nSilakan pilih berdasarkan angka yang tersedia!\n";
    }
}



// Main function
int main() {

    cout << "hai" << endl;
    
    // Users data
    vector<users> user_list = {
        {"admin", "admin"},
        {"jaki", "jaki"},
    };
    chance chance; // Regis chance data
    auth auth; // Auth login data


    choice1(user_list, chance, auth);
}