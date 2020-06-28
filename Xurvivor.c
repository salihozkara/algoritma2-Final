#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct
{
    int hp;
    struct
    {
        int parkurUzunlugu;
        int engelSayisi;
        int *engellerinYeri;
    }parkur;
}oyuncu;
void engel();
void oyun();
int asalmi(int,int);
void siralama();
void oku();
void kapanis(FILE *);
int main() {
    oyun();
    return 0;
}
void siralama()
{
    int tut;
    for (int i = 0; i <oyuncu.parkur.engelSayisi ; ++i) {
        for (int j = 0; j <oyuncu.parkur.engelSayisi-1 ; ++j) {
            if(oyuncu.parkur.engellerinYeri[j]>oyuncu.parkur.engellerinYeri[j+1])
            {
                tut=oyuncu.parkur.engellerinYeri[j+1];
                oyuncu.parkur.engellerinYeri[j+1]=oyuncu.parkur.engellerinYeri[j];
                oyuncu.parkur.engellerinYeri[j]=tut;
            }
        }
    }
}

int asalmi(int engelYeri,int hp)
{
    int kucuk;
    if(engelYeri>hp)
        kucuk=hp;
    else
        kucuk=engelYeri;
    for (int i = 2; i <kucuk ; ++i) {
        if(hp%i==0 && engelYeri%i==0)
            return 0;
    }
        return 1;//aralarında asal olmadığı için 0 dönüyor
}
void oyun()
{
    FILE *dosya;
    dosya=fopen("log.txt","w");
    srand(time(NULL));
    oyuncu.hp=100;
    oyuncu.parkur.parkurUzunlugu=50+rand()%50;
    fprintf(dosya,"Yarisma Basladi !!!\nYarismaci HP=%d\n",oyuncu.hp);
    engel();//engellerin yeri belirleniyor
    siralama();//engelleri sıralanıyor
    for (int i = 0; i < oyuncu.parkur.engelSayisi ; ++i) {
        if(asalmi(oyuncu.parkur.engellerinYeri[i],oyuncu.hp))//0 dönmüşşe aralarında asal değil başka değer ise o değer asal olan engel yeri
        {
            fprintf(dosya,"Yarismaci %d. metreye kadar bir engelle karsilasmadi.\n%d. metrede"
                          " bir engel var.%d ve %d aralarinda asaldir\n",oyuncu.parkur.engellerinYeri[i],oyuncu.parkur.engellerinYeri[i]
                    ,oyuncu.hp,oyuncu.parkur.engellerinYeri[i]);
            oyuncu.hp -= *(oyuncu.parkur.engellerinYeri + i);//hp den karşılaşılan metre düşüşü
        }
        else
            fprintf(dosya,"Yarismaci %d. metreye kadar bir engelle karsilasmadi.\n%d. metrede"
                          " bir engel var.%d ve %d aralarinda asal degildir\n",oyuncu.parkur.engellerinYeri[i],oyuncu.parkur.engellerinYeri[i]
                    ,oyuncu.hp,oyuncu.parkur.engellerinYeri[i]);
        fprintf(dosya,"Yarismaci HP=%d\n",oyuncu.hp);
        if(oyuncu.hp<=0) {//hp kontrolü
            fprintf(dosya, "UZGUNUM!!! yarismayi tamamlayamadin\n");
            kapanis(dosya);
        }
    }
    fprintf(dosya,"TEBRIKLER!!! yarismayi %d hp ile bitirdiniz.",oyuncu.hp);
    kapanis(dosya);
}
void engel()
{
    oyuncu.parkur.engelSayisi=2+rand()%3;
    oyuncu.parkur.engellerinYeri=malloc(oyuncu.parkur.engelSayisi* sizeof(int));//dizi için yer ayrılıyor
    for (int i = 0; i <oyuncu.parkur.engelSayisi ; ++i) {
        *(oyuncu.parkur.engellerinYeri+i)=5+rand()%(oyuncu.parkur.parkurUzunlugu-5);
        for (int j = 1; j <=i ; ++j) {
            if(oyuncu.parkur.engellerinYeri[i]==oyuncu.parkur.engellerinYeri[i-j])//aynı engel yerini iki kere seçmemesi için kontrol
                i--;//aynı engel var ise for un tekrar dönmesini sağlıyor
        }
    }
}
void oku()//oyun bitince ekranda gözükmesi için
{
    int karakterSayisi=0;
    FILE *dosya;
    dosya=fopen("log.txt","r");
    if(!dosya)//dosya kontrolü
    {
        printf("dosya bulunamadi.\n");
        system("pause");
        exit(1);
    }
    while(1){
        if(fgetc(dosya) != EOF)//dosya sonuna gelmiş mi diye kontrol ediyor
            karakterSayisi++;
        else
            break;
    }
    rewind(dosya);
    char *dizi;
    dizi=malloc(karakterSayisi* sizeof(char));
    for (int i = 0; i <karakterSayisi ; ++i) {
        fscanf(dosya,"%c",&dizi[i]);
    }
    printf("%s\n",dizi);
    free(dizi);
    fclose(dosya);
    system("pause");
}
void kapanis(FILE *fp)
{
    fclose(fp);
    free(oyuncu.parkur.engellerinYeri);//ayrılan yer geri veriliyor
    oku();
}