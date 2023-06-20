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

    int numEdges;
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
    char type;
};

struct node {
    double Bias;
    double value;
    int designator;
    struct edge **nextEdges;
    struct edge **prevEdges;
};

struct edge {
    struct node* node1;
    struct node* node2;
    double weight;
};

// Function declarations
struct NeuralNetwork* create_neuralNetwork(int, int, int, int);
void printNeuralNetwork(struct NeuralNetwork*, FILE*);
void freeNeuralNetwork(struct NeuralNetwork*);

double randomNum();
double sigmoid(double);
double sigmoidDerivative(double);

void shuffleCases(struct testCase **, int );

int main(void) {
    /*
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
*/
    // Store the initial Neural Network in a text file
    FILE *fp;
    fp = fopen("Initial_Neural_Network.txt", "w");
    struct NeuralNetwork* NN = create_neuralNetwork(numInputs, numHiddenLayers, numHiddenNodes_PerLayer, numOutputs);
    printNeuralNetwork(NN, fp);
    freeNeuralNetwork(NN);
    fclose(fp);
    return 0;
}

double sigmoidDerivative(double val){
    return (val*(1-val));
}

double sigmoid(double val){
    return (1 / (1+exp(-val)));
}
//Initialize weights to randomNum value 0-1
double randomNum(){
    return ((double)rand()/((double)RAND_MAX));
};

//Fisher-Yates shuffle algorithm...time complexity O(n)
//Start at last element and place in randomNum spot from its place to the left... iterate leftward and repeat until first element
void shuffleCases(struct testCase **testCases, int numTestData){
    int i;
    for(i= numTestData-1 ; i > 0; i--){
        int newIndex = rand() % (i+1);
        struct testCase *temp = testCases[i];
        testCases[i] = testCases[newIndex];
        testCases[newIndex] = temp;
    }
}

void trainLoop(int numRounds, struct NeuralNetwork *NN){
    //Run through training loop a parameter designated number of times
    for (int round = 0; round < numRounds; round++){
        //Shuffle up all the test cases
        shuffleCases(NN->testCases, NN->numTestCases);
        //Run through every test case for each round
        for(int test = 0; test < NN->numTestCases; test++){

        }
    }
}

//Iterate through node in each layer, then point each of these nodes to each node in next layer up
void printNeuralNetwork(struct NeuralNetwork* NN, FILE* fp){
    int layer;
    int e;
    int node1;
    int node2;
    //Main loop to iterate through all layers of the neural network
    for(layer=0; layer < NN->numLayers; layer++){
        switch(NN->layers[layer]->type){
            case 'i':
                fprintf(fp, "Layer: %d Type: Input\n",layer+1);
                break;
            case 'h':
                fprintf(fp, "Layer: %d Type: Hidden\n",layer+1);
                break;
            case 'o':
                fprintf(fp, "Layer: %d Type: Output\n",layer+1);
                break;
        }
        //Iterate through all nodes
        for(node1=0; node1 < NN->layers[layer]->numNodes; node1++){
            fprintf(fp, "\tNode: %d\n",NN->layers[layer]->nodes[node1]->designator);
            fprintf(fp, "\t\tBias = %lf\n",NN->layers[layer]->nodes[node1]->Bias);
        }
    }
    fprintf(fp, "\n\nEDGES\n");
    for(e = 0; e < NN->numEdges; e ++){
        fprintf(fp, "Edge %d:\n\tnode %d and %d\n\t%f\n", e ,NN->edges[e]->node1->designator, NN->edges[e]->node2->designator, NN->edges[e]->weight);
    }
}

struct NeuralNetwork* create_neuralNetwork( int num_inputs, int num_hiddenLayers, int num_hiddenNodes_perLayer, int num_outputs){
    struct NeuralNetwork* NN;
    int layer, edge, num_currnode, num_nextnode, prevnode;
    int numLayers = num_hiddenLayers + 2;
    //Setup entire Neural Network struct
    NN = malloc(sizeof(struct NeuralNetwork));
    NN->numLayers = numLayers;
    NN->layers = malloc(NN->numLayers * sizeof(struct layer));
    int node_designator = 0;

