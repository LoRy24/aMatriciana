// Libs
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// Defs
typedef struct {
    int righe;
    int colonne;
    float **matrice;
} Matrix;

#pragma region Testate Necessarie

// X Ricorsione
float minorComplementary(Matrix A, int i, int j);
float complement(Matrix A, int i, int j);
float determinant(Matrix A);

#pragma endregion

#pragma region Matrici

Matrix createMatrix(const int righe, const int colonne) {
    // Definisci la matrice
    Matrix m;
    m.righe = righe;
    m.colonne = colonne;

    // Alloca la matrice
    m.matrice = (float ** ) malloc(righe * sizeof(float *));
    for (int i = 0; i < righe; i++) {
        m.matrice[i] = (float *) malloc(colonne * sizeof(float));
    }

    // Ritorna la matrice
    return m;
}

void freeMatrix(const Matrix *m) {
    // Libera la matrice
    for (int i = 0; i < m->righe; i++) {
        free(m->matrice[i]);
    }
    free(m->matrice);
}

void printMatrix(const Matrix A) {
    for (int i = 0; i < A.righe; i++) {
        for (int j = 0; j < A.colonne; j++) {
            printf("%8.3f ", A.matrice[i][j]);
        }
        printf("\n");
    }
}

Matrix readMatrix() {
    // Chiedi righe e colonne
    int righe, colonne;

    printf("Righe >> ");
    scanf("%d", &righe); // NOLINT(*-err34-c)

    printf("Colonne >> ");
    scanf("%d", &colonne); // NOLINT(*-err34-c)

    // Crea la matrice
    Matrix m = createMatrix(righe, colonne);

    // Inserisci i valori
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("Riga %d Colonna %d >> ", i, j);
            scanf("%f", &m.matrice[i][j]); // NOLINT(*-err34-c)
        }
    }

    // Scrivi la matrice
    printf("\nMatrice %d x %d\n", righe, colonne);
    printMatrix(m);

    // Ritorna la matrice
    return m;
}

Matrix sumMatrix(const Matrix A, const Matrix B) {
    // Crea la matrice somma
    const Matrix S = createMatrix(A.righe, A.colonne);

    // Somma le matrici
    for (int i = 0; i < A.righe; i++) {
        for (int j = 0; j < A.colonne; j++) {
            S.matrice[i][j] = A.matrice[i][j] + B.matrice[i][j];
        }
    }

    // Ritorna la matrice somma
    return S;
}

Matrix subMatrix(const Matrix A, const Matrix B) {
    // Crea la matrice differenza
    const Matrix D = createMatrix(A.righe, A.colonne);

    // Sottrai le matrici
    for (int i = 0; i < A.righe; i++) {
        for (int j = 0; j < A.colonne; j++) {
            D.matrice[i][j] = A.matrice[i][j] - B.matrice[i][j];
        }
    }

    // Ritorna la matrice differenza
    return D;
}

Matrix multMatrix(const Matrix A, const Matrix B) {
    // Crea la matrice moltiplicazione
    const Matrix M = createMatrix(A.righe, B.colonne);

    // Moltiplica le matrici
    for (int i = 0; i < A.righe; i++) {
        for (int j = 0; j < B.colonne; j++) {
            M.matrice[i][j] = 0;
            for (int k = 0; k < A.colonne; k++) {
                M.matrice[i][j] += A.matrice[i][k] * B.matrice[k][j];
            }
        }
    }

    // Ritorna la matrice moltiplicazione
    return M;
}

Matrix transMatrix(const Matrix A) {
    // Crea la matrice trasposta
    const Matrix T = createMatrix(A.colonne, A.righe);

    // Trasponi la matrice
    for (int i = 0; i < A.righe; i++) {
        for (int j = 0; j < A.colonne; j++) {
            T.matrice[j][i] = A.matrice[i][j];
        }
    }

    // Ritorna la matrice trasposta
    return T;
}

