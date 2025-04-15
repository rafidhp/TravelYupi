#include <cstring>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

// Struct untuk data pengguna
struct users {
    string username;
    string password;
};

// Struct untuk kesempatan registrasi
struct chance {
    int regis_chance;
    chance() : regis_chance(3) {} // Constructor untuk inisialisasi
};

// Struct untuk autentikasi login
struct auth {
    string user_login;
};

// Struct untuk data tiket
struct tiket {
    string id;
    string kota_asal;
    string kota_tujuan;
    string tanggal;
    string jam;
    string harga;
    int kursi_tersedia;
};

// Struct untuk data pesanan
struct pesanan {
    string username;
    string id_tiket;
    string kota_asal;
    string kota_tujuan;
    string tanggal;
    string jam;
    string nama_penumpang;
    string no_telp;
    string email;
    int jumlah_tiket;
    string nomor_kursi;
    string total_harga;
    string metode_pembayaran;
    string waktu_pemesanan;
};



struct Node {
    pesanan data;       // Data pesanan
    Node* next;         // Pointer ke node berikutnya
    Node* prev;         // Pointer ke node sebelumnya
};

// Pointer untuk head dan tail linked list
Node* head = nullptr;
Node* tail = nullptr;

// Nama file database
const string USER_DATABASE = "users_database.txt";
const string TIKET_DATABASE = "tiket_database.txt";
const string PESANAN_DATABASE = "pesanan_database.txt";

// Deklarasi fungsi
void feature_choice(auth &auth);
void regis(vector<users> &user_list, chance &chance, auth &auth);
void login(vector<users> &user_list, auth &auth);
void choice1(vector<users> &user_list, chance &chance, auth &auth);
void loadUsersFromFile(vector<users> &user_list);
void saveUserToFile(const users &user);
void lihatTiket(auth &auth);
void pesanTiket(auth &auth);
void riwayatPesanan(auth &auth);
void loadTiketFromFile(vector<tiket> &tiket_list);
void loadPesananFromFile(vector<pesanan> &pesanan_list);
void savePesananToFile(const pesanan &new_pesanan);
string getCurrentDateTime();
void updateTiketDatabase(const vector<tiket> &tiket_list);
void addPesananToHistory(pesanan new_pesanan);


// Fungsi utama
int main() {
    vector<users> user_list;
    chance chance;
    auth auth;
    
    loadUsersFromFile(user_list);
    choice1(user_list, chance, auth);
    
    return 0;
}

// Fungsi untuk memuat data pengguna dari file
void loadUsersFromFile(vector<users> &user_list) {
    ifstream file(USER_DATABASE);
    if (!file.is_open()) {
        cout << "Database file tidak ditemukan. Membuat database baru.\n";
        return;
    }

    string username, password;
    while (getline(file, username) && getline(file, password)) {
        users temp_user;
        temp_user.username = username;
        temp_user.password = password;
        user_list.push_back(temp_user);
    }
    
    file.close();
}

// Fungsi untuk menyimpan data pengguna ke file
void saveUserToFile(const users &user) {
    ofstream file(USER_DATABASE, ios::app);
    if (!file.is_open()) {
        cout << "Error: Tidak dapat membuka file database.\n";
        return;
    }

    file << user.username << endl;
    file << user.password << endl;
    
    file.close();
}

