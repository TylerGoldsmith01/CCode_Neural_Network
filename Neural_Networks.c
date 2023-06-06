#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Parameters to determine the size of the neural network (NN)
#define numInputs 2
#define numHiddenNodes_PerLayer 2
#define numHiddenLayers 1
#define numOutputs 1
#define numTrainingSamples 4

struct NeuralNetwork {
    int numLayers;
    struct layer** layers;
};

struct layer {
    int numNodes;
    struct node** nodes;
    // 0 for input, 1 for hidden, 2 for output
    int type;
};

struct node {
    double Bias;
    struct node** next;
    struct connection* connections;
};

struct connection {
    struct node* node2;
    double weight;
};

// Function declarations
struct NeuralNetwork* generateNeuralNetwork();
struct layer* generateLayer(int numNodes, int type);

int main(void) {
    struct NeuralNetwork* NN = generateNeuralNetwork();
    freeNeuralNetwork(NN);

}

void freeNeuralNetwork(struct NeuralNetwork* NN){
    // Free allocated memory
    for (int i = 0; i < NN->numLayers; i++) {
        struct layer* currLayer = &NN->layers[i];
        for (int j = 0; j < currLayer->numNodes; j++) {
            free(currLayer->nodes[j]);
        }
        free(currLayer->nodes);
    }
    free(NN->layers);
    free(NN);
}

// Create neural network, then populate with each layer of the neural network
struct NeuralNetwork* generateNeuralNetwork() {
    struct NeuralNetwork* NN = malloc(sizeof(struct NeuralNetwork));
    NN->numLayers = numHiddenLayers + 2; // Input layer + hidden layers + output layer

    NN->layers = malloc(NN->numLayers * sizeof(struct layer));

    int i;
    for (i = 0; i < numInputs; i++) {
        NN->layers[i] = generateLayer(numInputs, 0);
    }
    for (i = 0; i < numHiddenLayers; i++) {
        NN->layers[i] = generateLayer(numHiddenNodes_PerLayer, 1);
    }
    for (i = 0; i < numOutputs; i++) {
        NN->layers[i] = generateLayer(numOutputs, 2);
    }

    return NN;
}

// Create layer, then populate with blank nodes
struct layer* generateLayer(int numNodes, int type) {
    struct layer* currLayer = malloc(sizeof(struct layer));
    currLayer->type = type;
    currLayer->numNodes = numNodes;

    currLayer->nodes = malloc(numNodes * sizeof(struct node*));
    for (int i = 0; i < numNodes; i++) {
        printf("Creating node %d of %d\n",i,numNodes);
        currLayer->nodes[i] = malloc(sizeof(struct node));
    }

    return currLayer;
}