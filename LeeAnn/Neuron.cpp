#include "Neuron.h"
#include <iostream>
using namespace std;
class NeuroCube;
InputEvent::InputEvent()
{
	
}
InputEvent::InputEvent(vector<double> data, vector<double>outputweights,vector<double> outputdata)
{
	Data = data;
	OutputWeights = outputweights;
	OutputData = outputdata;
	seconds = time(NULL);
}


Neuron::Neuron():MyPosition(Position(-1,-1,-1)),vOutputWeights(vector<double>(0)),vInputEvents( vector<InputEvent>(0)),vInputData( vector<double>(0)),iNumInputs(0),
	iNumOutputs(0),lastReward(time(NULL)),iInputsSinceLastReward(0),bNull(false),bPushed(false),bSetOEC(false){}

Neuron::Neuron( const Neuron * n)
{
	this->vOutputWeights = n->vOutputWeights;
	this->vInputEvents = n->vInputEvents;
	vInputData = n->vInputData;
	iNumInputs = n->iNumInputs;
	iNumOutputs = n->iNumOutputs;
	lastReward = n->lastReward;
	iInputsSinceLastReward = n->iInputsSinceLastReward;
	bNull = n->bNull;
	bPushed = n->bPushed;
	bSetOEC = n->bSetOEC;
	oCatcher = n->oCatcher;
	Outputs = n->Outputs;
	MyPosition = n->MyPosition;
	vInputs = n->vInputs;
	vOutputs = n->vOutputs;
};
Neuron::~Neuron()
{
	
}

void Neuron::setNull(bool null)
{
	bNull = null;
}
bool Neuron::isNull()
{
	return bNull;
}
void Neuron::Push(shared_ptr<Algorithm> alg, NeuroCube & cube)
{
	if(bSetOEC)
		bSetOEC = true;
	if(!bPushed)
	{
		//std::cout << "Pushing " << X << " " << Y << " "<< Z << "\n";
		//bPushed = true;
		//if(bSetOEC)
			//bSetOEC = true;
		if(vInputData.size() > 0)
		{
			for(int i=0;i<vOutputs.size();i++)
			{
				int ner = cube.getNeuron(vOutputs[i]);
				Neuron * n = &Neuron();
				if(ner == -1)
				{
					for(int o=0;o<cube.DedicatedOutputs.size();o++)
					{
						if(cube.DedicatedOutputs[o].MyPosition == vOutputs[i])
						{
							n = &cube.DedicatedOutputs[o];
							break;
						}
					}
				}
				else
					n = &cube.Neurons[ner];
				if(n->bSetOEC)
					int a = 1;
				//if(!n->WasPushed())
					//n->Push(alg, cube);
				//delete &n;
			}
			Outputs = vector<double>();
			alg->CalculateOutputs(vInputData,vOutputs,vOutputWeights,Outputs, cube);
			InputEvent ie = InputEvent(vInputData,vOutputWeights,Outputs);
			vInputEvents.push_back(ie);
			iInputsSinceLastReward++;
			if(bSetOEC)
			{
				oCatcher->onOutput(vInputData);
				vInputData = vector<double>();
			}
			vInputData = vector<double>();
		}

	}
}
bool Neuron::WasPushed()
{
	return bPushed;
}
void Neuron::setOutputEventCatcher(shared_ptr<OutputEventCatcher> oec)
{
	oCatcher = shared_ptr<OutputEventCatcher>(oec);
	bSetOEC = true;
}
void Neuron::Wait(NeuroCube & cube)
{
	if(bPushed)
	{
		//std::cout << "Waited\n";
		bPushed = false;
		for(int i=0;i<iNumOutputs;i++)
		{
			int num = cube.getNeuron(vOutputs[i]);
			if(num != -1)
				cube.Neurons[num].Wait(cube);
			else
			{
				for(int o=0;o<cube.DedicatedOutputs.size();o++)
				{
					if(cube.DedicatedOutputs[o].MyPosition == vOutputs[i])
					{
						cube.DedicatedOutputs[o].Wait(cube);
						break;
					}
					
				}
			}
		}
	}
}
void Neuron::Input(double data)
{
	vInputData.push_back(data);
	//std::cout << "Got input " << data << " whogotit " << MyPosition.X << " " << MyPosition.Y << " "<< MyPosition.Z << "\n";
}
void Neuron::Input(vector<double> data)
{
	for(int i=0;i<data.size();i++)
		Input(data[i]);
}
void Neuron::addInput(Position fromNeuron, NeuroCube & cube)
{
	if(iNumInputs == 0)
	vInputs = deque<Position>();
	vInputs.push_back(fromNeuron);
	Neuron n = Neuron(this);
	int num = cube.getNeuron(fromNeuron);
	cube.Neurons[num].addOutput(MyPosition, cube);
	iNumInputs++;
}
void Neuron::addOutput(Position toNeuron, NeuroCube & cube)
{
	//std::cout << "added output " << "\n";
	addOutput(toNeuron,NeuronMath::RandomClamped(), cube);
}
void Neuron::addOutput(Position toNeuron, double weight, NeuroCube & cube)
{
	//std::cout << "added output " << "\n";
	if(iNumOutputs == 0)
		vOutputs = deque<Position>(0);
	vOutputs.push_back(toNeuron);
	vOutputWeights.push_back(weight);
	iNumOutputs++;
}
void Neuron::addInputOutput(Position  theNeuron, NeuroCube & cube)
{
	//addInput(theNeuron);
	addOutput(theNeuron,cube);	
	//TODO Add output from nueron again.
	//cube.Neurons[cube.getNeuron(theNeuron)].addOutput(MyPosition,cube);
}
void Neuron::Reward(double reward, NeuroCube & cube)
{
	InputEvent * ie = &vInputEvents[vInputEvents.size()-1];
	int c = vInputs.size() / 2;
	if(c == 0)
		c = 1;
	vector<int> temp = vector<int>(c);
	double decrese = reward / iInputsSinceLastReward;
	for(int ev=0;ev<iInputsSinceLastReward;ev++)
	{
		ie--;
		if(ie->seconds <= lastReward)
			break;
		for(int i=0;i<c;i++)
		{
			int highest = 0;
			for(int o=0;o<iNumOutputs;o++)
			{
				if(vOutputWeights[o] > vOutputWeights[highest])
				{
					bool gotit = false;
					for(int b=0;b<c;b++)
					{
						if(temp[b] == i)
						{
							gotit = true;
							break;
						}
					}
					if(!gotit)
					{
						highest = o;
					}
				}
			}
			temp[i] = highest;
			vOutputWeights[highest] += reward;
			//int num = cube.getNeuron(vOutputs[highest]);
			//Neuron * ner = &cube.Neurons[num];
			//ner->Reward(reward,cube);
			//delete &ner;
			//vOutputs[highest]->Reward(reward);
			if(vOutputWeights[highest] > 1)
				vOutputWeights[highest] = 1;
		}
		for(int i=0;i<c;i++)
		{
			bool gotit = false;
			for(int b=0;b<c;b++)
			{
				if(temp[b] == i)
				{
					gotit = true;
					break;
				}
			}
			if(!gotit)
			{
				vOutputWeights[i] -= reward;
				if(vOutputWeights[i] < -1)
					vOutputWeights[i] = -1;
				
				cube.Neurons[cube.getNeuron(vOutputs[i])].Reward(reward,cube);
			}
		}
		reward -=decrese;
	}
}