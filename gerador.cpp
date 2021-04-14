#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <cstring>

int main(){
	FILE *arquivo;
	arquivo = fopen("arquivo.txt", "w");
	
	int tamanho;
	printf("informe o tamanho:\n");
	scanf("%d", &tamanho);
	int v[tamanho];
	char aux[tamanho];
	char aux2[tamanho];
	
	for(int i =0; i<tamanho;i++){
		v[i]= rand() %4;
	}
	for(int i=0;i<tamanho;i++){
		if(v[i]==0){aux[i]='a';}
		else if(v[i]==1){aux[i]='g';}
		else if(v[i]==2){aux[i]='t';}
		else if(v[i]==3){aux[i]='u';}
	}aux[tamanho]= '\0';
	strcpy(aux2,aux);
	for (int i = 0; i < tamanho; i++){
		int r = rand() % tamanho;
		int temp = aux2[i];
		aux2[i] = aux2[r];
		aux2[r] = temp;
	}aux2[tamanho]= '\0';
	fprintf(arquivo,"%s %s",aux,aux2);
	printf("Blocos Gerados\n");
	fclose(arquivo);
	printf("Blocos Salvos");
	return 0;
}
