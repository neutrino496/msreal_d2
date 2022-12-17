#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
			unos_novog_studenta();
			break;
		case 2:
			izmena();
			break;
		case 3:
			brisanje();
			break;
		case 4:
			citanje();
			break;
		case 5:
			end=1;
			break;
		}
	}
}
