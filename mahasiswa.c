#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Struktur Data
struct Sks
{
    int eval1;
    int eval2;
    int eval3;
    int uas;
    float rataRata;
    int maks;
    int min;
    char grade[3];
};

struct Mahasiswa
{
    char nrp[11];
    char name[30];
    struct Sks nilai;
};

struct Mahasiswa mhs[100]; // Array untuk menyimpan data mahasiswa
int mhs_count = 0;

// Deklarasi Fungsi
void output1();
void input1(sqlite3 *db);
int createDatabase(sqlite3 **db);
void addData(sqlite3 *db);
void showAllData(sqlite3 *db);
void editData(sqlite3 *db);
void deleteData(sqlite3 *db);
int callback(void *NotUsed, int argc, char **argv, char **azColName);
void sortData(sqlite3 *db);

char *querySort = "";

int main()
{
    sqlite3 *db;
    int rc = createDatabase(&db);

    if (rc != 0)
    {
        fprintf(stderr, "Gagal membuat atau membuka database.\n");
        return 1;
    }

    // Menampilkan menu dan menerima input
    printf("\n\t\tSelamat Datang di Program Mahasiswa\n");
    printf("\tOleh : Muhammad Rafi Putra Hidayat NRP 3124600009\n");
    printf("================================================================\n");
    while (1)
    {
        output1();
        input1(db);
    }

    sqlite3_close(db);
    return 0;
}

void output1()
{

    printf("Silahkan pilih menu anda : \n");
    printf("1. Menampilkan semua data mahasiswa\n");
    printf("2. Menambah data mahasiswa\n");
    printf("3. Mengedit data mahasiswa\n");
    printf("4. Menghapus data mahasiswa\n");
    printf("5. Mengurutkan data mahasiswa\n");
    printf("6. Selesai\n");
    printf("================================================================\n");
}

void input1(sqlite3 *db)
{
    fflush(stdin);
    char n;

    printf("=> ");
    scanf("%c", &n);

    switch (n)
    {
    case '1':
        showAllData(db);
        break;
    case '2':
        addData(db);
        break;
    case '3':
        editData(db);
        break;
    case '4':
        deleteData(db);
        break;
    case '5':
        sortData(db);
        break;

    case '6':
        printf("Keluar program.\n");
        sqlite3_close(db);
        exit(0);
        break;

    default:
        printf("Input tidak valid.\n");
        sqlite3_close(db);
        exit(0);
        break;
    }
}

void softFun(sqlite3 *db, char *query)
{
    mhs_count = 0; // Reset jumlah mahasiswa setiap kali SELECT
    char *err_msg = 0;

    // Eksekusi query SELECT dengan callback
    int rc = sqlite3_exec(db, query, callback, db, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        if (mhs_count == 0)
        {
            printf("=================================================================================================================\n");
            printf("| No\t | NRP \t\t | Nama\t\t | Eval1 | Eval2 | Eval3 | UAS\t | Rata-Rata\t| Maks\t| Min\t| Grade\t|\n");
            printf("=================================================================================================================\n");
            printf("|\t\t\t\t\t\tDATA MAHASISWA KOSONG\t\t\t\t\t\t|\n");
            printf("=================================================================================================================\n\n\n");
        }
        else
        {
            printf("=================================================================================================================\n");
            printf("|No\t|NRP\t\t|Nama\t\t|Eval1\t|Eval2\t|Eval3\t|UAS\t|Rata-Rata\t|Maks\t|Min\t|Grade\t|\n");
            printf("=================================================================================================================\n");
            for (int i = 0; i < mhs_count; i++)
            {
                printf("|%d\t|%s\t|%s\t\t|%d\t|%d\t|%d\t|%d\t|%.f\t\t|%d\t|%d\t|%s\t|\n",
                       i + 1,
                       mhs[i].nrp,
                       mhs[i].name,
                       mhs[i].nilai.eval1,
                       mhs[i].nilai.eval2,
                       mhs[i].nilai.eval3,
                       mhs[i].nilai.uas,
                       mhs[i].nilai.rataRata,
                       mhs[i].nilai.maks,
                       mhs[i].nilai.min,
                       mhs[i].nilai.grade);
            }
            printf("=================================================================================================================\n\n\n");
        }
    }
}

