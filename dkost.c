#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define limitKamar 1000
#define dataKamar "dbKamar.bin"
#define dataPenyewa "dbPenyewa.bin"
#define dataPengajuan "dbPengajuan.bin"
#define dataBookmark "dbBookmark.bin"
#define limitBookmark 1000

typedef struct{
    int noKamar;
    char username[30];
    char nama[50];
    char noHp[20];
    char status[20];   
} Pengajuan;

typedef struct datakos
{
    int no;
    char tipe[50];
    int harga;
    int status;        
    int pengajuan;     
} kamar;

typedef struct {
    char username[30];
    char password[30];
} Penyewa;

typedef struct{
    char username[30];
    int noKamar;
} Bookmark;

kamar listkamar[limitKamar];
int jmlkamar = 0;

Bookmark listBookmark[limitBookmark];
int jmlBookmark = 0;

void bukaDataKamar();
void ajukanPenyewaan();
void lihatStatusPengajuan(char username[]);
void bukaDataBookmark();
void bookmarkKamar(char username[]);
void lihatBookmark(char username[]);
void bukadata();
void simpandata();

void bukaDataKamar(){
    FILE *file = fopen(dataKamar, "rb");

    if(file == NULL){
        jmlkamar = 0;
        return;
    }

    jmlkamar = fread(listkamar, sizeof(kamar), limitKamar, file);
    fclose(file);
}

void ajukanPenyewaan(){

    FILE *file;
    Pengajuan p;

    int noKamar;
    int ketemu = 0;
    int tersedia = 0;

    bukaDataKamar();

    if(jmlkamar == 0){
        printf("\nBelum ada data kamar!\n");
        return;
    }

    printf("\n========== DAFTAR KAMAR TERSEDIA ==========\n");

    for(int i = 0; i < jmlkamar; i++){

        if(listkamar[i].status == 0){

            tersedia = 1;

            printf("\nNo Kamar : %d", listkamar[i].no);
            printf("\nTipe     : %s", listkamar[i].tipe);
            printf("\nHarga    : Rp %d", listkamar[i].harga);
            printf("\nStatus   : %s\n", listkamar[i].status == 0 ? "Kosong" : "Terisi");
        }
    }

    if(tersedia == 0){
        printf("\nTidak ada kamar yang tersedia!\n");
        return;
    }

    printf("\nMasukkan nomor kamar yang ingin disewa : ");
    scanf("%d", &noKamar);

    for(int i = 0; i < jmlkamar; i++){

        if(listkamar[i].no == noKamar){

            ketemu = 1;

            if(listkamar[i].status != 0){
                printf("\nKamar sudah terisi!\n");
                return;
            }

            printf("\n========== DATA PENYEWA ==========\n");

            printf("Username : ");
            scanf("%s", p.username);

            printf("Nama     : ");
            scanf(" %[^\n]", p.nama);

            printf("No HP    : ");
            scanf("%s", p.noHp);

            p.noKamar = noKamar;
            strcpy(p.status, "Menunggu");

            file = fopen(dataPengajuan, "ab");

            if(file == NULL){
                printf("\nGagal menyimpan data!\n");
                return;
            }

            fwrite(&p, sizeof(Pengajuan), 1, file);
            fclose(file);

            printf("\n======================================");
            printf("\nPengajuan berhasil dikirim!");
            printf("\nStatus Pengajuan : Menunggu");
            printf("\nSilakan menunggu konfirmasi Admin.");
            printf("\n======================================\n");

            return;
        }
    }

    if(ketemu == 0){
        printf("\nNomor kamar tidak ditemukan!\n");
    }
}

void lihatStatusPengajuan(char username[]){

    FILE *file = fopen(dataPengajuan, "rb");
    Pengajuan p;
    int ketemu = 0;

    if(file == NULL){
        printf("\nBelum ada data pengajuan!\n");
        return;
    }

    printf("\n========== STATUS PENGAJUAN ==========\n");

    while(fread(&p, sizeof(Pengajuan), 1, file) == 1){

        if(strcmp(p.username, username) == 0){

            ketemu = 1;

            printf("\nNo Kamar : %d", p.noKamar);
            printf("\nNama     : %s", p.nama);
            printf("\nNo HP    : %s", p.noHp);
            printf("\nStatus   : %s\n", p.status);
        }
    }

    fclose(file);

    if(ketemu == 0){
        printf("\nAnda belum pernah mengajukan penyewaan.\n");
    }
}

