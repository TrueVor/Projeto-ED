#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>

using namespace std;

/* ================================ CÓDIGO DO SEQUENCE SET  ================================*/

const char NOMEARQUIVO[20] = "trab.dat";

// Pacote de dados
struct pacote  {
    unsigned indice; 
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char infomarcao[1650];
};

//CABEÇALHO DO SEQUENCE SET
struct Cabecalho { 
    unsigned posPrimeiro; //armazena a posição do primeiro bloco.
    unsigned posProximo; //armazena a posição relativa do proximo bloco a ser criado
    int num; //armarzena a quantidade de blocos
};

//CABEÇALHO DE CADA BLOCO
struct InfoBloco {
    unsigned quantidade; //quantidade de elementos validos no bloco;
    int proximo ; //posição relativa do proximo bloco no arquivo
};
    

class Bloco { 
    friend class SeqSet;
    friend class BPlus;
    private:
    int idBloco; // Posição Relativa do Bloco
    InfoBloco cabBloco; // Cabeçalho do Bloco
    pacote dados[80]; // Pacote de dados de tamanho máximo de 80 e mínimo de 40
    public:
    Bloco();

};

class SeqSet {
    private:
    Cabecalho cabSS;
    public:
    SeqSet();
	
    int BuscarBloco(pacote& _p); //retorna o id do bloco encontrado 
    int Inserir(pacote& _p);
    bool BuscarPacote(pacote& _p);
    pacote EncontraPacote(unsigned tamanho, unsigned indice);
    void AlterarPacote(pacote& _p);

    void ImprimirSS();
};

Bloco::Bloco() {
    cabBloco.proximo = -1;
    cabBloco.quantidade = 0;
    idBloco = 0;
}

SeqSet::SeqSet() {
    ofstream arq(NOMEARQUIVO);  //cria o arquivo
    //inicializando o cabeçalho
    cabSS.num = 0;
    cabSS.posPrimeiro = 0;
    cabSS.posProximo = 0; 
    arq.write((char *) &cabSS, sizeof(Cabecalho)); //insere os dados do cabeçalho
    arq.close();
}

int SeqSet::BuscarBloco(pacote& _p) {
    Bloco aux;
    ifstream arq(NOMEARQUIVO, ios::binary);
    int tam; //armazena pos do ultimo elemento de um bloco
    if (arq) {
        arq.read((char*) &cabSS, sizeof(Cabecalho)); //lê o cabeçalho
        if (cabSS.num > 0) {
            arq.read((char*) &aux, sizeof(Bloco)); //lê o primeiro bloco
            while (aux.cabBloco.proximo != -1) { 
                tam = (aux.cabBloco.quantidade) - 1;
                if (_p.tamanho < aux.dados[tam].tamanho) { //achou o bloco
                    arq.close();
                    return aux.idBloco;
                }
                else if (_p.tamanho > aux.dados[tam].tamanho) { //compara chave primária com ultimo elemento do bloco
                    int posAbs = (sizeof(Cabecalho)+(sizeof(Bloco) * aux.cabBloco.proximo));
                    arq.seekg(posAbs);
                    arq.read((char*) &aux, sizeof(Bloco)); //lê o proximo bloco
                }
                else if (_p.tamanho == aux.dados[tam].tamanho) { //caso seja igual, fazer comparações com a chave secundária
                    if (_p.indice < aux.dados[tam].indice) {
                        arq.close();
                        return aux.idBloco;
                    }
                    else {
                        int posAbs = (sizeof(Cabecalho)+(sizeof(Bloco) * aux.cabBloco.proximo));
                        arq.seekg(posAbs);
                        arq.read((char*) &aux, sizeof(Bloco));
                    }
                }
            }
        }
    }
    arq.close();
    return aux.idBloco; //retorna o id do ultimo bloco, caso nenhum outro seja encontrado
}

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

void selectionSort(pacote arr[], unsigned n){  
    unsigned i, j, min_idx;  
  
     
    for (i = 0; i < n-1; i++){
		//acha o menor
        min_idx = i;
        for (j = i+1; j < n; j++)
        	if (arr[j].tamanho < arr[min_idx].tamanho)  
           		min_idx = j;
            else if (arr[j].tamanho == arr[min_idx].tamanho && arr[j].indice < arr[min_idx].indice)
                min_idx = j;
  
        //troca o menor de pos
        swap(arr[min_idx], arr[i]);  
    }  
}

