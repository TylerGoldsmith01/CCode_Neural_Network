#include <Neural_Networks.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Parameters to determine the size of the neural network (NN)
#define numInputs 2
#define numHiddenNodes_PerLayer 3
#define numHiddenLayers 1
#define numOutputs 1
#define numTrainingSamples 4

int main(void) {
    // Store the initial Neural Network in a text file
    FILE *fp;
    fp = fopen("Initial_Neural_Network.txt", "w");
    struct NeuralNetwork* NN = create_neuralNetwork(numInputs, numHiddenLayers, numHiddenNodes_PerLayer, numOutputs);
    printNeuralNetwork(NN, fp);
    FILE *runResults;

    //Functionality for forward propagation with test cases
    runResults = fopen("Neural_Network_Tests.txt", "w");
    freeNeuralNetwork(NN);
    fclose(fp);
    return 0;
}