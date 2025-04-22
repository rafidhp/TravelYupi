#include <cstring>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <cctype>
#include <conio.h> 
#include <limits>
#include <set>

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
bool validasiKembali(const string& input); 

string capitalize(const string& input) {
    string result = input;
    if (!result.empty()) {
        for (char &c : result) {
            c = tolower(c);
        }
        result[0] = toupper(result[0]);
    }
    return result;
}

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
    vector<tiket> tiket_list;
    loadTiketFromFile(tiket_list); // Memuat data tiket dari file

    cout << "\n========== Daftar Semua Tiket Tersedia ==========\n" << endl;
    
    if (tiket_list.empty()) {
        cout << "Tidak ada tiket tersedia saat ini.\n";
    } else {
        // Header tabel
        cout << setw(5) << "No" 
             << setw(10) << "ID" 
             << setw(15) << "Kota Asal" 
             << setw(15) << "Kota Tujuan"
             << setw(15) << "Tanggal" 
             << setw(10) << "Jam" 
             << setw(15) << "Harga" 
             << setw(10) << "Kursi" 
             << endl;
        cout << string(90, '-') << endl;
        
        // Menampilkan semua tiket
        for (size_t i = 0; i < tiket_list.size(); i++) {
            cout << setw(5) << (i + 1) 
                 << setw(10) << tiket_list[i].id 
                 << setw(15) << tiket_list[i].kota_asal 
                 << setw(15) << tiket_list[i].kota_tujuan
                 << setw(15) << tiket_list[i].tanggal 
                 << setw(10) << tiket_list[i].jam 
                 << setw(15) << tiket_list[i].harga 
                 << setw(10) << tiket_list[i].kursi_tersedia 
                 << endl;
        }
    }
    
    // Validasi input untuk kembali ke menu menggunakan fungsi validasiKembali
    string input;
    do {
        cout << "\nTekan 1 untuk kembali: ";
        getline(cin, input);
    } while (!validasiKembali(input));
    
    feature_choice(auth);
}

// Fungsi validasi nama (hanya huruf dan spasi)
bool validasiNama(const string& nama) {
    regex hanya_huruf("^[A-Za-z ]{3,}$");
    return regex_match(nama, hanya_huruf);
}

// Fungsi validasi nomor telepon (hanya angka, minimal 11 angka)
bool validasiNoTelp(const string& no_telp) {
    regex hanya_angka("^[0-9]{11,15}$");
    return regex_match(no_telp, hanya_angka);
}

// Fungsi validasi email (huruf kecil, tidak boleh ada spasi)
bool validasiEmail(const string& email) {
    regex format_email("^[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,}$");
    return regex_match(email, format_email);
}

// Fungsi validasi untuk kota (hanya huruf)
bool validasiKota(const string& kota, const vector<tiket>& tiket_list, bool asal) {
    // 1. Validasi format (hanya huruf dan spasi)
    regex hanya_huruf("^[A-Za-z ]{3,}$");
    if (!regex_match(kota, hanya_huruf)) {
        cout << "Nama kota hanya boleh mengandung huruf\n";
        return false;
    }

    // 2. Cek ketersediaan kota di data tiket
    string kota_capitalized = capitalize(kota);
    bool kota_tersedia = false;
    
    for (const auto& tiket : tiket_list) {
        if (asal && capitalize(tiket.kota_asal) == kota_capitalized) {
            kota_tersedia = true;
            break;
        }
        if (!asal && capitalize(tiket.kota_tujuan) == kota_capitalized) {
            kota_tersedia = true;
            break;
        }
    }

    if (!kota_tersedia) {
        cout << "Kota " << kota_capitalized << " tidak tersedia dalam data tiket\n";
        
        // Tampilkan kota yang tersedia
        set<string> kota_tersedia_set;
        for (const auto& tiket : tiket_list) {
            if (asal) {
                kota_tersedia_set.insert(capitalize(tiket.kota_asal));
            } else {
                kota_tersedia_set.insert(capitalize(tiket.kota_tujuan));
            }
        }
        
        cout << "Kota yang tersedia: ";
        for (const auto& k : kota_tersedia_set) {
            cout << k << " ";
        }
        cout << endl;
        
        return false;
    }

    return true;
}


