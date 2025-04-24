#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> matrixFerramentas; // matriz de ferramentas
unsigned int m, n, c;                  // ferramentas, sequencia de tarefas e capacidade do magazine
// m = t
vector<int> carregadas;
int u = 0; // ferramentas no magazine
int prioridades;
int magazine;
int trocas = 0;

vector<int> vidaUtil; // vida util de cada ferramenta
vector<int> tempoProcessamento;
vector<int> original; // armazenar a vida util original para quando tiver que resetar a vida das ferramentas devido a sua reposicao

void KTNS(const vector<int> processos, bool debug) // sequencia de tarefas = n

    int main()
{
    cin >> m >> n >> c;
    vidaUtil.resize(m);
    tempoProcessamento.resize(n);
    matrixFerramentas.resize(m, vector<int>(n));
    carregadas.resize(m, 0);
    prioridades.resize(m, vector<int>(n));
    magazine.resize(m, vector<int>(n))

        for (int i = 0; i < m; ++i)
    {
        cin >> vidaUtil[i];
    }
    // armazenar tudo da vida util original em outro vetor->
    for (int i = 0; i < vidaUtil.size(); i++)
    {
        original[i] = vidaUtil[i];
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> matrixFerramentas[i][j];
        }
    }

    vector<int> processos(n);

    // inicializaçao com o primeiro projeto
    for (unsigned j = 0; j < t; j++)
    {
        carregadas[j] = matrixFerramentas[j][processos[0]];
        if (matrixFerramentas[j][processos[0]] == 1)
        {
            ++u; // quantas ferramentas foram carregadas
            // verificar qual a vida util dela aqui
            if (vidaUtil[j] <= tempoProcessamento[j])
            {
                trocas++;                  // somar 1 troca
                vidaUtil[j] = original[j]; // voltar com o valor original
            }
            else if (vidaUtil[j] > tempoProcessamento[j])
            {
                vidaUtil[j] = vidaUtil[j] - tempoProcessamento[j];
            }
        }
        for (unsigned i = 0; i < processos.size(); i++)
        {
            magazine[j][i] = matrixFerramentas[j][processos[i]];
            if (debug)
            {
                cout << magazine[j][i] << " ";
            }
        }
        if (debug)
        {
            cout << endl;
        }
    }

    // Preenche a matriz de prioridades
    for (unsigned i = 0; i < t; ++i)
    {
        for (unsigned j = 0; j < processos.size(); ++j)
        {
            if (magazine[i][j] == 1)
                prioridades[i][j] = 0;
            else
            {
                int proxima = 0;
                bool usa = false;
                for (unsigned k = j + 1; k < processos.size(); ++k)
                {
                    ++proxima;
                    if (magazine[i][k] == 1)
                    {
                        usa = true;
                        break;
                    }
                }
                if (usa)
                    prioridades[i][j] = proxima;
                else
                    prioridades[i][j] = -1;
            }
        }
    }
    if (debug)
    {

        for (unsigned j = 0; j < t; j++)
        {
            for (unsigned i = 0; i < processos.size(); i++)
            {
                cout << prioridades[j][i] << " ";
            }
            cout << endl;
        }

        cout << "Ferramentas carregadas: " << endl;
        for (unsigned j = 0; j < t; j++)
        {
            if (carregadas[j] == 33)
                exit(0);
            cout << carregadas[j] << endl;
        }
    }
    
    // Calcula as trocas
    if (debug)
    {
        cout << u << " carregadas na primeira tarefa" << endl;
    }
    trocas += c; // pra ja começar considerando as trocas iniciais
    for (unsigned i = 1; i < processos.size(); ++i)
    {
        for (unsigned j = 0; j < t; ++j)
        {
            if ((magazine[j][i] == 1) && (carregadas[j] == 0))
            {
                carregadas[j] = 1;
                ++u;
            }
        }
        if (debug)
        {
            cout << u << " Ferramentas carregadas" << endl;
        }
        while (u > c)
        {
            int maior = 0;
            int pMaior = -1;
            for (unsigned j = 0; j < t; ++j)
            {
                if (magazine[j][i] != 1)
                { // Ferramenta não utilizada pelo processo atual
                    if ((carregadas[j] == 1) && (prioridades[j][i] == -1))
                    { // Essa ferramenta não será mais utilizada e é um excelente candidato a remoção
                        pMaior = j;
                        break;
                    }
                    else
                    {
                        if ((prioridades[j][i] > maior) && carregadas[j] == 1)
                        {
                            maior = prioridades[j][i];
                            pMaior = j;
                        }
                    }
                }
            }
            carregadas[pMaior] = 0;
            if (debug)
            {
                cout << "Retirou " << i << ":" << pMaior << endl;
            }
            --u;
            ++trocas;
            if (debug)
            {
                cout << trocas << " trocas " << endl;
            }
        }
        if (debug)
        {

            cout << "Ferramentas carregadas: " << endl;
            for (unsigned j = 0; j < t; j++)
            {
                cout << carregadas[j] << endl;
            }
        }
    }
    if (debug)
    {
        cout << ": " << trocas << "trocas" << endl;
    }
    return trocas;
}
double completionTime(std::vector<unsigned> tProcessamento, std::vector<int> &tarefas)
{
    extern unsigned tempoTroca;
    double tPr = 0;
    for (std::vector<int>::const_iterator i = tarefas.begin(); i != tarefas.end(); ++i)
        tPr += tProcessamento[*i];
    long nTrocas = KTNS(tarefas);
    double tTrocas = nTrocas * tempoTroca;
    return (tPr + tTrocas);
}
}