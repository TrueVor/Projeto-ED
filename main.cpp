#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>

using namespace std;

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
    Bloco* auxiliar;
    public:
    SeqSet();
    Bloco BuscarBloco(pacote& _p); 
    void Inserir(pacote& _p);  
    bool BuscarPacote(pacote& _p);
    void AlterarPacote(pacote& _p);

    void ImprimirSS();
   
   
};

Bloco::Bloco() {
    cabBloco.proximo = -1;
    cabBloco.quantidade = 0;
    idBloco = 0;
}

SeqSet::SeqSet() {
    ofstream arq("trab.dat");  //cria o arquivo
    //inicializando o cabeçalho
    cabSS.num = 0;
    cabSS.posPrimeiro = 0;
    cabSS.posProximo = 0; 
    arq.write((char *) &cabSS, sizeof(Cabecalho)); //insere os dados do cabeçalho
    arq.close();
}

Bloco SeqSet::BuscarBloco(pacote& _p) {
    Bloco aux;
    bool achou = false;
    ifstream arq;
    if (arq) {
        arq.read((char*) &cabSS, sizeof(Cabecalho));
        if (cabSS.num == 0) {
             throw runtime_error("Erro na busca: arquivo vazio!");
        }
        //nesse ponto o ponteiro de leitura já vai estar no final do cabeçalho 
        else {
            arq.read((char*) &aux, sizeof(Bloco)); //lê o primeiro Bloco
            unsigned tam = aux.cabBloco.quantidade -1; //pos do ultimo elemento valido do vetor
            if (_p.tamanho < aux.dados[tam].tamanho) { //se o elemento pertencer ao primeiro bloco
                return aux; 
            }
            else {
                unsigned PosAbs;
                while ((achou == (false)) && (aux.cabBloco.proximo != -1)) {
                    PosAbs = (sizeof(Bloco)*aux.cabBloco.proximo) + sizeof(Cabecalho);
                    arq.seekg(PosAbs); //arruma o ponteiro de leitura
                    arq.read((char*) &aux, sizeof(Bloco)); //passa o bloco do arquivo pra memória
                    tam = aux.cabBloco.quantidade -1;
                    if (_p.tamanho < aux.dados[tam].tamanho) {
                        achou = true;
                        return aux;
                    }
                }
                // O if abaixo garante que o Bloco final seja enviado sem que o idBloco seja editado para -1
                // evitando escrever na memoria o valor idBloco do ultimo bloco como -1
                if(aux.cabBloco.proximo == -1 && achou == false)
                    return aux;
            }
        }
    }
    aux.idBloco = -1; // manda um bloco inválido
    return aux;
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

void SeqSet::Inserir(pacote& _p) {
    Bloco aux;
    Bloco aux2; // Utilizado como segundo auxiliar na hora de dividir blocos cheios
    unsigned limBloco = 80; // Limite de qntdade de dados para cada bloco
    fstream arq;
    if (arq) {
        arq.read((char*) &cabSS, sizeof(Cabecalho)); //lê o cabeçalho
        if(cabSS.num == 0) {
            cabSS.num = 1;
            cabSS.posPrimeiro = 0;
            cabSS.posProximo = 1;
            arq.seekp(0);
            arq.write((char*) &cabSS, sizeof(Cabecalho));
            //inserindo primeiro dado e modificando cabeçalho do bloco
            aux.dados[0] = _p;
            aux.cabBloco.quantidade = 1;
            aux.cabBloco.proximo = -1;
            arq.seekp(sizeof(Cabecalho)); // Corrige o ponteiro para a posição relativa 0
            arq.write((char*) &aux, sizeof(Bloco));
        } else {
            aux = BuscarBloco(_p);

            arq.seekg((sizeof(Bloco)*aux.idBloco) + sizeof(Cabecalho)); // arruma o ponteiro de leitura
            arq.read((char*) &aux, sizeof(Bloco)); //passa o bloco do arquivo pra memória
            if(aux.cabBloco.quantidade == limBloco){ // Se o bloco estiver cheio, dividir
                for(unsigned i = 0; i < (limBloco/2); i++){
                    aux2.dados[0+i] = aux.dados[(limBloco/2)+i]; // Move o dado de aux para aux2
                    aux.dados[(limBloco/2)+i] = {}; // Limpa o dado em aux
                }
                // Atualiza os devidos cabeçalhos
                aux2.idBloco = cabSS.posProximo;
                aux2.cabBloco.quantidade = limBloco/2;
                aux2.cabBloco.proximo = aux.cabBloco.proximo;
                cabSS.num += 1;
                cabSS.posProximo = cabSS.num;
                aux.cabBloco.quantidade = limBloco/2;
                aux.cabBloco.proximo = cabSS.posProximo;
                // Confere qual dos dois blocos inserir o valor _p
                if(_p.tamanho <= aux.dados[(limBloco/2)-1].tamanho){
                    aux.dados[limBloco/2] = _p;
                    aux.cabBloco.quantidade += 1;
                    selectionSort(aux.dados, aux.cabBloco.quantidade);
                } else { // Insere no pacote criado
                    aux2.dados[limBloco/2] = _p;
                    aux2.cabBloco.quantidade += 1;
                    selectionSort(aux2.dados, aux2.cabBloco.quantidade);
                }
                // Inserir na memória o Cabeçalho , Bloco aux e Bloco aux2
                arq.seekp(0);
                arq.write((char*) &cabSS, sizeof(Cabecalho));

                arq.seekp((sizeof(Bloco)*aux.idBloco) + sizeof(Cabecalho));
                arq.write((char*) &aux, sizeof(Bloco));

                arq.seekp((sizeof(Bloco)*aux2.idBloco) + sizeof(Cabecalho));
                arq.write((char*) &aux2, sizeof(Bloco));
                
            } else { // Do contrário, apenas inserir o dado no bloco
                aux.dados[aux.cabBloco.quantidade] = _p;
                aux.cabBloco.quantidade += 1;
                selectionSort(aux.dados, aux.cabBloco.quantidade);
                arq.seekp((sizeof(Bloco)*aux.idBloco) + sizeof(Cabecalho)); // Posição Absoluta do Bloco aux
                arq.write((char*) &aux, sizeof(Bloco));
            }
        }
        arq.close();
        
    }
}

bool SeqSet::BuscarPacote(pacote& _p) {
    Bloco aux;
    aux = BuscarBloco (_p);
    if (aux.idBloco == -1) {
        return false;
    }
    else {
        int tam = aux.cabBloco.quantidade;
        for (int i = 0; i < tam -1; i++) {
            if (aux.dados[i].indice == _p.indice && aux.dados[i].tamanho == _p.tamanho) {
                return true;
            }
        }
    }
    return false;
}


void SeqSet::AlterarPacote(pacote& _p) {
    if (BuscarPacote(_p)) {
        Bloco aux = BuscarBloco(_p); //traz pra memória o bloco em que o dado está armazenado 
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
        ofstream arq; //abre o arquivo para escrita
        if(arq) {
            unsigned posAbs = (sizeof(Bloco)*aux.cabBloco.proximo) + sizeof(Cabecalho);
            arq.seekp(posAbs);
            arq.write((char*) &aux, sizeof(Bloco)); //escreve o bloco altrado no arquivo
            arq.close();
            cout << "Alteração feita com sucesso!" << endl;
        }
        else {
            cout << "Erro na alteração!" << endl;
        }
    }
}
 
void SeqSet::ImprimirSS() {
    ifstream arq;
    Bloco percorre;
    string source, dest, info, prot; //para imprimir os vetores de char
    arq.read((char*) &cabSS,sizeof(Cabecalho));
    cout << "Numero de Blocos: " << cabSS.num << "   Pos do primeiro: " 
    << cabSS.posPrimeiro << "   Pos do proximo disponível: " <<  cabSS.posProximo << endl;
    //lendo o primeiro bloco
    arq.seekg(cabSS.posPrimeiro*sizeof(Bloco)); 
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
        cout << percorre.dados[i].tamanho;
        info = percorre.dados[i].infomarcao;
        cout << info << endl;
    }
    for (int i = 1; i < cabSS.num; i++) {
        arq.seekg(percorre.cabBloco.proximo*sizeof(Bloco)+sizeof(Cabecalho));
        arq.read((char*) &percorre, sizeof(Bloco));
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
            cout << percorre.dados[i].tamanho;
            info = percorre.dados[i].infomarcao;
            cout << info << endl;
        }
    }
    arq.close();
}



