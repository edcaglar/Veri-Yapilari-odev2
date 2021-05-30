#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct n{
  int kimlikno;
  char *isim;
  int *arkadas;             //Kullanicilarin bilgilerinin tutuldugu yapi.
  struct n *right;
  struct n*left;
};
typedef struct n node;

//Agaca kullanici eklemek icin kullandigimiz fonksiyon
    node *insert(node *agac,int numara,char *name,int *arkadasnumara){
  int i;
  if(agac == NULL){         //Eger su an gosterilen node'un ici bos ise  node burada olusturuluyor
    node *root=malloc(sizeof(node));
    root->kimlikno=numara;
    root->arkadas=malloc(sizeof(arkadasnumara));
    root->isim=malloc(sizeof(name));   //Degerleri aktarabilmek icin pointerlara alan yarattim.

    for(i=0;name[i] != '\0'; i++)
      root->isim[i]=name[i];    //Dizinin bitis karakterini gorene kadar structimizdaki isime kopyalayacak.
    i=0;

    while(arkadasnumara[i] != 0){   //Dosyadan okudugumuz yerde her arkadas dizisinin sonuna 0 koydugum icin
        root->arkadas[i]=arkadasnumara[i];    //0 gorene kadar diziyi structa aktariyor.
        i++;
    }
    return root;
  }
  //Asagidaki kodlar eklenicek dugumu bulmak icin fonksiyonu tekrar tekrar cagiriyor.
  if(agac->kimlikno < numara){
    agac->right=insert(agac->right,numara,name,arkadasnumara);
    return agac;
  }
  agac->left=insert(agac->left,numara,name,arkadasnumara);
  return agac;
}


void dolas(node *agac){
  int i=0;
  if(agac==NULL)   //Agac bossa NULL dondurur.
    return;
  dolas(agac->left);        //Agaci isim ve kimlik numaralariyla LNR seklinde okuduk.
  printf("%d ",agac->kimlikno);
  printf("%s",agac->isim);
  printf("\n");
  dolas(agac->right);
}

void greater(node*agac, int x){
  int i=0;
  if(agac==NULL)   //Agac bossa NULL dondurur.
    return;
  greater(agac->left,x);        //Agaci isim ve kimlik numaralariyla LNR seklinde okuduk.
  if(agac->kimlikno > x){
    printf("%d ",agac->kimlikno);
    printf("%s",agac->isim);
    printf("\n");
  }
  greater(agac->right,x);
}




node *maxdeger(node *agac){
  while(agac->right!=NULL)    //Agacin max degeri en saginda oldugu icin
    agac=agac->right;         //agac->right i NUll olana kadar sola gittik
  return agac;                //ve en sagindaki nodeu return ettik.
}

node * mindeger(node *agac){
  while(agac->left!=NULL)   //Agacin min degeri en solunda oldugu icin
    agac=agac->left;        //agac->left i NUll olana kadar sola gittik
  return agac;              //ve en solundaki nodeu return ettik.
}


node *delete(node*agac,int x){
  if(agac == NULL)
  return NULL;

  if(agac->kimlikno == x){
    if(agac->right == NULL && agac->left == NULL) //Agacin sag ve sol alt cocugu yoksa
      return NULL;   //Silinmesi saglaniyor.

    if(agac->right != NULL){  //Agacin sag alt agaci NULL degil ise
      agac->kimlikno=(mindeger(agac->right))->kimlikno;//Sag alt agacinin en kucuk degeri o nodela yer degisiyor
      agac->isim=(mindeger(agac->right))->isim;        //Recursive sekilde  alt agacta da degistirilen node siliniyor.
      agac->arkadas=(mindeger(agac->right))->arkadas;
      agac->right=delete(agac->right,(mindeger(agac->right))->kimlikno);
      return agac;
    }
    agac->kimlikno=(maxdeger(agac->left))->kimlikno;//Eger agacin sag alt agaci NULL ve solu doluysa
    agac->isim=(maxdeger(agac->left))->isim;        //Agacin sol alt agacindaki en buyuk degerle yer degisiyor.
    agac->arkadas=(maxdeger(agac->left))->arkadas;  //Recusive sekilde sol alt agacta da degistirilen node siliniyor.
    agac->left=delete(agac->left,(maxdeger(agac->left))->kimlikno);
    return agac;

  }
  if(agac->kimlikno < x){   //Deger bulunamadiysa ve node degeri istenen degerden kucukse
    agac->right=delete(agac->right,x);  //sag alt agacta deger araniyor.
    return agac;
  }
  agac->left =delete(agac->left,x);//Istenen deger nodedan kucukse sol alt agaca bakiliyor.
  return agac;
}