// Fungsi untuk memuat data tiket dari file
void loadTiketFromFile(vector<tiket> &tiket_list) {
    ifstream file(TIKET_DATABASE);
    if (!file.is_open()) {
        // Jika file tidak ada, buat tiket default
        tiket default_tikets[] = {
            {"TY001", "Jakarta", "Bandung", "15-04-2025", "08:00", "100000", 40},
            {"TY002", "Jakarta", "Bandung", "15-04-2025", "10:00", "120000", 40},
            {"TY003", "Jakarta", "Surabaya", "15-04-2025", "09:00", "250000", 40},
            {"TY004", "Jakarta", "Yogyakarta", "15-04-2025", "07:30", "200000", 40},
            {"TY005", "Bandung", "Jakarta", "15-04-2025", "14:00", "110000", 40},
            {"TY006", "Surabaya", "Jakarta", "15-04-2025", "15:30", "260000", 40},
            {"TY007", "Yogyakarta", "Jakarta", "15-04-2025", "16:00", "210000", 40},
            {"TY008", "Bandung", "Jakarta", "15-04-2025", "13:00", "100000", 40},
            {"TY009", "Bandung", "Jakarta", "15-04-2025", "11:00", "100000", 40}


        };
        
        for (size_t i = 0; i < sizeof(default_tikets)/sizeof(default_tikets[0]); i++) {
            tiket_list.push_back(default_tikets[i]);
        }
        
        // Simpan tiket default ke file
        ofstream outFile(TIKET_DATABASE);
        if (outFile.is_open()) {
            for (size_t i = 0; i < tiket_list.size(); i++) {
                outFile << tiket_list[i].id << endl;
                outFile << tiket_list[i].kota_asal << endl;
                outFile << tiket_list[i].kota_tujuan << endl;
                outFile << tiket_list[i].tanggal << endl;
                outFile << tiket_list[i].jam << endl;
                outFile << tiket_list[i].harga << endl;
                outFile << tiket_list[i].kursi_tersedia << endl;
            }
            outFile.close();
        }
        return;
    }

    tiket temp;
    string kursi;
    while (getline(file, temp.id) && 
           getline(file, temp.kota_asal) && 
           getline(file, temp.kota_tujuan) &&
           getline(file, temp.tanggal) && 
           getline(file, temp.jam) && 
           getline(file, temp.harga) && 
           getline(file, kursi)) {
        temp.kursi_tersedia = stoi(kursi);
        tiket_list.push_back(temp);
    }
    
    file.close();
}

// Fungsi untuk memperbarui database tiket
void updateTiketDatabase(const vector<tiket> &tiket_list) {
    ofstream file(TIKET_DATABASE);
    if (!file.is_open()) {
        cout << "Error: Tidak dapat membuka file database tiket.\n";
        return;
    }

    for (size_t i = 0; i < tiket_list.size(); i++) {
        file << tiket_list[i].id << endl;
        file << tiket_list[i].kota_asal << endl;
        file << tiket_list[i].kota_tujuan << endl;
        file << tiket_list[i].tanggal << endl;
        file << tiket_list[i].jam << endl;
        file << tiket_list[i].harga << endl;
        file << tiket_list[i].kursi_tersedia << endl;
    }
    
    file.close();
}

// Fungsi untuk memuat data pesanan dari file
void loadPesananFromFile(vector<pesanan> &pesanan_list) {
    ifstream file(PESANAN_DATABASE);
    if (!file.is_open()) {
        return; // File tidak ada, belum ada pesanan
    }

    pesanan temp;
    string jumlah;
    while (getline(file, temp.username) && 
           getline(file, temp.id_tiket) && 
           getline(file, temp.kota_asal) && 
           getline(file, temp.kota_tujuan) &&
           getline(file, temp.tanggal) && 
           getline(file, temp.jam) && 
           getline(file, temp.nama_penumpang) && 
           getline(file, temp.no_telp) && 
           getline(file, temp.email) &&
           getline(file, jumlah) && 
           getline(file, temp.nomor_kursi) && 
           getline(file, temp.total_harga) && 
           getline(file, temp.metode_pembayaran) &&
           getline(file, temp.waktu_pemesanan)) {
        temp.jumlah_tiket = stoi(jumlah);
        pesanan_list.push_back(temp);
    }
    
    file.close();
}

