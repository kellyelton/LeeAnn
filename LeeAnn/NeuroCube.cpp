#include "Neuron.h"
#include <iostream>

//x=Neurons Wide
//y=Neurons High
//z=Neurons Deep
//alg=General Algorithm for all neurons to follow
NeuroCube::NeuroCube(int width, int height, int depth,  shared_ptr<Algorithm> alg)
{
	Width = width;
	Height = height;
	Depth = depth;
	algorithm = alg;
	Neuron n;
	for(int z=0;z<depth;z++)
	{
		for(int y=0;y<height;y++)
		{
			for(int x=0;x<width;x++)
			{
				n = Neuron();
				n.MyPosition = Position(x,y,z);
				Neurons.push_back(n);
			}
		}
	}
	DedicatedInputs = deque<Neuron>();
	DedicatedOutputs = deque<Neuron>();
	NullNeuron = Neuron();
	NullNeuron.setNull(true);
	MakeConnections();
}


NeuroCube::~NeuroCube(void)
{

}

void NeuroCube::AddDedicatedInput(Neuron neuron, int x, int y, int z)
{
	if(z >= Depth || z < 0)
		return;
	if(y >= Height || y < 0)
		return;
	if(x >= Width || x < 0)
		return;
	DedicatedInputs.push_back(neuron);
		DedicatedInputs[DedicatedInputs.size() - 1].MyPosition = Position(-x,-y,-z);
	DedicatedInputs[DedicatedInputs.size() - 1].addOutput(Position(x,y,z),*this);
	
}
void NeuroCube::AddDedicatedOutput(Neuron neuron, int x, int y, int z)
{
	if(z >= Depth || z < 0)
		return;
	if(y >= Height || y < 0)
		return;
	if(x >= Width || x < 0)
		return;

	DedicatedOutputs.push_back(neuron);
	//Layers[z].getNeuron(x,y)->addOutput(DedicatedOutputs[DedicatedOutputs.size() - 1].MyPosition,*this);
		DedicatedOutputs[DedicatedOutputs.size() - 1].MyPosition = Position(-x,-y,-z);
	Neurons[XYZtoIndex(Position(x,y,z))].addOutput(DedicatedOutputs[DedicatedOutputs.size() - 1].MyPosition,*this);
	//TODO There should be a way to intercept the input to the output neuron, but there isn't yet.
}
void NeuroCube::RewardAllInputs(double reward)
{
	
	for(int i=0;i<DedicatedInputs.size();i++)
	{
		DedicatedInputs[i].Reward(reward,*this);
	}
}
int NeuroCube::getNeuron(int x, int y, int z)
{
	return getNeuron(Position(x,y,z));
}
int NeuroCube::getNeuron(Position pos)
{
	bool endit = false;
	if(pos.Z >= Depth || pos.Z < 0)
		endit = true;
	if(pos.Y >= Height || pos.Y < 0)
		endit = true;
	if(pos.X >= Width || pos.X < 0)
		endit = true;
	if(endit)
	{
		return -1;
	}
	return XYZtoIndex(pos);
}
void NeuroCube::Update(NeuroCube & cube)
{
	//Neurons[0].Push(algorithm,cube);
	for(int i=0;i<DedicatedInputs.size();i++)
	{
		DedicatedInputs[i].Push(algorithm, cube);
	}
	for(int i=0;i<Neurons.size();i++)
	{
		Neurons[i].Push(algorithm,cube);
	}
	for(int i=0;i<DedicatedOutputs.size();i++)
	{
		DedicatedOutputs[i].Push(algorithm,cube);
	}
	return;
}
void NeuroCube::MakeConnections()
{
	for(int layer=0;layer<Depth;layer++)
	{
		for(int y=0;y<Height;y++)
		{
			for(int x=0;x<Width;x++)
			{
				//Top Right
				int n = getNeuron(x,y,layer);
				Neuron * ner = &Neurons[n];
				if(y > 0 && x < Width-1)
					ner->addInputOutput(Position(x+1,y-1,layer),*this);
				//Middle Right
				if(x < Width -1)
					ner->addInputOutput(Position(x+1,y,layer),*this);
				//Bottom Right
				if(x < Width -1 && y < Height-1)
					ner->addInputOutput(Position(x+1,y+1,layer),*this);
				//Bottom Middle
				if(y < Height-1)
					ner->addInputOutput(Position(x,y+1,layer),*this);
				if(layer<Depth-1)
				{
					//Top Left
					if(x > 0 && y > 0)
						ner->addInputOutput(Position(x-1,y-1,layer+1),*this);
					//Top Middle
					if(y > 0)
						ner->addInputOutput(Position(x,y-1,layer+1),*this);
					//Top Right
					if(y > 0 && x+1 < Width-1)
						ner->addInputOutput(Position(x+1,y-1,layer+1),*this);
					//Middle Right
					if(x+1 < Width -1)
						ner->addInputOutput(Position(x+1,y,layer+1),*this);
					//Bottom Right
					if(x+1 < Width -1 && y+1 < Height-1)
						ner->addInputOutput(Position(x+1,y+1,layer+1),*this);
					//Bottom Middle
					if(y+1 < Height-1)
						ner->addInputOutput(Position(x,y+1,layer+1),*this);
					//Bottom Left
					if(x > 0 && y+1 < Height-1)
						ner->addInputOutput(Position(x-1,y+1,layer+1),*this);
					//Middle Left
					if(x > 0)
						ner->addInputOutput(Position(x-1,y,layer+1),*this);
					//Middle Middle
					ner->addInputOutput(Position(x,y,layer+1),*this);
				}
				//delete &ner;
			}
		}
	}
}
int NeuroCube::XYZtoIndex(Position p)
{
	int x = p.X;
	int y = p.Y;
	int z = p.Z;
	return x+(y * Height)+(Width-Height)+(z*Width*Height);
}