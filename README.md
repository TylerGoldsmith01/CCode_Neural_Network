#Neural Network Coded in C Programming Language#<br>
##Data Structures of Neural Network##<br>
I decided to use data structures to orgranize the nural network rather than arrays because they are easier to visualize, and better support customizability and expandability of the neural network.<br>
**node Struct**<br>
Sometimes known as neuron's these are the lowest level piece of the neural network. They are simple and contain the bias of the nodes, and an array cointaining all the connections to the next node of the neural network.
**connection Struct**<br>
Connections are used to interconnect the nodes of the neural network to allow calculation paths in the learning and decision process. Each connection contains a pointer to the next node, as well as a double value that stores the weight as calculated by the learning process.<br //>
**layer struct**<br>
The layers struct is mostly used in the connection routing process and for keeping track of where you are in the neural network. Each of the nodes that make up the neural network are stored as an array of nodes in the associated layer struct. Additionally this structure stores the number of nodes, as well as the type of node (input, hidden, or output)<br>
**NeuralNetwork struct**<br>
Finally the neural network struct is the outermost organization of the neural network, and it contains all the othe rlower level nodes within in a hierarchical structure. Specifically it contains an array of all the layers (each of which stores the subsequent nodes) as well as the number of layers that make up the neural network<br>