void bukaDataBookmark(){
    FILE *file = fopen(dataBookmark, "rb");

    if(file == NULL){
        jmlBookmark = 0;
        return;
    }

    jmlBookmark = fread(listBookmark, sizeof(Bookmark), limitBookmark, file);
    fclose(file);
}

void isiBookmark(Bookmark *b, int noKamar, char username[]){
    b->noKamar = noKamar;
    strcpy(b->username, username);
}

void bookmarkKamar(char username[]){

    bukaDataKamar();
    bukaDataBookmark();

    int noKamar;
    int ketemu = 0;

    if(jmlkamar == 0){
        printf("\nBelum ada data kamar!\n");
        return;
    }

    printf("\n========== DAFTAR KAMAR ==========\n");

    for(int i = 0; i < jmlkamar; i++){
        printf("\nNo Kamar : %d", listkamar[i].no);
        printf("\nTipe     : %s", listkamar[i].tipe);
        printf("\nHarga    : Rp %d", listkamar[i].harga);
        printf("\nStatus   : %s\n", listkamar[i].status == 0 ? "Kosong" : "Terisi");
    }

    printf("\nMasukkan nomor kamar yang ingin di-bookmark : ");
    scanf("%d", &noKamar);

    for(int i = 0; i < jmlkamar; i++){
        if(listkamar[i].no == noKamar){
            ketemu = 1;
            break;
        }
    }

    if(ketemu == 0){
        printf("\nNomor kamar tidak ditemukan!\n");
        return;
    }

    for(int i = 0; i < jmlBookmark; i++){
        if(strcmp(listBookmark[i].username, username) == 0 && listBookmark[i].noKamar == noKamar){
            printf("\nKamar ini sudah ada di bookmark Anda!\n");
            return;
        }
    }

    Bookmark *b = &listBookmark[jmlBookmark];
    isiBookmark(b, noKamar, username);
    jmlBookmark++;

    FILE *file = fopen(dataBookmark, "wb");

    if(file == NULL){
        printf("\nGagal menyimpan bookmark!\n");
        return;
    }

    fwrite(listBookmark, sizeof(Bookmark), jmlBookmark, file);
    fclose(file);

    printf("\nKamar berhasil ditambahkan ke daftar bookmark!\n");
}

void lihatBookmark(char username[]){

    bukaDataBookmark();
    bukaDataKamar();

    int ketemu = 0;

    printf("\n========== DAFTAR BOOKMARK  ==========\n");

    for(int i = 0; i < jmlBookmark; i++){

        if(strcmp(listBookmark[i].username, username) == 0){

            for(int j = 0; j < jmlkamar; j++){

                if(listkamar[j].no == listBookmark[i].noKamar){

                    ketemu = 1;

                    printf("\nNo Kamar : %d", listkamar[j].no);
                    printf("\nTipe     : %s", listkamar[j].tipe);
                    printf("\nHarga    : Rp %d", listkamar[j].harga);
                    printf("\nStatus   : %s\n", listkamar[j].status == 0 ? "Kosong" : "Terisi");
                }
            }
        }
    }

    if(ketemu == 0){
        printf("\nAnda belum memiliki kamar favorit.\n");
    }
}

void registrasiPenyewa() {
    FILE *file;
    Penyewa p, temp;
    int ketemu = 0;

    printf("\n========== REGISTRASI PENYEWA ==========\n");
    printf("Username Baru : ");
    scanf("%s", p.username);

    file = fopen(dataPenyewa, "rb");
    if (file != NULL) {
        while (fread(&temp, sizeof(Penyewa), 1, file) == 1) {
            if (strcmp(temp.username, p.username) == 0) {
                ketemu = 1;
                break;
            }
        }
        fclose(file);
    }

    if (ketemu == 1) {
        printf("\nUsername sudah digunakan!\n");
        return;
    }

    printf("Password Baru : ");
    scanf("%s", p.password);

    if ((file = fopen(dataPenyewa, "ab")) != NULL) {
        fwrite(&p, sizeof(Penyewa), 1, file);
        fclose(file);
        printf("\nRegistrasi berhasil! Silakan Login.\n");
    }
}

