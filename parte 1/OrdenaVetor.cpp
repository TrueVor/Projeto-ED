#include <iostream>
#include "classes.h"
#include "OrdenaVetor.h"

using namespace std;


void swap(pacote& A, pacote& B) {
	//variaveis auxiliares para troca.
	unsigned indice = A.indice;; 
    float tempo = A.tempo; 
    char origem[40]; 
    char destino[40]; 
    char protocolo[18];
    unsigned tamanho = A.tamanho; 
    char infomarcao[1650];
    
    //Passando elementos de B pra A
    A.indice=B.indice;
    A.tempo=B.tempo;
    A.tamanho=B.tamanho; 
    for (int i=0; i<40;i++) {
		origem[i]=A.origem[i];
		destino[i]=A.destino[i];
		A.origem[i]=B.origem[i];
		A.destino[i]=B.destino[i];
	}
	for (int i=0;i<18;i++) {
		protocolo[i]=A.protocolo[i];
		A.protocolo[i] = B.protocolo[i];
	}
	for (int i=0;i<1650;i++) {
		infomarcao[i] = A.infomarcao[i];
		A.infomarcao[i]=B.infomarcao[i];
	}
	//Passando elementos de A pra B
	B.indice = indice;
	B.tempo = tempo;
	B.tamanho = tamanho;
	for (int i=0; i<40;i++) {
		B.origem[i] = origem[i];
		B.destino[i] = destino[i];
	}
	for (int i=0;i<18;i++){
		B.protocolo[i] = protocolo[i];
	}
	for (int i=0;i<1650;i++) {
		B.infomarcao[i] = infomarcao[i];
	}
}

void selectionSort(pacote arr[], int n){  
    int i, j, min_idx;  
  
     
    for (i = 0; i < n-1; i++){
		//acha o menor
        min_idx = i;
        for (j = i+1; j < n; j++)
        	if (arr[j].tamanho < arr[min_idx].tamanho)  
           		min_idx = j;  
  
        //troca o menor de pos
        swap(arr[min_idx], arr[i]);  
    }  
}

void Imprimir (pacote A[], int tam) { //imprime só o tempo e tamanho para teste
	for (int i=0;i<tam;i++) {
		cout << A[i].indice << " "<< A[i].tamanho << endl;
	}
}

/*
int main() { //TESTE
	pacote p1[5];
	string aux; //para digitar os vetores de char
	for (int i =0;i<5;i++) {
		cin>>p1[i].indice; 
		cin>>p1[i].tempo;
		cin >> aux;
		for (unsigned j=0;j<aux.size();j++) { 
			aux[i]=p1[i].origem[j]; 
		}
		cin >> aux;
		for (unsigned j=0;j<aux.size();j++) {
			aux[i]=p1[i].protocolo[j];
		}
		cin >> p1[i].protocolo;
		cin >> p1[i].tamanho;
		cin >> aux;
		for (unsigned j=0;j<aux.size();j++) { 
			aux[i]=p1[i].infomarcao[j];
		}
	}
	selectionSort(p1,5);
	Imprimir(p1,5);
}*/