float minorComplementary(const Matrix A, const int i, const int j) { // NOLINT(*-no-recursion)
    // Crea la matrice complementare
    const Matrix C = createMatrix(A.righe - 1, A.colonne - 1);

    // Copia i valori nella matrice complementare
    for (int r = 0; r < A.righe; r++) {
        for (int c = 0; c < A.colonne; c++) {
            if (r != i - 1 && c != j - 1) {
                C.matrice[r - (r > i - 1)][c - (c > j - 1)] = A.matrice[r][c];
            }
        }
    }

    // Calcola il determinante della matrice complementare
    const float det = determinant(C);

    // Libera la matrice complementare
    freeMatrix(&C);

    // Ritorna il determinante
    return det;
}

float complement(const Matrix A, const int i, const int j) { // NOLINT(*-no-recursion)
    return (float)((i + j) % 2 == 0 ? 1 : -1) * minorComplementary(A, i, j);
}

float determinant(const Matrix A) { // NOLINT(*-no-recursion)
    if (A.righe == 1 && A.colonne == 1) {
        return A.matrice[0][0];
    }

    // Definisce il determinante
    float det = 0;
    for (int j = 0; j < A.colonne; j++) {
        det += A.matrice[0][j] * complement(A, 1, j + 1);
    }

    // Ritorna il determinante
    return det;
}

Matrix inverse(const Matrix A) {
    // Crea la matrice inversa
    const Matrix I = createMatrix(A.righe, A.colonne);

    // Determinante
    const float det = determinant(A);

    // Calcola l'inversa
    for (int i = 0; i < A.righe; i++) {
        for (int j = 0; j < A.colonne; j++) {
            I.matrice[i][j] = complement(A, i + 1, j + 1) / det;
        }
    }

    // Trasponi la matrice inversa
    const Matrix T = transMatrix(I);

    // Libera la matrice inversa
    freeMatrix(&I);

    // Ritorna la matrice inversa
    return T;
}

Matrix scaleProdMatrix(const Matrix A, const float k) {
    // Crea la matrice moltiplicata per valore scalare
    const Matrix S = createMatrix(A.righe, A.colonne);

    // Moltiplica
    for (int i = 0; i < A.righe; i++) {
        for (int j = 0; j < A.colonne; j++) {
            S.matrice[i][j] = A.matrice[i][j] * k;
        }
    }

    // Ritorna la matrice moltiplicata per valore scalare
    return S;
}

#pragma endregion

#pragma region Funzioni Programma

void wait() {
    printf("Premi un tasto per continuare...\n");
    getch();
}

void somma() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Matrice B
    printf("\nMatrice B\n");
    const Matrix B = readMatrix();

    // Verifica dimensione
    if (A.righe != B.righe || A.colonne != B.colonne) {
        printf("Le matrici non hanno le stesse dimensioni!\n");
        return;
    }

    // Crea la matrice somma
    const Matrix S = sumMatrix(A, B);
    printf("\nMatrice Somma\n");
    printMatrix(S);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&S);

    // Aspetta
    printf("Premi un tasto per continuare...\n");
    getch();
}

void sottrazione() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Matrice B
    printf("\nMatrice B\n");
    const Matrix B = readMatrix();

    // Verifica dimensione
    if (A.righe != B.righe || A.colonne != B.colonne) {
        printf("Le matrici non hanno le stesse dimensioni!\n");
        return;
    }

    // Crea la matrice differenza
    const Matrix D = subMatrix(A, B);
    printf("\nMatrice Differenza\n");
    printMatrix(D);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&D);

    // Aspetta
    wait();
}

void moltiplicazione() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Matrice B
    printf("\nMatrice B\n");
    const Matrix B = readMatrix();

    // Verifica dimensione
    if (A.colonne != B.righe) {
        printf("Le matrici non hanno le dimensioni corrette per la moltiplicazione!\n");
        return;
    }

    // Crea la matrice moltiplicazione
    const Matrix M = multMatrix(A, B);
    printf("\nMatrice Moltiplicazione\n");
    printMatrix(M);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&M);

    // Aspetta
    wait();
}

void trasposizione() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Crea la matrice trasposta
    const Matrix T = transMatrix(A);
    printf("\nMatrice Trasposta\n");
    printMatrix(T);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);
    freeMatrix(&T);

    // Aspetta
    wait();
}

