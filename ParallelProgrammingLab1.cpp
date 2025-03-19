#include <chrono>
#include <vector>
#include <iostream>
using namespace std;
const int MAX = 1000;
//Implementar y comparar los 2 bucles anidados de la pagina 22
#include <vector>

void Nested()
{
    // Replace static arrays with vectors
    std::vector<std::vector<double>> A(MAX, std::vector<double>(MAX));
    std::vector<double> x(MAX);
    std::vector<double> y(MAX);

    // Initialize A and x, assign y = 0
    for (int i = 0; i < MAX; i++)
    {
        x[i] = i;
        y[i] = 0;
    }
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
            A[i][j] = i;
    }
    A[1][1] = 0;

    auto start = std::chrono::system_clock::now();
    // First pair of loops
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            y[i] += (A[i][j] * x[j]);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Page 22 - Time: " << elapsed.count() << " ms" << endl;

    // Assign y = 0
    for (int i = 0; i < MAX; i++)
        y[i] = 0;

    // Second pair of loops
    start = std::chrono::system_clock::now();
    for (int j = 0; j < MAX; j++)
        for (int i = 0; i < MAX; i++)
            y[i] += A[i][j] * x[j];
    end = std::chrono::system_clock::now();
    elapsed = end - start;
    cout << "Page 22 - Time: " << elapsed.count() << " ms" << endl;
}
//Implementar la mult matrix clasica, 3 bucles. Desempeno de acuerdo al tamano.
void ClassicMM(vector<vector<double>>& A, vector<vector<double>>& B, vector<vector<double>>& C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
//Implementar la versión por bloques, seis bucles anidados, evaluar su desempeño y compararlo con la multiplicación de matrices clásica.
void BlockMM(vector<vector<double>>& A, vector<vector<double>>& B, vector<vector<double>>& C, int n, int blockSize) {
    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            for (int k = 0; k < n; k += blockSize) {
                for (int ii = i; ii < min(i + blockSize, n); ii++) {
                    for (int jj = j; jj < min(j + blockSize, n); jj++) {
                        for (int kk = k; kk < min(k + blockSize, n); kk++) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int n = 1000;
    int blockSize = 50;
    vector<vector<double>> A(n, vector<double>(n, 1.0));
    vector<vector<double>> B(n, vector<double>(n, 2.0));
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    auto start = chrono::high_resolution_clock::now();
    ClassicMM(A, B, C, n);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Classic Time: " << duration.count() << endl;
    /*
    std::cout << "Test 1 \n";
    Nested();
    std::cout << "\n";
    
    
    start = chrono::high_resolution_clock::now();
    BlockMM(A, B, C, n, blockSize);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Block Time: " << duration.count() << endl;
    */

    
    return 0;
}

