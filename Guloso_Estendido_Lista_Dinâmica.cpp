#include <iostream>
#include <cstring>
#include <stdio.h>
#include <time.h>

using namespace std;

struct Bloco{
	int tamanho;
	int ini_str_1;
	int fin_str_1;
	int ini_str_2;
	int fin_str_2;
	int tamanho_struct;
	char bloco[4000];
};

bool Testar(int teste[], int tam){
	for(int m =0; m<tam;m++){if(teste[m]==0){return false;}}
	return true;
}

void Copia(char a_x[],char b_x[], char a_[], char b_[],int tam_str){
	int y =0;
	while(y<tam_str){
		a_x[y]=a_[y];
		b_x[y]=b_[y];
		y++;
	}
	a_x[y]='\0';
	b_x[y]='\0';
}

bool Invade(int ini, int fin, int cini, int cfin){
	if((ini>=cini) && (ini <=cfin)){ return true;}
	if((fin>=cini) && (fin <=cfin)){ return true;}
	return false;
}

void Gerador(char aux[],int i, int f, char ax[]){
	int k =0;
	for(int ini=i; ini<=f; ini++){
		aux[k] = ax[ini];
		k++;
	}aux[k]='\0';
}

void preKmp(char *x, int t, int kmpNext[]) {
   int i, j;
   i = 0;
   j = kmpNext[0] = -1;
   while (i < t) {
      while (j > -1 && x[i] != x[j])
         j = kmpNext[j];
      i++;
      j++;
      if (x[i] == x[j])kmpNext[i] = kmpNext[j];
      else kmpNext[i] = j;
   }
}

int KMP(char *x, int t, char *y, int n, int maior_teste[2]) {  
   int i, j, kmpNext[t];
   preKmp(x, t, kmpNext);
   i = j = 0;
   while (j < n) {
      while (i > -1 && x[i] != y[j])
         i = kmpNext[i];
      i++;
      j++;
      maior_teste[0] = j-i;
      maior_teste[1] = (j-i) + (t-1);
      if (i >= t) return j-i;
   }
   return -1;
}

struct Bloco *Guloso_Estendido(char bloco[], char str_1[], char str_2[], int candidato[5]){

	struct Bloco *res = (struct Bloco*)malloc(sizeof(struct Bloco));
	if(res == NULL){cout<<"Erro ao alocar memoria"<<endl;}
	strcpy(res[0].bloco, bloco);
	res[0].ini_str_1 = candidato[0];
	res[0].fin_str_1 = candidato[1];
	res[0].ini_str_2 = candidato[2];
	res[0].fin_str_2 = candidato[3];
	res[0].tamanho = candidato[4];

	for(int l= candidato[0]; l<=candidato[1]; l++){str_1[l]='-';}
	for(int l= candidato[2]; l<=candidato[3]; l++){str_2[l]='+';}
	
	int g = 1;
	int maior_teste[2];
	int t = strlen(str_1);
	int a = t;
	int i = 0;
	int f = t;

	bool teste = true;
	while(teste){
		int tam = f;
		char aux[tam];
		Gerador(aux,i,f-1,str_1);
		if(KMP(aux,strlen(aux),str_2,strlen(str_2),maior_teste)>=0){
			int marcador =0;
			bool var = true;
			while(marcador<g){
				if((Invade(res[marcador].ini_str_1, res[marcador].fin_str_1, i, f-1)) 
				|| (Invade(res[marcador].ini_str_2, res[marcador].fin_str_2, maior_teste[0], maior_teste[1]))){
					var = false;
					marcador = g;
				}
				marcador++;
			}
			if(var){
				res = (struct Bloco*) realloc(res,sizeof(struct Bloco)*(g+1));
				if(res == NULL){cout<<"Erro ao alocar memoria"<<endl;}
				strcpy(res[g].bloco, aux);
				res[g].ini_str_1 = i;
				res[g].fin_str_1 = f-1;
				res[g].ini_str_2 = maior_teste[0];
				res[g].fin_str_2 = maior_teste[1];
				res[g].tamanho = strlen(aux);
				g++;
				for(int l= i; l<=f-1; l++){str_1[l]='-';}
				for(int l= maior_teste[0]; l<=maior_teste[1]; l++){str_2[l]='+';}
			}
		}
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
			else{teste = false;}
		}
	}
	res[0].tamanho_struct = g-1;
	return res;
}

struct Bloco *TodosBlocos(char ax[], char bx[]){
	int maior_teste[2];
	int cont = 0;
	int v = 0;
	int t = strlen(ax);
	int x = 1;
	int a = t;
	int i = 0;
	int f = t;
	bool teste = true;
	struct Bloco *blco = (struct Bloco*)malloc(sizeof(struct Bloco));
	if(blco == NULL){cout<<"Erro ao alocar memoria"<<endl;}
	while(teste){
		int tam = f;
		char aux[tam];
		Gerador(aux,i,f-1,ax);
		if((strlen(aux)>1)&&(KMP(aux,strlen(aux),bx,strlen(bx),maior_teste)>=0)){
			blco[v].ini_str_1 = i;
			blco[v].fin_str_1 = f-1;
			blco[v].ini_str_2 = maior_teste[0];
			blco[v].fin_str_2 = maior_teste[1];
			blco[v].tamanho = strlen(aux);
			strcpy(blco[v].bloco,aux);
			x++;
			v++;
			blco = (struct Bloco*) realloc(blco,sizeof(struct Bloco)*x);
			if(blco == NULL){cout<<"Erro ao alocar memoria"<<endl;}
			cont++;
		}
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
			else {teste = false;}
		}
	}

	blco[0].tamanho_struct = cont;
	return blco; 
}

int main(){
	char str_1[4000];
	char str_2[4000];
	clock_t t;
	t = clock();
	FILE *arq;
	arq = fopen("arquivo.txt", "r");
	if(arq == NULL)cout<<"Erro ao Ler o Arquivo!!!"<<endl;
	fscanf(arq,"%s %s",str_1, str_2);
	
	char a[strlen(str_1)];
	char b[strlen(str_2)];
	
		
	struct Bloco *todosBlocos;
	todosBlocos = TodosBlocos(str_1,str_2);
	struct Bloco *gulosoEst;
	int resultado = 1000;
	float porcao = todosBlocos[0].tamanho_struct * 0.05;
	int maximo = (int)porcao;

	for(int i =0; i< maximo; i++){
		Copia(a,b,str_1,str_2,strlen(str_1));
		int candidato[5];
		candidato[0] =todosBlocos[i].ini_str_1;
		candidato[1] =todosBlocos[i].fin_str_1;
		candidato[2] =todosBlocos[i].ini_str_2;
		candidato[3] =todosBlocos[i].fin_str_2;
		candidato[4] =todosBlocos[i].tamanho;
		gulosoEst = Guloso_Estendido(todosBlocos[i].bloco,a,b,candidato);
		if(resultado>gulosoEst[0].tamanho_struct){resultado=gulosoEst[0].tamanho_struct;}
	}
	cout<<"Resultado: "<<resultado<<endl;
	t = clock() - t;
	printf("Tempo de execucao: %lf", ((double)t)/CLOCKS_PER_SEC);
	fclose(arq);
	return 0;
}












