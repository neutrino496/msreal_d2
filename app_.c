#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *fp;
struct storage{
	char name[15];
	char surname[15];
	int index;
	int bodovi;
};
struct storage s[];

void ucitaj_studenta(FILE *fp, struct storage storage_s[], int *n) {
	*n =0;
	while(fscanf(fp, "%s %s EE%d=%d\n",storage_s[*n].name,storage_s[*n].surname,&storage_s[*n].index,&storage_s[*n].bodovi) != EOF) {
		(*n)++;
	}
}

void izmena(int len, struct storage storage_s[]){
	fp = fopen ("/dev/storage", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	int n;
	int br_ind, points;

	ucitaj_studenta(fp, storage_s, &n);
	fclose(fp);
	fp = fopen ("/dev/storage", "w");
	fclose(fp);
	printf("unesi broj indeksa i novi broj bodova");
	scanf("%d %d",&br_ind, &points);
	fp = fopen ("/dev/storage", "a+");
	for(int i = 0; i<len; i++){
		if(br_ind == storage_s[i].index){
			storage_s[i].bodovi = points;
		}
		fprintf(fp2, "%s %s EE%d=%d\n",storage_s[i].name,storage_s[i].surname,storage_s[i].index,storage_s[i].bodovi);
	}
	fclose(fp);
}


void unos_novog_studenta(struct storage storage_s[]){
	fp = fopen ("/dev/storage", "a");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	printf("ime:\n");
	scanf("%s",storage_s[].name);
	printf("prezime:\n");
	scanf("%s",storage_s[].surname);
	printf("broj indeksa:\n");
	scanf("%d",&storage_s[].index);
	printf("broj bodova:\n");
	scanf("%d",&storage_s[].bodovi);
	fprintf(fp,"%s %s EE%d=%d\n",storage_s[].name,storage_s[].surname,storage_s[].index,storage_s[].bodovi);
	if(fclose(fp))
	{
			puts("Problem pri zatvaranju /dev/storage");
			return -1;
	}
}

void brisanje(int len, struct storage storage_s[]){
	char ime[], prezime[];
	int br_ind;
	fp = fopen ("/dev/storage", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	ucitaj_studenta(fp, storage_s, &n);
	fclose(fp);
	fp = fopen ("/dev/storage", "w");
	fclose(fp);
	printf("unesi ime, prezime, indeks");
	scanf("%s %s EE%d/2020", ime,prezime,&br_ind);
	fp = fopen ("/dev/storage", "a");
	for(int i = 0; i<len; i++){
		if(ime == storage_s[i].name && prezime == storage_s[i].surname && br_ind == storage_s[i].index){
			storage_s[i].name = '/0';
			storage_s[i].surname = '/0';
			storage_s[i].index = 0;
			storage_s[i].bodovi = 0;
		}
		if( storage_s[i].name != '/0' &&
			storage_s[i].surname != '/0' &&
			storage_s[i].index != 0 &&
			storage_s[i].bodovi != 0)
		fprintf(fp, "%s %s EE%d=%d\n",storage_s[i].name,storage_s[i].surname,storage_s[i].index,storage_s[i].bodovi);
	}
	fclose(fp);
}

void citanje(int len, struct storage storage_s[]){
	fp = fopen ("/dev/storage", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	ucitaj_studenta(fp, storage_s, &n);
	for(int i=0; i<len; i++){
		if( storage_s[i].name != '/0' &&
			storage_s[i].surname != '/0' &&
			storage_s[i].index != 0 &&
			storage_s[i].bodovi != 0)
		printf("%s %s EE%d=%d\n", storage_s[i].name, storage_s[i].surname, storage_s[i].index, storage_s[i].bodovi);
	}
	fclose(fp);
}

int main () {
	int end=0;        
	int pom;
	int l = sizeof(s[].bodovi)/sizeof(s[0].bodovi);
	while(!end)
	{
		do{
			printf("Pritisnite odgovajuci broj za zeljenu funkciju\n");
			printf("1.Unos novog studenta\n");
			printf("2.Izmena broja bodova studenta\n");
			printf("3.Brisanje studenta\n");
			printf("4.Citanje liste\n");
			printf("5.Izlaz\n");
			scanf("%d",&pom);
			if (pom>5 && pom<1)
			printf("Pritisnut je pogresan broj");
		}
		while(pom>5 && pom<1);
		switch(pom){
		case 1:
			unos_novog_studenta(s);
			break;
		case 2:
			izmena(l, s);
			break;
		case 3:
			brisanje(l, s);
			break;
		case 4:
			citanje(l, s);
			break;
		case 5:
			end=1;
			break;
		}
	}
}
