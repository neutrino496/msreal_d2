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
struct storage s[10];

void ucitaj_studenta(FILE *fp, struct storage storage_s[], int *n) {
	*n =0;
	while(fscanf(fp, "%s %s EE%d=%d\n",storage_s[*n].name,storage_s[*n].surname,&storage_s[*n].index,&storage_s[*n].bodovi) != EOF) {
		(*n)++;
	}
}

void izmena(int n, struct storage storage_s[]){
	fp = fopen ("/dev/storage", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	//int n;
	int br_ind, points;

	ucitaj_studenta(fp, storage_s, &n);
	fclose(fp);
	fp = fopen ("/dev/storage", "w");
	fclose(fp);
	printf("unesi broj indeksa i novi broj bodova");
	scanf("%d %d",&br_ind, &points);
	fp = fopen ("/dev/storage", "a+");
	for(int i = 0; i<n; i++){
		if(br_ind == storage_s[i].index){
			storage_s[i].bodovi = points;
		}
		fprintf(fp, "%s %s EE%d=%d\n",storage_s[i].name,storage_s[i].surname,storage_s[i].index,storage_s[i].bodovi);
	}
	fclose(fp);
}


void unos_novog_studenta(int n, struct storage storage_s[]){
	fp = fopen ("/dev/storage", "a");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	for(int i=0; i<n; i++) {
		printf("ime:\n");
		scanf("%s",storage_s[i].name);
		printf("prezime:\n");
		scanf("%s",storage_s[i].surname);
		printf("broj indeksa:\n");
		scanf("%d",&storage_s[i].index);
		printf("broj bodova:\n");
		scanf("%d",&storage_s[i].bodovi);
		fprintf(fp,"%s %s EE%d=%d\n",storage_s[i].name,storage_s[i].surname,storage_s[i].index,storage_s[i].bodovi);
	}
	if(fclose(fp))
	{
			puts("Problem pri zatvaranju /dev/storage");
			return -1;
	}
}

void brisanje(int n, struct storage storage_s[]){
	char ime[11], prezime[11];
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
	for(int i = 0; i<n; i++){
		if(ime == storage_s[i].name && prezime == storage_s[i].surname && br_ind == storage_s[i].index){
			strcpy(storage_s[i].name, " ");
			strcpy(storage_s[i].surname, " ");
			storage_s[i].index = 0;
			storage_s[i].bodovi = 0;
		}
		if( strcmp(storage_s[i].name, " ") &&
			strcmp(storage_s[i].surname, " ") &&
			storage_s[i].index != 0 &&
			storage_s[i].bodovi != 0)
		fprintf(fp, "%s %s EE%d=%d\n",storage_s[i].name,storage_s[i].surname,storage_s[i].index,storage_s[i].bodovi);
	}
	fclose(fp);
}

void citanje(int n, struct storage storage_s[]){
	fp = fopen ("/dev/storage", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	ucitaj_studenta(fp, storage_s, &n);
	for(int i=0; i<n; i++){
		if( storage_s[i].name != " " &&
			storage_s[i].surname != " " &&
			storage_s[i].index != 0 &&
			storage_s[i].bodovi != 0)
		printf("%s %s EE%d=%d\n", storage_s[i].name, storage_s[i].surname, storage_s[i].index, storage_s[i].bodovi);
	}
	fclose(fp);
}

int main () {
	int end=0;        
	int pom;
	int n;
	//int l = sizeof(s[].bodovi)/sizeof(s[0].bodovi);
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
			unos_novog_studenta(n, s);
			break;
		case 2:
			izmena(n, s);
			break;
		case 3:
			brisanje(n, s);
			break;
		case 4:
			citanje(n, s);
			break;
		case 5:
			end=1;
			break;
		}
	}
}

