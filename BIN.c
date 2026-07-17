#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define limitKamar 1000

#define dataKamar "dbKamar.bin"
#define dataPengajuan "dbPengajuan.bin"

typedef struct{
    int no;
    char tipe[50];
    int harga;
    char status[20];
} Kamar;

typedef struct{
    int noKamar;
    char username[30];
    char nama[50];
    char noHp[20];
    char status[20];   
} Pengajuan;

void bukaDataKamar();
void ajukanPenyewaan();
void lihatStatusPengajuan(char username[]);
void bukaDataBookmark();
void bookmarkKamar(char username[]);
void lihatBookmark(char username[]);

Kamar listKamar[limitKamar];
int jmlKamar = 0;

void bukaDataKamar(){
    FILE *file = fopen(dataKamar, "rb");

    if(file == NULL){
        jmlKamar = 0;
        return;
    }

    jmlKamar = fread(listKamar, sizeof(Kamar), limitKamar, file);
    fclose(file);
}

void ajukanPenyewaan(){

    FILE *file;
    Pengajuan p;

    int noKamar;
    int ketemu = 0;
    int tersedia = 0;

    bukaDataKamar();

    if(jmlKamar == 0){
        printf("\nBelum ada data kamar!\n");
        return;
    }

    printf("\n========== DAFTAR KAMAR TERSEDIA ==========\n");

    for(int i = 0; i < jmlKamar; i++){

        if(strcmp(listKamar[i].status, "Kosong") == 0){

            tersedia = 1;

            printf("\nNo Kamar : %d", listKamar[i].no);
            printf("\nTipe     : %s", listKamar[i].tipe);
            printf("\nHarga    : Rp %d", listKamar[i].harga);
            printf("\nStatus   : %s\n", listKamar[i].status);
        }
    }

    if(tersedia == 0){
        printf("\nTidak ada kamar yang tersedia!\n");
        return;
    }

    printf("\nMasukkan nomor kamar yang ingin disewa : ");
    scanf("%d", &noKamar);

    for(int i = 0; i < jmlKamar; i++){

        if(listKamar[i].no == noKamar){

            ketemu = 1;

            if(strcmp(listKamar[i].status, "Kosong") != 0){
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
#define dataBookmark "dbBookmark.bin"
#define limitBookmark 1000

typedef struct{
    char username[30];
    int noKamar;
} Bookmark;

Bookmark listBookmark[limitBookmark];
int jmlBookmark = 0;

void bukaDataBookmark(){
    FILE *file = fopen(dataBookmark, "rb");

    if(file == NULL){
        jmlBookmark = 0;
        return;
    }

    jmlBookmark = fread(listBookmark, sizeof(Bookmark), limitBookmark, file);
    fclose(file);
}
// pointer
void isiBookmark(Bookmark *b, int noKamar, char username[]){
    b->noKamar = noKamar;
    strcpy(b->username, username);
}
void bookmarkKamar(char username[]){

    bukaDataKamar();
    bukaDataBookmark();

    int noKamar;
    int ketemu = 0;

    if(jmlKamar == 0){
        printf("\nBelum ada data kamar!\n");
        return;
    }

    printf("\n========== DAFTAR KAMAR ==========\n");

    for(int i = 0; i < jmlKamar; i++){
        printf("\nNo Kamar : %d", listKamar[i].no);
        printf("\nTipe     : %s", listKamar[i].tipe);
        printf("\nHarga    : Rp %d", listKamar[i].harga);
        printf("\nStatus   : %s\n", listKamar[i].status);
    }

    printf("\nMasukkan nomor kamar yang ingin di-bookmark : ");
    scanf("%d", &noKamar);

    for(int i = 0; i < jmlKamar; i++){
        if(listKamar[i].no == noKamar){
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

            for(int j = 0; j < jmlKamar; j++){

                if(listKamar[j].no == listBookmark[i].noKamar){

                    ketemu = 1;

                    printf("\nNo Kamar : %d", listKamar[j].no);
                    printf("\nTipe     : %s", listKamar[j].tipe);
                    printf("\nHarga    : Rp %d", listKamar[j].harga);
                    printf("\nStatus   : %s\n", listKamar[j].status);
                }
            }
        }
    }

    if(ketemu == 0){
        printf("\nAnda belum memiliki kamar favorit.\n");
    }
}