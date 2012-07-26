#include <iostream>
#include "Neuron.h"

using namespace std;
bool GotOutput = false;
class oEvent : public OutputEventCatcher
{
	void onOutput(vector<double> outputs)
	{
		for(int i=0;i<outputs.size();i++)
		{
			std::cout << "o" << i << ": " << NeuronMath::dtos(outputs[i]) << "\n";
			GotOutput = true;
		}
		
	}
};
class Basic  : public Algorithm
{
public:
	Basic()
	{
		Name = "Basic";
	}
	void CalculateOutputs(vector<double> data, deque<Position> &outputs, vector<double> &outputWeights, vector<double> &outputValues, NeuroCube & cube)
	{
		double activation = 0;
		for(int i=0;i<data.size();++i)
		{
			activation += data[i];
		}
		std::cout << "data size: " << data.size() << "\n";
		double a2 = 0;
		//Neuron * pt = &outputs[0];
		for(int i=0;i<outputs.size();i++)
		{
			a2 = activation * outputWeights[i];
			double e = exp(-a2 / 1.0);
			e = e;
			e = 1.0 + e;
			e = e;
			e = 1.0/e;
			int illshit= cube.getNeuron(outputs[i]);
			Neuron * n = &Neuron();
			if(illshit == -1)
			{
				for(int o=0;o<cube.DedicatedOutputs.size();o++)
				{
					if(cube.DedicatedOutputs[o].MyPosition == outputs[i])
					{
						n = &cube.DedicatedOutputs[o];
						cube.DedicatedOutputs[o].Input(e);
						break;
					}
				}
			}
			else
				cube.Neurons[illshit].Input(e);
			//pt++;
		}

	}
};

int main( int argc, const char* argv[])
{
	Basic b = Basic();
	Neuron ni = Neuron();
	//Neuron ni2 = Neuron();
	Neuron out = Neuron();
	oEvent oe = oEvent();
	out.setOutputEventCatcher(shared_ptr<OutputEventCatcher>(&oe));
	NeuroCube nc = NeuroCube(3,3,3,shared_ptr<Algorithm>(&b));
	nc.AddDedicatedInput(ni,0,0,0);
	//nc.AddDedicatedInput(ni2,0,0,0);
	nc.AddDedicatedOutput(out,1,1,1);
		bool done = false;
	int TestCount = 0;
	GotOutput = true;
	system("Pause");
	while(!done)
	{
		if(GotOutput)
		{
			int rand = abs(NeuronMath::RandomClamped() * 100);
			int rand2 = abs(NeuronMath::RandomClamped() * 100);
			cout << "\n\nTest " << TestCount << "\n--------------------\n";
			cout << "Equation: " << rand << "+" << rand2 << "=" << rand+rand2 << "\n";
			nc.DedicatedInputs[0].Input(rand);
			nc.DedicatedInputs[0].Input(rand2);
			//ni2.Input(rand2);
			GotOutput = false;
			TestCount++;
		}
		nc.Update(nc);
		system("Pause");
	}
	system("Pause");
	return 0;
}