void determinante() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Verifica dimensione
    if (A.righe != A.colonne) {
        printf("La matrice non e' quadrata!\n");
        return;
    }

    // Calcola il determinante
    const float det = determinant(A);
    printf("\nDeterminante: %.3f\n", det);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);

    // Aspetta
    wait();
}

void inversa() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Verifica dimensione
    if (A.righe != A.colonne) {
        printf("La matrice non e' quadrata!\n");
        return;
    }

    // Verifica determinante
    const float det = determinant(A);
    if (det == 0) {
        printf("La matrice non e' invertibile!\n");
        return;
    }

    // Calcola l'inversa
    const Matrix I = inverse(A);
    printf("\nMatrice Inversa\n");
    printMatrix(I);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);
    freeMatrix(&I);

    // Aspetta
    wait();
}

void divisione() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Matrice B
    printf("\nMatrice B\n");
    const Matrix B = readMatrix();

    // Verifica che siano quadrate
    if (A.righe != A.colonne || B.righe != B.colonne) {
        printf("Le matrici non sono quadrate!\n");
        return;
    }

    // Verifica dimensione
    if (A.righe != B.righe || A.colonne != B.colonne) {
        printf("Le matrici non hanno le stesse dimensioni!\n");
        return;
    }

    // Verifica determinanti
    const float detB = determinant(B);
    if (detB == 0) {
        printf("La matrice B non e' invertibile!\n");
        return;
    }

    // Crea la matrice inversa di B
    const Matrix Bi = inverse(B);

    // Crea la matrice divisione
    const Matrix D = multMatrix(A, Bi);

    // Stampa la matrice divisione
    printf("\nMatrice Divisione\n");
    printMatrix(D);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&Bi);
    freeMatrix(&D);

    // Aspetta
    wait();
}

void prodottoScalare() {
    // Matrice A
    printf("\nMatrice A\n");
    const Matrix A = readMatrix();

    // Valore scalare
    float k;
    printf("\nValore scalare >> ");
    scanf("%f", &k); // NOLINT(*-err34-c)

    // Crea la matrice moltiplicata per valore scalare
    const Matrix S = scaleProdMatrix(A, k);

    // Stampa la matrice moltiplicata per valore scalare
    printf("\nMatrice Moltiplicata per %.3f\n", k);
    printMatrix(S);
    printf("\n");

    // Pulisci la memoria
    freeMatrix(&A);
    freeMatrix(&S);

    // Aspetta
    wait();
}

#pragma endregion

#pragma region Routine

void printMenu() {
    // Pulisci lo schermo
    system("cls");

    // Stampa il menu
    printf("aMatriciana v1.0.0 - Scegli operazione\n");
    printf("\n");
    printf("1. Somma di matrici\n");
    printf("2. Sottrazione di matrici\n");
    printf("3. Moltiplicazione di matrici\n");
    printf("4. Trasposizione di matrici\n");
    printf("5. Determinante di una matrice\n");
    printf("6. Inversa di una matrice\n");
    printf("7. Divisione di matrici\n");
    printf("8. Prodotto scalare tra matrice e numero\n");
    printf("\n");
    printf("0. Esci\n\n");
}

int main(void) {
    // Scrivi il menu
    printMenu();

    // Leggi l'ingresso dell'utente
    int scelta = -1;
    while (scelta != 0) {
        printf("Scelta >> ");
        scanf("%d", &scelta); // NOLINT(*-err34-c)

        switch (scelta) {
            case 0: {
                printf("Arrivederci!\n");
                break;
            }

            case 1: {
                somma();
                printMenu();
                break;
            }

            case 2: {
                sottrazione();
                printMenu();
                break;
            }

            case 3: {
                moltiplicazione();
                printMenu();
                break;
            }

            case 4: {
                trasposizione();
                printMenu();
                break;
            }

            case 5: {
                determinante();
                printMenu();
                break;
            }

            case 6: {
                inversa();
                printMenu();
                break;
            }

            case 7: {
                divisione();
                printMenu();
                break;
            }

            case 8: {
                prodottoScalare();
                printMenu();
                break;
            }

            default: {
                printf("Operazione invalida!\n");
                break;
            }
        }
    }

    // OK
    return 0;
}

#pragma endregion