// Fungsi validasi format dan logika tanggal
bool validasiTanggal(const string& tanggal, const vector<tiket>& tiket_list) {
    // 1. Cek format DD-MM-YYYY dengan regex yang benar
    regex format_tanggal("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-[0-9]{4}$");
    if (!regex_match(tanggal, format_tanggal)) {
        cout << "Format tanggal tidak valid. Gunakan DD-MM-YYYY (contoh: 15-04-2025)\n";
        return false;
    }

    // 2. Ekstrak komponen tanggal dengan benar
    int hari, bulan, tahun;
    try {
        hari = stoi(tanggal.substr(0, 2));
        bulan = stoi(tanggal.substr(3, 2));
        tahun = stoi(tanggal.substr(6, 4));
    } catch (...) {
        cout << "Terjadi kesalahan dalam konversi tanggal\n";
        return false;
    }

    // 3. Validasi hari dan bulan
    if (hari < 1 || hari > 31) {
        cout << "Hari harus antara 1-31\n";
        return false;
    }
    if (bulan < 1 || bulan > 12) {
        cout << "Bulan harus antara 1-12\n";
        return false;
    }

    // 4. Validasi hari per bulan
    if ((bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11) && hari > 30) {
        cout << "Bulan " << bulan << " hanya memiliki 30 hari\n";
        return false;
    }

    if (bulan == 2) {
        bool kabisat = (tahun % 400 == 0) || (tahun % 100 != 0 && tahun % 4 == 0);
        if (kabisat && hari > 29) {
            cout << "Februari " << tahun << " hanya memiliki 29 hari (tahun kabisat)\n";
            return false;
        }
        if (!kabisat && hari > 28) {
            cout << "Februari " << tahun << " hanya memiliki 28 hari\n";
            return false;
        }
    }

    // 5. Cek ketersediaan tanggal di data tiket
    bool tanggal_tersedia = false;
    for (const auto& tiket : tiket_list) {
        if (tiket.tanggal == tanggal) {
            tanggal_tersedia = true;
            break;
        }
    }

    if (!tanggal_tersedia) {
        cout << "Tidak ada tiket tersedia untuk tanggal " << tanggal << "\n";
        
        // Tampilkan tanggal yang tersedia (unik)
        set<string> tanggal_tersedia_set;
        for (const auto& tiket : tiket_list) {
            tanggal_tersedia_set.insert(tiket.tanggal);
        }
        
        cout << "Tanggal yang tersedia: ";
        for (const auto& tgl : tanggal_tersedia_set) {
            cout << tgl << " ";
        }
        cout << endl;
        
        return false;
    }

    return true;
}

// Fungsi validasi input kembali (hanya angka 1)
bool validasiKembali(const string& input) {
    if (input.empty()) {
        cout << "Input tidak boleh kosong!\n";
        return false;
    }
    
    if (input.length() > 1 || !isdigit(input[0])) {
        cout << "Hanya menerima input angka 1!\n";
        return false;
    }
    
    if (input != "1") {
        cout << "Hanya menerima input angka 1!\n";
        return false;
    }
    
    return true;
}

// Fungsi validasi input lanjut (hanya angka 1)
bool validasiLanjut(const string& input) {
    if (input.empty()) {
        cout << "Input tidak boleh kosong!\n";
        return false;
    }
    
    if (input.length() > 1 || !isdigit(input[0])) {
        cout << "Hanya menerima input angka 1!\n";
        return false;
    }
    
    if (input != "1") {
        cout << "Hanya menerima input angka 1!\n";
        return false;
    }
    
    return true;
}

