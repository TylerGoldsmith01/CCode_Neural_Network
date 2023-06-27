//#include "C:\Users\Tyler\Desktop\Neural_Network\CCode_Neural_Network\Functional_Code\Neural_Networks.h"
#include "..\Functional_Code\Neural_Networks.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Parameters to determine the size of the neural network (NN)
#define numInputs 2
#define numHiddenNodes_PerLayer 3
#define numHiddenLayers 1
#define numOutputs 1
#define numTrainingSamples 4

#define inFileName "C:\\Users\\Tyler\\Desktop\\Neural_Network\\CCode_Neural_Network\\Test_XOR\\Inputs_XOR.txt"
#define outFileName "C:\\Users\\Tyler\\Desktop\\Neural_Network\\CCode_Neural_Network\\Test_XOR\\Outputs_XOR.txt"

int main(void) {
    // Store the initial Neural Network in a text file
    FILE *fp;
    fp = fopen("Initial_Neural_Network.txt", "w");


    FILE *infp;
    FILE *outfp;

    infp = fopen(inFileName, "r");

    if (infp==NULL || outfp==NULL){
        printf("Did not open file");
        return NULL;
    }

    outfp = fopen(outFileName, "r");

    struct testCase** Cases = generateCaseData(infp,outfp, numInputs, numOutputs);

    fclose(infp);
    fclose(outfp);

    struct NeuralNetwork* NN = create_neuralNetwork(numInputs, numHiddenLayers, numHiddenNodes_PerLayer, numOutputs);
    printNeuralNetwork(NN, fp);
    FILE *runResults;

    //Functionality for forward propagation with test cases
    runResults = fopen("Neural_Network_Tests.txt", "w");
    freeNeuralNetwork(NN);
    fclose(fp);
    return 0;
}