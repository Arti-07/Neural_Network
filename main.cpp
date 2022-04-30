#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;

const int CLUSTERS_COUNT = 4;

struct Neuron {
    vector<double> w;
};

double DistanceToVec(Neuron neuron, const double *mas, int count) {
    double res = 0;

    for (int i = 0; i < count; i++) {
        res += (neuron.w[i] - mas[i]) * (neuron.w[i] - mas[i]);
    }

    return res;
}

Neuron ChangeMass(Neuron neuron, const double *mas, int count, double v) {
    for (int i = 0; i < count; i++) {
        neuron.w[i] = neuron.w[i] + v * (mas[i] - neuron.w[i]);
    }
    return neuron;
}

int main() {
    srand(time(nullptr));

    int vector_count = 0;
    const int count = 7;                            //   Properties of each input vector
    double students_matrix[20][count] = {0};        // Matrix of the number of students and their properties

    /* READ FROM FILE */

    ifstream stream;
    stream.open("C:\\Programmirovanie\\2nd TERM\\Neural Network\\log.txt");
    if (!stream) {
        cout << "File reading error!" << endl;
        return 0;
    }
    double file_mas[count];
    while (stream >> file_mas[0]) {
        stream >> file_mas[1] >> file_mas[2] >> file_mas[3] >> file_mas[4] >> file_mas[5] >> file_mas[6];
        for (int i = 0; i < count; i++) {
            students_matrix[vector_count][i] = file_mas[i];
        }
        vector_count++;
    }
    stream.close();
    cout << "End read file\n" << endl;

    cout << "Count of students = " << vector_count << endl;
    cout << "Input data :" << endl;
    for (int i = 0; i < vector_count; i++) {
        for (int j = 0; j < count; j++) {
            cout << students_matrix[i][j] << "\t";
        }
        cout << endl;
    }


    vector<Neuron> neurons(CLUSTERS_COUNT);
    /* RANDOM WEIGHT COEFFICIENTS */

    cout << "WEIGHT COEFFICIENTS" << endl;
    for (int i = 0; i < CLUSTERS_COUNT; i++) {
        for (int j = 0; j < count; j++) {
            neurons[i].w.push_back(
                    round((((((double) rand() / RAND_MAX) * 2) - 1.0) / sqrt(vector_count) + 0.5) * 100) / 100);
        }
    }
    for (int i = 0; i < CLUSTERS_COUNT; i++) {
        for (int j = 0; j < count; j++) {
            cout << neurons[i].w[j] << "\t";
        }
        cout << endl;
    }


    double rate = 3.0;
    bool *isWinner = new bool[vector_count];      // Array for selecting the winner
    int index = 0;

    int *countChange = new int[CLUSTERS_COUNT];     // Array of the number of cluster changes
    for (int i = 0; i < CLUSTERS_COUNT; i++) {
        countChange[i] = 0;
    }


    int index_min_vec = 0;
    double min = 1000;

    /* START LEARNING */

    for (double v = 0.3; v > 0; v -= 0.05) {
        cout << "\n\t RATE = " << v << endl;

        for (int i = 0; i < vector_count; i++) {
            isWinner[i] = false;
        }

        //проход ро выборке
        for (int j = 0; j < vector_count; j++) {
            bool s = true;

            while (true) {
                index = rand() % vector_count;

                if (!isWinner[index]) {
                    isWinner[index] = true;
                    s = false;
                    break;
                }
            }
            cout << "\n num xi = " << index << endl;

            /* CORRECT CLUSTER */

            index_min_vec = 0;
            min = 1000;
            for (int i = 0; i < CLUSTERS_COUNT; i++) {
                double R = sqrt(DistanceToVec(neurons[i], students_matrix[index], count));

                if (R < min) {
                    min = R;
                    index_min_vec = i;
                }
            }

            neurons[index_min_vec] = ChangeMass(neurons[index_min_vec], students_matrix[index], count, v);

            cout << "Winner neuron = " << index_min_vec;
            countChange[index_min_vec]++;
        }
    }

    delete[] isWinner;

    cout << endl;
    cout << "\n\tWEIGHT COEFFICIENTS AFTER TRAINING :" << endl;
    for (int i = 0; i < CLUSTERS_COUNT; i++) {
        for (int j = 0; j < count; j++) {
            cout << neurons[i].w[j] << "   ";
        }
        cout << endl;
    }

    /* INFORMATION ABOUT COUNTS OF CHANGES */

    cout << endl;
    for (int i = 0; i < CLUSTERS_COUNT; i++) {
        cout << "Cluster[" << i << "]   count change = " << countChange[i] << endl;
        countChange[i] = 0;
    }

    /* CLUSTERING OF THE SAMPLE */

    cout << endl;
    for (int n = 0; n < vector_count; n++) {
        //correction mass
        index_min_vec = 0;
        min = 1000;

        for (int i = 0; i < CLUSTERS_COUNT; i++) {
            double R = sqrt(DistanceToVec(neurons[i], students_matrix[n], count));

            if (R < min) {
                min = R;
                index_min_vec = i;
            }
        }

        for (int i = 0; i < count; i++) {
            cout << students_matrix[n][i] << "\t";
        }

        cout << "\tCluster = " << index_min_vec << endl;

        countChange[index_min_vec]++;
    }

    /* Clustering results */

    cout << endl;
    for (int i = 0; i < CLUSTERS_COUNT; i++) {
        cout << "Cluster[" << i << "]   count = " << countChange[i] << endl;
    }
    delete[] countChange;

    return 0;
}