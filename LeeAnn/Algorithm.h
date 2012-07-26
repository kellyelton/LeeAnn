#pragma once
/*
#include <string.h>
#include <vector>
#include "Neuron.h"
using namespace std;
class Algorithm
{
public:
	string Name;
	virtual void CalculateOutputs(vector<double> data, vector<Neuron*> &outputs, vector<double> &outputWeights, vector<double> &outputValues)
	{
		double activation = 0;
		for(int i=0;i<data.size();++i)
		{
			activation += data[i];
		}
		double a2 = 0;
		for(int i=0;i<outputs.size();i++)
		{
			a2 = activation * outputWeights[i];
			outputs[i]->Input(( 1 / ( 1 + exp(-a2 / 1.0))));
		}
	}
	
};*/