// Retorna a Posição Relativa do Bloco que fora inserido os dados
int SeqSet::Inserir(pacote& _p) {
    int Bl = 0; // Variavel para armazenar a posição relativa do bloco que fora inserido os dados
    Bloco aux;
    Bloco aux2; // Utilizado como segundo auxiliar na hora de dividir blocos cheios
    unsigned limBloco = 80; // Limite de qntdade de dados para cada bloco
    unsigned long long PosAbs;
    fstream arq(NOMEARQUIVO, ios::binary | ios::in | ios::out);
    if (arq) {
        
        arq.read((char*) &cabSS, sizeof(Cabecalho)); //lê o cabeçalho
        if(cabSS.num == 0) {
            cabSS.num = 1;
            cabSS.posPrimeiro = 0;
            cabSS.posProximo = 1;
            PosAbs = 0;
            arq.seekp(PosAbs);
            arq.write((char*) &cabSS, sizeof(Cabecalho));
            //inserindo primeiro dado e modificando cabeçalho do bloco
            aux.dados[0] = _p;
            aux.cabBloco.quantidade = 1;
            aux.cabBloco.proximo = -1;
            PosAbs = sizeof(Cabecalho);

            arq.seekp(PosAbs);
            arq.write((char *) &aux, sizeof(Bloco));
        } else {
            
            int pos = BuscarBloco(_p);

            PosAbs = (sizeof(Bloco)*pos) + sizeof(Cabecalho);
            arq.seekp(PosAbs);
            arq.read((char*) &aux, sizeof(Bloco));

            if(aux.cabBloco.quantidade == limBloco){ // Se o bloco estiver cheio, dividir
                for(unsigned i = 0; i < (limBloco/2); i++){
                    aux2.dados[0+i] = aux.dados[(limBloco/2)+i]; // Move o dado de aux para aux2
                    aux.dados[(limBloco/2)+i] = {}; // Limpa o dado em aux
                }
                // Atualiza os devidos cabeçalhos
                aux2.idBloco = cabSS.posProximo;
                aux2.cabBloco.quantidade = limBloco/2;
                aux2.cabBloco.proximo = aux.cabBloco.proximo;
                
                aux.cabBloco.quantidade = limBloco/2;
                aux.cabBloco.proximo = cabSS.posProximo;

                cabSS.num += 1;
                cabSS.posProximo = cabSS.num;
                // Confere qual dos dois blocos inserir o valor _p
                if(_p.tamanho < aux.dados[(limBloco/2)-1].tamanho){
                    aux.dados[limBloco/2] = _p;
                    aux.cabBloco.quantidade += 1;
                    selectionSort(aux.dados, aux.cabBloco.quantidade);
                    Bl = aux.idBloco;
                } else { // Insere no pacote criado
                    aux2.dados[limBloco/2] = _p;
                    aux2.cabBloco.quantidade += 1;
                    selectionSort(aux2.dados, aux2.cabBloco.quantidade);
                    Bl = aux2.idBloco;
                }
                // Inserir na memória o Cabeçalho , Bloco aux e Bloco aux2
                PosAbs = 0;
                arq.seekp(PosAbs);
                arq.write((char *) &cabSS, sizeof(Cabecalho));

                PosAbs = (sizeof(Bloco)*aux.idBloco) + sizeof(Cabecalho);
                arq.seekp(PosAbs);
                arq.write((char *) &aux, sizeof(Bloco));

                PosAbs = (sizeof(Bloco)*aux2.idBloco) + sizeof(Cabecalho);
                arq.seekp(PosAbs);
                arq.write((char *) &aux2, sizeof(Bloco));
                
            } else { // Do contrário, apenas inserir o dado no bloco
                
                aux.dados[aux.cabBloco.quantidade] = _p;
                
                aux.cabBloco.quantidade += 1;
                selectionSort(aux.dados, aux.cabBloco.quantidade);
                PosAbs = (sizeof(Bloco)*pos) + sizeof(Cabecalho);
                arq.clear();
                arq.seekp(PosAbs); // Posição Absoluta do Bloco aux
                arq.write((char *) &aux, sizeof(Bloco));
                Bl = pos;
            }
        }
        
        arq.close();
    }
    return Bl; // Retorna a posição relativa do bloco que fora inserido os dados
}

