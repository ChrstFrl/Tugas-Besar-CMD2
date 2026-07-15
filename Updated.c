#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define limitKamar 1000
#define data "dbKamar.bin"

typedef struct datakos // ini struct
{
    int no;
    char tipe[50];
    int harga;
    int status; 
} kamar;

kamar listkamar[limitKamar];
int jmlkamar = 0;

void bukadata(){ // ini file sequential bagian buka
    FILE *file = fopen(data, "rb");
    if (file == NULL) {
        jmlkamar = 0;
        return;
    }
    jmlkamar = fread(listkamar, sizeof(kamar), limitKamar, file);
    fclose(file);
}

void simpandata(){ // ini file sequential juga, tp bagian simpan
    FILE *file = fopen(data, "wb");
    if(file == NULL){
        printf("\nGagal menyimpan data!!!");
        return;
    }
    fwrite(listkamar, sizeof(kamar), jmlkamar, file);
    fclose(file);
}

int loginAdmin(){ // ini function login admin 
    char un[50];
    char pw[20];
    int n = 3;

    printf("\n==== SISTEM MANAJEMEN HOTEL ====");

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
    printf("\nTerminating program..........\n");
    return 0;
}

void add() { // ini tambah kamar lek 
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

    jmlkamar++;
    simpandata(); // langsung simpan
    printf("[+] Data kamar berhasil ditambahkan (Status: Kosong)!\n");
}

void upd() { //ini perbaharui kamar
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

void del() { //ini hapus kamar
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
            listkamar[i] = listkamar[i + 1]; // Geser data
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

    for (int i = 0; i < jmlkamar - 1; i++) { //bubble sort
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

int main(){ //menu admin
    if (!loginAdmin())
    {
        return 0;
    }
    bukadata();

    int menu;
    do
    {
        system("cls");
        printf("\n==== Menu Manajemen D'Kost ====");
        printf("\n1. Tambah Kamar");
        printf("\n2. Hapus Kamar");
        printf("\n3. Perbaharui Kamar");
        printf("\n4. Tampilkan Daftar Kamar");
        printf("\n5. Ubah Status Kamar (Kosong/Terisi)"); 
        printf("\n6. Logout"); 
        printf("\nPilih menu: ");
        scanf("%d", &menu);

        system("cls");

        switch (menu)
        {
        case 1: add(); break;
        case 2: del(); break;
        case 3: upd(); break;
        case 4: ls(); break;
        case 5: ubahStatus(); break; 
        case 6: printf("Sampai jumpa admin! Semoga harimu menyenangkan.\n"); break;
        
        default: 
            printf("Menu tidak valid!\n"); 
            break;
        }

        if (menu != 6) 
        {
            printf("\n");
            system("pause");
        }
    
    } while (menu != 6); 
    
    return 0;
}