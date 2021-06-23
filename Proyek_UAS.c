// Kelompok 5
//Anggota: 
//1. Ghulam Izzul Fuad - 2006534953
//2. M. Farhan Haniftyaji - 2006468711
//3. Valentinus - 2006468472


#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

//define ASCII
#define ENTER 13
#define BKSP 8

//define struct untuk menyimpan detail belanja pembeli
struct detail_belanja{
    char judul[100];
    int kuantiti;
    float harga;
    struct detail_belanja *next;
};

typedef struct detail_belanja belanjaUser;
typedef belanjaUser *belanja;

//define struct untuk menyimpan username pembeli setelah memilih buku yang ingin dibeli
struct detail_user {
    char username [15];
    belanja linked_belanja;
    float sum_belanja;
    struct detail_user *next;
};

typedef struct detail_user info_user;
typedef info_user *user;

//define struct untuk menyimpan informasi buku
struct detail_buku {
    char nama_buku[100];
    char author[100];
    int halaman;
    float harga_buku;
    char rak[10];
    struct detail_buku *next;
};

typedef struct detail_buku detail;
typedef detail *buku;

char pwd[100];
//define variable password untuk menyimpan password admin setelah di load dari file
char password[16];

//prototype modular function
void daftar_pembelian(user awal);
void list_pembeli(user awal);
void savepass();
void save(buku head);
void pass_baru();
void lihat_pembeli(user awal);
void beli_buku(buku head, user *awal);
void hapus_buku (buku *head);
void cari_buku (buku head);
void list_buku(buku head);
int openf(int *counter);
void loaddata(buku *head, int counter);
void selamat_datang();
void welcome_screen(buku head, user awal);
void loadpass();
int login_admin();
void menu(int *user_atau_admin, buku head, user awal);
void input_user(buku *head);
void tambah_buku(buku *head, char rak[10], char judul[100], char author[100],int *halaman, float *harga);

//main function
int main() {
    //define kepala linked list buku
    buku head = NULL;
    
    //define kepala linked list username pembeli
    user awal = NULL;

    FILE *fptr;
    FILE *passptr;
    int counter = 0;

    //melakukan cek apakah file data buku ada di komputer dan menghitung jumlah baris yang ada didalam file
    if(openf(&counter) == 1)
    {
        //load data buku dari file dan disimpan didalam linked list buku
        loaddata(&head, counter);
    }

    //load password admin dari file
    loadpass(passptr);
    
    //menu utama program
    welcome_screen(head, awal);

    return 0;
}

void welcome_screen(buku head, user awal) {
    int input1, countersalah = 0;
    FILE *fptr;
    FILE *passptr;

    do {
        printf("************************************");
        printf("\n**                                **");
        printf("\n**          WELCOME TO            **");
        printf("\n**          BOOKSTORE             **");
        printf("\n**          MANAGEMENT            **");
        printf("\n**          SYSTEM                **");
        printf("\n**                                **");
        printf("\n************************************\n\n");
        printf("1. Admin\n2. User\n3. Keluar\nLogin Sebagai: ");
        scanf("%d", &input1);

        switch (input1) {
            case 1:
                system("CLS");
                inputpass : 
                if(login_admin() == 1) {
                    system("CLS");
                    printf("**************************************");
                    printf("\n**                                  **");
                    printf("\n**                                  **");
                    printf("\n**          SELAMAT DATANG          **");
                    printf("\n**         SELAMAT DATANG           **");
                    printf("\n**        SELAMAT DATANG            **");
                    printf("\n**                                  **");
                    printf("\n**                                  **");
                    printf("\n**************************************\n\n");
                    system("pause");

                    menu(&input1, head, awal);
                }
                else {
                    printf("\nPassword salah\n");
                    countersalah++;
                    if(countersalah == 3)
                    {
                        printf("\nTerlalu banyak password salah!\n");
                        exit(0);
                    }
                    goto inputpass;
                }
                
                break;
            case 2: 
                system("CLS");
                menu(&input1, head, awal);
                break;
            case 3:
                system("CLS");
                printf("**************************************");
                printf("\n**                                  **");
                printf("\n**                                  **");
                printf("\n**          TERIMA KASIH            **");
                printf("\n**         TERIMA KASIH             **");
                printf("\n**        TERIMA KASIH              **");
                printf("\n**                                  **");
                printf("\n**                                  **");
                printf("\n**************************************\n\n");
                system("pause");
                system("CLS");
                save(head);
                savepass();
                exit(0);
                break;
            default:
                system("CLS");
                printf("invalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\n");
                system("pause");
                fflush(stdin);
                break;
        }
    }while (input1 != 3);
}

 //function ini mengecek apakah password yang dimasukkan benar khusus untuk admin