// Fungsi mengurutkan data
void sortData(sqlite3 *db)
{
    fflush(stdin);
    char n;
    printf("Silahkan pilih menu mengurutkan data : \n");
    printf("1. Mengurutkan berdasarkan nilai rata-rata\n");
    printf("2. Mengurutkan berdasarkan nilai eval1\n");
    printf("3. Mengurutkan berdasarkan nilai eval2\n");
    printf("4. Mengurutkan berdasarkan nilai eval3\n");
    printf("5. Mengurutkan berdasarkan nilai uas\n");
    printf("6. Batal\n");
    printf("================================================================\n");

    printf("=> ");
    scanf("%c", &n);

    switch (n)
    {
    case '1':
        printf("Mengurutkan berdasarkan nilai rata-rata\n\n");
        querySort = "SELECT * FROM Mahasiswa ORDER BY RataRata DESC;";
        softFun(db, querySort);
        break;
    case '2':
        printf("Mengurutkan berdasarkan nilai eval1\n\n");
        querySort = "SELECT * FROM Mahasiswa ORDER BY Eval1 DESC;";
        softFun(db, querySort);
        break;
    case '3':
        printf("Mengurutkan berdasarkan nilai eval2\n\n");
        querySort = "SELECT * FROM Mahasiswa ORDER BY Eval2 DESC;";
        softFun(db, querySort);
        break;
    case '4':
        printf("Mengurutkan berdasarkan nilai eval3\n\n");
        querySort = "SELECT * FROM Mahasiswa ORDER BY Eval3 DESC;";
        softFun(db, querySort);
        break;
    case '5':
        printf("Mengurutkan berdasarkan nilai uas\n\n");
        querySort = "SELECT * FROM Mahasiswa ORDER BY UAS DESC;";
        softFun(db, querySort);
        break;
    case '6':
        main();
        break;

    default:
        printf("Input tidak valid.\n");
        sqlite3_close(db);
        exit(0);
        break;
    }
}

// Fungsi Membuat Database dan Tabel
int createDatabase(sqlite3 **db)
{
    char *err_msg = 0;
    int rc;

    // Membuka database
    rc = sqlite3_open("database.db", db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Tidak bisa membuka database: %s\n", sqlite3_errmsg(*db));
        return 1;
    }

    // Membuat tabel jika belum ada
    char *sql = "CREATE TABLE IF NOT EXISTS Mahasiswa("
                "NRP TEXT PRIMARY KEY, "
                "Name TEXT, "
                "Eval1 INT, "
                "Eval2 INT, "
                "Eval3 INT, "
                "UAS INT, "
                "RataRata REAL, "
                "Maks INT, "
                "Min INT, "
                "Grade TEXT);";

    rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        return 1;
    }

    // printf("Tabel berhasil dibuat atau sudah ada.\n");
    return 0;
}

// Fungsi Menambahkan Data ke Database
void addData(sqlite3 *db)
{
    struct Mahasiswa temp;
    printf("NRP: ");
    scanf("%s", temp.nrp);
    fflush(stdin);
    printf("Nama: ");
    scanf("%s", temp.name);
    fflush(stdin);
    printf("Nilai (Eval1, Eval2, Eval3, UAS): ");
    scanf("%d %d %d %d",
          &temp.nilai.eval1,
          &temp.nilai.eval2,
          &temp.nilai.eval3,
          &temp.nilai.uas);

    // Membuat query SQL untuk menambahkan data
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Mahasiswa (NRP, Name, Eval1, Eval2, Eval3, UAS) "
             "VALUES ('%s', '%s', %d, %d, %d, %d);",
             temp.nrp, temp.name,
             temp.nilai.eval1, temp.nilai.eval2, temp.nilai.eval3, temp.nilai.uas);

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        printf("Data berhasil ditambahkan!\n\n");
    }
}