// Fungsi untuk menyimpan pesanan baru ke file
void savePesananToFile(const pesanan &new_pesanan) {
    ofstream file(PESANAN_DATABASE, ios::app);
    if (!file.is_open()) {
        cout << "Error: Tidak dapat membuka file database pesanan.\n";
        return;
    }

    file << new_pesanan.username << endl;
    file << new_pesanan.id_tiket << endl;
    file << new_pesanan.kota_asal << endl;
    file << new_pesanan.kota_tujuan << endl;
    file << new_pesanan.tanggal << endl;
    file << new_pesanan.jam << endl;
    file << new_pesanan.nama_penumpang << endl;
    file << new_pesanan.no_telp << endl;
    file << new_pesanan.email << endl;
    file << new_pesanan.jumlah_tiket << endl;
    file << new_pesanan.nomor_kursi << endl;
    file << new_pesanan.total_harga << endl;
    file << new_pesanan.metode_pembayaran << endl;
    file << new_pesanan.waktu_pemesanan << endl;
    
    file.close();
}

// Fungsi untuk mendapatkan waktu sekarang
string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << ltm->tm_mday << "-"
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-"
       << (1900 + ltm->tm_year) << " "
       << setfill('0') << setw(2) << ltm->tm_hour << ":"
       << setfill('0') << setw(2) << ltm->tm_min;
    
    return ss.str();
}

// Fungsi untuk melihat tiket
void lihatTiket(auth &auth) {
    string kota_asal, kota_tujuan;
    vector<tiket> tiket_list;
    
    loadTiketFromFile(tiket_list);
    
    cout << "\n========== Lihat Semua Tiket ==========\n" << endl;
    cout << "Masukkan kota asal: "; cin >> kota_asal;
    cout << "Masukkan kota tujuan: "; cin >> kota_tujuan;
    
    cout << "\nJadwal kereta dari " << kota_asal << " ke " << kota_tujuan << ":\n" << endl;
    cout << setw(5) << "No" << setw(10) << "ID" << setw(15) << "Tanggal" << setw(10) << "Jam" 
         << setw(15) << "Harga" << setw(10) << "Kursi" << endl;
    cout << string(60, '-') << endl;
    
    int count = 0;
    for (size_t i = 0; i < tiket_list.size(); i++) {
        if (tiket_list[i].kota_asal == kota_asal && tiket_list[i].kota_tujuan == kota_tujuan) {
            count++;
            cout << setw(5) << count << setw(10) << tiket_list[i].id << setw(15) << tiket_list[i].tanggal << setw(10) << tiket_list[i].jam 
                 << setw(15) << tiket_list[i].harga << setw(10) << tiket_list[i].kursi_tersedia << endl;
        }
    }
    
    if (count == 0) {
        cout << "Tidak ada tiket tersedia untuk rute ini.\n";
    }
    
    cout << "\nTekan 1 untuk kembali: ";
    int kembali;
    cin >> kembali;
    feature_choice(auth);
}













// Feature choice function
void feature_choice(auth &auth) {
    string choice;

    cout << "\n========== Feature Choice ==========\n" << endl;
    cout << "[1]Pesan Tiket Kereta\n[2]Lihat Semua Tiket\n[3]Logout\n[4]Exit\nSilakan pilih fitur berdasarkan angka: "; cin >> choice;

    if(choice == "1") {
        cout << "\n========== Tiket Kereta ==========\n" << endl;
        cout << "Pilihan Fitur untuk tiket kereta";
    } else if(choice == "2") {
        cout << "\n========== Daftar Tiket ==========\n" << endl;
        cout << "List semua tiket, tiket pesawat dan kereta di kelompokkan sesuai jenisnya";
    } else if(choice == "3") {
        cout << "\nLogout berhasil! Sampai jumpa lagi " << auth.user_login << "!";
    } else if(choice == "4") {
        cout << "\nTerimakasih telah menggunakan TravelYupi!";
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

    // Users data
    vector<users> user_list = {
        {"admin", "admin"},
        {"jaki", "jaki"},
    };
    chance chance; // Regis chance data
    auth auth; // Auth login data


    choice1(user_list, chance, auth);
}