// para saber o tamanho de cada campo, fiz um primeiro teste para
// pegar o tamanho máximo de cada campo do tipo texto
// adicionei mais uma posição para armazenar o término de string '\0'
/*struct pacote {
    unsigned indice; 
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char infomarcao[1650];
};*/

int main(){
    SeqSet Seq;
    std::ifstream arquivo_csv("captura_pacotes.csv");
    if (!arquivo_csv) {
        return EXIT_FAILURE;
    }

    string linha;
    int starting_line = 2076361;  //pos da primeira linha a ser lida
    for (int i = 1; i < starting_line; i += 1) {
        if(!getline(arquivo_csv,linha)) {  
            return EXIT_FAILURE;
        }
    }


    string campo; // variável para obter um campo de cada linha lida
    string delimitador = "\",\""; // delimitador entre os campos
    unsigned posFimCampo; // posição final do campo

    pacote umPacote;

    while(getline(arquivo_csv,linha) && umPacote.indice <  2162875) {
        campo = linha.erase(0,1); // remove primeiro caracter da linha (")

        // obtendo primeiro campo, um inteiro - No.
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.indice = stoul(campo);

        // obtendo segundo campo, um float - Time
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tempo = stof(campo);

        // obtendo terceiro campo, um texto - Source
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.origem, campo.c_str());

        // obtendo quarto campo, um texto - Destination
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.destino, campo.c_str());

        // obtendo quinto campo, um texto - Protocol
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.protocolo, campo.c_str());

        // obtendo sexto campo, um texto - Length
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tamanho = stoul(campo);

        // obtendo último campo, um texto - Info
        // nesse caso, procuro o último aspas, fazendo busca reversa
        posFimCampo = linha.rfind("\"");
        campo = linha.substr(0, posFimCampo);
        strcpy(umPacote.infomarcao, campo.c_str());


        
    }

    return 0;
}