// Callback Function untuk query SELECT
int callback(void *db_pointer, int argc, char **argv, char **azColName)
{
    sqlite3 *db = (sqlite3 *)db_pointer;
    char sql[512];
    char *err_msg = 0;

    // Salin data dari hasil query ke array `mhs`
    strcpy(mhs[mhs_count].nrp, argv[0] ? argv[0] : "NULL");
    strcpy(mhs[mhs_count].name, argv[1] ? argv[1] : "NULL");
    mhs[mhs_count].nilai.eval1 = argv[2] ? atoi(argv[2]) : 0;
    mhs[mhs_count].nilai.eval2 = argv[3] ? atoi(argv[3]) : 0;
    mhs[mhs_count].nilai.eval3 = argv[4] ? atoi(argv[4]) : 0;
    mhs[mhs_count].nilai.uas = argv[5] ? atoi(argv[5]) : 0;

    // Hitung rata-rata, nilai maksimum, dan minimum
    int evals[4] = {mhs[mhs_count].nilai.eval1, mhs[mhs_count].nilai.eval2, mhs[mhs_count].nilai.eval3, mhs[mhs_count].nilai.uas};
    int sum = 0, max = evals[0], min = evals[0];
    for (int i = 0; i < 4; i++)
    {
        sum += evals[i];
        if (evals[i] > max)
            max = evals[i];
        if (evals[i] < min)
            min = evals[i];
    }
    mhs[mhs_count].nilai.rataRata = ceil(sum / 4.0);
    mhs[mhs_count].nilai.maks = max;
    mhs[mhs_count].nilai.min = min;

    // Assign grade
    if (mhs[mhs_count].nilai.rataRata >= 86)
        strcpy(mhs[mhs_count].nilai.grade, "A");
    else if (mhs[mhs_count].nilai.rataRata >= 81)
        strcpy(mhs[mhs_count].nilai.grade, "A-");
    else if (mhs[mhs_count].nilai.rataRata >= 76)
        strcpy(mhs[mhs_count].nilai.grade, "AB");
    else if (mhs[mhs_count].nilai.rataRata >= 71)
        strcpy(mhs[mhs_count].nilai.grade, "B+");
    else if (mhs[mhs_count].nilai.rataRata >= 66)
        strcpy(mhs[mhs_count].nilai.grade, "B");
    else if (mhs[mhs_count].nilai.rataRata >= 61)
        strcpy(mhs[mhs_count].nilai.grade, "BC");
    else if (mhs[mhs_count].nilai.rataRata >= 56)
        strcpy(mhs[mhs_count].nilai.grade, "C");
    else if (mhs[mhs_count].nilai.rataRata >= 41)
        strcpy(mhs[mhs_count].nilai.grade, "D");
    else
        strcpy(mhs[mhs_count].nilai.grade, "E");

    // Update data mahasiswa di tabel
    snprintf(sql, sizeof(sql),
             "UPDATE Mahasiswa SET RataRata = %.2f, Maks = %d, Min = %d, Grade = '%s' WHERE NRP = '%s';",
             mhs[mhs_count].nilai.rataRata, mhs[mhs_count].nilai.maks,
             mhs[mhs_count].nilai.min, mhs[mhs_count].nilai.grade,
             mhs[mhs_count].nrp);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    mhs_count++; // Tambahkan jumlah mahasiswa
    return 0;
}

// Fungsi Menampilkan Semua Data
void showAllData(sqlite3 *db)
{
    mhs_count = 0; // Reset jumlah mahasiswa setiap kali SELECT
    char *err_msg = 0;
    char *sql = "SELECT * FROM Mahasiswa ORDER BY NRP ASC";

    // Eksekusi query SELECT dengan callback
    int rc = sqlite3_exec(db, sql, callback, db, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        if (mhs_count == 0)
        {
            printf("=================================================================================================================\n");
            printf("| No\t | NRP \t\t | Nama\t\t | Eval1 | Eval2 | Eval3 | UAS\t | Rata-Rata\t| Maks\t| Min\t| Grade\t|\n");
            printf("=================================================================================================================\n");
            printf("|\t\t\t\t\t\tDATA MAHASISWA KOSONG\t\t\t\t\t\t|\n");
            printf("=================================================================================================================\n\n\n");
        }
        else
        {
            printf("=================================================================================================================\n");
            printf("|No\t|NRP\t\t|Nama\t\t|Eval1\t|Eval2\t|Eval3\t|UAS\t|Rata-Rata\t|Maks\t|Min\t|Grade\t|\n");
            printf("=================================================================================================================\n");
            for (int i = 0; i < mhs_count; i++)
            {
                printf("|%d\t|%s\t|%s\t\t|%d\t|%d\t|%d\t|%d\t|%.f\t\t|%d\t|%d\t|%s\t|\n",
                       i + 1,
                       mhs[i].nrp,
                       mhs[i].name,
                       mhs[i].nilai.eval1,
                       mhs[i].nilai.eval2,
                       mhs[i].nilai.eval3,
                       mhs[i].nilai.uas,
                       mhs[i].nilai.rataRata,
                       mhs[i].nilai.maks,
                       mhs[i].nilai.min,
                       mhs[i].nilai.grade);
            }
            printf("=================================================================================================================\n\n\n");
        }
    }
}