pacote SeqSet::EncontraPacote(unsigned tamanho, unsigned indice) {
    Bloco aux;
    pacote _p;
    _p.indice = indice;
    _p.tamanho = tamanho;
    int posBloco = BuscarBloco(_p);
    ifstream arq(NOMEARQUIVO, ios::binary);
    //lendo dados do arquivo
    arq.read((char*) &cabSS, sizeof(Cabecalho));
    int posAbs = (sizeof(Cabecalho) + (sizeof(Bloco) * posBloco));
    arq.seekg(posAbs);
    arq.read((char*) &aux, sizeof(Bloco));
    arq.close();
    //busca sequencial no vetor
    int tam = aux.cabBloco.quantidade - 1;
    for (int i = 0; i < tam; i++) {
        if ((aux.dados[i].tamanho == tamanho) && (aux.dados[i].indice == indice))
            return aux.dados[i];
    }
    _p = {};
    return _p;
}

bool SeqSet::BuscarPacote(pacote& _p) {
    Bloco aux;
    int posBloco = BuscarBloco(_p);
    ifstream arq(NOMEARQUIVO, ios::binary);
    //lendo dados do arquivo
    arq.read((char*) &cabSS, sizeof(Cabecalho));
    int posAbs = (sizeof(Cabecalho) + (sizeof(Bloco) * posBloco));
    arq.seekg(posAbs);
    arq.read((char*) &aux, sizeof(Bloco));
    arq.close();
    //busca sequencial no vetor
    int tam = aux.cabBloco.quantidade - 1;
    for (int i = 0; i < tam; i++) {
        if ((aux.dados[i].tamanho == _p.tamanho) && (aux.dados[i].indice == _p.indice))
            return true;
    }
    return false;
}


void SeqSet::AlterarPacote(pacote& _p) {
    if (!BuscarPacote(_p)) {
        cerr << "Pacote não encontrado!";
    }
    else {
        Bloco aux;
        int pos = BuscarBloco(_p);
        ifstream arq(NOMEARQUIVO, ios::binary);
        
        //lendo dados do arquivo
        arq.read((char*) &cabSS, sizeof(Cabecalho));
        int posAbs = (sizeof(Cabecalho) + (sizeof(Bloco) * pos));
        arq.seekg(posAbs);
        arq.read((char*) &aux, sizeof(Bloco));
        arq.close();
        
        float tempo;
        string altera;
        bool achou = false;
        int tam = aux.cabBloco.quantidade-1;

        for (int i = 0; i < tam && achou == false; i++ ) {
            if (_p.tamanho == aux.dados[i].tamanho && _p.indice == aux.dados[i].indice) {
                cout << "Digite os dados para alteração:" << endl;
                cout << "tempo: ";
                cin >> tempo;
                aux.dados[i].tempo = tempo;
                cout << "origem: ";
                cin >> altera;
                strcpy(aux.dados[i].origem, altera.c_str()); //copia a string como um vetor de char
                cout << "destino: ";
                cin >> altera;
                strcpy(aux.dados[i].destino, altera.c_str());
                cout << "protocolo: ";
                cin >> altera;
                strcpy(aux.dados[i].protocolo, altera.c_str());
                cout << "info: ";
                cin >> altera;
                strcpy(aux.dados[i].infomarcao, altera.c_str());
                achou = true;
            }
        }
        ofstream arqOUT(NOMEARQUIVO); //abre o arquivo para escrita
        if(arq) {
            posAbs = (sizeof(Bloco)*pos) + sizeof(Cabecalho);
            arqOUT.seekp(posAbs);
            arqOUT.write((char*) &aux, sizeof(Bloco)); //escreve o bloco alterado no arquivo
            arqOUT.close();
            cout << "Alteração feita com sucesso!" << endl;
        }
        else {
            cout << "Erro na alteração!" << endl;
        }
    }
}

 
void SeqSet::ImprimirSS() {
    ifstream arq(NOMEARQUIVO, ios::binary);
    Bloco percorre;
    string source, dest, info, prot; //para imprimir os vetores de char
    unsigned long long PosAbs;
    arq.read((char*) &cabSS,sizeof(Cabecalho));
    cout << "Numero de Blocos: " << cabSS.num << "   Pos do primeiro: " 
    << cabSS.posPrimeiro << "   Pos do proximo disponível: " <<  cabSS.posProximo << endl;
    //lendo o primeiro bloco
    PosAbs = sizeof(Cabecalho) + cabSS.posPrimeiro*sizeof(Bloco);
    arq.seekg(PosAbs);
    arq.read((char*) &percorre,sizeof(Bloco));
    //imprimindo os dados 
    cout << "BLOCO " << percorre.idBloco << ":" << endl;
    for (unsigned i = 0; i < percorre.cabBloco.quantidade; i++) {
        cout << percorre.dados[i].indice << " ";
        cout << percorre.dados[i].tempo << " ";
        source = percorre.dados[i].origem;
        cout << source << " ";
        dest =  percorre.dados[i].destino;
        cout << dest << " ";
        prot =  percorre.dados[i].protocolo;
        cout << prot << " ";
        cout << percorre.dados[i].tamanho << " ";
        info = percorre.dados[i].infomarcao;
        cout << info << endl;
    }
    for (int i = 1; i < cabSS.num; i++) {
        PosAbs = percorre.cabBloco.proximo*sizeof(Bloco)+sizeof(Cabecalho);
        arq.seekg(PosAbs);
        arq.read((char*) &percorre, sizeof(Bloco));
        cout << "BLOCO " << percorre.idBloco << ":" << endl;

        for (unsigned i = 1; i < percorre.cabBloco.quantidade; i++) {
            cout << percorre.dados[i].indice << " ";
            cout << percorre.dados[i].tempo << " ";
            source = percorre.dados[i].origem;
            cout << source << " ";
            dest =  percorre.dados[i].destino;
            cout << dest << " ";
            prot =  percorre.dados[i].protocolo;
            cout << prot << " ";
            cout << percorre.dados[i].tamanho;
            info = percorre.dados[i].infomarcao;
            cout << info << endl;
        }
    }
    arq.close();
}

