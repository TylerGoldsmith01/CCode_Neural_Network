# Neural Network Coded in C Programming Language<br>
## Data Structures of Neural Network<br>
I decided to use data structures to orgranize the nural network rather than arrays because they are easier to visualize, and better support customizability and expandability of the neural network.<br>
### testCases Struct<br>
Stores an array of the inputs to be fed to the input nodes and the expected values of the output nodes of the test/training data that is to be fed into the neural network.<br>
### node Struct<br>
Sometimes known as neuron's these are the lowest level piece of the neural network. They are simple and contain the bias of the nodes, and an array containing pointers to all the subsequent edges as well as pointers to the previous edges to allow network traversal.<br>
### edge Struct<br>
Connections are used to interconnect the nodes of the neural network to allow calculation paths in the learning and decision process. Each connection contains a pointer to the next node, a pointer to the previous node, as well as a double value that stores the weight as calculated by the learning process.<br>
### layer struct<br>
The layers struct is mostly used in the connection routing process and for keeping track of where you are in the neural network. Each of the nodes that make up the neural network are stored as an array of nodes in the associated layer struct. Additionally this structure stores the number of nodes, as well as the type of node (input, hidden, or output)<br>
### NeuralNetwork struct<br>
Finally the neural network struct is the outermost organization of the neural network, and it contains all the other lower level nodes within in a hierarchical structure. It contains an array of layers which points to all the nodes within the layer. Additionally it contains a global list of all the edge that connect the nodes.<br>