int loginPenyewa(char *loggedInUser) {
    FILE *file;
    Penyewa temp;
    char inputUser[30], inputPass[30];
    int attempt = 0;

    printf("\n========== LOGIN PENYEWA ==========\n");
    while (attempt < 3) {
        printf("Username : ");
        scanf("%s", inputUser);
        printf("Password : ");
        scanf("%s", inputPass);

        file = fopen(dataPenyewa, "rb");
        if (file != NULL) {
            while (fread(&temp, sizeof(Penyewa), 1, file) == 1) {
                if (strcmp(temp.username, inputUser) == 0 && strcmp(temp.password, inputPass) == 0) {
                    strcpy(loggedInUser, temp.username);
                    fclose(file);
                    printf("\nLogin Berhasil! Selamat datang, %s.\n", loggedInUser);
                    return 1;
                }
            }
            fclose(file);
        }
        
        attempt++;
        printf("\nUsername atau Password salah! (Sisa: %d)\n\n", 3 - attempt);
    }
    printf("Gagal login 3 kali. Akses ditolak!\n");
    return 0;
}

void lihatDaftarKamarTersedia() {
    bukaDataKamar();

    if (jmlkamar == 0) {
        printf("\nBelum ada data kamar!\n");
        return;
    }

    int ada = 0;
    printf("\n========== DAFTAR KAMAR TERSEDIA ==========\n");
    for (int i = 0; i < jmlkamar; i++) {
        if (listkamar[i].status == 0) {
            ada = 1;
            printf("\nNo Kamar : %d\nTipe     : %s\nHarga    : Rp %d\nStatus   : %s\n", 
                   listkamar[i].no, listkamar[i].tipe, listkamar[i].harga, listkamar[i].status == 0 ? "Kosong" : "Terisi");
        }
    }

    if (!ada) printf("\nTidak ada kamar yang tersedia.\n");
}

void cariKamar()
{
    int noCari;
    int ketemu = 0;

    bukadata(); 

    if (jmlkamar == 0)
    {
        printf("\nBelum ada data kamar!\n");
        return;
    }

    printf("\n=== PENCARIAN DATA KAMAR ===\n");
    printf("Masukkan Nomor Kamar : ");
    scanf("%d", &noCari);

    for(int i = 0; i < jmlkamar; i++)
    {
        if(listkamar[i].no == noCari)
        {
            ketemu = 1;

            printf("\n===== DATA KAMAR DITEMUKAN =====\n");
            printf("Nomor Kamar : %d\n", listkamar[i].no);
            printf("Tipe Kamar  : %s\n", listkamar[i].tipe);
            printf("Harga       : Rp %d\n", listkamar[i].harga);

            if(listkamar[i].status == 0)
            {
                printf("Status      : Kosong\n");
            }
            else
            {
                printf("Status      : Terisi\n");
            }

            break;
        }
    }

    if(ketemu == 0)
    {
        printf("\nData kamar tidak ditemukan!\n");
    }
}

void tampilPengajuan()
{
    if (jmlkamar == 0)
    {
        printf("\nBelum ada data kamar!\n");
        return;
    }

    printf("\n========== DAFTAR PENGAJUAN ==========\n");

    for(int i = 0; i < jmlkamar; i++)
    {
        printf("\nNomor Kamar : %d\n", listkamar[i].no);
        printf("Tipe Kamar  : %s\n", listkamar[i].tipe);

        printf("Status Pengajuan : ");

        if(listkamar[i].pengajuan == 0)
            printf("Menunggu\n");
        else if(listkamar[i].pengajuan == 1)
            printf("Disetujui\n");
        else
            printf("Ditolak\n");
    }
}

