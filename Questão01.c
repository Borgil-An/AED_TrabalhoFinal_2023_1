#include <stdio.h>
#include <stdlib.h>

struct matrix {
	struct matrix* right;
	struct matrix* below;
	int line;
	int column;
	float info;
};

typedef struct matrix Matrix;

// Funções pedidas
Matrix* matrix_create( void );
void matrix_destroy( Matrix* m );
void matrix_print( Matrix* m );
Matrix* matrix_add( Matrix* m, Matrix* n );
Matrix* matrix_multiply( Matrix* m, Matrix* n );
Matrix* matrix_transpose( Matrix* m );
float matrix_getelem( Matrix* m, int x, int y );
void matrix_setelem( Matrix* m, int x, int y, float elem );

// Função extras para auxiliar
Matrix* matrix_inicializator ( int l, int c ); // Cria head's

int main( void ) {
/* Inicializacao da aplicacao ... */
Matrix *A = matrix_create();
matrix_print( A );

Matrix *B = matrix_create(); 
matrix_print( B );

printf ("\n");
Matrix *C = matrix_add( A, B ); 
matrix_print( C );
matrix_destroy( C );

printf ("\n");
C = matrix_multiply( A, B ); 
matrix_print( C );
matrix_destroy( C );

printf ("\n");
C = matrix_transpose( A ); 
matrix_print( C );

printf ("\n");
matrix_destroy( C );
matrix_destroy( A );
matrix_destroy( B );
return 0;
}

Matrix *matrix_create(void) {
    int l, c, lt, ct;
    float value;
    
    printf("Digite o número de linhas e colunas da matriz:\n");
    if (scanf("%d %d", &l, &c) != 2) {
        printf("Erro ao ler entrada.\n");
        return NULL;
    }

    Matrix *newMatrix = matrix_inicializator(l, c);

    printf("Digite as triplas (linha, coluna, valor) para os elementos diferentes de zero da matriz (%d x %d):\n", l, c);

    while (1) {
        if (scanf("%d %d %f", &lt, &ct, &value) == 3) {
            if (lt == 0 && ct == 0 && value == 0) {
                printf("Parando a leitura da matriz\n");
                break;
            }

            if (lt <= 0 || lt > l || ct <= 0 || ct > c) {
                printf("Posição fora dos limites da matriz.\n");
            } 
          else {
                matrix_setelem(newMatrix, lt, ct, value);
          }
        } else {
            printf("Erro ao ler entrada.\n");
            break;
        }
    }
   return newMatrix;
}

void matrix_destroy(Matrix* m) {
    Matrix* currentLine = m->below;
    while (currentLine != m) {
        Matrix* current = currentLine->right;
        while (current != currentLine) {
            Matrix* temp = current;
            current = current->right;
            free(temp);
        }
        Matrix* tempLine = currentLine;
        currentLine = currentLine->below;
        free(tempLine);
    }

    printf ("\nDesalocando memória alocado para a matriz\n");
    free(m);
}


void matrix_print(Matrix* m) {
    Matrix* currentLine = m->below;

    while (currentLine != m) {
        Matrix* current = currentLine->right;
        while (current != NULL && current != currentLine) {
            if (current->info != 0) {
                printf("Linha: %d, Coluna: %d, Valor: %.2f\n", current->line, current->column, current->info);
            }
            current = current->right;
        }
        currentLine = currentLine->below;
    }
}




Matrix* matrix_add(Matrix* m, Matrix* n) {
    if (m == NULL || n == NULL) {
        printf("Uma das matrizes não existe.\n");
        return NULL;
    }
    Matrix* resultMatrix = matrix_inicializator(1, 1); 
    for (int x = 1; x <= m->line; x++) {
        for (int y = 1; y <= m->column; y++) {
            float addedNumber = matrix_getelem(m, x, y) + matrix_getelem(n, x, y);
            matrix_setelem(resultMatrix, x, y, addedNumber);
        }
    }
    return resultMatrix;
}

Matrix* matrix_multiply(Matrix* m, Matrix* n) {
    if (m == NULL || n == NULL) {
        printf("Uma das matrizes não existe.\n");
        return NULL;
    }

    if (m->column != n->line) {
        printf("Número de colunas da primeira matriz deve ser igual ao número de linhas da segunda matriz para multiplicação.\n");
        return NULL;
    }

    Matrix* resultMatrix = matrix_inicializator(m->line, n->column);

    for (int x = 1; x <= resultMatrix->line; x++) {
        for (int y = 1; y <= resultMatrix->column; y++) {
            float multipliedNumber = 0.0;
            for (int k = 1; k <= m->column; k++) {
                multipliedNumber += matrix_getelem(m, x, k) * matrix_getelem(n, k, y);
            }
            matrix_setelem(resultMatrix, x, y, sum);
        }
    }

    return resultMatrix;
}