int login_admin() {
   char pw[16], ch;
   int p = 0;
  
   printf("Enter your password. Hit ENTER to confirm.\n");
   /* 13 adalah ASCII dari Enter key */
   while((ch = getch()) != ENTER){
       if(p < 0)
           p = 0;
       /* 8 adalah ASCII dari BACKSPACE character */
       if(ch == BKSP){
           printf("\b \b"); 
           p--;
       continue;
       }
       pw[p++] = ch;
       putch('*'); // password akan terlihat seperti "*******" saat diinput user
   }
   pw[p] = '\0';  

    if (strcmp(password,pw) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void menu(int *user_atau_admin, buku head, user awal) {
    int input1, input2, lanjut, cari_atau_list;
    FILE *fptr;
    FILE *passptr;

    //sebagai ADMIN
    if (*user_atau_admin == 1) {
        do {
            system("CLS");
            fflush(stdin);
            
            printf("\t\tsistem manajemen buku\n");
            printf(" _____________________________________________________\n");
            printf("|             ******   ADMIN   ******                 |\n");
            printf("|                                                     |\n");
            printf("|                 1. pembeli                          |\n");
            printf("|                 2. tambah buku                      |\n");
            printf("|                 3. cari buku                        |\n");
            printf("|                 4. list buku                        |\n");
            printf("|                 5. hapus buku                       |\n");
            printf("|                 6. perbarui password                |\n");
            printf("|                 7. kembali ke menu awal             |\n");
            printf("|                 8. keluar                           |\n");
            printf("|_____________________________________________________|\n\n");

            printf("masukkan pilihan :  ");
            scanf("%d", &input1);
            fflush(stdin);
            switch (input1) {
                case 1: 
                    system("CLS");
                    lihat_pembeli(awal);
                    system("pause");
                    break;
                case 2:
                    system("CLS");
                    input_user(&head);
                    save(head);
                    break;
                
                case 3:
                    system("CLS");
                    cari_buku(head);
                    system("pause");
                    break;
            
                case 4:
                    system("CLS");
                    list_buku(head);
                    system("pause");
                    break;
            
                case 5:
                    system("CLS");
                    hapus_buku(&head);
                    break;
                
                case 6:
                    system("CLS");
                    pass_baru();
                    break;
                
                case 7: 
                    system("CLS");
                    welcome_screen(head,awal);
                    break;
                    
                case 8:
                    system("CLS");
                    printf("**************************************");
                    printf("\n**                                  **");
                    printf("\n**                                  **");
                    printf("\n**          TERIMA KASIH            **");
                    printf("\n**         TERIMA KASIH             **");
                    printf("\n**        TERIMA KASIH              **");
                    printf("\n**                                  **");
                    printf("\n**                                  **");
                    printf("\n**************************************\n\n");
                    system("pause");
                    system("cls");
                    save(head);
                    savepass();
                    exit(0);
                    break;

                default:
                    system("CLS");
                    printf("invalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\n");
                    system("pause");
                    fflush(stdin);
                    system("CLS");
                }
            } while (input1 != 7 || input1 != 8);
    }

    //sebagai USER
    if (*user_atau_admin == 2) {
        do {
            system("CLS");
            fflush(stdin);
            
            printf("\t\tsistem manajemen buku\n");
            printf(" _____________________________________________________\n");
            printf("|             ******   USER   ******                  |\n");
            printf("|                                                     |\n");
            printf("|             1. cari buku                            |\n");
            printf("|             2. list buku                            |\n");
            printf("|             3. pembelian buku                       |\n");
            printf("|             4. total belanja                        |\n");
            printf("|             5. kembali ke menu sebelumnya           |\n");
            printf("|             6. keluar                               |\n");
            printf("|_____________________________________________________|\n\n");

            printf("masukkan pilihan :  ");
            scanf("%d", &input2);
            fflush(stdin);
            switch (input2) {
                case 1:
                    system("CLS");
                    cari_buku(head);
                    system("pause");
                    break;
                case 2:
                    system("CLS");
                    list_buku(head);
                    system("pause");
                    break;
                case 3: 
                    system("CLS");
                    beli_buku(head, &awal);
                    system("pause");
                    break;
                case 4: 
                    system("CLS");
                    daftar_pembelian(awal);
                    system("pause");
                    break;
                case 5: 
                    system("CLS");
                    welcome_screen(head,awal);
                    break;
                case 6:
                    system("CLS");
                    printf("**************************************");
                    printf("\n**                                  **");
                    printf("\n**                                  **");
                    printf("\n**          TERIMA KASIH            **");
                    printf("\n**         TERIMA KASIH             **");
                    printf("\n**        TERIMA KASIH              **");
                    printf("\n**                                  **");
                    printf("\n**                                  **");
                    printf("\n**************************************\n\n");
                    system("pause");
                    system("cls");
                    save(head);
                    savepass();
                    exit(0);

                default:
                    system("CLS");
                    printf("invalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\n");
                    fflush(stdin);
                    system("pause");
                    system("CLS");
                }
            } while (input2 != 5 || input2 != 6);
    }
}

//function untuk meminta input informasi buku yang akan ditambahkan
void input_user(buku *head) {
    char judul[100],rak[10],author[100];
    int halaman;
    float harga;

    system("CLS");
    
    printf("Input Informasi Buku\n");
    printf("Judul Buku :");
    fflush(stdin);
    scanf("%[^\n]s", judul);
    printf("Nama Penulis : ");
    fflush(stdin);
    scanf("%[^\n]s", author);
    printf("Jumlah Halaman : ");
    fflush(stdin);
    scanf("%d", &halaman);
    printf("Letak Rak Buku : ");
    fflush(stdin);
    scanf("%[^\n]s",rak);
    printf("Harga Buku : ");
    fflush(stdin);
    scanf("%f", &harga);
    
    //membuat node baru di linked list
    tambah_buku(head, rak, judul, author, &halaman, &harga);
}

//funtion untuk membuat node baru di linked list jika ada buku baru yang ditambahkan
void tambah_buku(buku *head, char rak[5], char judul[100], char author[100],int *halaman, float *harga) {

    buku temp = malloc (sizeof(detail));
    buku iterator = NULL;

    strcpy(temp->rak, rak);
    strcpy(temp->nama_buku, judul);
    strcpy(temp->author, author);
    temp->halaman = *halaman;
    temp->harga_buku = *harga;
    temp->next = NULL;

    //cek apakah kepala linked list masih kosong
    if(*head == NULL) 
	{
        *head = temp;
    }
    //membuat node baru di ujung linked list jika kepala linked list sudah ada
    else 
	{
		iterator = *head;
		while(1)
		{
			if(iterator->next == NULL)
			{
				iterator->next = temp;
				break;
			}
			iterator = iterator->next;
    	}
	}
}

void cari_buku(buku head) {

    buku temp = head;
    int found = 0, i, inputcari, panjang_buku, panjang_input;
    char caribuku[100];
    char temp_judul[100];
    char temp_author[100];
    system("CLS");
    
    if(temp == NULL)
    {
        printf("Daftar Buku Masih Kosong\n");
    }
    else
    { 
        printf("Cari Berdasarkan:\n1. Judul Buku\n2. Author / Penulis\nInput anda: ");
        fflush(stdin);
        scanf("%d", &inputcari);
        
        switch (inputcari) 
        {
            case 1 :
                system("CLS"); 

                printf("masukkan judul buku: ");
                fflush(stdin);
                scanf("%[^\n]s", caribuku);
                
                panjang_input = strlen(caribuku);
                for(i=0; i<panjang_input; i++)
                {
                    caribuku[i] = tolower(caribuku[i]);
                }
                
                printf("\n=====================================\n\n");

                while(temp!= NULL)
                {
                    panjang_buku = strlen(temp->nama_buku);
                    for(i=0; i<panjang_buku; i++) 
                    {
                        temp_judul[i] = tolower(temp->nama_buku[i]);
                    }
                    
                    if(strstr(temp_judul, caribuku))
                    {
                        printf("Judul             : %s\n", temp->nama_buku);
                        printf("Author            : %s\n", temp->author);
                        printf("Jumlah halaman    : %d\n",temp->halaman);
                        printf("Letak Buku di Rak : %s\n", temp->rak);
                        printf("Harga             : %.2f\n\n",temp->harga_buku);
                        found = 1;
                    }
                    temp = temp->next;
                }
                
                if (found == 0)
                {
                    printf("Maaf, buku yang anda cari tidak ada\n");
                }
                break;
            case 2 :
                system("CLS"); 
                
                printf("Masukkan nama author: ");
                fflush(stdin);
                scanf("%[^\n]s", caribuku);

                panjang_input = strlen(caribuku);
                for(i=0; i<panjang_input; i++)
                {
                    caribuku[i] = tolower(caribuku[i]);
                }
                printf("\n=====================================\n\n");

                while(temp!= NULL)
                {
                    panjang_buku = strlen(temp->author);
                    for(i=0; i<panjang_buku; i++) 
                    {
                        temp_author[i] = tolower(temp->author[i]);
                    }
                    if(strstr(temp_author, caribuku))
                    {
                        printf("Author            : %s\n", temp->author);
                        printf("Judul             : %s\n", temp->nama_buku);
                        printf("Jumlah halaman    : %d\n",temp->halaman);
                        printf("Letak Buku di Rak : %s\n", temp->rak);
                        printf("Harga             : %.2f\n\n",temp->harga_buku);
                        found = 1;
                    }
                    temp = temp->next;
                }
                
                if (found == 0)
                {
                    printf("Maaf, buku yang anda cari tidak ada\n");
                }
                break;
            default :
                system("CLS");
                printf("invalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\ninvalid input!\n");
                fflush(stdin);
                system("pause");
                system("CLS");
        }
    }
}

void list_buku(buku head)
{
    system("CLS");
    int sum=0, i = 1;
    buku current;
    current = head;
    #pragma omp parallel
    {
    	current = head;
    	#pragma omp master
    	{
    		while(current)
    		{
    			#pragma omp task firstprivate(current)
    			{
    				#pragma omp critical
    				sum++;
				}
				current = current->next;
			}
		}
	}
	
    buku temp = NULL;
    if(head == NULL)
    {
        printf("Belum ada data buku\n");
    }
    else
    {
        temp = head;
        printf("|==========================================================================================================================|\n");
        printf("|  NO  |%20sJudul Buku%20s|%12sAuthor%12s|Halaman| RAK |%7sHarga%7s|\n", " ", " ", " ", " "," "," ");
        printf("|==========================================================================================================================|\n");
        while(temp != NULL)
        {
            printf("|%6d|%50s|%30s|%7d|%5s|%18.2f |\n",i, temp->nama_buku, temp->author, temp->halaman, temp->rak, temp->harga_buku);
            printf("|==========================================================================================================================|\n");
            i++;
            temp = temp->next;
        }
    }
    printf("Jumlah Buku: %d\n\n", sum);
}

void beli_buku(buku head,user *awal)
{
    system("CLS");
    buku temp;
    buku temp1;
    user iterator;
    user iterator2;
    belanja pertama;
    belanja kedua;
    belanja first = NULL;
    int cari = 1, i = 0, pick;
    float harga[20];
    char keyword[100], temp_judul[100], judul[100];
    char atasnama[15];
    char judul_buku[20][100];
    int kuanti[20], jumlah, flag = 0, panjang_buku, panjang_input, j, found = 0;
    
    
    //kalau cari == 1 artinya user ingin membeli buku lain. jika tidak maka cari == 2
    while(cari == 1)
    {
        // "cari: " untuk looping kalau buku yang dicari tidak ada di database
        cari :
        temp = head;
        printf("Masukkan kata kunci judul buku yang ingin anda beli :\n");
        fflush(stdin);
        scanf("%[^\n]s", keyword);
        system("CLS"); 
        
        //membuat input user menjadi tidak kapital (supaya pada saat di input tidak case sensitive)
        panjang_input = strlen(keyword);
        for(j=0; j<panjang_input; j++)
        {
            keyword[j] = tolower(keyword[j]);
        }
        
        printf("\n=====================================\n\n");

        while(temp!= NULL)
        {
            panjang_buku = strlen(temp->nama_buku);
            for(j=0; j<panjang_buku; j++) 
            {
                temp_judul[j] = tolower(temp->nama_buku[j]);
            }
            
            //penggunaan function strstr agar buku" yang dicari tidak harus persis saat di input
            if(strstr(temp_judul, keyword))
            {
                printf("Judul             : %s\n", temp->nama_buku);
                printf("Author            : %s\n", temp->author);
                printf("Jumlah halaman    : %d\n",temp->halaman);
                printf("Letak Buku di Rak : %s\n", temp->rak);
                printf("Harga             : %.2f\n\n",temp->harga_buku);
                found = 1;
                //found akan = 1 jika buku ada di database, jika tidak maka found = 0 dan masuk ke if dibawah
            }
            temp = temp->next;
        }
        
        if (found == 0)
        {
            printf("Maaf, buku yang anda cari tidak ada\n");
            goto cari;
        }

        //kalau judul tidak ada maka printf "input judul salah" dan looping ke "beli: "
        beli :
        printf("Masukkan judul buku yang ingin anda beli dengan lengkap! (misal: Filosofi Teras)\nBuku: ");
        fflush(stdin);
        scanf("%[^\n]s", judul);
        
        temp1 = head;
        while(temp1 != NULL)
        {
            //penggunaan strcmpi supaya case insensitive
            if(strcmpi(temp1->nama_buku, judul) == 0)
            {
                printf("Berapa banyak jumlah buku yang ingin anda beli : ");
                scanf("%d", &jumlah);
                system("CLS");
                strcpy(judul_buku[i],temp1->nama_buku);
                harga[i] = temp1->harga_buku;
                kuanti[i] = jumlah;
                i++;
                flag = 1;
            }
            temp1 = temp1->next; 
        }
        if(flag == 0)
        {
            printf("\nInput Judul Salah\n");
            goto beli;
        }
        
        fflush(stdin);
        pilih :
        printf("Apakah anda ingin membeli buku lain?\n1. Ya\n2.Tidak\n");
        printf("Input anda (1/2): ");
        scanf("%d", &pick);

        system("CLS");
        
        if(pick == 1)
        {
            cari = 1;
        }
        else if(pick == 2)
        {
            //input username, data akan dikirim ke admin dan nanti username nya akan terlihat di komputer admin untuk pembayaran
            printf("Pembelian atas nama: ");
            fflush(stdin);
            scanf("%[^\n]s", atasnama);

            cari = 2;
        }
        else
        {
            printf("\nInput salah\n");
            fflush(stdin);
            goto pilih;
        }
    }

    system("CLS");

    float sum = 0;
    int k = 0;

    //paralel programming untuk menghitung total harga pembelian dan list pembelian buku dari user
    #pragma omp parallel
    {
   		#pragma omp for
  	 	for(k = 0; k<i; k++)
  	 	{ 
  	 		printf("- Judul buku: %s\n   Jumlah: %dpcs * Rp%.0f = Rp %.0f\n", judul_buku[k], kuanti[k], harga[k], kuanti[k]*harga[k]);

            //critical supaya tidak race condition
  	 		#pragma omp critical
  	 		sum+= (harga[k])*(kuanti[k]);
		}
    }
    printf("\nTotal = Rp %.0f\n", sum);
    printf("Pesanan Atas Nama : %s\n", atasnama);

    //looping untuk menyimpan data pembelian agar dapat ditampilkan di komputer admin
	for(k=0; k<i; k++)
    {
        pertama = malloc(sizeof(struct detail_belanja));
        strcpy(pertama->judul,judul_buku[k]);
        pertama->kuantiti = kuanti[k];
        pertama->harga = harga[k];
        pertama->next = NULL;
        if(first == NULL)
        {
        	first = pertama;
		}
		else
		{
			kedua = first;
			while(kedua != NULL)
			{
				if(kedua->next == NULL)
				{
					kedua->next = pertama;
                    break;
				}
				kedua = kedua->next;
			}
		}
    }
    
    iterator = malloc(sizeof(struct detail_user));
    strcpy(iterator->username, atasnama);
    iterator->sum_belanja = sum;
    iterator->next = NULL;
    iterator->linked_belanja = first;
    

    if(*awal == NULL)
    {
        *awal = iterator;
    }
    else
    {
        iterator2 = *awal;
        while(1)
        {
            if(iterator2->next == NULL)
            {
                iterator2->next = iterator;
                break;
            }
            iterator2 = iterator2->next;
        }
    }  
    
    printf("\nBooking anda sudah sukses, silahkan pergi ke kasir dengan menyebutkan ''%s'' untuk membayar\n\n", atasnama); 
}

//function untuk menghapus buku dari linked list
void hapus_buku (buku *head) {
    buku current = *head;
    buku temp1;
    buku temp2;
    char nama_buku[100];
    int hapus = 0;
    printf("Masukkan judul buku yang ingin dihapus : ");
    scanf(" %[^\n]s", nama_buku);
    while (current != NULL) {
        if(strcmpi(nama_buku,current->nama_buku) == 0) {
            printf("\nbuku dengan judul %s telah dihapus!\n", current->nama_buku);
            hapus = 1;
            if (current == *head) 
            {
                temp2 = current->next;
                *head = temp2;
            }
            else {
                temp2 = current->next;
                temp1->next = temp2;
            }
            save(*head);
            system("pause");
        }
        temp1 = current;
        current = current->next;
    }
    if(hapus == 0)
    {
        printf("\nBuku %s tidak ada\n", nama_buku);
        system("pause");
    }
    save(*head);
}

//function untuk mengganti password
void pass_baru() {
    char pwdlama[16], pwdbaru[16];
    int counter = 0;
    printf("masukkan password lama : ");
    scanf("%s", &pwdlama);
    
    
    if (strcmp(pwdlama, password) == 0) {
        ulangpassword :
        printf("masukkan password baru : ");
        scanf("%s", &pwdbaru);
        strcpy(password, pwdbaru);
        printf("password baru telah disimpan\n");
        savepass();
        system("pause");
    }
    else {
        printf("password yang dimasukkan salah!\n\n");
        system("pause");
        counter++;
    }
    
    if (counter == 3)
    {
        printf("\nTerlalu Banyak percobaan password salah\n");
        exit(0);
    }   
 }
//membuka file / membuat file (jika belum ada) dan menghitung jumlah baris atau line dalam file
 int openf(int *counter)
{
 	FILE *fptr;
 	
	char line;
	fptr = fopen("daftar_buku.txt","r");

    //jika belum ada buku, return 0
	if(fptr == NULL)
	{
        fclose(fptr);
        fopen("daftar_buku.txt","w");
        fclose(fptr);
        return 0;
	}

    //jika sudah ada buku, return 1
	else
	{
        while(!feof(fptr))
        {
            line = fgetc(fptr);
            if(line == '\n')
            {
                (*counter)++;
            }
        }
        fclose(fptr);
        return 1;
	}
 }

//load password dari file .txt
void loadpass()
{
	FILE *passptr;
    passptr = fopen("password.txt","r");
    fscanf(passptr," %[^\t]\t",password);
    fclose(passptr);
}

 //untuk membaca data dari file yang tersimpan di komputer dan dimasukkan linked list buku
void loaddata(buku *head, int counter)
{
    FILE *fptr;
    
	int i = 0;
	detail *iterator = NULL;
	fptr = fopen("daftar_buku.txt","r");

    //membuat node sebanyak counter (banyak buku yang sudah ada di file daftar buku)
	for(i = 0; i<counter; i++)
	{
		buku temp = malloc (sizeof(detail));

        //mengambil data buku dari file .txt (fscanf)
		fscanf(fptr," %[^\t]\t %[^\t]\t %[^\t]\t%f\t%d\n", temp->nama_buku, temp->author, temp->rak, &temp->harga_buku, &temp->halaman);
		temp->next = NULL;

		if(*head == NULL)
		{
			*head = temp;
		}
		else 
		{
			iterator = *head;
			while(1)
			{
                //kalau sudah bertemu akhir dari node linked list
				if(iterator->next == NULL)
				{
					iterator->next = temp;
					break;
				}
				else
				{
					iterator = iterator->next;
				}
				
	    	}
		}
	}
	fclose(fptr);
}

//function untuk admin memilih mencari pembeli berdasarkan username atau menampilkan semua pembeli
void lihat_pembeli(user awal)
{
    int pick = 0;
    printf("Lihat daftar pembelian berdasarkan : \n");
    printf("1. Username\n2. Tampilkan semua\n\nPilihan anda : ");
    scanf("%d", &pick);
    switch(pick)
    {
        case 1 :
            system("CLS");
            daftar_pembelian(awal);
            break;
        case 2 :
            system("CLS");
            list_pembeli(awal);
            break;
        default :
            printf("Input salah\n");
            fflush(stdin);
            break;
    }
}

//function untuk mencari pembeli berdasarkan username yang diinput
void daftar_pembelian(user awal)
{
    user temp = awal;
    belanja btemp;
    char userword[15];
    int flag = 0, i = 1;
    
    if(temp == NULL)
    {
        printf("Belum ada pembeli\n");
    }
    else
    {
        printf("Menu Keranjang Belanja\n");
        printf("Masukkan username anda : ");
        fflush(stdin);
        scanf("%[^\n]s", userword);
        
        system("CLS");
        
        while(temp != NULL)
        {
            if(strcmpi(userword, temp->username)==0)
            {
                printf("Username : %s\n", temp->username);
                printf("Total Belanja : Rp %.2f\n\n", temp->sum_belanja);
                printf("Rincian Belanja :\n");
                btemp = temp->linked_belanja;
                i = 1;
                while(btemp != NULL)
                {
                    printf("%d. Judul Buku : %s\n",i ,btemp->judul);
                    printf("   Harga per pcs x jumlah : Rp %.0f x %d = Rp %.0f\n", btemp->harga, btemp->kuantiti, (btemp->harga)*(btemp->kuantiti));
                    btemp = btemp->next;
                    i++;
                }
                flag = 1;
            }
            temp = temp->next;
        }
        if(flag == 0)
        {
            printf("Username tidak ada!\n");
        }
    }
}

//function untuk menampilkan linked list pembeli
void list_pembeli(user awal)
{
    int i;
    user temp = awal;
    belanja btemp;

    if(temp == NULL)
    {
        printf("\nBelum ada Pembeli!\n");
    }
    
    else 
    {
        printf("\t|---------List Pembelian Buku---------|\n\n");
        while(temp != NULL)
        {   
            i = 1;
            printf("Username : %s\n", temp->username);
            printf("Total Belanja : Rp %.2f\n\n", temp->sum_belanja);
            printf("Rincian Belanja :\n");
            btemp = temp->linked_belanja;
            while(btemp != NULL)
            {
                printf("%d. Judul Buku : %s\n",i ,btemp->judul);
                printf("   Harga per pcs x jumlah : Rp %.0f x %d = Rp %.0f\n", btemp->harga, btemp->kuantiti, (btemp->harga)*(btemp->kuantiti));
                btemp = btemp->next;
                i++;
            }
            printf("\n==========================================================\n\n");
            temp = temp->next;
        }
    }
        
}

//menyimpan linked list buku kedalam sebuah file daftar_buku.txt
void save(buku head)
{
	FILE *fptr;
	fptr = fopen("daftar_buku.txt","w");
	buku temp;
	temp = head;
	
	while(1)
	{
		if(temp != NULL)
		{
			fprintf(fptr,"%s\t%s\t%s\t%f\t%d\n",temp->nama_buku,temp->author,temp->rak,temp->harga_buku,temp->halaman);
		}
		else
		{
			break;
		}
		temp = temp->next;
	}
	fclose(fptr);
}

//menyimpan password admin kedalam sebuah file password.txt
void savepass()
{
	FILE *passptr;
    passptr = fopen("password.txt", "w");
    fprintf(passptr,"%s", password);
    fclose(passptr);
}