void konfirmasiPengajuan()
{
    int noCari;
    int pilihan;
    int index = -1;

    if(jmlkamar == 0)
    {
        printf("\nBelum ada data kamar!\n");
        return;
    }

    tampilPengajuan();

    printf("\nMasukkan Nomor Kamar : ");
    scanf("%d", &noCari);

    for(int i = 0; i < jmlkamar; i++)
    {
        if(listkamar[i].no == noCari)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        printf("\nNomor kamar tidak ditemukan!\n");
        return;
    }

    printf("\n1. Setujui Pengajuan");
    printf("\n2. Tolak Pengajuan");
    printf("\nPilihan : ");
    scanf("%d", &pilihan);

    if(pilihan == 1)
    {
        listkamar[index].pengajuan = 1;
        listkamar[index].status = 1;

        printf("\nPengajuan disetujui.");
        printf("\nStatus kamar menjadi TERISI.\n");
    }
    else if(pilihan == 2)
    {
        listkamar[index].pengajuan = 2;
        listkamar[index].status = 0;

        printf("\nPengajuan ditolak.");
        printf("\nStatus kamar tetap KOSONG.\n");
    }
    else
    {
        printf("\nPilihan tidak valid!\n");
        return;
    }

    simpandata();

    FILE *filePengajuan = fopen(dataPengajuan, "r+b"); // r+b digunakan agar bisa read dan write di file yang sama
    if (filePengajuan != NULL) {
        Pengajuan p;
        while (fread(&p, sizeof(Pengajuan), 1, filePengajuan) == 1) {
            if (p.noKamar == noCari && strcmp(p.status, "Menunggu") == 0) {
                
                if (pilihan == 1) {
                    strcpy(p.status, "Disetujui");
                } else if (pilihan == 2) {
                    strcpy(p.status, "Ditolak");
                }
                
                fseek(filePengajuan, -sizeof(Pengajuan), SEEK_CUR);
                fwrite(&p, sizeof(Pengajuan), 1, filePengajuan);
                break; 
            }
        }
        fclose(filePengajuan);
    }

}

void lihatStatusKamar()
{
    bukadata();

    if (jmlkamar == 0) {
        printf("\nBelum ada data kamar!\n");
        return;
    }

    int pilihan;
    int ditemukan = 0;

    printf("\n===== LIHAT STATUS KAMAR =====\n");
    printf("1. Kamar Kosong\n");
    printf("2. Kamar Terisi\n");
    printf("Pilih : ");
    scanf("%d", &pilihan);

    if (pilihan != 1 && pilihan != 2) {
        printf("\nPilihan tidak valid!\n");
        return;
    }

    printf("\n=====================================================\n");
    printf("%-10s %-20s %-12s %-10s\n",
           "No", "Tipe", "Harga", "Status");
    printf("=====================================================\n");

    for (int i = 0; i < jmlkamar; i++) {

        if ((pilihan == 1 && listkamar[i].status == 0) ||
            (pilihan == 2 && listkamar[i].status == 1)) {

            ditemukan = 1;

            printf("%-10d %-20s Rp %-8d %-10s\n",
                   listkamar[i].no,
                   listkamar[i].tipe,
                   listkamar[i].harga,
                   listkamar[i].status == 0 ? "Kosong" : "Terisi");
        }
    }

    if (!ditemukan) {
        printf("\nData Tidak Ditemukan!\n");
    }
}

void bukadata(){
    FILE *file = fopen(dataKamar, "rb");
    if (file == NULL) {
        jmlkamar = 0;
        return;
    }
    jmlkamar = fread(listkamar, sizeof(kamar), limitKamar, file);
    fclose(file);
}

void simpandata(){
    FILE *file = fopen(dataKamar, "wb");
    if(file == NULL){
        printf("\nGagal menyimpan data!!!");
        return;
    }
    fwrite(listkamar, sizeof(kamar), jmlkamar, file);
    fclose(file);
}

