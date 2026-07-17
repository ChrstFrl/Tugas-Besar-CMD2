#define dataKamar "dbKamar.bin"
#define dataPenyewa "dbPenyewa.bin"

typedef struct {
    char username[30];
    char password[30];
} Penyewa;

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

    if (jmlKamar == 0) {
        printf("\nBelum ada data kamar!\n");
        return;
    }

    int ada = 0;
    printf("\n========== DAFTAR KAMAR TERSEDIA ==========\n");
    for (int i = 0; i < jmlKamar; i++) {
        if (strcmp(listKamar[i].status, "Kosong") == 0) {
            ada = 1;
            printf("\nNo Kamar : %d\nTipe     : %s\nHarga    : Rp %d\nStatus   : %s\n", 
                   listKamar[i].no, listKamar[i].tipe, listKamar[i].harga, listKamar[i].status);
        }
    }

    if (!ada) printf("\nTidak ada kamar yang tersedia.\n");
}

void cariKamar() {
    bukaDataKamar();

    if (jmlKamar == 0) {
        printf("\nBelum ada data kamar!\n");
        return;
    }

    int pilihan, ketemu = 0;
    printf("\n========== CARI KAMAR ==========\n1. Nomor Kamar\n2. Tipe Kamar\nPilih : ");
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        int cariNo;
        printf("Masukkan Nomor Kamar : ");
        scanf("%d", &cariNo);

        for (int i = 0; i < jmlKamar; i++) {
            if (listKamar[i].no == cariNo) {
                ketemu = 1;
                printf("\nNo Kamar : %d\nTipe     : %s\nHarga    : Rp %d\nStatus   : %s\n", 
                       listKamar[i].no, listKamar[i].tipe, listKamar[i].harga, listKamar[i].status);
            }
        }
    } else if (pilihan == 2) {
        char cariTipe[50];
        printf("Masukkan Tipe Kamar : ");
        scanf(" %[^\n]", cariTipe);

        for (int i = 0; i < jmlKamar; i++) {
            if (strstr(listKamar[i].tipe, cariTipe) != NULL) {
                ketemu = 1;
                printf("\nNo Kamar : %d\nTipe     : %s\nHarga    : Rp %d\nStatus   : %s\n", 
                       listKamar[i].no, listKamar[i].tipe, listKamar[i].harga, listKamar[i].status);
            }
        }
    }

    if (!ketemu) printf("\nKamar tidak ditemukan!\n");
}
