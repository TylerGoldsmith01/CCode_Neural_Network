#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
#define numInputs 2
#define numHiddenNodes_PerLayer 3
#define numHiddenLayers 1
#define numOutputs 1
#define numTrainingSamples 4
*/

struct testCase {
    double* inputData;
    double* outputData;
    int testNumber;
};

struct node {
    double Bias;
    double value;
    int designator;

    int num_prevEdges;
    int num_nextEdges;
    struct edge** nextEdges;
    struct edge** prevEdges;
};

struct edge {
    struct node* node1;
    struct node* node2;
    double weight;
};

struct layer {
    int numNodes;
    struct node** nodes;
    char type;
};

struct NeuralNetwork {
    int numLayers;
    struct layer** layers;

    int numEdges;
    struct edge** edges;

    struct testCase** testCases;
    int numTestCases;
};

// Function declarations
struct NeuralNetwork* create_neuralNetwork(int, int, int, int);
struct testCase** generateCaseData(FILE*,FILE*, int, int);

void printNeuralNetwork(struct NeuralNetwork*, FILE*);
void freeNeuralNetwork(struct NeuralNetwork*);
void trainLoop(int numRounds, struct NeuralNetwork* NN, FILE* fp);
double sigmoidDerivative(double val);
double sigmoid(double val);
double randomNum();
void shuffleCases(struct testCase** testCases, int numTestData);

#endif // NEURAL_NETWORK_H