#include <iostream>
#include <cstring>
#include <stdio.h>

using namespace std;

bool Testar(int teste[], int tam){
	for(int m =0; m<tam;m++){
		if(teste[m]==0){return false;}	
	}
	return true;
}
//Função que verifica se as partições de uma string aparecem na outra
bool Encontra_blocos(char str_1[], char str_2[],int maior_teste[2]){
	int i = 0;
	int teste[strlen(str_1)];
	int aux;
	int tam_1 = strlen(str_1);
	int tam_2 = strlen(str_2);

	for(i = 0; i <= tam_2; i++){
		if(str_2[i] == str_1[0]){
			for(int k = 0; k<tam_1; k++){teste[k]=0;}
			maior_teste[0] = i;
			aux = i;
			for(int j = 0; j<tam_1; j++){
				if(str_1[j]==str_2[aux]){
					teste[j]=1;
					aux++;
				}
			}
			if(Testar(teste, tam_1)){
				maior_teste[1] = aux-1;
				return true;
			}
		}
	}
	return false;
}

struct Bloco{
	int tamanho;
	int ini_str_1;
	int fin_str_1;
	int ini_str_2;
	int fin_str_2;
	char bloco[4000];
};

//Função MCSP, basicamente é a responsavel por controlar todo a aplicação
int Guloso(char str_1[], char str_2[]){
	int maior_teste[2];
	int v=0;
	int t = strlen(str_1);
	int x = 1;
	int a = t;
	int i = 0;
	int f = t;
	int ini = i;
	struct Bloco *blc = (struct Bloco*)malloc(sizeof(struct Bloco)*x);
	bool teste = true;
	while(teste){
		int tam = f;
		char aux[tam];
		int k =0;
		
		for(ini=i; ini<f; ini++){
			aux[k] = str_1[ini];
			k++;
		}aux[k]='\0';

		if(Encontra_blocos(aux,str_2,maior_teste)){
			blc[v].ini_str_1 = i;
			blc[v].fin_str_1 = f-1;
			blc[v].ini_str_2 = maior_teste[0];
			blc[v].fin_str_2 = maior_teste[1];
			x++;
			blc[v].tamanho = strlen(aux);
			strcpy(blc[v].bloco,aux);
			v++;
			for(int u= i; u<=ini-1; u++){str_1[u]='-';}
			for(int u= maior_teste[0]; u<=maior_teste[1]; u++){	str_2[u]='+';}

			struct Bloco *blc2;
			blc2 = (struct Bloco*) realloc(blc,sizeof(struct Bloco)*x);
			if(blc2 != NULL){blc = blc2;}
		}
		cout<<str_1<<endl;
		cout<<str_2<<endl;
		if(f+1 <= t){
			i++;
			f++;
		}
		else{
			if(a-1 > 0){
				a = a-1;
				f = a;
				i = 0;
			}
			else{
				teste = false;
			}
		}
	}

	free(blc);
	return x-1;
}

int main (){
	char str_1[4000];
	char str_2[4000];
	int sol;

	FILE *arq_1;
	arq_1 = fopen("arquivo.txt", "r");

	if(arq_1 == NULL){cout<<"Erro ao Ler o Arquivo!!!"<<endl;}

	fscanf(arq_1,"%s %s",str_1, str_2);
	cout<<"STRING 1: "<<endl;
	cout<<str_1<<endl;
	cout<<"STRING 2: "<<endl;
	cout<<str_2<<endl;


	sol = Guloso(str_1,str_2);
	cout<<"SOLUCAO GULOSO ENCONTRADA: "<<sol<< " PARTICOES";

	fclose(arq_1);
	return 0;
}
