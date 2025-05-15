#include <iostream>
#include <string>
#include <conio.h>  // _getch() on Windows
#include <string>
#include <vector>
#include <windows.h>
using namespace std;
string code = R"(
CodeAI: Below is the implementation of the Neural Network code with Python

class Activation:
    @staticmethod
    def func(x):
        return 1.0/(1 + np.exp(-x))
    def derivative(self, x):
        return self.func(x) * (1.0 - self.func(x))

class Loss:
    @staticmethod
    def func(y_hat, y_true):
        return - np.sum(y_true * np.log(y_hat) + (1 - y_true) * np.log(1 - y_hat))
    @staticmethod
    def derivative(y_hat, y_true):
        return (y_hat - y_true)/(y_hat * (1 - y_hat))

class Layer:
    def __init__(self, input_size, output_size, actFunc):
        self.input_size = input_size
        self.output_size = output_size
        self.W =  np.random.randn(input_size, output_size) * 0.01
        self.b = np.ones((1, output_size))
        self.act = actFunc
        self.X = None
        self.Z = None
        self.A = None
        
    def forward(self, input):
        self.X = input
        self.Z = self.X @ self.W + self.b
        self.A = self.act.func(self.Z)
        return self.A

    def backward(self, alpha, dL_dA):
        dL_dZ = dL_dA * self.act.derivative(self.Z)
        dL_dX = dL_dZ @ self.W.T
        dL_dW = self.X.T @ dL_dZ
        dL_db = np.sum(dL_dZ, axis=0, keepdims=True)
        self.W -= alpha * dL_dW
        self.b -= alpha * dL_db
        return dL_dX
    
    def show(self):
        print("W = ")
        print(self.W)
        print(f"b = {self.b}")

class Model:
    def __init__(self, learning_rate, layer_size, lossFunc):
        self.alpha = learning_rate
        self.lossFunc = lossFunc
        self.y_hat = None
        self.layers = []
        for i in range(len(layer_size) - 1):
            layer = Layer(layer_size[i], layer_size[i + 1], Activation())
            self.layers.append(layer)

    def forward(self, x):
        for i in range(len(self.layers)):
            x = self.layers[i].forward(x)
        self.y_hat = x
        return x
    
    def backward(self, y_true):
        dL_dY = Loss.derivative(self.y_hat, y_true)
        for i in range(len(self.layers) - 1, -1, -1):
            dL_dY = self.layers[i].backward(self.alpha, dL_dY)

    def train(self, dataset, max_iter):
        x = dataset[:, :-1]
        y_true = dataset[:, -1].reshape(-1, 1)
        history = []
        for i in range(max_iter):
            self.forward(x)
            loss = Loss.func(self.y_hat, y_true)
            history.append(loss) 
            self.backward(y_true)
        return history
    
    def show(self):
        for i in range(len(self.layers)):
            print(f"Layer {i + 1}")
            self.layers[i].show()
            print()
            
# XOR Problem
dataset = np.array([[0, 0, 0], [0, 1, 1], [1, 0, 1], [1, 1, 0]])

learning_rate = 0.01
layer_size = [2, 2, 1]
lossFunc = Loss()

# model
model = Model(learning_rate, layer_size, lossFunc)

# train
fitness_history = model.train(dataset, 50000)
for i in range(len(fitness_history)):
    print(f"Generation {i}, loss = {fitness_history[i]:.2f}")

x = dataset[:, :-1]
print(model.forward(x))
model.show()
generations = list(range(len(fitness_history)))
plt.figure(figsize=(10, 5))
plt.plot(generations, fitness_history, marker='o', linestyle='-', color='b', label='Best Fitness')

plt.xlabel("Generation")
plt.ylabel("Best Fitness")
plt.title("Fitness Progress Over Generations")
plt.legend()
plt.grid(True)
plt.show()
)";

vector<string> result = {
    "Generation 0, loss = 3.25",
    "Generation 1, loss = 3.23",
    "Generation 2, loss = 3.21",
    "Generation 3, loss = 3.20",
    "Generation 4, loss = 3.18",
    "Generation 5, loss = 3.17",
    "Generation 6, loss = 3.15",
    "Generation 7, loss = 3.14",
    "Generation 8, loss = 3.12",
    "Generation 9, loss = 3.11",
    "Generation 10, loss = 3.10",
    "Generation 11, loss = 3.09",
    "Generation 12, loss = 3.07",
    "Generation 13, loss = 3.06",
    "Generation 14, loss = 3.05",
    "Generation 15, loss = 3.04",
    "Generation 16, loss = 3.03",
    "Generation 17, loss = 3.02",
    "Generation 18, loss = 3.01",
    "Generation 19, loss = 3.00",
    "Generation 20, loss = 2.99",
    "Generation 21, loss = 2.98",
    "Generation 22, loss = 2.98",
    "Generation 23, loss = 2.97",
    "Generation 24, loss = 2.96",
    "...",
    "Generation 49996, loss = 0.10",
    "Generation 49997, loss = 0.10",
    "Generation 49998, loss = 0.10",
    "Generation 49999, loss = 0.10",
};
int main() {
    size_t index = 0;
    while (index < code.size()) {
        _getch();
        cout << code[index++];
        cout.flush();
    }
    cout << endl;
    for (size_t i = 0; i < result.size(); i++) {
        Sleep(2);
        cout << result[i] << endl;
    }
    getchar();
    return 0;
}