/* ================================ CÓDIGO B+  ================================*/

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
    int pont_seq; //ponteiro usado nas folhas, para acesso ao sequence set
    bool ehfolha; //indica se a página é folha
    unsigned elementos; //guarda a quantidade de elementos váldos no vetor de indice
    public:
    Pagina();
};

Pagina::Pagina() {
    for (int i = 0; i < 80; i++) {
        idx[i] = {};
    }
    for (int i = 0; i < 81; i++) {
        pont_tree[i] = NULL;
    }
    pont_seq = 0;
    ehfolha = false;
    elementos = 0;
}

class BPlus {
    private:
    Pagina* raiz;
    // Funções auxiliares para Busca e acesso ao arquivo 
    Pagina* Buscar (unsigned c1, unsigned c2); //retorna uma paina folha
    int AcessarArquivo(Pagina* folha, unsigned c1, unsigned c2); //retorna id do bloco caso o elemento exista 
	
    public:
    BPlus();
    void Inserir (unsigned t, unsigned i, int bloco);
    void InserirInterno(unsigned t, unsigned i, Pagina* cursor, Pagina* filho);
    Pagina* EncontrarParente(Pagina* cursor, Pagina* filho);
    int PegarId (unsigned c1, unsigned c2); // Encontra folha, acessa o arquivo e retorna o id do bloco
    void Alterar (unsigned t, unsigned i);
};

BPlus::BPlus() {
    raiz = NULL;
}

int BPlus::PegarId (unsigned c1, unsigned c2) {
	Pagina* aux = Buscar(c1,c2);
	int id = AcessarArquivo(aux, c1, c2);
	return id;
}

//percorre a arvore e retorna a pagina folha em que o elemento deveria estar
Pagina* BPlus::Buscar(unsigned c1, unsigned c2) {
    Pagina* percorre = raiz;
    bool achou;
    if (percorre == NULL)
        return percorre;
    if (percorre -> ehfolha == true) 
        return percorre;
    
    while (percorre -> ehfolha == false ) {
        achou = false;
        for (unsigned i = 0; i < percorre -> elementos; i++ ) {
            if (c1 < percorre -> idx[i].tam) {
                percorre = percorre -> pont_tree[i]; //passa para pagina filha
                achou = true;
            }

            else if (c1 == percorre -> idx[i].tam) { //caso a chave primaria for igual, compara com a chave secundaria
                if (c2 < percorre -> idx[i].indice) {
                    percorre = percorre -> pont_tree[i];
                    achou = true;
                }
            }
        }
        if (percorre -> ehfolha == false && achou == false) { //caso c1 seja maior que a ultima posição
            percorre = percorre -> pont_tree [percorre->elementos]; //ponteiro do ultimo elemento
        }
    }
    return percorre; 
}

