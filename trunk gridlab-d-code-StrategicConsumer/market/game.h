#include <stdarg.h>
#include <map>
#include <vector>
#include "gridlabd.h"
#include "trainer.h"
#include "market.h"

#ifndef _market_game_theory_h_
#define _market_game_theory_h_
class game
{
public:
	trainer predictor;

	typedef struct nodes
	{
		int name;
		int group;
		int lastWorkingHour;
		int firstWorkingHourCalculated;
		double temperature;
		double capacity;
		std::vector<double> start;
		std::vector<bool> work;
	}node;
	void init();
	void addElement(char name[], int id, double pmonitor, double capacity);
	void setPredictions(char day[]);
	void game::setOnePrediction(char day[]);
	double game::getSinglePrediction();
	void printLists();
	std::vector<bool> getVector(int id);
	void setWorkingHours();
	void updateNodeTemperature(int id, double temp);
private:
	double comfortZone;
	double maxtemp;
	double waitingfactor;
	double tempRaisingRate;
	double defaultCapacity;
	double singlePrediction;
	int addElementCounter;
	int dayCounter;
	int random(int min, int max);
	void sortPrices();
	int mostExpensiveTimeframe;
	bool iterate(int iterationPoint);
	void beginIteration();
	int utilityFunction(double temp, double capacity, int endTime);
	std::vector<std::pair<int, double>> game::utilityFunctionForIterations(double temp, double capacity, int endTime);
	static std::map<int, node> list1;
	static std::vector<std::pair<int, game::node>> list1sortedByTemperature;
	static std::map<int, node> list2;
	static std::map<int, node> list3;
	static std::map<int, node> list4;
	static std::map<int, node> list5;
	static std::vector<std::pair<int, game::node>> list2sortedByTemperature;
	static std::vector<double> predictions;
	static std::vector<double> sortedPredictedPrices;

};

#endif