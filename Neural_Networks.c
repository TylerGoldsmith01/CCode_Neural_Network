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
    struct edge **edges;
    struct testCase** testCases;
    int numTestCases;
};

struct testCase{
    double *inputData;
    double *outputData;
    int testNumber;
};

struct layer {
    int numNodes;
    struct node** nodes;
};

struct node {
    double Bias;
    double value;
};

struct edge {
    struct node* node1;
    struct node* node2;
    double weight;
};

// Function declarations
void initNeuralNetwork(struct NeuralNetwork*, FILE*);
void freeNeuralNetwork(struct NeuralNetwork*);
struct NeuralNetwork* generateNeuralNetwork(int , int, int, int);
struct layer* generateLayer(int, int);
double initWeights();
double sigmoidFun(double);
double sigmoidDerivative(double);

//struct testCase ** initializeCases(double* , double*);
void shuffleCases(struct testCase **, int );
void swaptestCases(struct testCase*, struct testCase*);

int main(void) {
    //Quick Hardcoded Test Shuffle Functionality
    FILE *fpShuffleTest;
    fpShuffleTest = fopen("Shuffle_Test.txt", "w");
    struct testCase** testCases=malloc(10 * sizeof(struct testCase*));
    for (int i=0;i<10;i++){
        testCases[i]=malloc(sizeof(struct testCase));
        testCases[i]->testNumber = i;
    }

    for (int i=0 ; i < 100; i++){
        for (int j=0; j<10;j++){
            fprintf(fpShuffleTest, "%d ",testCases[j]->testNumber);
        }
        fprintf(fpShuffleTest, "\n");
        shuffleCases(testCases, 10);
    }
    for (int i=0;i<10;i++){
        free(testCases[i]);
    }
    free(testCases);
    fclose(fpShuffleTest);
    //EndQuick Hardcoded Test Shuffle Functionality


    // Store the initial Neural Network in a text file
    FILE *fp;
    fp = fopen("Initial_Neural_Network.txt", "w");

    struct NeuralNetwork* NN = generateNeuralNetwork(numInputs, numHiddenLayers, numHiddenNodes_PerLayer, numOutputs);
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

//Fisher-Yates shuffle algorithm...time complexity O(n)
//Start at last element and place in random spot from its place to the left... iterate leftward and repeat until first element
void shuffleCases(struct testCase **testCases, int numTestData){
    int i;
    for(i= numTestData-1 ; i > 0; i--){
        int newIndex = rand() % (i+1);
        struct testCase *temp = testCases[i];
        testCases[i] = testCases[newIndex];
        testCases[newIndex] = temp;
        //swaptestCases(testCases[i],testCases[newIndex]);
    }
}

void swaptestCases(struct testCase *TD1, struct testCase *TD2){
    struct testCase temp = *TD1;
    *TD1 = *TD2;
    *TD1 = temp;
}
/*
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
                fprintf(fp, "\t\tleadingEdges:\n");
                currLayer->nodes[node1]->leadingEdges = malloc(sizeof(struct edge) * nextLayer->numNodes);
                //Iterate through all nodes in next layer, and create leadingEdges between with a random weight
                for(node2=0; node2 < nextLayer->numNodes; node2++){
                    currLayer->nodes[node1]->leadingEdges[node2] = malloc(sizeof(struct leadingEdge));
                    currLayer->nodes[node1]->leadingEdges[node2]->next = nextLayer->nodes[node2];
                    currLayer->nodes[node1]->leadingEdges[node2]->weight = initWeights();
                    fprintf(fp, "\t\t\tLayer %d Node %d\n",layer+2,node2+1);
                    fprintf(fp, "\t\t\t\tWeight %lf\n",currLayer->nodes[node1]->leadingEdges[node2]->weight);
                }
            }
        }
    }
}
*/
void initialize_neuralNetwork(struct NeuralNetwork* NN,  int num_inputs, int num_hiddenLayers, int num_hiddenNodes_perLayer, int num_outputs){
    int layer, currnode, nextnode;
    int numLayers = numHiddenLayers + 2;
    //Setup entire Neural Network struct
    NN = malloc(sizeof(struct NeuralNetwork));
    NN->numLayers = numLayers;
    NN->layers = malloc(NN->numLayers * sizeof(struct layer));

    ////SETUP NODE STRUCTURE OF NEURAL NETWORK
    for(layer = 0 ; layer < numLayers; layer++){
        //Setup each layer within Neural Network
        NN->layers[layer] = malloc(sizeof(struct layer));
        NN->layers[layer]->nodes = malloc(sizeof(struct node*));
        NN->layers[layer]->numNodes = (layer==0)?(num_inputs):((layer==numLayers-1)?(num_outputs):(num_hiddenNodes_perLayer));
        for(currnode = 0; currnode < NN->layers[layer]->numNodes; currnode++){
            NN->layers[layer]->nodes[currnode] = malloc(sizeof(struct node));
        }
    }
    //SETUP EDGE STRUCTURE OF NEURAL NETWORK
    int input_to_hidden = (num_inputs * num_hiddenNodes_perLayer);
    int hidden_to_hidden = (num_hiddenNodes_perLayer * num_hiddenNodes_perLayer);
    int hidden_to_output = (num_hiddenNodes_perLayer * num_outputs);

    //Calculate total; number of edges in neural network
    int numEdges = input_to_hidden + (num_hiddenLayers-1) * hidden_to_hidden + hidden_to_output;

    NN->edges = malloc(numEdges * sizeof(struct edge*));
    for(layer = 0 ; layer < numLayers; layer++){
        int nodes_in_next_layer = (layer == numLayers-1)?(0):((layer==numLayers-2)?(num_outputs):(num_hiddenNodes_perLayer));
        int nodes_in_prev_layer = (layer == 0)?(0):((layer==1)?(num_inputs):(numHiddenNodes_PerLayer));
        for(currnode = 0; currnode < NN->layers[layer]->numNodes; currnode++){
            for(nextnode = 0; nextnode < nodes_in_next_layer; nextnode++){
                double weight = initWeights();
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
/*
// Create neural network, then populate with each layer of the neural network
struct NeuralNetwork* generateNeuralNetwork(int num_inputs, int num_hiddenLayers, int num_hiddenNodes_perLayer, int num_outputs) {
    struct NeuralNetwork* NN = malloc(sizeof(struct NeuralNetwork));
    NN->numLayers = num_hiddenLayers + 2; // Input layer + hidden layers + output layer

    NN->layers = malloc(NN->numLayers * sizeof(struct layer));

    int i;
    NN->layers[0] = generateLayer(num_inputs, 0);
    NN->layers[num_hiddenLayers + 1] = generateLayer(num_outputs, 2);
    for (i = 1; i < num_hiddenLayers+1; i++) {
        NN->layers[i] = generateLayer(num_hiddenNodes_perLayer, 1);
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
*/