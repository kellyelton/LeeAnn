#include <stdio.h>
#include <allegro5/allegro.h>
#include "Neuron.h"
#include "Maths.h"
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BOUNCER_SIZE = 32;

ALLEGRO_BITMAP *bImage =NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;
bool redraw = true;
class OEC: public OutputEventCatcher
{
public:
	void onOutput(vector<double> outputs)

	{
		/*
		cout << "Output: ";
		for(int i=0;i<outputs.size();i++)
		{
			cout << outputs[i];
		}
		cout << "\n";
		*/
	}
};
class Algr : public Algorithm
{
public:
	Algr()
	{
		Name = "Algr";
	}
	void CalculateOutputs(vector<double> data, deque<Position> &outputs, vector<double> &outputWeights, vector<double> &outputValues, NeuroCube & cube)
	{
		double activation = 0;
		for(int i=0;i<data.size();++i)
		{
			activation += data[i];
		}
		//std::cout << "data size: " << data.size() << "\n";
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
						outputValues.push_back(e);
						break;
					}
				}
			}
			else
			{
				cube.Neurons[illshit].Input(e);
				outputValues.push_back(e);
			}
			//pt++;

		}
		if(outputs.size() == 0)
		{
			a2 = activation * 1;
			double e = exp(-a2 / 1.0);
			e = e;
			e = 1.0 + e;
			e = e;
			e = 1.0/e;
			for(int i=0;i<data.size();i++)
				outputValues.push_back(data[i]);
		}

	}
};
class Creature
{
public:
	ALLEGRO_BITMAP *bImage;
	Maths::Position pLocation;
	NeuroCube ncBrain;
	shared_ptr<Algr> Alg;
	float DX;
	float DY;
	double curRotation ;
	double curSpeed ;
	shared_ptr<Neuron> ninFoodXY;
	shared_ptr<Neuron> ninMyXY;

	shared_ptr<Neuron> noutX;
	shared_ptr<Neuron> noutY;
	shared_ptr<OEC> oec;
	OEC roec;
	vector<double> prevOutput;
	Algr alg;


	Creature():alg(Algr()),ncBrain(NeuroCube(5,5,5,shared_ptr<Algr>(&alg)))
	{
		pLocation = Maths::Position(SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0,SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0);
		DX = -4.0;
		DY = 4.0;
		curRotation = 0;
		curSpeed = 0;
		prevOutput = vector<double>();
		prevOutput.push_back(0);
		prevOutput.push_back(0);
		ncBrain.AddDedicatedInput(Neuron(),0,0,0);
		ncBrain.AddDedicatedInput(Neuron(),1,0,0);
		ncBrain.AddDedicatedOutput(Neuron(),4,4,4);
		ncBrain.AddDedicatedOutput(Neuron(),3,4,4);
		ninFoodXY = shared_ptr<Neuron>(&ncBrain.DedicatedInputs[0]);
		ninMyXY = shared_ptr<Neuron>(&ncBrain.DedicatedInputs[1]);
		noutX = shared_ptr<Neuron>(&ncBrain.DedicatedOutputs[0]);
		noutY =shared_ptr<Neuron>(&ncBrain.DedicatedOutputs[1]);
		roec = OEC();
		oec = shared_ptr<OEC>(&roec);
		noutX->setOutputEventCatcher(oec);
	}
	void Update(Maths::Position closestFood)
	{
		vector<double> loc = vector<double>();
		loc.push_back(pLocation.X);
		loc.push_back(pLocation.Y);
		//ninMyXY->Input(loc);
		ncBrain.DedicatedInputs[1].Input((loc));
		loc = vector<double>();
		loc.push_back(closestFood.X);
		loc.push_back(closestFood.Y);
		//ninFoodXY->Input(loc);
		ncBrain.DedicatedInputs[0].Input((loc));
		ncBrain.Update(ncBrain);
		if(noutX->Outputs.size() > 0 && noutY->Outputs.size() > 0)
		{
			UpdateLocation(ncBrain.DedicatedOutputs[0].Outputs[0], ncBrain.DedicatedOutputs[1].Outputs[0] );
			prevOutput = vector<double>();
			prevOutput.push_back(noutX->Outputs[0]);
			prevOutput.push_back(noutY->Outputs[0]);
			ncBrain.DedicatedOutputs[0].Outputs = vector<double>();
			ncBrain.DedicatedOutputs[1].Outputs = vector<double>();

		}

	}
	void UpdateLocation(double lside, double rside)
	{

		//calculate steering forces
		double RotForce = lside - rside;

		//clamp rotation
		Maths::Clamp(RotForce, -1.0, 1.0);

		curRotation += RotForce;

		curSpeed = (lside + rside);	
		Maths::Position lookat = Maths::Position();
		//update Look At 
		lookat.X = -sin(curRotation);
		lookat.Y = cos(curRotation);
		lookat.X *= curSpeed;
		lookat.Y *= curSpeed;

		//update position
		pLocation += lookat ;
	}
private:

};