//recebe uma página folha e acessa o sequence set através dela
int BPlus::AcessarArquivo(Pagina* folha, unsigned c1, unsigned c2) {
    Bloco aux; //variavel para passar o bloco do arquivo para memória;
    if (folha != NULL && folha->ehfolha == true) {
        //achando pos do elemento desejado
        bool achou = false;
        unsigned pos = 0;
        while (achou != false && pos < folha->elementos) {
            if (folha->idx[pos].tam == c1 && folha->idx[pos].indice == c2) {
                achou = true;
            } else {
                pos++;
			}
        }
        if (achou) {
            ifstream arq(NOMEARQUIVO, ios::binary);
            int pos_relativa = folha->pont_seq; //posição do elemento encontrado no bloco
            
            //lendo dados do arquivo
            long long int posAbs = (sizeof(Cabecalho) + (sizeof(Bloco) * pos_relativa));
            arq.seekg(posAbs);
            arq.read((char*) &aux, sizeof(Bloco));
            arq.close();
            //nesse trecho o bloco está armazenado em aux
            for (int i = 0; i < aux.cabBloco.quantidade; i++) {
                if (aux.dados[i].tamanho == c1 && aux.dados[i].indice == c2 ) {
                    return aux.idBloco; //o elemento existe no arquivo
                }
			}
        }
        else { 
            cerr << "elemento não existe no arquivo!" << endl;
            return -1;
		}
    }
    else {
        cerr << "erro na busca!" << endl;
        return -1;
	}
}

// Inserir chaves (Tamanho e Índice) e como terceiro argumento Posição Relativa do Bloco no Sequence Set
void BPlus::Inserir(unsigned t, unsigned i, int bloco){

    if(raiz == NULL){
        raiz->idx[0].indice = i;
        raiz->idx[0].tam = t;
        raiz->ehfolha = true;
        raiz->elementos = 1;
    } else {
        Pagina* precursor = Buscar(t, i);

        // Inserindo as chaves no precursor que é folha
        if(precursor->elementos < 80){

            // Encontrando a posição
            int k = 0;
            while((t > precursor->idx[k].tam || (t == precursor->idx[k].tam && i > precursor->idx[k].indice)) && k < precursor->elementos){
                k++;
            }
            
            // Liberando espaço
            for(int j = precursor->elementos; j > k; j--){
                precursor->idx[j] = precursor->idx[j-1];
            }

            // Adicionando as chaves no espaço
            precursor->idx[k].tam = t;
            precursor->idx[k].indice = i;
            precursor->elementos++;
            precursor->pont_tree[precursor->elementos] = precursor->pont_tree[precursor->elementos-1];
            precursor->pont_tree[precursor->elementos-1] = NULL;
            precursor->pont_seq = bloco;
        } else { // Caso a Página estiver cheia, iniciar a divisão da mesma.
            
            // Criando nova página
            Pagina* novaPagina = new Pagina;
            // criando uma página virtual contendo as chaves da página e a chave a ser inserida
            indice PaginaVirtual[81];
            for(int k = 0; k < 80; k++){
                PaginaVirtual[k] = precursor->idx[k];
            }
            int k = 0, j;
            while((t > PaginaVirtual[k].tam || (t == PaginaVirtual[k].tam && i > PaginaVirtual[k].indice)) && k < 80){
                k++;
            }

            // Abrindo espaço para as chaves a serem inseridas
            for(int j = 81; j > k; j--){
                PaginaVirtual[j] = PaginaVirtual[j-1];
            }
            PaginaVirtual[k].tam = t;
            PaginaVirtual[k].indice = i;
            novaPagina->ehfolha = true;
            // Dividindo o precursor em duas páginas folha
            precursor->elementos = 40;
            novaPagina->elementos = 41;
            precursor->pont_seq = bloco;
            novaPagina->pont_seq = bloco;
            // Fazendo precursor apontar para a nova Página folha
            precursor->pont_tree[precursor->elementos] = novaPagina;
            // Fazendo a Nova Página apontar para a próxima página
            novaPagina->pont_tree[novaPagina->elementos] = precursor->pont_tree[81];
            precursor->pont_tree[81] = NULL;
            // Adicionando elementos na Nova Página
            for(k = 0; k < precursor->elementos; k++){
                precursor->idx[k] = PaginaVirtual[k];
            }
            for(i = 0, j = precursor->elementos; k < novaPagina->elementos; k++, j++){
                novaPagina->idx[k] = PaginaVirtual[j];
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
                precursor = raiz;
                Pagina* parente;
                bool achou = false;

                // No loop abaixo, o precursor irá para a página folha enquanto o parente seria a pagina anterior
                while(precursor->ehfolha == false){
                    parente = precursor;
                    for(int k = 0; k < precursor->elementos && achou == false; k++){
                        if(k == precursor->elementos - 1){
                            precursor->pont_tree[k+1];
                            achou = true;
                        } else if(t < precursor->idx[k].tam){
                            precursor = precursor->pont_tree[k];
                            achou = true;
                        } else if(t == precursor->idx[k].tam && i < precursor->idx[k].indice){
                            precursor = precursor->pont_tree[k];
                            achou = true;
                        }
                    }
                }
                InserirInterno(t, i, parente, novaPagina);
            }
        }
    }
}