node * arama(node *agac , int sayi){
  node *iter=agac;
  if(iter==NULL)      //Agac bossa NULL donduruyor.
      return agac;

  while(iter->kimlikno != sayi){  //Aranan sayi bulunana kadar bu dongu icinde
    if(iter->kimlikno > sayi)     //Buyuk veya kucuk olmasina gore sag ya da sol alt agaca geciliyor.
      iter=iter->left;
    else
      iter=iter->right;
    if(iter==NULL)              //Eger bu dongu icinde NULL bir degere gidilirse NULL donduruluyor.
        return NULL;
  }
  if(iter->kimlikno ==sayi){    //Eger deger bir nodeda bulunursa bulunan node donduruluyor.
    return iter;
  }
}



int main(){
    node *agac=NULL; //Agacimizi gosteren bir struct pointeri agac tanimladik.
    FILE *fp;
    fp=fopen("Input.txt","r");
    int secim,kimlikno;//Menuden yapilan secimlere gitmek icin integer bir deger tanimladim.
    node *iter;   //Agacin gosterdigi yeri degistirmemek icin gecici bir struct pointeri tanimladim.
    int *arkadasnumara=malloc(sizeof(int)*30);//Arkadaslari structa gondermeden once tutmak icin kullanilacak int dizi
    char *name=malloc(sizeof(char)*30);     //Kisilerin isimlerini tutacak dizi.
    int kimliknumara;
    char c;     //fgetc fonksiyonuyla beraber kullanabilmek icin tanimladim
    int i,j=0;    //while , for dongulerini kontrol edebilmek icin tanimladim.

    do{
      printf("----------------------------------------------------------------------------------------------------\n");
      printf("1-Yeni kullanici eklemek icin(insertNewUser)\n\n2-Kimlik numarasi vererek birini silmek icin(deleteUser)\n\n");
      printf("3-Kimlik numarasi vererek kisinin agacta olup olmadigini sorgulama icin.(Contains)\n\n");
      printf("4-Kimlik numarasi vererek kisinin arkadaslarini isimlerini ogrenmek icin.(friends)\n\n");
      printf("5-Agacta kac eleman oldugunu ogrenmek icin.(size)\n\n");
      printf("6-Kimlik numarasi verekek alt agactaki kisilerin kimlik numaralarina gore siralanmasi icin.(printNext)\n\n");
      printf("7-Kimlik numarasi verilen kisiden buyuk numarali kisiler icin.(printGreater)\n\n");
      printf("8-Agactaki kisilerin kimlik no ve isimlerine gore sirali bir sekilde gormek icin.(printInOrder)\n\n");
      printf("Uygulamadan cikis yapmak icin 0 giriniz.\n\n");
      printf("-------------------------------------------------------------------------------------------------------\n");
      scanf("%d",&secim);

      if(secim == 1){       //insertUser



        //Bu dongu dosyadan bilgileri alip agaca gondermek icin.
        while(!feof(fp)){//file pointerimiz dosyanin sonuna gelmedigi surece ekleme yapiliyor.

          fscanf(fp,"%d",&kimliknumara);// Ilk olarak kimlikno integerini aldik.
          c=fgetc(fp);    // Virgulu atlamak icin fgetc fonskiyonunu kullandim

          if(feof(fp))
            break;

          i=0;        //i degiskeninin degeri dongulerin icinde hep degistigi icin sifirlamamiz gerekiyor.
          while(c != EOF && (c = fgetc(fp)) != ','){ //Dosya sonuna veya virgul karakterine gelinmedigi surece okudugu
            if(c == '\n')                               //charlari name dizisine atiyor.
              break;                          //Eger arkadasi yok ise virgul gormeden new line karakteri gorecegi icin burda \n kontrolu yaptim
            name[i++]=c;                      //new line karakteri gorurse isim dizisine eklemesin diye donguden cikiyoruz.
          }
          i=0;
          if(c == '\n'){      //Eger arkadasi olmayan bir insansa burda agaca aktariliyor.Ve alttaki arkadas ekleyen while'a girmeden donguye devam ediyor.
            agac = insert(agac,kimliknumara,name,arkadasnumara);
            j++;
            free(name);
            name=malloc(sizeof(char)*30);
            continue;
          }
          //Eger arkadasi olan biriyse su an file pointerimiz virgulden sonraki integeri gosterdigi icin scanfle arkadas dizisine atiyoruz.
          while(1 == fscanf(fp,"%d",&arkadasnumara[i++])){
              c=fgetc(fp);        //Her arkadastan sonra bir tane virgul oldugu icin virgulu burada atliyoruz.
              if(c != '-')
                break;      //Eger - karakteri yerine eof veya \n okursa donguden cikiyor.
          }
          arkadasnumara[i]=0;   //Insert fonksiyounda 0 gorene kadar arkadaslarini eklemek icin burda son karakteri 0 a esitledim.
          agac = insert(agac,kimliknumara,name,arkadasnumara);//Burada arkadasi olan insanlari agaca ekliyoruz.

          //Dosyadan okumayi fonksiyon olarak yazmadigim icin her 10 kisi girildiginde burada siralanmis agac ekrana bastiriliyor.
          j++;//Ve size istenildiginde de j yi kullanacagim.
          if(j%10 == 0){
            printf("Ilk %d kisinin siralanmis hali.\n",j);
            dolas(agac);
            printf("\n");
          }

            //Pointerlarin ustune tekrar yazildiginda fazladan karakterlerden kurtulmak icin degerlerini yok edip tekrar tanimlamam gerekti.
          free(name);
          free(arkadasnumara);
          arkadasnumara=malloc(sizeof(int)*30);
          name=malloc(sizeof(char)*30);
        }


      }

      if(secim == 2){       //deleteUser
        printf("Silmek istediginiz kisinin kimlik numarasini giriniz.\nSilmek istediginiz kisi:");
        scanf("%d",&kimlikno);      //kimlikno ile agactan silinmesi istenen kisiyi aldik.
        agac=delete(agac,kimlikno); //delete fonksiyonuyla silindi.
        printf("\n silinmis sekilde agacin sirali hali:\n");
        dolas(agac);                //Silinmis haliyle agac ekrana bastirildi.
      }

      if(secim == 3){       //findUser
        printf("Bulmak istediginiz kisinin kimlik numarisini girin.\nKimlik no:");
        scanf("%d",&kimlikno);
        iter=arama(agac,kimlikno);  //node tipinde gosterici olan iteri arama fonksiyonu sonucunda buldugumuz node a esitledik.
        if(iter != NULL)
          printf("%s",iter->isim);//Buldugumuz node'daki kisinin ismini yazdirdik.
        else
          printf("Aranan kisi agacta yok");
              //Buldugumuz node'daki kisinin ismini yazdirdik.
      }

      if(secim == 4){       //Kimlik nosu verilen kisinin arkadaslarini bul.
        printf("Arkadaslarini bulmak istediginiz kisinin kimlik numarisini girin.\nKimlik no:");
        scanf("%d",&kimlikno);
        iter=arama(agac,kimlikno);  //Arama fonksiyonuyla kisinin bulundugu node'a gidiyorum.
        printf("\nArkadaslari:\n");
        if (iter != NULL){//Eger kisi bulunduysa ve iter NULL gostermiyorsa
          for(i=0;iter->arkadas[i] != 0 && iter->arkadas[i] <500 ;i++)//0 veya artik bir karakter gorene kadar
          printf("%s\n",arama(agac,iter->arkadas[i])->isim);  //arama fonksiyonuyla arkadaslarinin isimlerini bularak yazdiriyor.
        }
        else
          printf("Aranan kisi agacta bulunamadi.");
      }

      if(secim == 5){       // size
        printf("Agacta %d eleman bulunuyor",j); //Dosyadan eleman okurken j degerini her elemanda 1 artirmistik.
      }

      if(secim == 6){       //printNext
        printf("Sirali alt agaci istenen kisinin kimlik nosunu girin\nKimlik no:"); //
        scanf("%d",&kimlikno);
        printf("Alt agaci:\n");
        iter=arama(agac,kimlikno);//Bu fonksiyondan istenen kisinin node'unu iter e esitledik.
        dolas(iter);              //iteri yeni agacmis gibi fonksiyona sokarak butun elemanlarini sirayla yazdirdik.
      }

      if(secim == 7){
        printf("Kendisinden buyuk numarali kisileri bulmak istediginiz kisinin kimlik no girin\n");
        printf("Kimlik no:");
        scanf("%d",&kimlikno);
        printf("Kendisinden buyuk kisiler:\n");
        greater(agac,kimlikno);

      }

      if(secim == 8){   //printInOrder
        dolas(agac);
      }

      printf("\nDevam etmek icin 1 cikmak icin 0 giriniz.\nSecim:");
      scanf("%d",&secim);         //0 dan farkli bir sayi girilince menuye donuyor.

    }while(secim !=0);
    fclose(fp);
}
