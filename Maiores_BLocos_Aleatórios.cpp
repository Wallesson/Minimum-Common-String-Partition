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

struct Bloco *Guloso_Estendido(char bloco[], char str_1[], char str_2[], int candidato[5],int tv[1]){
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
	tv[0]=g-1;
	return res;
}

int Shift_Direita(char bloco[], char a_[], char b_[], int candidato[5]){
	int maior_teste[2];
	int aux_fin = candidato[1];
	int ini = candidato[0];
	int fin = candidato[1];
	int max = strlen(a_);
	char a_x[strlen(a_)];
	char b_x[strlen(b_)];
	int tam_str = strlen(a_);
	int tv[1];
	Copia(a_x,b_x,a_,b_,tam_str);
	struct Bloco *resultado_Dir;
	resultado_Dir = Guloso_Estendido(bloco, a_x, b_x, candidato,tv);
	bool cond = true;
	while(cond){
		char aux[]="";
		Gerador(aux, ini, fin,a_);
		if(KMP(aux,strlen(aux),b_,strlen(b_),maior_teste)>=0 && strlen(aux)>2){
			candidato[0] = ini;
			candidato[1] = fin;
			candidato[2] = maior_teste[0];
			candidato[3] = maior_teste[1];
			candidato[4] = strlen(aux);
			Copia(a_x,b_x,a_,b_,tam_str);
			struct Bloco *teste;
			teste = Guloso_Estendido(aux, a_x, b_x, candidato,tv);
			if(resultado_Dir[0].tamanho_struct > teste[0].tamanho_struct){resultado_Dir = teste;}
			free(teste);
			ini++;
			fin++;
			if(ini>aux_fin || fin == max){cond = false;}
		}else {
			ini++;
			fin++;
			if(ini>aux_fin || fin == max){cond = false;}	
		}
	}
	int res = resultado_Dir[0].tamanho_struct;
	free(resultado_Dir);
	return res;
}

int Shift_Esquerda(char bloco[], char a_[], char b_[], int candidato[5]){
	int tv[1];
	int maior_teste[2];
	int ini = candidato[0];
	int fin = candidato[1];
	int aux_i = ini;
	char a_x[strlen(a_)];
	char b_x[strlen(b_)];
	int tam_str = strlen(a_);
	Copia(a_x,b_x,a_,b_,tam_str);
	struct Bloco *resultado_Esq;
	resultado_Esq = Guloso_Estendido(bloco, a_x, b_x, candidato,tv);
	bool cond = true;
	while(cond){
		char aux[]="";
		Gerador(aux, ini, fin,a_);
		if(KMP(aux,strlen(aux),b_,strlen(b_),maior_teste)>=0 && strlen(aux)>2){
			candidato[0] = ini;
			candidato[1] = fin;
			candidato[2] = maior_teste[0];
			candidato[3] = maior_teste[1];
			candidato[4] = strlen(aux);
			Copia(a_x,b_x,a_,b_,tam_str);
			struct Bloco *teste;
			teste = Guloso_Estendido(aux, a_x, b_x, candidato,tv);
			teste[0].tamanho_struct = tv[0];
			if(resultado_Esq[0].tamanho_struct > teste[0].tamanho_struct){resultado_Esq = teste;}
			free(teste);
			ini--;
			fin--;
			if(ini<0 || fin < aux_i){cond=false;}
		}else {
			ini--;
			fin--;
			if(ini<0 || fin < aux_i){cond = false;}	
		}
	}
	int res = resultado_Esq[0].tamanho_struct;
	free(resultado_Esq);
	return res;
}

float Shift(struct Bloco *lista,char a_[], char b_[]){
	float porcao = lista[0].tamanho_struct * 0.05;
	int maximo = (int)porcao;
	int cand[maximo];
	for(int h = 0; h<maximo; h++){cand[h] = h;}
    for (int h = 0; h < maximo; h++){
		srand((unsigned)time(NULL));
		int r = rand() % maximo;
		int temp = cand[h];
		cand[h] = cand[r];
		cand[r] = temp;
	}
	char a_x[strlen(a_)];
	char b_x[strlen(b_)];
	int c =0;
	float resposta = 10;
	for (int i=0; i<4;i++){
		c++;
		int tv[1];
		int candidato[5];
		candidato[0] =lista[i].ini_str_1;
		candidato[1] =lista[i].fin_str_1;
		candidato[2] =lista[i].ini_str_2;
		candidato[3] =lista[i].fin_str_2;
		candidato[4] =lista[i].tamanho;
		Copia(a_x,b_x,a_,b_,strlen(a_));
		struct Bloco *gulosoEst;
		gulosoEst = Guloso_Estendido(lista[i].bloco,a_x,b_x,candidato, tv);
		Copia(a_x,b_x,a_,b_,strlen(a_));
		int shift_dir;
		shift_dir = Shift_Direita(lista[i].bloco, a_x, b_x, candidato);
		Copia(a_x,b_x,a_,b_,strlen(a_));
		int shift_esq;
		shift_esq = Shift_Esquerda(lista[i].bloco, a_x, b_x, candidato);
		if(shift_esq < shift_dir && shift_dir<gulosoEst[0].tamanho_struct){resposta +=shift_esq;}
		else if(shift_dir < shift_esq && shift_esq<gulosoEst[0].tamanho_struct){resposta +=shift_dir;}
		else{resposta +=gulosoEst[0].tamanho_struct;}	
	}
	return resposta/c;
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
	return blco;
}

int main (){
	clock_t t;
	char str_1[4000];
	char str_2[4000];
	FILE *arq;
	t = clock();
	arq = fopen("arquivo.txt", "r");
	if(arq == NULL)cout<<"Erro ao Ler o Arquivo!!!"<<endl;
	fscanf(arq,"%s %s",str_1, str_2);
	char a_x[strlen(str_1)];
	char b_x[strlen(str_2)];
	Copia(a_x,b_x,str_1,str_2,strlen(str_1));
	struct Bloco *todosBlocos;
	todosBlocos = TodosBlocos(a_x,b_x);
	Copia(a_x,b_x,str_1,str_2,strlen(str_1));
	float shift;
	shift = Shift(todosBlocos,a_x,b_x);
	printf("MBA: %4.2f\n", shift);
	t = clock() - t;
	printf("Tempo de execucao: %lf", ((double)t)/CLOCKS_PER_SEC);
	fclose(arq);
	return 0;
}















