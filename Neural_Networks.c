#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Parameters to determine the size of the neural network (NN)
#define numInputs 2
#define numHiddenNodes_PerLayer 3
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
    struct connection** connections;
};

struct connection {
    struct node* next;
    double weight;
};

// Function declarations
void initNeuralNetwork(struct NeuralNetwork*, FILE*);
void freeNeuralNetwork(struct NeuralNetwork*);
struct NeuralNetwork* generateNeuralNetwork();
struct layer* generateLayer(int, int);
double initWeights();
double sigmoidFun(double);
double sigmoidDerivative(double);

int main(void) {
    // Store the initial Neural Network in a text file
    FILE *fp;
    fp = fopen("Initial_Neural_Network.txt", "w");

    double training_inputs[numTrainingSamples][numInputs];
    double training_outputs[numTrainingSamples][numOutputs];
    struct NeuralNetwork* NN = generateNeuralNetwork();
    initNeuralNetwork(NN, fp);

    fclose(fp);
    freeNeuralNetwork(NN);
    return 0;
}
double sigmoidDerivative(double val){
    return (val*(1-val));
}

double sigmoid(double val){
    return (1 / (1+exp(-val)));
}
//Initialize weights to random value 0-1
double initWeights(){
    return ((double)rand()/((double)RAND_MAX));
};

//Iterate through node in each layer, then point each of these nodes to each node in next layer up
void initNeuralNetwork(struct NeuralNetwork* NN, FILE* fp){
    int layer;
    int node1;
    int node2;
    struct layer* currLayer;
    struct layer* nextLayer;
    struct node* currNode;
    //Main loop to iterate through all layers of the neural network
    for(layer=0; layer < NN->numLayers; layer++){
        switch(NN->layers[layer]->type){
            case 0:
                fprintf(fp, "Layer: %d Type: Input\n",layer+1);
                break;
            case 1:
                fprintf(fp, "Layer: %d Type: Hidden\n",layer+1);
                break;
            case 2:
                fprintf(fp, "Layer: %d Type: Output\n",layer+1);
                break;
        }
        currLayer = NN->layers[layer];
        if(layer!=NN->numLayers-1)
            nextLayer = NN->layers[layer+1];
        //Iterate through all nodes in current layer
        for(node1=0; node1 < currLayer->numNodes; node1++){
            fprintf(fp, "\tNode %d\n",node1+1);
            fprintf(fp, "\t\tBias = %lf\n",currLayer->nodes[node1]->Bias);
            if(layer!=NN->numLayers-1){
                fprintf(fp, "\t\tConnections:\n");
                currLayer->nodes[node1]->connections = malloc(sizeof(struct connection) * nextLayer->numNodes);
                //Iterate through all nodes in next layer, and create connections between with a random weight
                for(node2=0; node2 < nextLayer->numNodes; node2++){
                    currLayer->nodes[node1]->connections[node2] = malloc(sizeof(struct connection));
                    currLayer->nodes[node1]->connections[node2]->next = nextLayer->nodes[node2];
                    currLayer->nodes[node1]->connections[node2]->weight = initWeights();
                    fprintf(fp, "\t\t\tLayer %d Node %d\n",layer+2,node2+1);
                    fprintf(fp, "\t\t\t\tWeight %lf\n",currLayer->nodes[node1]->connections[node2]->weight);
                }
            }
        }
    }
}

void freeNeuralNetwork(struct NeuralNetwork* NN){
    // Free allocated memory
    for (int i = 0; i < NN->numLayers; i++) {
        for (int j = 0; j < NN->layers[i]->numNodes; j++) {
            free(NN->layers[i]->nodes[j]);
        }
        free(NN->layers[i]->nodes);
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
    NN->layers[0] = generateLayer(numInputs, 0);
    NN->layers[numHiddenLayers + 1] = generateLayer(numOutputs, 2);
    for (i = 1; i < numHiddenLayers+1; i++) {
        NN->layers[i] = generateLayer(numHiddenNodes_PerLayer, 1);
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
        fflush(stdout);
        currLayer->nodes[i] = malloc(sizeof(struct node));
    }

    return currLayer;
}