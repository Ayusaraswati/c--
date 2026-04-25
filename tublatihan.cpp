#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <iomanip>
using namespace std;

struct Mahasiswa {
    string nama, npm, kelas, posisi;
    int nilai;
};

vector<Mahasiswa> data;
bool visualMode = false;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void tampilTabel() {
    cout << left << setw(3) << "No"
        << setw(15) << "Name / Nama"
        << setw(12) << "SIN / NPM"
        << setw(8) << "Class"
        << setw(15) << "Position"
        << setw(6) << "Grade / Nilai" << endl;
    cout << string(60, '-') << endl;

    for (int i = 0; i < data.size(); i++) {
        cout << setw(3) << i+1
            << setw(15) << data[i].nama
            << setw(12) << data[i].npm
            << setw(8) << data[i].kelas
            << setw(15) << data[i].posisi
            << setw(6) << data[i].nilai << endl;
    }
}

void insertionSort(int &op) {
    for (int i = 1; i < data.size(); i++) {
        Mahasiswa key = data[i];
        int j = i - 1;

        while (j >= 0 && data[j].nilai < key.nilai) {
            data[j + 1] = data[j];
            j--;
            op++;
        }
        data[j + 1] = key;
    }
}

int partition(int low, int high, int &op) {
    int pivot = data[high].nilai;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (data[j].nilai > pivot) {
            i++;
            swap(data[i], data[j]);
        }
        op++;
    }
    swap(data[i + 1], data[high]);
    return i + 1;
}

void quickSort(int low, int high, int &op) {
    if (low < high) {
        int pi = partition(low, high, op);
        quickSort(low, pi - 1, op);
        quickSort(pi + 1, high, op);
    }
}

bool hampirTerurut() {
    int count = 0;
    for (int i = 0; i < data.size()-1; i++) {
        if (data[i].nilai < data[i+1].nilai) count++;
    }
    return count <= data.size()/4;
}

void smartSort() {
    int op = 0;
    if (data.size() < 10 || hampirTerurut())
        insertionSort(op);
    else
        quickSort(0, data.size()-1, op);

    cout << "Total operation: " << op << endl;
}

void tambahData() {
    Mahasiswa m;
    cin.ignore();

    cout << "Name / Nama       : "; getline(cin, m.nama);
    cout << "SIN / NPM         : "; getline(cin, m.npm);
    cout << "Class / Kelas     : "; getline(cin, m.kelas);
    cout << "Position / Posisi : "; getline(cin, m.posisi);

    do {
        cout << "Nilai (0-100): ";
        cin >> m.nilai;
    } while (m.nilai < 0 || m.nilai > 100);

    data.push_back(m);
    smartSort();
}

void hapusData() {
    tampilTabel();
    int idx;
    cout << "Delete number: ";
    cin >> idx;

    if (idx > 0 && idx <= data.size()) {
        data.erase(data.begin() + idx - 1);
        cout << "Data deleted!\n";
    } else {
        cout << "Invalid!\n";
    }
}

void editData() {
    tampilTabel();
    int idx;
    cout << "Edit number: ";
    cin >> idx;
    cin.ignore();

    if (idx > 0 && idx <= data.size()) {
        Mahasiswa &m = data[idx - 1];

        cout << "New Name / Nama baru: "; getline(cin, m.nama);
        cout << "New SIN / NPM baru: "; getline(cin, m.npm);
        cout << "New Class / Kelas baru: "; getline(cin, m.kelas);
        cout << "New Position / Posisi baru: "; getline(cin, m.posisi);
        cout << "New Grade / Nilai baru: "; cin >> m.nilai;

        smartSort();
    }
}

void cariData() {
    string key;
    cin.ignore();
    cout << "Search Name/SIN (Cari nama/NPM): ";
    getline(cin, key);

    for (auto &m : data) {
        if (m.nama.find(key) != string::npos ||
            m.npm.find(key) != string::npos) {
            cout << m.nama << " | " << m.npm << endl;
        }
    }
}

void statistik() {
    if (data.empty()) return;

    int total = 0, max = data[0].nilai, min = data[0].nilai;

    for (auto &m : data) {
        total += m.nilai;
        if (m.nilai > max) max = m.nilai;
        if (m.nilai < min) min = m.nilai;
    }

    cout << "Average / Rata-rata: " << total / data.size() << endl;
    cout << "Highest / Tertinggi: " << max << endl;
    cout << "Lowest / Terendah : " << min << endl;
}

void saveData() {
    ofstream f("data.txt");
    for (auto &m : data) {
        f << m.nama << "|"
        << m.npm << "|"
        << m.kelas << "|"
        << m.posisi << "|"
        << m.nilai << endl;
    }
}

void loadData() {
    ifstream f("data.txt");
    Mahasiswa m;

    while (getline(f, m.nama, '|')) {
        getline(f, m.npm, '|');
        getline(f, m.kelas, '|');
        getline(f, m.posisi, '|');
        f >> m.nilai;
        f.ignore();
        data.push_back(m);
    }
}

int main() {
    loadData();
    int pilih;

    do {
        cout << "\n=== SMART RANKING SYSTEM ===\n";
        cout << "1. Add Data / Tambah Data\n";
        cout << "2. Show Rank / Tampilkan Ranking\n";
        cout << "3. Edit Data\n";
        cout << "4. Delete Data / Hapus Data\n";
        cout << "5. Search Data / Cari Data\n";
        cout << "6. Statistic / Statistik\n";
        cout << "7. Save\n";
        cout << "8. Exit\n";
        cout << "Choose / Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: tambahData(); break;
            case 2: tampilTabel(); break;
            case 3: editData(); break;
            case 4: hapusData(); break;
            case 5: cariData(); break;
            case 6: statistik(); break;
            case 7: saveData(); break;
            case 8: saveData(); cout << "Quitting...\n"; break;
            default: cout << "Wrong input! / Salah input!\n";
        }

    } while (pilih != 8);

    return 0;
}
