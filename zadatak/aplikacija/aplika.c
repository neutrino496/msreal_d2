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

int izmena(char name[], char surname[], int br_ind, int bodovi)
{
	fp = fopen ("/dev/storage", "a");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	printf("ime studenta:\n");
	scanf("%s",name);
	printf("prezime studenta:\n");
	scanf("%s",surname);
	printf("broj indeksa studenta:\n");
	scanf("%d",&br_ind);
	printf("novi broj bodova studenta:\n");
	scanf("%d",&bodovi);
	fprintf(fp, "%s %s EE%d_2020 = %d\n",name,surname,br_ind,bodovi);
	fclose(fp);
	/*if(fclose(fp))
	{
		puts("Problem pri zatvaranju /dev/storage");
		return -1;
	}*/
}

void unos_novog_studenta(char name[], char surname[], int br_ind, int bodovi)
{
	fp = fopen ("/dev/storage", "a");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	printf("ime studenta:\n");
	scanf("%s",name);
	printf("prezime studenta:\n");
	scanf("%s",surname);
	printf("broj indeksa studenta:\n");
	scanf("%d",&br_ind);
	printf("broj bodova studenta:\n");
	scanf("%d",&bodovi);
	fprintf(fp, "%s %s EE%d_2020 = %d\n",name,surname,br_ind,bodovi);
	fclose(fp);
	/*if(fclose(fp))
	{
		puts("Problem pri zatvaranju /dev/storage");
		return -1;
	}*/
}

void brisanje(char name[],char surname[],int br_ind)
{
	fp = fopen ("/dev/storage", "a");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	printf("unesi ime studenta kog zelis da obrises:\n");
	scanf("%s",name);
	printf("unesi prezime studenta kog zelis da obrises:\n");
	scanf("%s",surname);
	printf("unesi broj indeksa studenta kog zelis da obrises:\n");
	scanf("%d",&br_ind);
	fprintf(fp, "izbrisi = %s %s %d \n",name,surname,br_ind);
	fclose(fp);
	/*if(fclose(fp))
	{
		puts("Problem pri zatvaranju /dev/storage");
		return -1;
	}*/
}

int citanje(int n, struct storage s[])
{
	int i;
	fp = fopen ("/dev/storage", "a+");
	if(fp==NULL)
	{
		puts("Problem pri otvaranju /dev/storage");
		return -1;
	}
	ucitaj_studenta(fp, s, &n);
	for(i=0;i<n;i++)
		printf("%s %s EE%d_2020 = %d\n", s[i].name, s[i].surname, s[i].index, s[i].points);
	fclose(fp);
	/*if(fclose(fp))
	{
		puts("Problem pri zatvaranju /dev/storage");
		return -1;
	}*/
}

int main () {
    char ime[11];
    char prezime[11];
    int indeks;
    int br_bodova;
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
			unos_novog_studenta(ime,prezime,indeks,br_bodova);
			break;
		case 2:
		    izmena(ime,prezime,indeks, br_bodova);
			break;
		case 3:
			brisanje(ime,prezime,indeks);
			break;
		case 4:
			citanje(n,s);
			break;
		case 5:
			end=1;
			break;
		}
	}
}