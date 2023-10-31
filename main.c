#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define NUM_MAQUINAS 4
#define MAX_TAREFAS 100

typedef struct Tarefa {
    int id;
    int tempo;
    int completada;
} Tarefa;

void alocarTarefa(Tarefa tarefas[], int indiceTarefa, int maquina, int tarefasAlocadas[][MAX_TAREFAS], int tarefasPorMaquina[]) {
    tarefas[indiceTarefa].completada = 1;
    tarefasAlocadas[maquina - 1][tarefasPorMaquina[maquina - 1]++] = indiceTarefa + 1;
    printf("Maquina %d: %d (tarefas atendidas)\n", maquina, indiceTarefa + 1);
}

int main() {
    int numTarefas, i, j;
    Tarefa tarefas[MAX_TAREFAS];

    clock_t inicioTempo = clock();

    // Leitura dos dados de entrada
    FILE *arquivoEntrada = fopen("I2.IN2", "r");
    if (arquivoEntrada == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        exit(1);
    }

    fscanf(arquivoEntrada, "%d", &numTarefas);
    for (i = 0; i < numTarefas; i++) {
        fscanf(arquivoEntrada, "%d", &tarefas[i].tempo);
        tarefas[i].id = i + 1;
        tarefas[i].completada = 0;
    }

    int matrizDependencias[MAX_TAREFAS][MAX_TAREFAS] = {0};

    // Inicialização das dependências
    for (i = 0; i < numTarefas; i++) {
        for (j = 0; j < numTarefas; j++) {
            matrizDependencias[i][j] = 0;
        }
    }

    // Leitura das dependências
    while (1) {
        int tarefa1, tarefa2;
        fscanf(arquivoEntrada, "%d,%d", &tarefa1, &tarefa2);
        if (tarefa1 == -1 || tarefa2 == -1) {
            break;
        }
        matrizDependencias[tarefa2 - 1][tarefa1 - 1] = 1;
    }
    fclose(arquivoEntrada);

    int tarefasCompletadas = 0;
    int maquina = 1; // Inicie com a Máquina 1

    int tarefasAlocadas[NUM_MAQUINAS][MAX_TAREFAS];
    int tarefasPorMaquina[NUM_MAQUINAS] = {0};

    int temposMaquina[NUM_MAQUINAS] = {0}; // Inicialize os tempos de cada máquina com zero

    while (tarefasCompletadas < numTarefas) {
        for (i = 0; i < numTarefas; i++) {
            if (!tarefas[i].completada) {
                int pronta = 1;
                for (j = 0; j < numTarefas; j++) {
                    if (matrizDependencias[i][j] && !tarefas[j].completada) {
                        pronta = 0;
                        break;
                    }
                }
                if (pronta) {
                    // Aloque a próxima tarefa na máquina atual e atualize os tempos
                    alocarTarefa(tarefas, i, maquina, tarefasAlocadas, tarefasPorMaquina);
                    temposMaquina[maquina - 1] += tarefas[i].tempo;
                    tarefasCompletadas++;
                }
            }
        }
        maquina = (maquina % NUM_MAQUINAS) + 1; // Próxima máquina
    }
    printf("----------------------------------------\n");
    // Impressão da alocação de tarefas em cada máquina
    for (i = 0; i < NUM_MAQUINAS; i++) {
        printf("Maquina %d: ", i + 1);
        for (j = 0; j < tarefasPorMaquina[i]; j++) {
            printf("%d", tarefasAlocadas[i][j]);
            if (j < tarefasPorMaquina[i] - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    // Encontre a máquina com o maior tempo total
    int maxTempoMaquina = 0;
    for (i = 0; i < NUM_MAQUINAS; i++) {
        if (temposMaquina[i] > maxTempoMaquina) {
            maxTempoMaquina = temposMaquina[i];
        }
    }

    // Impressão da Função Objetivo (FO)
    printf("FO: %d\n", maxTempoMaquina);

    clock_t fimTempo = clock();
    double execucaoTempo = (double)(fimTempo - inicioTempo) / CLOCKS_PER_SEC;

    printf("Tempo de execucao, em segundos: %.5f\n", execucaoTempo);
    printf("------------------------------------\n");

    return 0;
}
