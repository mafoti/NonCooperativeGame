#include <stdarg.h>
#include <map>
#include <vector>
#include "gridlabd.h"
#include "trainer.h"
#include "market.h"

#ifndef _market_hvac_game_theory_h_
#define _market_hvac_game_theory_h_
#define BTUPHPW		(3.4120)
#define KWPBTUPH	(1e-3/BTUPHPW)
class hvacGame
{
public:
	trainer predictor;

	typedef struct nodes
	{
		int name;
		int group;
		double indoorTemperature;
		double outdoorTemperature;
		double heatingSetPoint;
		double coolingSetpoint;
		double heatingDemand;
		double coolingDemand;
		bool willWork; 
		double designCoolingCapacity;
		double designHeatingCapacity;
		double initCoolingCOP;
		double initHeatingCOP;
		double calculatedCoolingCOP;
		double calculatedHeatingCOP;
		double calculatedCoolingCapacity;
		double calculatedHeatingCapacity;
		double loss;
	}node;
	typedef struct ganerators
	{
		int name;
		double supply;
	}generator;
	void init();
	void addGenerator(int id);
	void addElement(int id, double heatingSetPoint, double coolingSetpoint, double indoorTemp, double outdoorTemp, double designCoolingCapacity, double designHeatingCapacity, double initCoolingCOP, double initHeatingCOP);
	void updateNodeTemperatures(int id, double indoortemp, double outdoortemp);
	void updateNodeLoads(int id, double heatingDemand, double coolingDemand);
	void updateSuply(int id,double generatorSupply);
	void calculateTotalLoad();
	void calculateNashLoad();
	bool checkGameResults(int id);
	double getHeatingGameResult(int id);
	double getCoolingGameResult(int id);
	void setOnePrediction(char day[]);
	void setSupplyPrediction(char day[]);
	double getSinglePrediction();
	double lossfunction(int id);
	double calculateCoolingCOP(int id);
	double calculateHeatingCOP(int id);
	void clearLoads();
	static std::map<int, node> consumers;
	static std::map<int, generator> generators;
private:
	bool totalLoadCalculated;
	double totalSupply;
	double totalLoad;
	double singlePrediction;
	double range;
	double theta;
	double gama;
};

#endif