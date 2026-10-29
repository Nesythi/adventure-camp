
#include <stdio.h>
#include <string.h>

#define MAKS_TRANSAKSI 10
#define JUMLAH_PERALATAN 5
#define PANJANG_NAMA 100

/* Data peralatan camping */
char peralatan[JUMLAH_PERALATAN][40] = {
    "Tenda",
    "Sleeping Bag",
    "Kompor Portable",
    "Matras Camping",
    "Lampu Camping"
};

int harga[JUMLAH_PERALATAN] = {
    50000, 15000, 20000, 10000, 12000
};

/* Data riwayat transaksi */
char nama[MAKS_TRANSAKSI][PANJANG_NAMA];
int jumlah[MAKS_TRANSAKSI][JUMLAH_PERALATAN];
int lamaSewa[MAKS_TRANSAKSI];
long long subtotalData[MAKS_TRANSAKSI];
long long diskonData[MAKS_TRANSAKSI];
long long total[MAKS_TRANSAKSI];
int persenDiskon[MAKS_TRANSAKSI];

int transaksi = 0;

/* Membersihkan sisa input */
void bersihkanInput(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Memvalidasi input angka */
int inputAngka(const char pesan[], int minimum, int maksimum) {
    int nilai;
    int hasil;
    char sisa;

    while (1) {
        printf("%s", pesan);
        hasil = scanf("%d%c", &nilai, &sisa);

        if (hasil == 2 && sisa == '\n' &&
            nilai >= minimum && nilai <= maksimum) {
            return nilai;
        }

        bersihkanInput();
        printf("Input tidak valid. Masukkan angka %d-%d.\n",
               minimum, maksimum);
    }
}

/* Memasukkan nama penyewa */
void inputNama(char hasil[]) {
    size_t panjang;

    while (1) {
        printf("Nama penyewa: ");

        if (fgets(hasil, PANJANG_NAMA, stdin) == NULL) {
            hasil[0] = '\0';
            continue;
        }

        panjang = strlen(hasil);

        if (panjang > 0 && hasil[panjang - 1] == '\n') {
            hasil[panjang - 1] = '\0';
        } else {
            bersihkanInput();
        }

        if (strlen(hasil) > 0) {
            return;
        }

        printf("Nama tidak boleh kosong.\n");
    }
}

/* Menampilkan daftar peralatan */
void daftarPeralatan(void) {
    int i;

    printf("\n===============================================\n");
    printf("              DAFTAR PERALATAN\n");
    printf("===============================================\n");
    printf("%-4s %-30s %s\n", "No", "Peralatan", "Harga/Hari");
    printf("-----------------------------------------------\n");

    for (i = 0; i < JUMLAH_PERALATAN; i++) {
        printf("%-4d %-30s Rp%d\n",
               i + 1, peralatan[i], harga[i]);
    }

    printf("===============================================\n");
}

/* Menampilkan struk penyewaan */
void tampilkanStruk(int nomor) {
    int i;

    printf("\n===============================================\n");
    printf("                ADVENTURE CAMP\n");
    printf("                 STRUK PENYEWAAN\n");
    printf("===============================================\n");
    printf("Nama penyewa : %s\n", nama[nomor]);
    printf("Lama sewa    : %d hari\n", lamaSewa[nomor]);

    printf("-----------------------------------------------\n");
    printf("%-22s %5s %12s\n", "Peralatan", "Unit", "Subtotal");
    printf("-----------------------------------------------\n");

    for (i = 0; i < JUMLAH_PERALATAN; i++) {
        if (jumlah[nomor][i] > 0) {
            printf("%-22s %5d Rp%10lld\n",
                   peralatan[i],
                   jumlah[nomor][i],
                   (long long) harga[i] *
                   jumlah[nomor][i] * lamaSewa[nomor]);
        }
    }

    printf("-----------------------------------------------\n");
    printf("Subtotal     : Rp%lld\n", subtotalData[nomor]);
    printf("Diskon (%d%%) : Rp%lld\n",
           persenDiskon[nomor], diskonData[nomor]);
    printf("-----------------------------------------------\n");
    printf("TOTAL BAYAR  : Rp%lld\n", total[nomor]);
    printf("===============================================\n");
    printf("Terima kasih telah menyewa di Adventure Camp!\n");
}

/* Proses penyewaan peralatan */
void sewaPeralatan(void) {
    int i;
    int unit;
    int lama;
    int adaPesanan = 0;
    long long subtotal = 0;
    long long diskon;
    char pesan[100];

    if (transaksi >= MAKS_TRANSAKSI) {
        printf("\nKapasitas transaksi sudah penuh!\n");
        printf("Maksimal %d transaksi.\n", MAKS_TRANSAKSI);
        return;
    }

    printf("\n========== PENYEWAAN PERALATAN ==========\n");

    inputNama(nama[transaksi]);
    daftarPeralatan();

    printf("\nMasukkan jumlah unit setiap peralatan.\n");
    printf("Masukkan 0 jika tidak ingin menyewa.\n\n");

    for (i = 0; i < JUMLAH_PERALATAN; i++) {
        sprintf(pesan, "Jumlah %s: ", peralatan[i]);

        unit = inputAngka(pesan, 0, 1000000);
        jumlah[transaksi][i] = unit;

        if (unit > 0) {
            adaPesanan = 1;
        }
    }

    if (!adaPesanan) {
        printf("Penyewaan dibatalkan karena belum ada peralatan dipilih.\n");
        return;
    }

    lama = inputAngka("Lama penyewaan (hari): ", 1, 1000000);
    lamaSewa[transaksi] = lama;

    for (i = 0; i < JUMLAH_PERALATAN; i++) {
        subtotal += (long long) harga[i] *
                    jumlah[transaksi][i] * lama;
    }

    if (lama >= 5) {
        persenDiskon[transaksi] = 15;
    } else if (lama >= 3) {
        persenDiskon[transaksi] = 10;
    } else {
        persenDiskon[transaksi] = 0;
    }

    diskon = subtotal * persenDiskon[transaksi] / 100;

    subtotalData[transaksi] = subtotal;
    diskonData[transaksi] = diskon;
    total[transaksi] = subtotal - diskon;

    tampilkanStruk(transaksi);

    transaksi++;
}

/* Menampilkan riwayat transaksi dalam bentuk tabel */
void riwayatTransaksi(void) {
    int i;
    int j;
    long long pendapatan = 0;

    printf("\n===============================================\n");
    printf("                 RIWAYAT TRANSAKSI\n");
    printf("===============================================\n");
    printf("%-4s %-12s %-20s %s\n",
           "No", "Nama", "Peralatan", "Total");
    printf("-----------------------------------------------\n");

    if (transaksi == 0) {
        printf("Belum ada transaksi penyewaan.\n");
        printf("===============================================\n");
        return;
    }

    for (i = 0; i < transaksi; i++) {
        int pertama = 1;

        for (j = 0; j < JUMLAH_PERALATAN; j++) {
            if (jumlah[i][j] > 0) {
                if (pertama) {
                    printf("%-4d %-12s %-20s Rp%lld\n",
                           i + 1,
                           nama[i],
                           peralatan[j],
                           total[i]);

                    pertama = 0;
                } else {
                    printf("%-4s %-12s %-20s\n",
                           "", "", peralatan[j]);
                }
            }
        }

        pendapatan += total[i];
    }

    printf("-----------------------------------------------\n");
    printf("Jumlah Transaksi : %d\n", transaksi);
    printf("Total Pendapatan : Rp%lld\n", pendapatan);
    printf("===============================================\n");
}

/* Menu utama program */
int main(void) {
    int menu;

    do {
        printf("\n===============================================\n");
        printf("                 ADVENTURE CAMP\n");
        printf("          SISTEM PENYEWAAN PERALATAN\n");
        printf("===============================================\n");
        printf("1. Lihat Daftar Peralatan\n");
        printf("2. Sewa Peralatan\n");
        printf("3. Lihat Riwayat Transaksi\n");
        printf("4. Keluar\n");
        printf("===============================================\n");

        menu = inputAngka("Pilih menu [1-4]: ", 1, 4);

        switch (menu) {
            case 1:
                daftarPeralatan();
                break;

            case 2:
                sewaPeralatan();
                break;

            case 3:
                riwayatTransaksi();
                break;

            case 4:
                printf("\nTerima kasih telah menggunakan Adventure Camp!\n");
                printf("Sampai jumpa!\n");
                break;
        }

    } while (menu != 4);

    return 0;
}

