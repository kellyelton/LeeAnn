#pragma once
#include "Neuron.h"
#include <deque>
using namespace std;
class NeuroCube
{
public:
	deque<Neuron> DedicatedInputs;
	deque<Neuron> DedicatedOutputs;
	int Width;
	int Height;
	int Depth;
	deque<Neuron> Neurons;

	NeuroCube(int x, int y, int z, Algorithm alg);
	void AddDedicatedInput(Neuron neuron, int x, int y, int z);
	void AddDedicatedOutput(Neuron neuron, int x, int y, int z);
	void RewardAllInputs(double reward);
	int getNeuron(int x, int y, int z);
	int getNeuron(Position pos);
	void Update(NeuroCube & cube);
	int XYZtoIndex(Position p);
	~NeuroCube(void);
private:
	Neuron NullNeuron;
	Algorithm algorithm;

	void MakeConnections();
};

