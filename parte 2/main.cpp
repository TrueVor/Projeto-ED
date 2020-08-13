#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>

using namespace std;

//estrtura para armazenar o vetor de índices na página da B+
struct indice {
    unsigned tam;
    unsigned indice;
};

class Pagina {
    friend class BPlus;
    private:
    indice idx[80]; 
    Pagina* pont_tree[81]; //ponteiros usados para páginas que não são folhas
    int pont_seq[81]; //ponteiros usados nas folhas, para acesso ao sequence set
    bool ehfolha; //indica se a página é folha
    unsigned elementos; //guarda a quantidade de elementos válidos no vetor de indice
    public:
    Pagina();
};

Pagina::Pagina() {
    for (int i = 0; i < 80; i++) {
        idx[i] = {};
    }
    for (int i = 0; i < 81; i++) {
        pont_seq[i] = {};
        pont_tree[i] = NULL;
    }
    ehfolha = false;
    elementos = 0;
}

class BPlus {
    private:
    Pagina* raiz;
    public:
    BPlus();
    void Inserir (unsigned t, unsigned in);
    Pagina* Buscar (unsigned c1, unsigned c2); //retorna o endereço da página
    void Alterar (unsigned t, unsigned in);
};

BPlus::BPlus() {
    raiz = NULL;
}

//percorre a arvore e retorna a pagina folha em que o elemento deveria estar
Pagina* BPlus::Buscar(unsigned c1, unsigned c2) {
    Pagina* percorre = raiz;
    if (percorre -> ehfolha) 
        return percorre;
    
    while (percorre -> ehfolha == false ) {
        for (int i = 0; i < percorre -> elementos; i++ ) {
            if (c1 < percorre -> idx[i].tam) {
                percorre = percorre -> pont_tree[i]; //passa para pagina filha
            }

            else if (c1 == percorre -> idx[i].tam) { //caso a chave primaria for igual, compara com a chave secundaria
                if (c2 < percorre -> idx[i].indice) {
                    percorre = percorre -> pont_tree[i];
                }
            }
        }
        if (percorre -> ehfolha == false) { //caso ele seja maior que a ultima posição
            percorre = percorre -> pont_tree [percorre->elementos]; //ponteiro do ultimo elemento
        }
    }
    return percorre;
}

void BPlus::Inserir(unsigned t, unsigned in){

    if(raiz == NULL){
        raiz->idx[0].indice = in;
        raiz->idx[0].tam = t;
        raiz->ehfolha = true;
        raiz->elementos = 1;
    } else {
        Pagina* precursor = raiz;
        Pagina* parent;
        bool achou = false;

        // No loop abaixo, o precursor irá para a página folha onde possivelmente será inserido o dado.
        while(precursor->ehfolha == false){
            parent = precursor;
            for(int i = 0; i < precursor->elementos && achou == false; i++){
                if(i == precursor->elementos - 1){
                    precursor->pont_tree[i+1];
                    achou = true;
                } else if(t < precursor->idx[i].tam){
                    precursor = precursor->pont_tree[i];
                    achou = true;
                } else if(t == precursor->idx[i].tam && i < precursor->idx[i].indice){
                    precursor = precursor->pont_tree[i];
                    achou = true;
                }
            }
        }

        // Inserindo as chaves no precursos que é folha
        if(precursor->elementos < 80){

            // Encontrando a posição
            int i = 0;
            while((t > precursor->idx[i].tam || (t == precursor->idx[i].tam && in > precursor->idx[i].indice)) && i < precursor->elementos){
                i++;
            }
            
            // Liberando espaço
            for(int j = precursor->elementos; j > i; j--){
                precursor->idx[j] = precursor->idx[j-1];
            }

            // Adicionando as chaves no espaço
            precursor->idx[i].tam = t;
            precursor->idx[i].indice = in;
            precursor->elementos++;
            precursor->pont_tree[precursor->elementos] = precursor->pont_tree[precursor->elementos-1];
            precursor->pont_tree[precursor->elementos-1] = NULL;
        } else { // Caso a Página estiver cheia, iniciar a divisão da mesma.
            
            // Criando nova página
            Pagina* novaPagina = new Pagina;
            // criando uma página virtual contendo as chaves da página e a chave a ser inserida
            indice PaginaVirtual[81];
            for(int i = 0; i < 80; i++){
                PaginaVirtual[i] = precursor->idx[i];
            }
            int i = 0, j;
            while((t > PaginaVirtual[i].tam || (t == PaginaVirtual[i].tam && in > PaginaVirtual[i].indice)) && i < 80){
                i++;
            }

            // Abrindo espaço para as chaves a serem inseridas
            for(int j = 81; j > i; j--){
                PaginaVirtual[j] = PaginaVirtual[j-1];
            }
            PaginaVirtual[i].tam = t;
            PaginaVirtual[i].indice = in;
            novaPagina->ehfolha = true;
            // Dividindo o precursor em duas páginas folha
            precursor->elementos = 40;
            novaPagina->elementos = 41;
            // Fazendo precursor apontar para a nova Página folha
            precursor->pont_tree[precursor->elementos] = novaPagina;
            // Fazendo a Nova Página apontar para a próxima página
            novaPagina->pont_tree[novaPagina->elementos] = precursor->pont_tree[81];
            precursor->pont_tree[81] = NULL;
            // Adicionando elementos na Nova Página
            for(i = 0; i < precursor->elementos; i++){
                precursor->idx[i] = PaginaVirtual[i];
            }
            for(i = 0, j = precursor->elementos; i < novaPagina->elementos; i++, j++){
                novaPagina->idx[i] = PaginaVirtual[j];
            }

            if(precursor == raiz){
                // Se o precursor for raiz, criar nova raiz
                Pagina* novaRaiz = new Pagina;
                novaRaiz->idx[0] = novaPagina->idx[0];
                novaRaiz->pont_tree[0] = precursor;
                novaRaiz->pont_tree[1] = novaPagina;
                novaRaiz->ehfolha = false;
                novaRaiz->elementos = 1;
                raiz = novaRaiz;
            } else {
                
            }

        }

    }
}

int main(){};