// Fungsi Mengedit Data Berdasarkan NRP
void editData(sqlite3 *db)
{
    struct Mahasiswa temp;

    printf("Masukkan NRP mahasiswa yang ingin diedit: ");
    scanf("%s", temp.nrp);

    // Cek apakah mahasiswa dengan NRP tersebut ada dalam database
    char sql[512];
    snprintf(sql, sizeof(sql), "SELECT * FROM Mahasiswa WHERE NRP = '%s';", temp.nrp);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        printf("Error preparing query: %s\n", sqlite3_errmsg(db));
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        // Data ditemukan, tampilkan data mahasiswa
        printf("Data mahasiswa ditemukan: \n");
        printf("NRP: %s\n", sqlite3_column_text(stmt, 0));
        printf("Nama: %s\n", sqlite3_column_text(stmt, 1));
        printf("Eval1: %d\n", sqlite3_column_int(stmt, 2));
        printf("Eval2: %d\n", sqlite3_column_int(stmt, 3));
        printf("Eval3: %d\n", sqlite3_column_int(stmt, 4));
        printf("UAS: %d\n", sqlite3_column_int(stmt, 5));

        // Terima input baru untuk mengedit data
        fflush(stdin);
        printf("Masukkan Nama Baru: ");
        scanf("%s", temp.name); // supaya hanya terbanya kata pertama
        // fgets(temp.name, sizeof(temp.name), stdin);
        fflush(stdin);
        printf("Masukkan Nilai (Eval1, Eval2, Eval3, UAS): ");
        scanf("%d %d %d %d",
              &temp.nilai.eval1,
              &temp.nilai.eval2,
              &temp.nilai.eval3,
              &temp.nilai.uas);

        // Query untuk memperbarui data mahasiswa
        snprintf(sql, sizeof(sql),
                 "UPDATE Mahasiswa SET Name = '%s', Eval1 = %d, Eval2 = %d, Eval3 = %d, UAS = %d WHERE NRP = '%s';",
                 temp.name, temp.nilai.eval1, temp.nilai.eval2, temp.nilai.eval3, temp.nilai.uas, temp.nrp);

        // Eksekusi query UPDATE
        rc = sqlite3_exec(db, sql, 0, 0, 0);

        if (rc != SQLITE_OK)
        {
            printf("SQL error: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            printf("Data berhasil diperbarui.\n\n");
        }
    }
    else
    {
        printf("Data mahasiswa dengan NRP %s tidak ditemukan.\n\n\n", temp.nrp);
    }

    // Menutup statement
    sqlite3_finalize(stmt);
}

// Fungsi Menghapus Data Berdasarkan NRP
void deleteData(sqlite3 *db)
{
    struct Mahasiswa temp;
    char n;

    // Meminta NRP mahasiswa yang ingin dihapus
    printf("Masukkan NRP mahasiswa yang ingin dihapus: ");
    scanf("%s", temp.nrp);

    // Cek apakah mahasiswa dengan NRP tersebut ada dalam database
    char sql[512];
    snprintf(sql, sizeof(sql), "SELECT * FROM Mahasiswa WHERE NRP = '%s';", temp.nrp);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        printf("Error preparing query: %s\n", sqlite3_errmsg(db));
        return;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        // Menampilkan data mahasiswa yang ditemukan
        printf("Data yang ditemukan:\n");
        printf("NRP: %s\n", sqlite3_column_text(stmt, 0));
        printf("Nama: %s\n", sqlite3_column_text(stmt, 1));
        printf("Eval1: %d\n", sqlite3_column_int(stmt, 2));
        printf("Eval2: %d\n", sqlite3_column_int(stmt, 3));
        printf("Eval3: %d\n", sqlite3_column_int(stmt, 4));
        printf("UAS: %d\n", sqlite3_column_int(stmt, 5));

        // Meminta konfirmasi untuk menghapus
        printf("Apakah Anda yakin ingin menghapus data ini? (y/n): ");
        getchar(); // Membersihkan newline dari buffer input sebelumnya
        scanf("%c", &n);

        if (n == 'y' || n == 'Y')
        {
            // Query untuk menghapus data mahasiswa
            snprintf(sql, sizeof(sql), "DELETE FROM Mahasiswa WHERE NRP = '%s';", temp.nrp);

            // Eksekusi query DELETE
            rc = sqlite3_exec(db, sql, 0, 0, 0);

            if (rc != SQLITE_OK)
            {
                printf("SQL error: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                printf("Data berhasil dihapus.\n\n");
            }
        }
        else
        {
            printf("Penghapusan dibatalkan.\n\n");
        }
    }
    else
    {
        printf("Data mahasiswa dengan NRP %s tidak ditemukan.\n\n", temp.nrp);
    }

    // Menutup statement
    sqlite3_finalize(stmt);
}