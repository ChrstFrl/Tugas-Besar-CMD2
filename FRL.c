typedef struct datakos
{
    int no;
    char tipe[50];
    int harga;
    int status;        // 0 = Kosong, 1 = Terisi
    int pengajuan;     // 0 = Menunggu, 1 = Disetujui, 2 = Ditolak
} kamar;

add()
listkamar[jmlkamar].status = 0;
listkamar[jmlkamar].pengajuan = 0;

//ini fungsi buat fitur konfirmasi pengajuan



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

void tampilPengajuan() //konfirmasi pengajuan
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

void konfirmasiPengajuan() //konfirmasi pengajuan
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
