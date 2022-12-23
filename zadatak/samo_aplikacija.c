#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
FILE *fp;
struct storage{
	char name[11];
	char surname[11];
	int index;
	int points;
};
struct storage s[10];

void ucitaj_studenta(FILE *fp, struct storage s[], int *n) {
	*n =0;
	while(fscanf(fp, "%s %s EE%d_2020 = %d\n",s[*n].name,s[*n].surname,&s[*n].index,&s[*n].points) != EOF) {
		(*n)++;
	}
}

int izmena(int n, struct storage s[]){
	fp = fopen ("test.txt", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	//int n;
	int br_ind, bodovi;

	ucitaj_studenta(fp, s, &n);
	fclose(fp);
	fp = fopen ("test.txt", "w");
	fclose(fp);
	printf("unesi broj indeksa studenta cije bodove zelis da promenis: ");
	scanf("%d",&br_ind);
	printf("unesi novi broj bodova: ");
	scanf("%d",&bodovi);
	fp = fopen ("test.txt", "a");
	for(int i = 0; i<n; i++){
		if(br_ind == s[i].index){
			s[i].points = bodovi;
		}
		fprintf(fp, "%s %s EE%d_2020 = %d\n",s[i].name,s[i].surname,s[i].index,s[i].points);
	}
	fclose(fp);
}


int unos_novog_studenta(int n, struct storage s[]){
	fp = fopen ("test.txt", "a");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	for(int i=0; i<n; i++) {
		printf("ime:\n");
		scanf("%s",s[i].name);
		printf("prezime:\n");
		scanf("%s",s[i].surname);
		printf("broj indeksa:\n");
		scanf("%d",&s[i].index);
		printf("broj bodova:\n");
		scanf("%d",&s[i].points);
		fprintf(fp,"%s %s EE%d_2020 = %d\n",s[i].name,s[i].surname,s[i].index,s[i].points);
	}
	if(fclose(fp))
	{
			puts("Problem pri zatvaranju /dev/storage");
			return -1;
	}
}

int brisanje(int n, struct storage s[]){
	int br_ind;
	fp = fopen ("test.txt", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	ucitaj_studenta(fp, s, &n);
	fclose(fp);
	fp = fopen ("test.txt", "w");
	fclose(fp);
	printf(" unesi broj indeksa studenta kog zelis da obrises:\n");
	scanf("%d",&br_ind);
	fp = fopen ("test.txt", "a");
	for(int i = 0; i<n; i++){
		if(br_ind == s[i].index){
			strcpy(s[i].name, " ");
			strcpy(s[i].surname, " ");
			s[i].index = 0;
			s[i].points = 0;
		}
		if( strcmp(s[i].name, " ") &&
			strcmp(s[i].surname, " ") &&
			s[i].index != 0 )
		fprintf(fp, "%s %s EE%d_2020 = %d\n",s[i].name,s[i].surname,s[i].index,s[i].points);
	}
	fclose(fp);
}

int citanje(int n, struct storage s[]){
	fp = fopen ("test.txt", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	ucitaj_studenta(fp, s, &n);
	for(int i=0; i<n; i++){
		if( s[i].name != " " &&
			s[i].surname != " " &&
			s[i].index != 0 &&
			s[i].points != 0)
		printf("%s %s EE%d_2020 = %d\n", s[i].name, s[i].surname, s[i].index, s[i].points);
	}
	fclose(fp);
}

int main () {
	int end=0;        
	int pom;
	int n=10;
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
			unos_novog_studenta(1, s);
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