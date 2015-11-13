#include "gridlabd.h"
#include "hvacGame.h"
#include <math.h>

std::map<int, hvacGame::node> hvacGame::consumers;
std::map<int, hvacGame::generator> hvacGame::generators;

void hvacGame::init(){
	range = 2;
	totalLoad = 0;
	theta = 0.5;
	gama = 1.0;
	totalLoadCalculated = false;

}

void hvacGame::addElement(int id, double heatingSetPoint, double coolingSetpoint, double indoorTemp, double outdoorTemp, 
	double designCoolingCapacity, double designHeatingCapacity, double initCoolingCOP, double initHeatingCOP){
		node list;
		list.group = 2;
		list.name = id;
		list.indoorTemperature = indoorTemp;
		list.outdoorTemperature = outdoorTemp;
		list.coolingSetpoint = coolingSetpoint;
		list.heatingSetPoint = heatingSetPoint;
		list.designCoolingCapacity = designCoolingCapacity;
		list.designHeatingCapacity = designHeatingCapacity;
		list.initCoolingCOP = initCoolingCOP;
		list.initHeatingCOP = initHeatingCOP;
		consumers.insert(std::pair<int, node>(id, list));
		printf("addelement with id=%d, heatingSetPoint = %f, coolingSetpoint=%f, designCoolingCapacity=%f, designHeatingCapacity=%f \n", id, heatingSetPoint, coolingSetpoint, designCoolingCapacity, designHeatingCapacity);
}

void hvacGame::addGenerator(int id){
	generator list;
	list.name = id;
	generators.insert(std::pair<int, generator>(id, list));
}

void hvacGame::updateNodeTemperatures(int id, double indoortemp, double outdoortemp){
	totalLoadCalculated = false;
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		it->second.indoorTemperature = indoortemp;
		it->second.outdoorTemperature = outdoortemp;
		//printf("update indoor temperature for id=%d with indoor temperature = %f and outdoor temp = %f \n", id, indoortemp, outdoortemp);
	}
	
}

void hvacGame::updateNodeLoads(int id, double heatingDemand, double coolingDemand){
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		it->second.heatingDemand = heatingDemand;
		it->second.coolingDemand = coolingDemand;
	}
}

void hvacGame::updateSuply(int id, double generatorSupply){
	//printf("update supply, add %f \n", generatorSupply);
	std::map<int, generator>::iterator it;
	it = generators.find(id);
	if (it != generators.end()){
		it->second.supply = generatorSupply;
	}
}

void hvacGame::calculateTotalLoad(){
	if (!totalLoadCalculated){
		totalLoadCalculated = true;
		totalLoad = 0;
		for (std::map<int, node>::iterator itList = consumers.begin(); itList != consumers.end(); ++itList){
			double coolingmin = itList->second.coolingSetpoint - range;
			double coolingmax = itList->second.coolingSetpoint + range;
			double heatingmin = itList->second.heatingSetPoint - range;
			double heatingmax = itList->second.heatingSetPoint + range;
			//printf("heatingSetPoint = %f heatingmin = %f heatingmax = %f coolingSetpoint = %f coolingmin = %f coolingmax = %f\n", 
			//	itList->second.heatingSetPoint, heatingmin, heatingmax, itList->second.coolingSetpoint, coolingmin, coolingmax);
			if (itList->second.indoorTemperature > coolingmin){
				totalLoad += itList->second.coolingDemand;
			}
			else if (itList->second.indoorTemperature < heatingmax){
				totalLoad += itList->second.heatingDemand;
			}
		}

		/*for (std::map<int, generator>::iterator itList = generators.begin(); itList != generators.end(); ++itList){
			totalSupply += itList->second.supply;
		}*/
		if (totalLoad > 0.1){
			//printf("total load = %f total supply = %f \n", totalLoad, totalSupply);
			this->calculateNashLoad();
		}
	}
	
}