Creature creature = Creature();

int init()
{
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	timer = al_create_timer(2.0 / FPS);
	if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon
	font = al_load_ttf_font("pirulen.ttf",20,0 ); // load the font | Font from Larabie Free Fonts

	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	creature.bImage = al_create_bitmap(32, 32);
	if(!creature.bImage) {
		fprintf(stderr, "failed to create bouncer bitmap!\n");
		al_destroy_bitmap(creature.bImage);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	bImage = al_create_bitmap(12, 12);
	if(!bImage) {
		fprintf(stderr, "failed to create creature bitmap!\n");
		al_destroy_bitmap(creature.bImage);
		al_destroy_bitmap(bImage);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_set_target_bitmap(creature.bImage);

	al_clear_to_color(al_map_rgb(255, 0, 255));
	al_set_target_bitmap(bImage);

	al_clear_to_color(al_map_rgb(0, 255, 0));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_bitmap(creature.bImage);
		al_destroy_bitmap(bImage);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_clear_to_color(al_map_rgb(0,0,0));

	al_flip_display();

	al_start_timer(timer);
}
string dtos(double d)
{
	ostringstream buffer;

	//send the int to the ostringstream
	buffer << d;	

	//capture the string
	return buffer.str();	
}
int main(int argc, char **argv)
{
	init();
	
	Maths::Position food = Maths::Position(1,1);
	float DX,DY;
	DX = -4.0;
	DY = 4.0;
	while(1)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);



		if(ev.type == ALLEGRO_EVENT_TIMER) {
			if(food.X < 0 || food.X > SCREEN_W - 12) {
				DX= -DX;
			}

			if(food.Y < 0 || food.Y > SCREEN_H - 12) {
				DY = -DY;
			}

			food.X +=DX;
			food.Y += DY;
			if(creature.pLocation.X>= food.X - 32 && creature.pLocation.Y >= food.Y-32 && creature.pLocation.X <= food.X + 32 && creature.pLocation.Y <= food.Y + 32)
			{
				std::cout << "Reward: " << .005 << "\n";
				creature.ncBrain.RewardAllInputs(.005);
			}
			redraw = true;
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		if(redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0,0,0));

			al_draw_bitmap(creature.bImage, creature.pLocation.X, creature.pLocation.Y, 0);
			al_draw_bitmap(bImage, food.X, food.Y, 0);

			ostringstream buffer;
			buffer << "Creature: x=" + dtos(creature.prevOutput[0]) + " y=" + dtos(creature.prevOutput[1]);	
			al_draw_text(font, al_map_rgb(255,255,255), 0, 0,ALLEGRO_ALIGN_LEFT, buffer.str().c_str());// draw the text

			al_flip_display();
									creature.Update(food);
		}
				
	}

	al_destroy_bitmap(creature.bImage);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}