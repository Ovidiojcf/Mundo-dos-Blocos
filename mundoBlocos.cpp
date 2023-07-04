#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_set> //Biblioteca para o uso de conjuntos não ordenados. 

using namespace std;

// Estrutura que representa um estado do Mundo dos Blocos
struct Estado {
    vector<string> pilhas; // Pilhas de blocos
    string acao; // Ação que levou a este estado (movimento realizado)
    Estado* pai; // Estado pai (estado anterior)

    Estado(const vector<string>& p, const string& ac, Estado* pa) : pilhas(p), acao(ac), pai(pa) {}
};

// Função para imprimir uma representação do estado atual
void imprimirEstado(const Estado* estado) {
    cout << "Estado Atual: " << endl;
    for (const string& pilha : estado->pilhas) {
        cout << "Pilha: ";
        for (char bloco : pilha) {
            cout << bloco << " ";
        }
        cout << endl;
    }
    cout << "Acao: " << estado->acao << endl;
    cout << "---------------------------" << endl;
}

// Função para verificar se um estado é o estado final desejado
bool ehEstadoFinal(const Estado* estado, const vector<string>& estadoFinal) {
    return estado->pilhas == estadoFinal;
}

// Função para verificar se um estado já foi visitado anteriormente
bool estadoVisitado(const Estado* estado, unordered_set<string>& visitados) {
    string estadoString;
    for (const string& pilha : estado->pilhas) {
        estadoString += pilha + "|";
    }
    return visitados.count(estadoString) > 0;
}

// Função para executar o algoritmo de Busca em Largura
void buscaEmLargura(const vector<string>& estadoInicial, const vector<string>& estadoFinal) {
    queue<Estado*> fila; // Fila para armazenar os estados a serem visitados
    unordered_set<string> visitados; // Conjunto para armazenar os estados já visitados

    Estado* estadoAtual = new Estado(estadoInicial, "", nullptr); // Estado inicial
    fila.push(estadoAtual);
    visitados.insert(estadoInicial[0] + estadoInicial[1] + estadoInicial[2]);

    while (!fila.empty()) {
        estadoAtual = fila.front();
        fila.pop();

        if (ehEstadoFinal(estadoAtual, estadoFinal)) {
            // Encontrou o estado final
            break;
        }

        // Gerar novos estados possíveis
        for (size_t i = 0; i < estadoAtual->pilhas.size(); ++i) {
            for (size_t j = 0; j < estadoAtual->pilhas.size(); ++j) {
                if (i != j && !estadoAtual->pilhas[i].empty()) {
                    // Movendo o bloco do topo da pilha i para a pilha j
                    vector<string> novaConfig = estadoAtual->pilhas;
                    char bloco = novaConfig[i].back();
                    novaConfig[i].pop_back();
                    novaConfig[j] += bloco;

                    Estado* novoEstado = new Estado(novaConfig, "Mover " + string(1, bloco) + " de " + to_string(i) + " para " + to_string(j), estadoAtual);

                    if (!estadoVisitado(novoEstado, visitados)) {
                        fila.push(novoEstado);
                        visitados.insert(novaConfig[0] + novaConfig[1] + novaConfig[2]);
                    }
                }
            }
        }
    }

    // Reconstruir o caminho da solução
    vector<const Estado*> caminho;
    while (estadoAtual != nullptr) {
        caminho.push_back(estadoAtual);
        estadoAtual = estadoAtual->pai;
    }

    // Exibir o caminho da solução
    for (int i = static_cast<int>(caminho.size()) - 1; i >= 0; --i) {
        imprimirEstado(caminho[i]);
    }

    // Liberar memória dos estados
    for (const Estado* estado : caminho) {
        delete estado;
    }
}

int main() {
    // Definir o estado inicial e o estado final
    vector<string> estadoInicial = {"", "ABC", ""};
    vector<string> estadoFinal = {"", "ABC", ""};

    // Mostrar o espaço de estados, as operações possíveis e a simulação do problema resolvido
    cout << "Espaco de Estados: " << endl;
    cout << "Estado Inicial: " << endl;
    imprimirEstado(new Estado(estadoInicial, "", nullptr));
    cout << "Estado Final: " << endl;
    imprimirEstado(new Estado(estadoFinal, "", nullptr));

    cout << "Operacoes Possiveis: Mover A de i para j (onde A e o bloco a ser movido, i e a pilha de origem e j e a pilha de destino)" << endl;

    cout << "Simulacao da Resolucao do Problema: " << endl;
    buscaEmLargura(estadoInicial, estadoFinal);

    return 0;
}