int loginAdmin(){
    char un[50];
    char pw[20];
    int n = 3;

    printf("\n==== SISTEM MANAJEMEN D'kost ====");

    while (n > 0) {
        printf("\nUsername: ");
        scanf("%s", un);
        printf("Password: ");
        scanf("%s", pw);

        if (strcmp (un, "admin") == 0 && strcmp(pw, "6767") == 0)
        {
            printf("\nLogin Sukses!!!");
            printf("\nHaloooo Admin!\n");
            return 1;
        } else {
            n--;
            printf("\nLogin Gagal!!!");
            printf("\nSisa percobaan: %d", n);
        }
    }

    printf("\nPercobaan login tidak wajar terdeteksi!!!");
    printf("\nRedirecting ke Menu Utama.....\n");
    return 0;
}

void add() {
    if (jmlkamar >= limitKamar) {
        printf("\n[-] Kapasitas kamar sudah penuh!\n");
        return;
    }

    printf("\n=== Tambah Kamar ===");
    printf("\nNo Kamar   : ");
    scanf("%d", &listkamar[jmlkamar].no);
    printf("Tipe Kamar : ");
    scanf(" %[^\n]", listkamar[jmlkamar].tipe);
    printf("Harga      : ");
    scanf("%d", &listkamar[jmlkamar].harga);
    
    listkamar[jmlkamar].status = 0; 
    listkamar[jmlkamar].pengajuan = 0;

    jmlkamar++;
    simpandata();
    printf("[+] Data kamar berhasil ditambahkan (Status: Kosong)!\n");
}

void upd() {
    int noUpdate, index = -1;
    if (jmlkamar == 0) {
        printf("\n[-] Belum ada data kamar!\n");
        return;
    }

    printf("\n=== Perbarui Kamar ===");
    printf("\nMasukkan No Kamar yang mau diupdate: ");
    scanf("%d", &noUpdate);

    for (int i = 0; i < jmlkamar; i++) {
        if (listkamar[i].no == noUpdate) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        printf("Tipe Kamar Baru : ");
        scanf(" %[^\n]", listkamar[index].tipe);
        printf("Harga Baru      : ");
        scanf("%d", &listkamar[index].harga);
        
        simpandata();
        printf("[+] Data kamar berhasil diperbarui!\n");
    } else {
        printf("[-] Kamar nomor %d tidak ditemukan!\n", noUpdate);
    }
}

void del() {
    int noHapus, index = -1;
    if (jmlkamar == 0) {
        printf("\n[-] Belum ada data kamar!\n");
        return;
    }

    printf("\n=== Hapus Kamar ===");
    printf("\nMasukkan No Kamar yang mau dihapus: ");
    scanf("%d", &noHapus);

    for (int i = 0; i < jmlkamar; i++) {
        if (listkamar[i].no == noHapus) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (int i = index; i < jmlkamar - 1; i++) {
            listkamar[i] = listkamar[i + 1];
        }
        jmlkamar--;
        simpandata();
        printf("[+] Kamar nomor %d berhasil dihapus!\n", noHapus);
    } else {
        printf("[-] Kamar nomor %d tidak ditemukan!\n", noHapus);
    }
}

void ubahStatus() { 
    int noKamar, index = -1;
    if (jmlkamar == 0) {
        printf("\n[-] Belum ada data kamar!\n");
        return;
    }

    printf("\n=== Ubah Status Kamar ===");
    printf("\nMasukkan No Kamar: ");
    scanf("%d", &noKamar);

    for (int i = 0; i < jmlkamar; i++) {
        if (listkamar[i].no == noKamar) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        if (listkamar[index].status == 0) {
            listkamar[index].status = 1;
            printf("Status Kamar %d sekarang menjadi: TERISI\n", noKamar);
        } else {
            listkamar[index].status = 0;
            printf("Status Kamar %d sekarang menjadi: KOSONG\n", noKamar);
        }
        simpandata(); 
    } else {
        printf("[-] Kamar nomor %d tidak ditemukan!\n", noKamar);
    }
}