void hvacGame::calculateNashLoad(){
	
	double nashL = 0;
	double gamadoubled = pow(gama, 2.0);
	double arithmiths = 2*theta*gamadoubled*(totalLoad-totalSupply);
	double paronomasths = (consumers.size() + 1)*totalSupply;
	nashL = arithmiths / paronomasths;
	//printf("calculated nash l is : %f arithmitis = %f paronomasths = %f totalLoad = %f totalSupply = %f\n", nashL, arithmiths, paronomasths, totalLoad, totalSupply);
	for (std::map<int, node>::iterator itList = consumers.begin(); itList != consumers.end(); ++itList){
		itList->second.willWork = false;
		double calculatedLoad = 0.0;
		double coolingmin = itList->second.coolingSetpoint - range;
		double coolingmax = itList->second.coolingSetpoint + range;
		double heatingmin = itList->second.heatingSetPoint - range;
		double heatingmax = itList->second.heatingSetPoint + range;
		if (itList->second.indoorTemperature > coolingmin){
			calculatedLoad = ((2*theta*gamadoubled*itList->second.coolingDemand) - (nashL*totalLoad)) / ((2*theta*gamadoubled)+nashL);
			if (calculatedLoad >= itList->second.coolingDemand){
				itList->second.calculatedCoolingCapacity = itList->second.coolingDemand;
				itList->second.calculatedCoolingCOP = itList->second.initCoolingCOP;
				itList->second.calculatedHeatingCOP = itList->second.initHeatingCOP;
			}
			else{
				itList->second.calculatedCoolingCapacity = calculatedLoad;
				itList->second.calculatedCoolingCOP = calculateCoolingCOP(itList->first);
				itList->second.calculatedHeatingCOP = itList->second.initHeatingCOP;
			}
			if (calculatedLoad >= itList->second.coolingDemand){
				itList->second.willWork = true;
			}
			//printf("original load was : %f calculated load is : %f \n", itList->second.coolingDemand, calculatedLoad);
		}
		else if (itList->second.indoorTemperature < heatingmax){
			calculatedLoad = ((2 * theta*gamadoubled*itList->second.heatingDemand) - (nashL*totalLoad)) / ((2 * theta*gamadoubled) + nashL);

			if (calculatedLoad >= itList->second.heatingDemand){
				itList->second.calculatedHeatingCapacity = itList->second.heatingDemand;
				itList->second.calculatedCoolingCOP = itList->second.initCoolingCOP;
				itList->second.calculatedHeatingCOP = itList->second.initHeatingCOP;
			}
			else{
				itList->second.calculatedHeatingCapacity = calculatedLoad;
				itList->second.calculatedCoolingCOP = itList->second.initCoolingCOP;
				itList->second.calculatedHeatingCOP = calculateHeatingCOP(itList->first);
			}
			if (calculatedLoad >= itList->second.heatingDemand){
				itList->second.willWork = true;
			}
			//printf("original load was : %f calculated load is : %f \n", itList->second.heatingDemand, calculatedLoad);
		}
		
	}
}

bool hvacGame::checkGameResults(int id){
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		return it->second.willWork;
	}
	return true;
}

double hvacGame::getHeatingGameResult(int id){
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		return it->second.calculatedHeatingCOP;
	}
	return 0;
}

double hvacGame::getCoolingGameResult(int id){
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		return it->second.calculatedCoolingCOP;
	}
	return 0;
}

void hvacGame::setOnePrediction(char day[]){
	singlePrediction = predictor.getSinglePrediction(day);
}

void hvacGame::setSupplyPrediction(char day[]){
	//totalSupply = 5;
	totalSupply = predictor.getSingleLoadPrediction(day);
	//totalSupply = totalSupply - 2400;
	if (totalSupply < 0){
		totalSupply = 50;
	}
}

double hvacGame::getSinglePrediction(){
	return singlePrediction;
}

double hvacGame::lossfunction(int id){
	double result = 0;
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		double coolingmin = it->second.coolingSetpoint - range;
		double coolingmax = it->second.coolingSetpoint + range;
		double heatingmin = it->second.heatingSetPoint - range;
		double heatingmax = it->second.heatingSetPoint + range;
		if (it->second.indoorTemperature > coolingmin){
			double difference = it->second.indoorTemperature - it->second.coolingSetpoint;
			result = pow(difference, 2.0);
			printf("indoortemp = %f loss = %f coolingLoad = %f \n", it->second.indoorTemperature, result, it->second.coolingDemand);
		}
		else if (it->second.indoorTemperature < heatingmax){
			double difference = it->second.indoorTemperature - it->second.heatingSetPoint;
			result = pow(difference, 2.0);
			printf("indoortemp = %f loss = %f heatingLoad = %f \n", it->second.indoorTemperature, result, it->second.heatingDemand);
		}
		it->second.loss = result;
		
	}
	return result;
}

void hvacGame::clearLoads(){
	totalLoad = 0;
	totalSupply = 0;
}

double hvacGame::calculateHeatingCOP(int id){
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		double pTout = it->second.outdoorTemperature;
		double heatingCOP = (it->second.designHeatingCapacity*(0.34148808 + 0.00894102*(pTout)+0.00010787*(pTout)*(pTout))*
			(2.03914613 - 0.03906753*(pTout)+0.00045617*(pTout)*(pTout)-0.00000203*(pTout)*(pTout)*(pTout))) / it->second.calculatedHeatingCapacity*KWPBTUPH;
		return heatingCOP;
	}
	return 0;
}

double hvacGame::calculateCoolingCOP(int id){
	std::map<int, node>::iterator it;
	it = consumers.find(id);
	if (it != consumers.end()){
		double pTout = it->second.outdoorTemperature;
		double coolingCOP = (it->second.designCoolingCapacity * (1.48924533 - 0.00514995*(pTout)) * (-0.01363961 + 0.01066989*(pTout))) / it->second.calculatedCoolingCapacity*KWPBTUPH;
		return coolingCOP;
	}
	return 0;
}