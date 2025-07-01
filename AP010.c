#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define MAX_MEMORY_SMALL 1000000
#define MAX_MEMORY_LARGE 10000000
#define MAX_TAPE_FILES 3
#define MAX_LINE_LENGTH 100

typedef struct {
    long int value;
    int tape_index;
} HeapNode;

// Função de comparação para o heap
int compare_heap_nodes(const void* a, const void* b) {
    const HeapNode* na = (const HeapNode*)a;
    const HeapNode* nb = (const HeapNode*)b;
    return (na->value > nb->value) - (na->value < nb->value);
}

// Função para criar blocos ordenados
void create_sorted_blocks(FILE* input, int memory_size) {
    long int* buffer = (long int*)malloc(memory_size * sizeof(long int));
    FILE* tape_files[MAX_TAPE_FILES];
    char filename[20];
    int current_tape = 0;
    int elements_read;
    
    // Inicializar fitas
    for (int i = 0; i < MAX_TAPE_FILES; i++) {
        sprintf(filename, "tape%d.txt", i);
        tape_files[i] = fopen(filename, "w+");
        if (!tape_files[i]) {
            perror("Erro ao criar arquivo de fita");
            exit(EXIT_FAILURE);
        }
    }

    // Fase de distribuição
    while (!feof(input)) {
        elements_read = 0;
        
        // Ler blocos do arquivo de entrada
        while (elements_read < memory_size && fscanf(input, "%ld", &buffer[elements_read]) == 1) {
            elements_read++;
        }

        // Ordenar o bloco na memória
        qsort(buffer, elements_read, sizeof(long int), compare_heap_nodes);

        // Escrever no arquivo temporário atual
        for (int i = 0; i < elements_read; i++) {
            fprintf(tape_files[current_tape], "%ld\n", buffer[i]);
        }

        // Alternar entre as fitas
        current_tape = (current_tape + 1) % MAX_TAPE_FILES;
    }

    // Fechar arquivos e liberar memória
    for (int i = 0; i < MAX_TAPE_FILES; i++) {
        rewind(tape_files[i]);
        fclose(tape_files[i]);
    }
    free(buffer);
}

// Função para intercalar os blocos
void merge_blocks(FILE* output, int memory_size) {
    FILE* tape_files[MAX_TAPE_FILES];
    char filename[20];
    HeapNode* heap = (HeapNode*)malloc(MAX_TAPE_FILES * sizeof(HeapNode));
    int heap_size = 0;
    int active_tapes = MAX_TAPE_FILES;
    long int value;
    
    // Abrir fitas para leitura
    for (int i = 0; i < MAX_TAPE_FILES; i++) {
        sprintf(filename, "tape%d.txt", i);
        tape_files[i] = fopen(filename, "r");
        if (!tape_files[i] || fscanf(tape_files[i], "%ld", &value) != 1) {
            active_tapes--;
        } else {
            heap[heap_size].value = value;
            heap[heap_size].tape_index = i;
            heap_size++;
        }
    }

    // Construir heap mínimo
    for (int i = heap_size / 2 - 1; i >= 0; i--) {
        // Heapify down
        int parent = i;
        while (parent < heap_size) {
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;
            int smallest = parent;
            
            if (left < heap_size && heap[left].value < heap[smallest].value)
                smallest = left;
            if (right < heap_size && heap[right].value < heap[smallest].value)
                smallest = right;
                
            if (smallest != parent) {
                HeapNode temp = heap[parent];
                heap[parent] = heap[smallest];
                heap[smallest] = temp;
                parent = smallest;
            } else {
                break;
            }
        }
    }

    // Fase de intercalação
    while (heap_size > 0) {
        // Extrair o menor elemento
        HeapNode min_node = heap[0];
        fprintf(output, "%ld\n", min_node.value);
        
        // Ler próximo elemento da mesma fita
        if (fscanf(tape_files[min_node.tape_index], "%ld", &value) == 1) {
            heap[0].value = value;
        } else {
            // Fita esgotada
            heap[0] = heap[heap_size - 1];
            heap_size--;
            active_tapes--;
        }
        
        // Manter propriedade do heap
        int parent = 0;
        while (parent < heap_size) {
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;
            int smallest = parent;
            
            if (left < heap_size && heap[left].value < heap[smallest].value)
                smallest = left;
            if (right < heap_size && heap[right].value < heap[smallest].value)
                smallest = right;
                
            if (smallest != parent) {
                HeapNode temp = heap[parent];
                heap[parent] = heap[smallest];
                heap[smallest] = temp;
                parent = smallest;
            } else {
                break;
            }
        }
    }

    // Fechar arquivos e liberar memória
    for (int i = 0; i < MAX_TAPE_FILES; i++) {
        fclose(tape_files[i]);
        sprintf(filename, "tape%d.txt", i);
        remove(filename);
    }
    free(heap);
}

// Função principal de ordenação externa
void external_sort(const char* input_file, const char* output_file, int memory_size) {
    clock_t start_time = clock();
    FILE* input = fopen(input_file, "r");
    FILE* output = fopen(output_file, "w");
    
    if (!input || !output) {
        perror("Erro ao abrir arquivos");
        exit(EXIT_FAILURE);
    }
    
    printf("Iniciando ordenacao externa...\n");
    printf("Tamanho da memoria interna: %d elementos\n", memory_size);
    
    // Fase 1: Criar blocos ordenados
    clock_t phase1_start = clock();
    create_sorted_blocks(input, memory_size);
    double phase1_time = (double)(clock() - phase1_start) / CLOCKS_PER_SEC;
    printf("Fase de distribuicao concluida em %.2f segundos\n", phase1_time);
    
    // Fase 2: Intercalar blocos
    clock_t phase2_start = clock();
    merge_blocks(output, memory_size);
    double phase2_time = (double)(clock() - phase2_start) / CLOCKS_PER_SEC;
    printf("Fase de intercalacao concluida em %.2f segundos\n", phase2_time);
    
    // Resultados finais
    double total_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("Ordenacao concluida em %.2f segundos\n", total_time);
    
    fclose(input);
    fclose(output);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida> [tamanho_memoria]\n", argv[0]);
        printf("Tamanho de memoria padrao: %d elementos\n", MAX_MEMORY_SMALL);
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* output_file = argv[2];
    int memory_size = (argc > 3) ? atoi(argv[3]) : MAX_MEMORY_SMALL;
    
    // Garantir que o tamanho da memória seja válido
    if (memory_size <= 0) {
        printf("Tamanho de memoria invalido. Usando valor padrao.\n");
        memory_size = MAX_MEMORY_SMALL;
    }
    
    external_sort(input_file, output_file, memory_size);
    
    return 0;
}