void ls() {
    if (jmlkamar == 0) {
        printf("\n[-] Belum ada data kamar!\n");
        return;
    }

    kamar temp[limitKamar];
    for (int i = 0; i < jmlkamar; i++) {
        temp[i] = listkamar[i];
    }

    for (int i = 0; i < jmlkamar - 1; i++) {
        for (int j = 0; j < jmlkamar - i - 1; j++) {
            if (temp[j].harga > temp[j + 1].harga) {
                kamar t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    printf("\n=== Daftar Kamar (Harga Termurah) ===\n");
    printf("No   | Tipe Kamar             | Harga       | Status\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < jmlkamar; i++) {
        char statusTeks[10];
        if (temp[i].status == 1) {
            strcpy(statusTeks, "Terisi");
        } else {
            strcpy(statusTeks, "Kosong");
        }
        
        printf("%-4d | %-22s | Rp %-8d | %s\n", temp[i].no, temp[i].tipe, temp[i].harga, statusTeks);
    }
    printf("----------------------------------------------------------\n");
}

int main(){
    int pilihanUtama;
    char loggedInUser[30];

    bukadata();

    do {
        system("cls");
        printf("\n==== SELAMAT DATANG DI SISTEM D'KOST ====\n");
        printf("1. Login Admin\n");
        printf("2. Login Penyewa\n");
        printf("3. Registrasi Penyewa\n");
        printf("4. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihanUtama);

        switch(pilihanUtama) {
            case 1:
                if (loginAdmin()) {
                    int menuAdmin;
                    do {
                        system("cls");
                        printf("\n==== Menu Manajemen D'Kost (Admin) ====");
                        printf("\n1. Tambah Kamar");
                        printf("\n2. Hapus Kamar");
                        printf("\n3. Perbaharui Kamar");
                        printf("\n4. Tampilkan Daftar Kamar");
                        printf("\n5. Ubah Status Kamar (Kosong/Terisi)");
                        printf("\n6. Konfirmasi Pengajuan");
                        printf("\n7. Logout");
                        printf("\nPilih menu: ");
                        scanf("%d", &menuAdmin);

                        system("cls");

                        switch (menuAdmin) {
                            case 1: add(); break;
                            case 2: del(); break;
                            case 3: upd(); break;
                            case 4: ls(); break;
                            case 5: ubahStatus(); break;
                            case 6: konfirmasiPengajuan(); break;
                            case 7: printf("Sampai jumpa admin! Semoga harimu menyenangkan.\n"); break;
                            default: printf("Menu tidak valid!\n"); break;
                        }

                        if (menuAdmin != 7) {
                            printf("\n");
                            system("pause");
                        }
                    } while (menuAdmin != 7);
                }
                break;

            case 2:
                if (loginPenyewa(loggedInUser)) {
                    int menuPenyewa;
                    do {
                        system("cls");
                        printf("\n==== Menu Penyewa ====");
                        printf("\n1. Lihat Daftar Kamar Tersedia");
                        printf("\n2. Cari Kamar");
                        printf("\n3. Lihat Status Kamar");
                        printf("\n4. Ajukan Penyewaan");
                        printf("\n5. Lihat Status Pengajuan");
                        printf("\n6. Bookmark Kamar");
                        printf("\n7. Lihat Daftar Bookmark");
                        printf("\n8. Logout");
                        printf("\nPilih menu: ");
                        scanf("%d", &menuPenyewa);

                        system("cls");

                        switch (menuPenyewa) {
                            case 1: lihatDaftarKamarTersedia(); break;
                            case 2: cariKamar(); break;
                            case 3: lihatStatusKamar(); break;
                            case 4: ajukanPenyewaan(); break;
                            case 5: lihatStatusPengajuan(loggedInUser); break;
                            case 6: bookmarkKamar(loggedInUser); break;
                            case 7: lihatBookmark(loggedInUser); break;
                            case 8: printf("Sampai jumpa, %s!\n", loggedInUser); break;
                            default: printf("Menu tidak valid!\n"); break;
                        }

                        if (menuPenyewa != 8) {
                            printf("\n");
                            system("pause");
                        }
                    } while (menuPenyewa != 8);
                }
                break;

            case 3:
                system("cls");
                registrasiPenyewa();
                printf("\n");
                system("pause");
                break;

            case 4:
                printf("\nTerima kasih telah menggunakan sistem D'Kost!\n");
                break;

            default:
                printf("\nPilihan tidak valid!\n");
                system("pause");
                break;
        }
    } while (pilihanUtama != 4);

    return 0;
}