    //////SETUP NODE STRUCTURE OF NEURAL NETWORK///////
    for(layer = 0 ; layer < numLayers; layer++){
        //Setup each layer within Neural Network
        struct layer* currLayer = malloc(sizeof(struct layer));
        currLayer->numNodes = (layer==0)?(num_inputs):((layer==numLayers-1)?(num_outputs):(num_hiddenNodes_perLayer));
        currLayer->nodes = malloc(currLayer->numNodes * sizeof(struct node*));
        currLayer->type = (layer==0)?('i'):((layer==numLayers-1)?('o'):('h'));

        int numNodes_nextLayer=0;
        int numNodes_prevLayer=0; 
        if(layer != numLayers - 1) numNodes_nextLayer= (layer==numLayers-2)?(num_outputs):(num_hiddenNodes_perLayer);
        if(layer != 0) numNodes_prevLayer = (layer==1)?(num_inputs):(num_hiddenNodes_perLayer);
        
        for(num_currnode = 0; num_currnode < currLayer->numNodes; num_currnode++){
            //Define node in neural network
            struct node* currNode = malloc(sizeof(struct node));
            currNode->designator = node_designator;
            node_designator ++;
            currNode->Bias = 1;
            currLayer->nodes[num_currnode] = currNode;
            if(layer != numLayers - 1) currNode->nextEdges = malloc(numNodes_nextLayer * sizeof(struct edge*));
            if(layer != 0) currNode->prevEdges = malloc(numNodes_prevLayer * sizeof(struct edge*));
        }

        NN->layers[layer] = currLayer;
    }

    //////SETUP EDGE STRUCTURE OF NEURAL NETWORK//////
    int input_to_hidden = (num_inputs * num_hiddenNodes_perLayer);
    int hidden_to_hidden = (num_hiddenNodes_perLayer * num_hiddenNodes_perLayer);
    int hidden_to_output = (num_hiddenNodes_perLayer * num_outputs);
    int numEdges = input_to_hidden + ((num_hiddenLayers-1) * hidden_to_hidden) + hidden_to_output;
    NN->numEdges = numEdges;

    NN->edges = malloc(numEdges * sizeof(struct edge*));
    for(edge = 0; edge < numEdges; edge ++)
        NN->edges[edge] = malloc(sizeof(struct edge));

    int edge_counter=0;

    for(layer = 0 ; layer < numLayers-1; layer++){
        struct layer* currLayer = NN->layers[layer];
        struct layer* nextLayer = NN->layers[layer+1];
        int nodes_in_next_layer = (layer == numLayers-1) ? (0):(
            (layer == numLayers - 2) ? (num_outputs):(num_hiddenNodes_perLayer)
            );

        for(num_currnode = 0; num_currnode < currLayer->numNodes; num_currnode++){
            struct node* currNode = currLayer->nodes[num_currnode];
            for(num_nextnode = 0; num_nextnode < nodes_in_next_layer; num_nextnode++){
                    struct node* nextNode = nextLayer->nodes[num_nextnode];
                    struct edge* currEdge= NN->edges[edge_counter];

                    currNode->nextEdges[num_nextnode] = currEdge;
                    nextNode->prevEdges[num_currnode] = currEdge;
                    currEdge->node1 = currNode;
                    currEdge->node2 = nextNode;
                    currEdge->weight = randomNum();
                    edge_counter ++;
            }
        }
    }
    return NN;
}

void freeNeuralNetwork(struct NeuralNetwork* NN){
    int i;
    //Free allocated edges
    for(i=0 ; i < NN->numEdges ; i++)
        free(NN->edges[i]);
    // Free allocated memory
    for (i = 0; i < NN->numLayers; i++) {
        for (int j = 0; j < NN->layers[i]->numNodes; j++) {
            free(NN->layers[i]->nodes[j]);
        }
        free(NN->layers[i]->nodes);
    }
    free(NN->layers);
    free(NN);
}