// Fungsi untuk memesan tiket
void pesanTiket(auth &auth) {
    string kota_asal, kota_tujuan, tanggal;
    int jumlah_tiket;
    vector<tiket> tiket_list;
    vector<tiket> filtered_tiket;
    
    
    loadTiketFromFile(tiket_list);
    
    regex hanya_huruf("^[A-Za-z ]+$");

    cout << "\n========== Pesan Tiket ==========\n" << endl;
    
    
    // Validasi kota asal
    do {
        cout << "Masukkan kota asal: ";
        getline(cin, kota_asal);
    } while (!validasiKota(kota_asal, tiket_list, true));

    // Validasi kota tujuan
    do {
        cout << "Masukkan kota tujuan: ";
        getline(cin, kota_tujuan);
    } while (!validasiKota(kota_tujuan, tiket_list, false));
    
    // Validasi input tanggal
    do {
        cout << "Tanggal keberangkatan (DD-MM-YYYY): ";
        getline(cin, tanggal);
    } while (!validasiTanggal(tanggal, tiket_list));


    // Validasi input jumlah tiket
    string input_jumlah;
    do {
        cout << "Banyak tiket: ";
        getline(cin, input_jumlah);
        
        if (input_jumlah.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        
        bool is_number = true;
        for (char c : input_jumlah) {
            if (!isdigit(c)) {
                is_number = false;
                break;
            }
        }
        
        if (!is_number) {
            cout << "Hanya boleh diisi dengan angka!\n";
            continue;
        }
        
        jumlah_tiket = stoi(input_jumlah);
        
        if (jumlah_tiket <= 0) {
            cout << "Jumlah tiket harus lebih dari 0!\n";
            continue;
        }
        
        // Jika semua validasi berhasil, keluar dari loop
        break;
        
    } while (true);

    kota_asal = capitalize(kota_asal);
    kota_tujuan = capitalize(kota_tujuan);
    
    
    // Filter tiket berdasarkan input
    for (size_t i = 0; i < tiket_list.size(); i++) {
        if (capitalize(tiket_list[i].kota_asal) == kota_asal && capitalize(tiket_list[i].kota_tujuan) == kota_tujuan && 
            tiket_list[i].tanggal == tanggal && tiket_list[i].kursi_tersedia >= jumlah_tiket) {
            filtered_tiket.push_back(tiket_list[i]);
        }
    }
      
    // Tampilkan tiket yang tersedia
    cout << "\nTiket yang tersedia:\n" << endl;
    cout << setw(5) << "No" << setw(10) << "ID" << setw(15) << "Tanggal" << setw(10) << "Jam" 
         << setw(15) << "Harga" << setw(10) << "Kursi" << endl;
    cout << string(60, '-') << endl;
    
    for (size_t i = 0; i < filtered_tiket.size(); i++) {
        cout << setw(5) << (i+1) << setw(10) << filtered_tiket[i].id << setw(15) << filtered_tiket[i].tanggal 
             << setw(10) << filtered_tiket[i].jam << setw(15) << filtered_tiket[i].harga 
             << setw(10) << filtered_tiket[i].kursi_tersedia << endl;
    }
    
    // Pilih tiket dengan validasi ketat
    int pilihan_tiket;
    string input_pilihan;
    bool pilihan_valid = false;

    while (!pilihan_valid) {
        cout << "\nPilih tiket (1-" << filtered_tiket.size() << "): ";
        getline(cin, input_pilihan);
        
        // Cek jika input kosong
        if (input_pilihan.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        
        // Cek jika input bukan angka
        bool is_number = true;
        for (char c : input_pilihan) {
            if (!isdigit(c)) {
                is_number = false;
                break;
            }
        }
        
        if (!is_number) {
            cout << "Hanya menerima input angka!\n";
            continue;
        }
        
        // Konversi ke integer
        pilihan_tiket = stoi(input_pilihan);
        
        // Cek jika angka di luar range
        if (pilihan_tiket < 1 || pilihan_tiket > filtered_tiket.size()) {
            cout << "Pilihan harus antara 1-" << filtered_tiket.size() << "!\n";
            // Validasi input jumlah tiket
string input_jumlah;
do {
    cout << "Banyak tiket: ";
    getline(cin, input_jumlah);
    
    if (input_jumlah.empty()) {
        cout << "Input tidak boleh kosong!\n";
        continue;
    }
    
    bool is_number = true;
    for (char c : input_jumlah) {
        if (!isdigit(c)) {
            is_number = false;
            break;
        }
    }
    
    if (!is_number) {
        cout << "Hanya boleh diisi dengan angka!\n";
        continue;
    }
    
    jumlah_tiket = stoi(input_jumlah);
    
    if (jumlah_tiket <= 0) {
        cout << "Jumlah tiket harus lebih dari 0!\n";
        continue;
    }
    
    // Jika semua validasi berhasil, keluar dari loop
    break;
    
} while (true);
            // Tampilkan daftar tiket lagi
            cout << "\nTiket yang tersedia:\n" << endl;
            cout << setw(5) << "No" << setw(10) << "ID" << setw(15) << "Tanggal" 
                << setw(10) << "Jam" << setw(15) << "Harga" << setw(10) << "Kursi" << endl;
            cout << string(60, '-') << endl;
            
            for (size_t i = 0; i < filtered_tiket.size(); i++) {
                cout << setw(5) << (i+1) << setw(10) << filtered_tiket[i].id 
                    << setw(15) << filtered_tiket[i].tanggal << setw(10) << filtered_tiket[i].jam 
                    << setw(15) << filtered_tiket[i].harga << setw(10) << filtered_tiket[i].kursi_tersedia << endl;
            }
            
            continue;
        }
        
        pilihan_valid = true;
    }

    tiket selected_tiket = filtered_tiket[pilihan_tiket - 1];
    
    // Tampilkan kursi
    cout << "\n========== Pilih Kursi ==========\n" << endl;
    cout << "Kursi tersedia: " << selected_tiket.kursi_tersedia << endl;
    cout << "Status kursi (O=tersedia, X=terisi):\n" << endl;
    
    // Menggunakan pointer dan array dinamis
    bool* kursi_status = new bool[40]; // Array dinamis yang berfungsi untuk menyimpan status kursi (terisi/kosong)

    // Inisialisasi array kursi_status dengan false (belum ada yang terisi)
    for (int i = 0; i < 40; i++) {
        kursi_status[i] = false;
    }
    
    // Cek kursi yang sudah terisi
    vector<pesanan> pesanan_list;
    loadPesananFromFile(pesanan_list);
    
    for (size_t i = 0; i < pesanan_list.size(); i++) {
        if (pesanan_list[i].id_tiket == selected_tiket.id) {
            istringstream iss(pesanan_list[i].nomor_kursi);
            string kursi;
            while (getline(iss, kursi, ',')) {
                int nomor = stoi(kursi);
                if (nomor >= 1 && nomor <= 40) {
                    kursi_status[nomor-1] = true;
                }
            }
        }
    }
    
    // Tampilkan status kursi (2x2 layout)
    for (int i = 0; i < 40; i += 4) {
        // Baris pertama (kursi 1-4, 5-8, dst)
        cout << " " << setw(2) << (i+1) << (kursi_status[i] ? "(X)" : "(O)") << " ";
        cout << setw(2) << (i+2) << (kursi_status[i+1] ? "(X)" : "(O)") << "    ";
        cout << setw(2) << (i+3) << (kursi_status[i+2] ? "(X)" : "(O)") << " ";
        cout << setw(2) << (i+4) << (kursi_status[i+3] ? "(X)" : "(O)") << endl;
    }

    delete[] kursi_status;
    
    // Pilih kursi
    string nomor_kursi = "";
for (int i = 0; i < jumlah_tiket; i++) {
    int kursi;
    bool valid = false;
    string input_kursi;
    
    while (!valid) {
        cout << "Pilih kursi: " ;
        getline(cin, input_kursi);
        
        // Validasi input tidak kosong
        if (input_kursi.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        
        // Validasi input harus angka
        bool is_number = true;
        for (char c : input_kursi) {
            if (!isdigit(c)) {
                is_number = false;
                break;
            }
        }
        
        if (!is_number) {
            cout << "Input harus berupa angka!\n";
            continue;
        }
        
        kursi = stoi(input_kursi);
        
        // Validasi range nomor kursi
        if (kursi < 1 || kursi > 40) {
            cout << "Nomor kursi tidak valid. Pilih antara 1-40.\n";
            continue;
        }
        
        // Validasi kursi sudah terisi
        if (kursi_status[kursi-1]) {
            cout << "Kursi sudah terisi. Pilih kursi lain.\n";
            continue;
        }
        
        valid = true;
        kursi_status[kursi-1] = true;
        
        if (i > 0) nomor_kursi += ",";
        nomor_kursi += to_string(kursi);
    }
}
    
    // Isi formulir pemesanan
    pesanan* new_pesanan = new pesanan;  // Pointer ke struct pesanan

    // Isi data dari tiket yang dipilih
    new_pesanan->username = auth.user_login;
    new_pesanan->id_tiket = selected_tiket.id;
    new_pesanan->kota_asal = selected_tiket.kota_asal;
    new_pesanan->kota_tujuan = selected_tiket.kota_tujuan;
    new_pesanan->tanggal = selected_tiket.tanggal;
    new_pesanan->jam = selected_tiket.jam;
    new_pesanan->jumlah_tiket = jumlah_tiket;
    new_pesanan->nomor_kursi = nomor_kursi;

    cout << "\n========== Formulir Pemesanan ==========\n" << endl;

    // Input Nama
    string nama;
    do {
        cout << "Nama: "; cin.ignore(); getline(cin, nama);
        if (!validasiNama(nama)) {
            cout << "Nama Hanya Boleh Diisi Dengan Huruf. Mohon Masukkan Nama Anda Dengan Benar!" << endl;
        }
    } while (!validasiNama(nama));
    new_pesanan->nama_penumpang = nama;

    // Input No. Telepon
    string no_telp;
    do {
        cout << "No. Telepon: "; getline(cin, no_telp);
        if (!validasiNoTelp(no_telp)) {
            cout << "No. Telepon Hanya Boleh Diiisi Dengan Angka dan harus 12 digit. Mohon Masukkan No. Telepon Dengan Benar!" << endl;
        }
    } while (!validasiNoTelp(no_telp));
    new_pesanan->no_telp = no_telp;

    // Input Email
    string email;
    do {
        cout << "Email: "; getline(cin, email);
        if (!validasiEmail(email)) {
            cout << "Email Tidak Valid. Mohon Masukkan Email Dengan Benar " << endl;
        }
    } while (!validasiEmail(email));
    new_pesanan->email = email;

    string input_lanjut;
    do {
        cout << "\nTekan 1 untuk lanjut: ";
        getline(cin, input_lanjut);
    } while (!validasiLanjut(input_lanjut));

    // Hitung total harga
    int total = stoi(selected_tiket.harga) * jumlah_tiket;
    new_pesanan->total_harga = to_string(total);

    // Tampilkan detail
    cout << "\n========== Detail Pemesanan ==========\n" << endl;
    cout << "ID Tiket: " << new_pesanan->id_tiket << endl;
    cout << "Rute: " << new_pesanan->kota_asal << " - " << new_pesanan->kota_tujuan << endl;
    cout << "Tanggal/Jam: " << new_pesanan->tanggal << " / " << new_pesanan->jam << endl;
    cout << "Nama: " << new_pesanan->nama_penumpang << endl;
    cout << "Jumlah Tiket: " << new_pesanan->jumlah_tiket << endl;
    cout << "Nomor Kursi: " << new_pesanan->nomor_kursi << endl;
    cout << "Total Harga: Rp " << new_pesanan->total_harga << endl;

    do {
        cout << "\nTekan 1 untuk lanjut: ";
        getline(cin, input_lanjut);
    } while (!validasiLanjut(input_lanjut));

    // Metode pembayaran
    cout << "\n========== Metode Pembayaran ==========\n" << endl;
    cout << "[1] Transfer\n[2] Cash\nPilih metode pembayaran: ";
    string input_metode;
    do {
        getline(cin, input_metode);
        if (input_metode.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        if (input_metode != "1" && input_metode != "2") {
            cout << "Pilihan tidak valid! Silakan pilih 1 atau 2: ";
            continue;
        }
        break;
    } while (true);

    if (input_metode == "1") {
        new_pesanan->metode_pembayaran = "Transfer";
    } else {
        new_pesanan->metode_pembayaran = "Cash";
    }

    // Form pembayaran
    cout << "\n========== Form Pembayaran ==========\n" << endl;
    cout << "Total yang harus dibayar: Rp " << new_pesanan->total_harga << endl;

    string jumlah_bayar;
    do {
        cout << "Masukkan jumlah pembayaran: Rp ";
        getline(cin, jumlah_bayar);
        
        if (jumlah_bayar.empty()) {
            cout << "Input tidak boleh kosong!\n";
            continue;
        }
        
        // Cek apakah input adalah angka
        bool is_number = true;
        for (char c : jumlah_bayar) {
            if (!isdigit(c)) {
                is_number = false;
                break;
            }
        }
        
        if (!is_number) {
            cout << "Hanya boleh diisi dengan angka!\n";
            continue;
        }
        
        if (stoi(jumlah_bayar) < stoi(new_pesanan->total_harga)) {
            cout << "Jumlah pembayaran kurang. Silakan masukkan jumlah yang cukup.\n";
            continue;
        }
        
        break;
    } while (true);

    if (stoi(jumlah_bayar) > stoi(new_pesanan->total_harga)) {
        cout << "Kembalian: Rp " << (stoi(jumlah_bayar) - stoi(new_pesanan->total_harga)) << endl;
    }

    // Input lanjut setelah pembayaran
    do {
        cout << "\nTekan 1 untuk lanjut: ";
        getline(cin, input_lanjut);
    } while (!validasiLanjut(input_lanjut));

    // Selesaikan pemesanan
    new_pesanan->waktu_pemesanan = getCurrentDateTime();

    // Update tiket
    for (int i = 0; i < tiket_list.size(); ++i) {
        if (tiket_list[i].id == selected_tiket.id) {
            tiket_list[i].kursi_tersedia -= jumlah_tiket;
            break;
        }
    }
    updateTiketDatabase(tiket_list);

    // Simpan ke database dan histori
    savePesananToFile(*new_pesanan);
    addPesananToHistory(*new_pesanan);

    // Cetak struk
    ofstream struk("struk_" + new_pesanan->username + "_" + new_pesanan->id_tiket + ".txt");
    if (struk.is_open()) {
        struk << "=============== STRUK PEMBAYARAN ===============" << endl;
        struk << "TravelYupi - Tiket Kereta" << endl;
        struk << "Waktu Pemesanan: " << new_pesanan->waktu_pemesanan << endl;
        struk << "==============================================" << endl;
        struk << "ID Tiket       : " << new_pesanan->id_tiket << endl;
        struk << "Rute           : " << new_pesanan->kota_asal << " - " << new_pesanan->kota_tujuan << endl;
        struk << "Tanggal/Jam    : " << new_pesanan->tanggal << " / " << new_pesanan->jam << endl;
        struk << "Nama Penumpang : " << new_pesanan->nama_penumpang << endl;
        struk << "Jumlah Tiket   : " << new_pesanan->jumlah_tiket << endl;
        struk << "Nomor Kursi    : " << new_pesanan->nomor_kursi << endl;
        struk << "Total Harga    : Rp " << new_pesanan->total_harga << endl;
        struk << "Metode Pembayaran: " << new_pesanan->metode_pembayaran << endl;
        struk << "==============================================" << endl;
        struk << "Terima kasih telah menggunakan TravelYupi!" << endl;
        struk.close();

        cout << "\nStruk berhasil dicetak ke file struk_" << new_pesanan->username << "_" << new_pesanan->id_tiket << ".txt" << endl;
    } else {
        cout << "\nGagal mencetak struk." << endl;
    }

    cout << "\nPemesanan berhasil!\n";
    
    string input;
    do {
        cout << "\nTekan 1 untuk kembali: ";
        getline(cin, input);
    } while (!validasiKembali(input));
    
    feature_choice(auth);


    delete new_pesanan; // Hapus pointer
}

void addPesananToHistory(pesanan new_pesanan) {
    // Membuat node baru
    Node* newNode = new Node;
    newNode->data = new_pesanan;  // Menyimpan data pesanan
    newNode->next = nullptr;      // Node baru tidak punya next (akhir list)
    newNode->prev = tail;         // Pointer prev menunjuk ke node terakhir (tail)

    // Jika linked list tidak kosong, hubungkan tail dengan node baru
    if (tail != nullptr) {
        tail->next = newNode;  // Menyambungkan node terakhir dengan node baru
    }

    // Update tail untuk menunjuk ke node baru
    tail = newNode;

    // Jika linked list kosong (head null), set head ke node pertama
    if (head == nullptr) {
        head = newNode;
    }
}

// Fungsi untuk melihat riwayat pesanan
void riwayatPesanan(auth &auth) {
    Node* current = head;  // Pointer untuk traversal dari head
    
    cout << "\n========== Riwayat Pesanan ==========\n" << endl;
    
    bool ada_pesanan = false;
    while (current != nullptr) {
        // Cek apakah pesanan cocok dengan username yang sedang login
        if (current->data.username == auth.user_login) {
            ada_pesanan = true;
            cout << "ID Tiket: " << current->data.id_tiket << endl;
            cout << "Rute: " << current->data.kota_asal << " - " << current->data.kota_tujuan << endl;
            cout << "Tanggal/Jam: " << current->data.tanggal << " / " << current->data.jam << endl;
            cout << "Nama: " << current->data.nama_penumpang << endl;
            cout << "Jumlah Tiket: " << current->data.jumlah_tiket << endl;
            cout << "Nomor Kursi: " << current->data.nomor_kursi << endl;
            cout << "Total Harga: Rp " << current->data.total_harga << endl;
            cout << "Metode Pembayaran: " << current->data.metode_pembayaran << endl;
            cout << "Waktu Pemesanan: " << current->data.waktu_pemesanan << endl;
            cout << string(50, '-') << endl;
        }
        current = current->next;  // Pindah ke node berikutnya
    }
    
    if (!ada_pesanan) {
        cout << "Anda belum memiliki riwayat pesanan.\n";
    }

    string input;
    do {
        cout << "\nTekan 1 untuk kembali: ";
        getline(cin, input);
    } while (!validasiKembali(input));
    
    feature_choice(auth);
}

// Fungsi untuk menu fitur
void feature_choice(auth &auth) {
    string choice;
    bool valid = false;

    cout << "\n========== Menu Fitur ==========\n" << endl;
    cout << "[1] Lihat Semua Tiket\n[2] Pesan Tiket\n[3] Riwayat Pesanan\n[4] Logout\n";

    while (!valid) {
        cout << "Silakan pilih fitur berdasarkan angka: ";
        getline(cin, choice);

        if (choice.empty()) {
            cout << "Input tidak boleh kosong! Silakan isi angka 1-4.\n" << endl;
        } else if (choice == "1") {
            lihatTiket(auth);
            valid = true;
        } else if (choice == "2") {
            pesanTiket(auth);
            valid = true;
        } else if (choice == "3") {
            riwayatPesanan(auth);
            valid = true;
        } else if (choice == "4") {
            cout << "\nLogout berhasil! Sampai jumpa lagi " << auth.user_login << "!" << endl;
            auth.user_login = ""; // Reset user login
            vector<users> user_list;
            chance chance;
            loadUsersFromFile(user_list);
            choice1(user_list, chance, auth);
            valid = true;
        } else {
            cout << "Pilihan tidak valid! Silakan pilih angka dari 1 hingga 4.\n" << endl;
        }
    }
}


// Fungsi untuk ambil input password dengan masking
string getHiddenPassword() {
    string password;
    char ch;

    while (true) {
        ch = _getch(); // Ambil karakter tanpa menampilkan di layar

        if (ch == 13) { // Enter
            cout << endl;
            break;
        } else if (ch == 8) { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Hapus karakter di layar
            }
        } else {
            password.push_back(ch);
            cout << '*'; // Tampilkan bintang
        }
    }

    return password;
}

// Fungsi untuk registrasi pengguna baru
void regis(vector<users> &user_list, chance &chance, auth &auth) {
    regex usernameFormat("^[a-zA-Z0-9_]{3,20}$");

    if (chance.regis_chance == 0) {
        cout << "\nKesempatan registrasi habis! Silakan mencoba lagi!\n";
        chance.regis_chance = 3; // Reset kesempatan
        choice1(user_list, chance, auth);
        return;
    }

    cout << "\n========== Registrasi ==========\n";
    
    users new_user;

    // LOOP UNTUK VALIDASI USERNAME
    while (true) {
        cout << "Masukkan username: ";
        getline(cin, new_user.username);

        // Cek input kosong
        if (new_user.username.empty()) {
            cout << "\nUsername harus diisi!\n";
            continue;
        }

        // Cek format
        if (!regex_match(new_user.username, usernameFormat)) {
            cout << "\nUsername hanya boleh mengandung huruf, angka, dan underscore (_).\n";
            cout << "Minimal 3 karakter, maksimal 20 karakter.\n";
            continue;
        }

        // Cek apakah username sudah terdaftar
        bool username_sudah_ada = false;
        for (const users &user : user_list) {
            if (user.username == new_user.username) {
                username_sudah_ada = true;
                break;
            }
        }

        if (username_sudah_ada) {
            chance.regis_chance -= 1;
            cout << "\nUsername sudah terdaftar! Silakan gunakan username lain.\n";
            cout << "Kesempatan tersisa: " << chance.regis_chance << "x\n";

            if (chance.regis_chance == 0) {
                cout << "\nKesempatan registrasi habis! Silakan mencoba lagi!\n";
                chance.regis_chance = 3;
                choice1(user_list, chance, auth);
                return;
            }
            continue;
        }

        break; // Username valid
    }

    // LOOP UNTUK VALIDASI PASSWORD
    while (true) {
        cout << "Masukkan password: ";
        new_user.password = getHiddenPassword();

        if (new_user.password.empty()) {
            cout << "\nPassword harus diisi!\n";
            continue;
        }

        break; // Password valid
    }

    // Jika valid, simpan user baru
    user_list.push_back(new_user);
    saveUserToFile(new_user);
    cout << "\nRegistrasi berhasil! Silakan login.\n";
    
    // Kembali ke menu utama
    choice1(user_list, chance, auth);
}

// Fungsi untuk login pengguna
void login(vector<users> &user_list, auth &auth) {
    string username, password;

    cout << "\n========== Login ==========\n" << endl;
    cout << "Masukkan username: "; 
    getline(cin, username);
    cout << "Masukkan password: ";
    password = getHiddenPassword();

    // Periksa apakah user_list kosong
    if (user_list.empty()) {
        cout << "\nBelum ada pengguna terdaftar! Silakan registrasi terlebih dahulu.\n";
        chance temp_chance;
        choice1(user_list, temp_chance, auth);
        return;
    }

    for(size_t i = 0; i < user_list.size(); i++) {
        if(user_list[i].username == username && user_list[i].password == password) {
            auth.user_login = username;
            cout << "\nLogin berhasil! Selamat datang " << auth.user_login << "!" << endl;
            feature_choice(auth);
            return;
        }
    }

    cout << "\nUsername atau password salah! Silakan coba lagi.\n";
    chance temp_chance;
    choice1(user_list, temp_chance, auth);
}

// Fungsi untuk keluar dari program
void exitProgram() {
    cout << "\n========== Terima Kasih ==========\n" << endl;
    cout << "Terima kasih sudah menggunakan layanan TravelYupi!" << endl;
    cout << "Sampai jumpa kembali di perjalanan berikutnya.\n" << endl;
    exit(0); // Exit the program
}

// Fungsi untuk menu utama (login/registrasi)
void choice1(vector<users> &user_list, chance &chance, auth &auth) {
    string choice;
    bool valid = false;

    cout << "\n========== Selamat Datang di TravelYupi ==========\n" << endl;
    cout << "[1] Login\n[2] Registrasi\n[3] Keluar\n";

    while (!valid) {
        cout << "Silakan pilih menu berdasarkan angka: ";
        getline(cin, choice);

        if (choice.empty()) {
            cout << "Input tidak boleh kosong! Silakan isi angka 1-3.\n" << endl;
        } else if (choice == "1") {
            login(user_list, auth);
            valid = true;
        } else if (choice == "2") {
            regis(user_list, chance, auth);
            valid = true;
        } else if (choice == "3") {
            exitProgram();
            valid = true;
        } else {
            cout << "Pilihan tidak valid! Silakan pilih angka 1-3.\n" << endl;
        }
    }
}