Matrix* matrix_transpose(Matrix* m) {
    if (m == NULL) {
        printf("Matriz inexistente\n");
        return NULL;
    }
    Matrix* transposedMatrix = matrix_inicializator(m->column, m->line);
    Matrix* currentLine = m->below;

    while (currentLine != m) {
        Matrix* currentCell = currentLine->right;
        while (currentCell != currentLine) {
            if (currentCell->info != 0) {
                matrix_setelem(transposedMatrix, currentCell->column, currentCell->line, currentCell->info);
            }
            currentCell = currentCell->right;
        }
        currentLine = currentLine->below;
    }

    return transposedMatrix;
}



float matrix_getelem(Matrix* m, int x, int y) {
    Matrix* currentLine = m->below;
    while (currentLine != m && currentLine->line < x) {
        currentLine = currentLine->below;
    }

    if (currentLine == m || currentLine->line != x) {
        return 0.0f;
    }

    Matrix* current = currentLine->right;
    while (current != currentLine && current->column <= y) {
        if (current->column == y) {
            return current->info;
        }
        current = current->right;
    }

    return 0.0f;
}


void matrix_setelem(Matrix* m, int x, int y, float elem) {
    if (x <= 0 || y <= 0) {
        printf("Posição inválida (Índice 0)\n");
        return;
    }

    Matrix* currentLine = m->below;
    Matrix* prevLine = m;

    while (currentLine != m && currentLine->line < x) {
        prevLine = currentLine;
        currentLine = currentLine->below;
    }

    if (currentLine == m || currentLine->line != x) {
        Matrix* newLine = matrix_inicializator(x, m->column);
        newLine->below = currentLine;
        prevLine->below = newLine;
        currentLine = newLine;
    }

    Matrix* current = currentLine;

    while (current->right != currentLine && current->right->column <= y) {
    current = current->right;
    }
    
    if (current->right == currentLine || current->right->column != y) {
        Matrix* newCell = (Matrix*)malloc(sizeof(Matrix));
        if (newCell == NULL) {
            printf("Incapaz de alocar memória para a célula\n");
            return;
        }
        newCell->line = x;
        newCell->column = y;
        newCell->info = elem;

        newCell->right = current->right;
        current->right = newCell;

        Matrix* currentColumn = m;
        while (currentColumn->right != m && currentColumn->right->column < y) {
            currentColumn = currentColumn->right;
        }

        if (currentColumn->right == m || currentColumn->right->column != y) {
            newCell->below = currentColumn->right;
            currentColumn->right = newCell;
        }

        printf("Célula alocada: Linha %d, Coluna %d, Valor %.2f\n", x, y, elem);
    } else {
        current->right->info = elem;
    }
} 

Matrix* matrix_inicializator(int l, int c) {
    Matrix* matrixI = (Matrix*)malloc(sizeof(Matrix));
    if (matrixI == NULL) {
        printf("Incapaz de alocar memória para sua matriz\n");
        return NULL;
    }

    matrixI->line = l;
    matrixI->column = c;
    matrixI->info = -1;
    matrixI->right = matrixI;
    matrixI->below = matrixI;

   
    Matrix* currentLine = matrixI;
    for (int i = 0; i < l; i++) {
        Matrix* newLine = (Matrix*)malloc(sizeof(Matrix));
        if (newLine == NULL) {
            printf("Incapaz de alocar memória para a linha da matriz\n");
            matrix_destroy(matrixI); 
            return NULL;
        }
        newLine->line = i + 1;
        newLine->column = 0;
        newLine->info = 0;
        newLine->right = newLine; 
        newLine->below = currentLine->below; 
        currentLine->below = newLine; 
        currentLine = newLine; 
    }
        currentLine = matrixI;
    
     for (int i = 0; i < c; i++) {
        Matrix* newColumn = (Matrix*)malloc(sizeof(Matrix));
        if (newColumn == NULL) {
            printf("Incapaz de alocar memória para a coluna da matriz\n");
            matrix_destroy(matrixI); 
            return NULL;
        }
        newColumn->line = 0;
        newColumn->column = i + 1;
        newColumn->info = 0;
        newColumn->below = newColumn; 
        newColumn->right = currentLine->right; 
        currentLine->right = newColumn; 
        currentLine = newColumn; 
    }

    return matrixI;
}


