#pragma once
#include <time.h>
#include <functional>
#include <vector>
#include <deque>
#include <boost/shared_ptr.hpp>
#include "NeuronMath.h"
//#include "Algorithm.h"
class Algorithm;
class Neuron;
using namespace std;
class Baby
{
public:
	deque<shared_ptr<Baby>> vInputs;
	int X;
	int Y;
	int Z;
	Baby():numInputs(0),isNull(false),wasTickled(false),X(0),Y(0),Z(0)
	{

	}
	void addInput(shared_ptr<Baby> baby)
	{
		if(numInputs == 0)
			vInputs = deque<shared_ptr<Baby>>(0);
		vInputs.push_back(baby);
		numInputs++;
	}
	void setXYZ(int x, int y, int z)
	{
		X = x;
		Y = y;
		Z = z;
	}
	void Tickle()
	{
		if(!wasTickled)
			wasTickled = true;
		else
			return;
		for(int i=0;i<numInputs;i++)
		{
			vInputs[i]->Tickle();
		}
	}
	void setNull(bool isnull)
	{
		isNull = isnull;
	}
private:
	int numInputs;
	bool isNull;
	bool wasTickled;
};
class BabyLayer
{
public:
	int Width;
	int Height;
	BabyLayer()
	{
		Width = 0;
		Height = 0;
	}
	BabyLayer(int width, int height)
	{
		Width = width;
		Height = height;
		vecBabies = std::deque<deque<Baby>>(0);
		for(int i=0;i<height;i++)
		{
			deque<Baby> row = deque<Baby>(0);
			for(int i=0;i<width;i++)
			{
				row.push_back(Baby());
			};
			vecBabies.push_back(row);
		}
		MakeConnections();
	}

	Baby * getBaby(int x, int y)
	{
		Baby n = Baby();
		n.setNull(true);
		if(x >= Width || x <0)
			return &n;
		if(y >= Height || y < 0)
			return &n;
		n.setNull(false);
		return &vecBabies[y][x];
	}
	~BabyLayer(void)
	{

	}
private:
	std::deque<deque<Baby>> vecBabies;
	void MakeConnections()
	{
		for(int y=0;y<Height;y++)
		{
			for(int x=0;x<Width;x++)
			{
				//Top Right
				if(y > 0 && x < Width-1)
					vecBabies[y][x].addInput(shared_ptr<Baby>(&vecBabies[y-1][x+1]));
				//Middle Right
				if(x < Width -1)
					vecBabies[y][x].addInput(shared_ptr<Baby>(&vecBabies[y][x+1]));
				//Bottom Right
				if(x < Width -1 && y < Height-1)
					vecBabies[y][x].addInput(shared_ptr<Baby>(&vecBabies[y+1][x+1]));
				//Bottom Middle
				if(y < Height-1)
					vecBabies[y][x].addInput(shared_ptr<Baby>(&vecBabies[y+1][x]));
			}
		}
	}
};
class BabyCube
{
public:
	int X;
	int Y;
	int Z;
	BabyCube(int x, int y, int z)
	{
		X = x;
		Y = y;
		Z = z;
		Layers = deque<BabyLayer>();
		for(int i=0;i<z;i++)
		{
			BabyLayer lay = BabyLayer(x,y);
			Layers.push_back(lay);
		}
		NullBaby = Baby();
		NullBaby.setNull(true);
		MakeConnections();
	}
	void MakeConnections()
	{
		int l = Layers.size();
		if(l == 0 || l == 1)
			return;
		for(int layer=0;layer<l;layer++)
		{
			BabyLayer * lay = &Layers[layer];
			if(layer< l-1)
			{
				for(int y=0;y<lay->Height;y++)
				{
					for(int x=0;x<lay->Width;x++)
					{
						//Top Left
						if(x > 0 && y > 0)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x-1,y-1)));
						//Top Middle
						if(y > 0)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x,y-1)));
						//Top Right
						if(y > 0 && x+1 < lay->Width-1)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x+1,y-1)));
						//Middle Right
						if(x+1 < lay->Width -1)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x+1,y)));
						//Bottom Right
						if(x+1 < lay->Width -1 && y+1 < lay->Height-1)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x+1,y+1)));
						//Bottom Middle
						if(y+1 < lay->Height-1)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x,y+1)));
						//Bottom Left
						if(x > 0 && y+1 < lay->Height-1)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x-1,y+1)));
						//Middle Left
						if(x > 0)
							Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x-1,y)));
						//Middle Middle
						Layers[layer].getBaby(x,y)->addInput(shared_ptr<Baby>(Layers[layer+1].getBaby(x,y)));
					}

				}
			}
		}
	}
	Baby * getBaby(int x, int y, int z)
	{

		if(z >= Layers.size() || z < 0)
			return &NullBaby;
		if(y >= Layers[z].Height || y < 0)
			return &NullBaby;
		if(x >= Layers[z].Width || x < 0)
			return &NullBaby;

		return Layers[z].getBaby(x,y);
	}
	void Update()
	{

	}
	~BabyCube(void)
	{

	}
private:
	deque<BabyLayer> Layers;
	Baby NullBaby;
};