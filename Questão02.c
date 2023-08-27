#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float* matrix_create( unsigned int size );
void matrix_destroy( float* matrix );
float* matrix_multiply_normal( float* matrix1, float* matrix2, unsigned int size );
float* matrix_multiply( float* matrix1, float* matrix2, unsigned int size );
float* matrix_transpose( float* matrix, unsigned int size );
void test_performance( float* A, float* B, unsigned int size );

int main(void) {
    unsigned int size = 5000;

    float* A = matrix_create(size);
    float* B = matrix_create(size);

    test_performance(A, B, size);

    matrix_destroy(A);
    matrix_destroy(B);

    return 0;
}

float* matrix_create(unsigned int size) {
    float* matrix = (float*)malloc(size * size * sizeof(float));
    if (matrix == NULL) {
        printf("Não foi possível alocar memória para a matriz\n");
        return NULL;
    }

    srand(time(NULL));

    for (unsigned int i = 0; i < size * size; i++) {
        matrix[i] = (float)rand() / RAND_MAX;
    }

    return matrix;
}

void matrix_destroy(float* matrix) {
    if (matrix != NULL) {
        free(matrix);
    }
    printf("Desalocando o espaço da matriz\n");
}

float* matrix_multiply_normal(float* matrix1, float* matrix2, unsigned int size) {
    float* result = (float*)malloc(size * size * sizeof(float));
    if (result == NULL) {
        printf("Não foi possível alocar memória para a matriz resultante\n");
        return NULL;
    }

    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < size; j++) {
            float result = 0.0;
            for (unsigned int k = 0; k < size; k++) {
                result += matrix1[i * size + k] * matrix2[k * size + j];
            }
            result[i * size + j] = result;
        }
    }

    return result;
}

float* matrix_multiply(float* matrix1, float* matrix2, unsigned int size) {
    float* result = (float*)malloc(size * size * sizeof(float));
    if (result == NULL) {
        printf("Não foi possível alocar memória para a matriz resultante\n");
        return NULL;
    }

    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < size; j++) {
            float result = 0.0;
            for (unsigned int k = 0; k < size; k++) {
                result += matrix1[i * size + k] * matrix2[k * size + j];
            }
            result[i * size + j] = result;
        }
    }

    return result;
}

float* matrix_transpose(float* matrix, unsigned int size) {
    float* transposed = (float*)malloc(size * size * sizeof(float));
    if (transposed == NULL) {
        printf("Não foi possível alocar memória para a matriz transposta\n");
        return NULL;
    }

    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < size; j++) {
            transposed[j * size + i] = matrix[i * size + j];
        }
    }

    return transposed;
}

void test_performance(float* A, float* B, unsigned int size) {
    clock_t start, end;
    double cpu_time_used;

    printf("Teste de Desempenho:\n");

    // Multiplicação de matrizes esparsas
    start = clock();
    float* C_sparse = matrix_multiply(A, B, size);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Multiplicação de Matrizes Esparsas: %.2f segundos\n", cpu_time_used);
    matrix_destroy(C_sparse);

    // Multiplicação de matrizes normais
    start = clock();
    float* C_normal = matrix_multiply_normal(A, B, size);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Multiplicação de Matrizes Normais: %.2f segundos\n", cpu_time_used);
    matrix_destroy(C_normal);

    // Transposição de matriz
    start = clock();
    float* A_transposed = matrix_transpose(A, size);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Transposição de Matriz: %.2f segundos\n", cpu_time_used);
    matrix_destroy(A_transposed);
}