void BPlus::InserirInterno(unsigned t, unsigned i, Pagina* cursor, Pagina* filho){
    if(cursor == NULL){
        cerr << "erro na inserção!" << endl;
    } else {
        if(cursor->elementos < 80){
            //Se o cursor não estiver cheio
            //Encontrar a posição correta para a nova chave
            int k = 0;
            while((t > cursor->idx[k].tam || (t == cursor->idx[k].tam && i > cursor->idx[k].indice)) && k < cursor->elementos)
                k++;
            // Abrindo espaço para nova chave
            for(int j = cursor->elementos; j > k; j--){
                cursor->idx[j] = cursor->idx[j-1];
            }
            //Abrindo espaço para o novo ponteiro
            for(int j = cursor->elementos+1; j > k+1; j--){
                cursor->pont_tree[j] = cursor->pont_tree[j-1];
            }
            cursor->idx[k].tam = t;
            cursor->idx[k].indice = i;
            cursor->elementos++;
            cursor->pont_tree[k+1] = filho;
        } else {
            //Se o cursor estiver cheio
            //Iniciar a divisão
            Pagina* novoInterno = new Pagina;
            //Criando Pagina Interna Virtual
            indice ChaveVirtual[81];
            Pagina* PonteiroVirtual[82];
            for(int k = 0; k < 80; k++){
                ChaveVirtual[k] = cursor->idx[k];
            }
            for(int k = 0; k < 81; k++){
                PonteiroVirtual[k] = cursor->pont_tree[k];
            }
            int k = 0, j;
            while( (t > ChaveVirtual[k].tam || (t == ChaveVirtual[k].tam && i > ChaveVirtual[k].indice)) && k < 80)
                k++;
            //Abrindo espaço para as novas chaves
            for(j = 81; j > k; j--){
                ChaveVirtual[j] = ChaveVirtual[j-1];
            }
            ChaveVirtual[k].indice = i;
            ChaveVirtual[k].tam = t;

            //Abrindo espaço para o novo ponteiro
            for(j = 81; j > k; j--){
                PonteiroVirtual[j] = PonteiroVirtual[j-1];
            }
            PonteiroVirtual[k+1] = filho;

            novoInterno->ehfolha = false;
            //Dividindo o cursor em duas páginas
            cursor->elementos = 40;
            novoInterno->elementos = 41;
            // Adicionando elementos e ponteiros na Nova Página
            for(k = 0, j = cursor->elementos+1; k < novoInterno->elementos; k++, j++){
                novoInterno->idx[k] = ChaveVirtual[j];
            }
            for(k = 0, j = cursor->elementos+1; k < novoInterno->elementos+1; k++, j++){
                novoInterno->pont_tree[k] = PonteiroVirtual[j];
            }
            if(cursor == raiz){ // Se o cursor for Página Raiz, criamos uma nova raiz
                Pagina* novaRaiz = new Pagina;
                novaRaiz->idx[0] = novoInterno->idx[0];
                novaRaiz->pont_tree[0] = cursor;
                novaRaiz->pont_tree[1] = novoInterno;
                novaRaiz->ehfolha = false;
                novaRaiz->elementos = 1;
                raiz = novaRaiz;
            } else {
                // Recursão
                InserirInterno(novoInterno->idx[0].tam, novoInterno->idx[0].indice, EncontrarParente(raiz, cursor), novoInterno);
            }
        }
    }
}

// Encontra o Pai
Pagina* BPlus::EncontrarParente(Pagina* cursor, Pagina* filho){
    Pagina* parente;
    // Ignora páginas folha, pois folha não pode ser pai
    if(cursor->ehfolha || (cursor->pont_tree[0])->ehfolha){
        return NULL;
    }
    for(int i = 0; i < cursor->elementos+1; i++){
        if(cursor->pont_tree[i] == filho){
            parente = cursor;
            return parente;
        } else {
            parente = EncontrarParente(cursor->pont_tree[i], filho);
            if(parente != NULL){
                return parente;
            }
        }
    }
    return parente;
}

int main(){} 
