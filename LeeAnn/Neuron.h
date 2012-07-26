#pragma once
#include <time.h>
#include <functional>
#include <vector>
#include <deque>
#include "NeuronMath.h"
//#include "Algorithm.h"
using namespace std;
class Algorithm;
class NeuroCube;
struct InputEvent
{
	vector<double> Data;
	vector<double> OutputWeights;
	vector<double> OutputData;
	time_t seconds;
	InputEvent();
	InputEvent(vector<double> data, vector<double>outputweights,vector<double> outputdata);
};
struct Position
{
	bool operator ==(const Position& a)
	{
		if(a.X == X && a.Y == Y && a.Z == Z)
			return true;
		return false;
	}
	int X,Y,Z;
	Position():X(-1),Y(-1),Z(-1)
	{
	}
	Position(int x, int y, int z)
	{
		X = x;
		Y = y;
		Z = z;
	}
};
class OutputEventCatcher
{
public:
	virtual void onOutput(vector<double> outputs)
	{}
};
class Neuron
{
public:
	Position MyPosition;
		vector<double> Outputs;
	Neuron();
	Neuron(const Neuron * n);
	~Neuron();
	void Input (double data);
	void Input(vector<double> data);
	void Input(shared_ptr<Neuron> fromNeuron, double data, shared_ptr<Algorithm> alg);
	void addInput(Position fromNeuron, NeuroCube & cube);
	void addOutput(Position toNeuron, double weight, NeuroCube & cube);
	void addOutput(Position toNeuron, NeuroCube & cube);
	void addInputOutput(Position theNeuron, NeuroCube & cube);
	void Reward(double reward, NeuroCube & cube);
	void setNull(bool null);
	bool isNull();
	void Push(shared_ptr<Algorithm> alg, NeuroCube & cube);
	bool WasPushed();
	void setOutputEventCatcher(shared_ptr<OutputEventCatcher> oec);
	void Wait(NeuroCube & cube);
private:
	time_t lastReward;
	deque<Position> vInputs;
	//deque<shared_ptr<Neuron>> vOutputs;
	deque<Position> vOutputs;
	vector<double> vOutputWeights;
	vector<InputEvent> vInputEvents;
	vector<double> vInputData;

	shared_ptr<OutputEventCatcher>  oCatcher;

	bool bNull;
	bool bPushed;
	bool bSetOEC;

	int iNumInputs;
	int iNumOutputs;
	int iInputsSinceLastReward;
	
	void Output(double data);
};
class NeuroCube;
class Algorithm
{
public:
	string Name;
	virtual void CalculateOutputs(vector<double> data, deque<Position> &outputs, vector<double> &outputWeights, vector<double> &outputValues, NeuroCube & cube)
	{

	}

};
class NeuroCube
{
public:
	deque<Neuron> DedicatedInputs;
	deque<Neuron> DedicatedOutputs;
	int Width;
	int Height;
	int Depth;
	deque<Neuron> Neurons;

	NeuroCube(int x, int y, int z, shared_ptr<Algorithm> alg);
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
	shared_ptr<Algorithm> algorithm;

	void MakeConnections();
};
