#include "gridlabd.h"
#include "game.h"
#include <math.h>
#include <algorithm> 

std::map<int, game::node> game::list1;
std::map<int, game::node> game::list2;
std::map<int, game::node> game::list3;
std::map<int, game::node> game::list4;
std::map<int, game::node> game::list5;
std::vector<double> game::predictions;
std::vector<double> game::sortedPredictedPrices;
std::vector<std::pair<int, game::node>> game::list1sortedByTemperature;
std::vector<std::pair<int, game::node>> game::list2sortedByTemperature;

void game::init(){
	addElementCounter = 0;
	dayCounter = 0;
	comfortZone = 5;
	maxtemp = 110.5;
	waitingfactor = 1.02;
	tempRaisingRate = 0.05;
	printf("id,temp,firstWorkingHourCalculated\n");
}

void game::addElement(char name[], int id, double temperature, double capacity){
	std::string member1 = "controller5B_tm_B_5_645";
	std::string member2 = "controller16B_tm_B_16_645";
	std::string member3 = "controller16B_tm_B_16_646";
	std::string member4 = "controller6A_tm_A_6_652";
	std::string member5 = "controller24A_tm_A_24_652";
	std::string member6 = "controller32A_tm_A_32_652";
	std::string member7 = "controller34A_tm_A_34_652";
	//if (member1.compare(name) == 0 || member2.compare(name) == 0 || member3.compare(name) == 0 || member4.compare(name) == 0 || member5.compare(name) == 0 || member6.compare(name) == 0 || member7.compare(name) == 0){

	/*double temp = maxtemp - temperature;
	int firstId = (int)(temp / 0.11);
	node list;
	list.group = 1;
	list.name = id;
	list.temperature = temperature;
	list.firstWorkingHourCalculated = firstId;
	list.capacity = capacity;
	list1.insert(std::pair<int, node>(id, list));*/
	

	if (temperature <= 106.5 && temperature>105){

		double temp = maxtemp - temperature;
		int firstId = (int)(temp / 0.11);
		node list;
		list.group = 1;
		list.name = id;
		list.temperature = temperature;
		list.firstWorkingHourCalculated = firstId;
		list.capacity = capacity;
		list1.insert(std::pair<int, node>(id, list));
	}
	else{
		double temp = maxtemp - temperature;
		int firstId = (int)(temp / 0.11);
		node list;
		list.group = 2;
		list.name = id;
		list.temperature = temperature;
		list.firstWorkingHourCalculated = firstId;
		list.capacity = capacity;
		list2.insert(std::pair<int, node>(id, list));
	}
	/*else if (temperature >= 104 && temperature<106){
		double temp = maxtemp - temperature;
		int firstId = (int)(temp / 0.11);
		node list;
		list.group = 3;
		list.name = id;
		list.temperature = temperature;
		list.firstWorkingHourCalculated = firstId;
		list.capacity = capacity;
		list3.insert(std::pair<int, node>(id, list));
	}
	else if (temperature >= 106 && temperature<108){
		double temp = maxtemp - temperature;
		int firstId = (int)(temp / 0.11);
		node list;
		list.group = 4;
		list.name = id;
		list.temperature = temperature;
		list.firstWorkingHourCalculated = firstId;
		list.capacity = capacity;
		list4.insert(std::pair<int, node>(id, list));
	}
	else if (temperature >= 108){
		double temp = maxtemp - temperature;
		int firstId = (int)(temp / 0.11);
		node list;
		list.group = 5;
		list.name = id;
		list.temperature = temperature;
		list.firstWorkingHourCalculated = firstId;
		list.capacity = capacity;
		list5.insert(std::pair<int, node>(id, list));
	}*/

	addElementCounter++;
}

void game::setPredictions(char day[]){
	
	if ((dayCounter % 96) == 0){
		predictions = predictor.getPredictions(day);
		setWorkingHours();
	}
	dayCounter++;
}

void game::setOnePrediction(char day[]){
	singlePrediction = predictor.getSinglePrediction(day);
}

double game::getSinglePrediction(){
	return singlePrediction;
}

void game::printLists(){

	printf("list1 : \n");
	for (std::map<int, node>::iterator it = list1.begin(); it != list1.end(); ++it)
		printf("id = %d temperature = %f capacity=%f \n", it->first, it->second.temperature, it->second.capacity);

	printf("\n predicted prices : \n");
	for (std::vector<double>::iterator it = predictions.begin(); it != predictions.end(); ++it){
		printf("%f , ", *it);
	}

}

std::vector<bool> game::getVector(int id){
	std::map<int, node>::iterator it;
	it = list1.find(id);
	if (it != list1.end())
		return it->second.work;

	it = list2.find(id);
	if (it != list2.end())
		return it->second.work;

	it = list3.find(id);
	if (it != list3.end())
		return it->second.work;

	it = list4.find(id);
	if (it != list4.end())
		return it->second.work;

	it = list5.find(id);
	if (it != list5.end())
		return it->second.work;

}

void game::setWorkingHours(){
	std::vector<bool> defaultvector(96, false);
	/*for (std::map<int, node>::iterator itList = list1.begin(); itList != list1.end(); ++itList){
	itList->second.work.clear();
	itList->second.work = defaultvector;
	int start = utilityFunction(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
	start = 95 - start;
	//printf("%d ", start);
	if (start > -1){
	itList->second.work[start] = true;
	itList->second.work[start - 1] = true;
	itList->second.work[start - 2] = true;
	}
	}*/

	for (std::map<int, node>::iterator itList = list1.begin(); itList != list1.end(); ++itList){
		itList->second.work.clear();
		itList->second.work = defaultvector;
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		int start = 95 - startVector[0].first;
		
		if (start > -1){
			itList->second.work[start] = true;
			itList->second.work[start - 1] = true;
			itList->second.work[start - 2] = true;
		}
	}

	for (std::map<int, node>::iterator itList = list2.begin(); itList != list2.end(); ++itList){
		itList->second.work.clear();
		itList->second.work = defaultvector;
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		int start = 95 - startVector[0].first;
		
		if (start > -1){
			itList->second.work[start] = true;
			itList->second.work[start - 1] = true;
			itList->second.work[start - 2] = true;
		}
	}

	for (std::map<int, node>::iterator itList = list3.begin(); itList != list3.end(); ++itList){
		itList->second.work.clear();
		itList->second.work = defaultvector;
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		int start = 95 - startVector[0].first;
		//printf("%d ", start);
		if (start > -1){
			itList->second.work[start] = true;
			itList->second.work[start - 1] = true;
			itList->second.work[start - 2] = true;
		}
	}

	for (std::map<int, node>::iterator itList = list4.begin(); itList != list4.end(); ++itList){
		itList->second.work.clear();
		itList->second.work = defaultvector;
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		int start = 95 - startVector[0].first;
		//printf("%d ", start);
		if (start > -1){
			itList->second.work[start] = true;
			itList->second.work[start - 1] = true;
			itList->second.work[start - 2] = true;
		}
	}

	for (std::map<int, node>::iterator itList = list5.begin(); itList != list5.end(); ++itList){
		itList->second.work.clear();
		itList->second.work = defaultvector;
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		int start = 95 - startVector[0].first;
		//printf("%d ", start);
		if (start > -1){
			itList->second.work[start] = true;
			itList->second.work[start - 1] = true;
			itList->second.work[start - 2] = true;
		}
	}

	beginIteration();


}

void game::updateNodeTemperature(int id, double temp){

	std::map<int, node>::iterator it;
	it = list1.find(id);
	if (it != list1.end())
		it->second.temperature = temp;

}

int game::random(int min, int max) //range : [min, max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (max - min);
}

void game::sortPrices(){

	list1sortedByTemperature.clear();
	sortedPredictedPrices.clear();
	std::vector<double> temp = predictions;

	mostExpensiveTimeframe = 0;
	double globalmax = 0;

	for (int i = 0; i < predictions.size(); i++){
		if (globalmax < predictions[i]){
			globalmax = predictions[i];
			mostExpensiveTimeframe = i;
		}
		double max = 0;
		int maxPlace = 0;
		int counter = 0;
		for (std::vector<double>::iterator it = temp.begin(); it != temp.end(); ++it){
			if (max < *it){
				max = *it;
				maxPlace = counter;
			}
			counter++;
		}
		sortedPredictedPrices.push_back(temp[maxPlace]);
		temp.erase(temp.begin() + maxPlace);
	}

	std::map<int, game::node> templist1 = list1;
	for (std::map<int, node>::iterator itList = list1.begin(); itList != list1.end(); ++itList){
		node max;
		max.temperature = 0;
		int maxId = 0;
		int counter = 0;
		for (std::map<int, node>::iterator itTempList = templist1.begin(); itTempList != templist1.end(); ++itTempList){
			if (max.temperature < itTempList->second.temperature){
				max = itTempList->second;
				maxId = itTempList->first;
			}
			counter++;
		}
		list1sortedByTemperature.push_back(std::pair<int, node>(maxId, max));
		templist1.erase(maxId);
	}


}

std::vector<std::pair<int, double>> game::utilityFunctionForIterations(double temp, double capacity, int endTime){
	std::vector<std::pair<int, double>> results;

	endTime = 95 - endTime;
	int startingTime = endTime - 3;
	if (startingTime < 0){
		startingTime = 0;
		endTime = 3;
	}

	double result = -1;
	double minUtility = 1000000;
	int minUtilityTimeSlot = -1;

	for (int i = 0; i <= 93; i++){
		result = 0;

		for (int j = i; j < i + 3; j++){
			result += (predictions[j] * capacity);
		}
		int timeF = startingTime - i;
		double timeFactor = 0;
		if (timeF > 0){
			timeFactor = (int)timeF;
			timeFactor = timeFactor / 4;
		}
		else{
			timeFactor = (int)abs(timeF);
		}

		result = result + timeFactor;

		results.push_back(std::pair<int, double>(i, result));

	}
	std::vector<std::pair<int, double>> temporary = results;
	std::vector<std::pair<int, double>> sortedResults;

	for (int i = 0; i < results.size(); i++){
		double min = 1000000;
		int minPlace = 0;
		int counter = 0;
		for (int j = 0; j < temporary.size(); j++){
			if (min > temporary[j].second){
				min = temporary[j].second;
				minPlace = j;
			}
		}
			
		sortedResults.push_back(temporary[minPlace]);
		temporary.erase(temporary.begin() + minPlace);
	}
	
	return sortedResults;
}

int game::utilityFunction(double temp, double capacity, int endTime){

	endTime = 95 - endTime;
	int startingTime = endTime - 3;
	if (startingTime < 0){
		startingTime = 0;
		endTime = 11;
	}


	double result = -1;
	double minUtility = 1000000;
	int minUtilityTimeSlot = -1;

	for (int i = 0; i <= 93; i++){
		result = 0;

		for (int j = i; j < i + 3; j++){
			result += (predictions[j] * capacity);
		}
		int timeF = startingTime - i;
		double timeFactor = 0;
		if (timeF > 0){
			timeFactor = (int)timeF;
			timeFactor = timeFactor / 4;
		}
		else{
			timeFactor = (int)abs(timeF);
		}

		result = result + timeFactor;
		if (result < minUtility){
			minUtility = result;
			minUtilityTimeSlot = i;
		}

	}
	return minUtilityTimeSlot;

	/*int endTimeExtended = 0;
	int startingTimeExtended = 0;

	int startingTime = endTime + 8;
	if (startingTime > 95){
	startingTime = 95;
	}

	endTime = 95 - endTime;
	startingTime = 95 - startingTime;
	startingTimeExtended = startingTime - 8;
	endTimeExtended = endTime + 8;

	if (startingTimeExtended < 0){
	startingTimeExtended = 0;
	endTimeExtended = 24;
	}
	else if (endTimeExtended > 95){
	startingTimeExtended = 71;
	endTimeExtended = 95;
	}

	double result = -1;
	double minUtility = 1000000;
	int minUtilityTimeSlot = -1;

	for (int i = startingTimeExtended; i <= endTimeExtended - 8; i++){
	result = 0;

	for (int j = i; j < i + 8; j++){
	result += (predictions[j] * capacity);
	}

	if (result < minUtility){
	minUtility = result;
	minUtilityTimeSlot = i;
	}

	}
	return minUtilityTimeSlot;*/
}

void game::beginIteration(){
	std::vector<bool> defaultvectorbool(96, false);
	
	for (std::map<int, node>::iterator itList = list1.begin(); itList != list1.end(); ++itList){

		std::vector<int> defaultvector(96, 0);
		for (std::map<int, node>::iterator itList1 = list1.begin(); itList1 != list1.end(); ++itList1){
			for (int i = 0; i < 96; i++){
				if (itList1->second.work[i]){
					defaultvector[i]++;
				}
			}
		}
		/*for (int i = 0; i < 96; i++){
			printf("%d , %d \n", i, defaultvector[i]);
		}*/


		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		if ((startVector[0].second + 0.25) >= startVector[1].second){
			int start = 95 - startVector[1].first;
		
			itList->second.work.clear();
			itList->second.work = defaultvectorbool;
			
			if (start > -1){
				itList->second.work[start] = true;
				itList->second.work[start - 1] = true;
				itList->second.work[start - 2] = true;
			}
			std::vector<int> compairevector(96, 0);
			for (std::map<int, node>::iterator itListTemp = list1.begin(); itListTemp != list1.end(); ++itListTemp){
				for (int i = 0; i < 96; i++){
					if (itListTemp->second.work[i]){
						compairevector[i]++;
					}
				}
			}
			int previousSum = 0;
			int currentSum = 0;
			int previousStart = 95 - startVector[0].first;
			for (int i = 0; i < 3; i++){
				previousSum = previousSum + defaultvector[previousStart - i];
			}
			for (int i = 0; i < 3; i++){
				currentSum = currentSum + compairevector[start - i];
			}
			printf("previousSum = %d, currentSum = %d \n", previousSum, currentSum);
			if (currentSum < previousSum){
				printf("I changed working time from %d to %d \n", startVector[0].first, startVector[1].first);
			}
			else{
				printf("I go back to previous working time %d \n", startVector[0].first);
				int start = 95 - startVector[0].first;
				itList->second.work.clear();
				itList->second.work = defaultvectorbool;
				//printf("%d ", start);
				if (start > -1){
					itList->second.work[start] = true;
					itList->second.work[start - 1] = true;
					itList->second.work[start - 2] = true;
				}
			}

		}
		/*for (int i = 1; i < startVector.size(); i++){
			int previousstart = 95 - startVector[0].first;
			int start = 95 - startVector[i].first;
			int currentend = start - 2;

			//printf("defaultvector[previousstart]:%d, >= , defaultvector[currentend]:%d", defaultvector[previousstart], defaultvector[currentend]);
			if (defaultvector[previousstart] >= defaultvector[currentend]){
				itList->second.work.clear();
				itList->second.work = defaultvectorbool;
				//printf("%d ", start);
				if (start > -1){
					itList->second.work[start] = true;
					itList->second.work[start - 1] = true;
					itList->second.work[start - 2] = true;
				}

				for (int i = 0; i < 96; i++){
					defaultvector[i] = 0;
				}
				for (std::map<int, node>::iterator itList2 = list1.begin(); itList2 != list1.end(); ++itList2){
					for (int i = 0; i < 96; i++){
						if (itList2->second.work[i]){
							defaultvector[i]++;
						}
					}
				}
			}
		}*/
	}

	/*for (int i = 0; i < 96; i++){
		defaultvector[i] = 0;
	}

	for (std::map<int, node>::iterator itList = list2.begin(); itList != list2.end(); ++itList){
		for (int i = 0; i < 96; i++){
			if (itList->second.work[i]){
				defaultvector[i]++;
			}
		}
	}

	for (std::map<int, node>::iterator itList = list2.begin(); itList != list2.end(); ++itList){
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		for (int i = 1; i < startVector.size(); i++){
			int previousstart = 95 - startVector[0].first;
			int start = 95 - startVector[i].first;
			int currentend = start - 2;

			//printf("defaultvector[previousstart]:%d, >= , defaultvector[currentend]:%d", defaultvector[previousstart], defaultvector[currentend]);
			if (defaultvector[previousstart] >= defaultvector[currentend]){
				itList->second.work.clear();
				itList->second.work = defaultvectorbool;
				//printf("%d ", start);
				if (start > -1){
					itList->second.work[start] = true;
					itList->second.work[start - 1] = true;
					itList->second.work[start - 2] = true;
				}

				for (int i = 0; i < 96; i++){
					defaultvector[i] = 0;
				}
				for (std::map<int, node>::iterator itList2 = list2.begin(); itList2 != list2.end(); ++itList2){
					for (int i = 0; i < 96; i++){
						if (itList2->second.work[i]){
							defaultvector[i]++;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 96; i++){
		defaultvector[i] = 0;
	}

	for (std::map<int, node>::iterator itList = list3.begin(); itList != list3.end(); ++itList){
		for (int i = 0; i < 96; i++){
			if (itList->second.work[i]){
				defaultvector[i]++;
			}
		}
	}

	for (std::map<int, node>::iterator itList = list3.begin(); itList != list3.end(); ++itList){
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		for (int i = 1; i < startVector.size(); i++){
			int previousstart = 95 - startVector[0].first;
			int start = 95 - startVector[i].first;
			int currentend = start - 2;

			//printf("defaultvector[previousstart]:%d, >= , defaultvector[currentend]:%d", defaultvector[previousstart], defaultvector[currentend]);
			if (defaultvector[previousstart] >= defaultvector[currentend]){
				itList->second.work.clear();
				itList->second.work = defaultvectorbool;
				//printf("%d ", start);
				if (start > -1){
					itList->second.work[start] = true;
					itList->second.work[start - 1] = true;
					itList->second.work[start - 2] = true;
				}

				for (int i = 0; i < 96; i++){
					defaultvector[i] = 0;
				}
				for (std::map<int, node>::iterator itList2 = list3.begin(); itList2 != list3.end(); ++itList2){
					for (int i = 0; i < 96; i++){
						if (itList2->second.work[i]){
							defaultvector[i]++;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 96; i++){
		defaultvector[i] = 0;
	}

	for (std::map<int, node>::iterator itList = list4.begin(); itList != list4.end(); ++itList){
		for (int i = 0; i < 96; i++){
			if (itList->second.work[i]){
				defaultvector[i]++;
			}
		}
	}

	for (std::map<int, node>::iterator itList = list4.begin(); itList != list4.end(); ++itList){
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		for (int i = 1; i < startVector.size(); i++){
			int previousstart = 95 - startVector[0].first;
			int start = 95 - startVector[i].first;
			int currentend = start - 2;

			//printf("defaultvector[previousstart]:%d, >= , defaultvector[currentend]:%d", defaultvector[previousstart], defaultvector[currentend]);
			if (defaultvector[previousstart] >= defaultvector[currentend]){
				itList->second.work.clear();
				itList->second.work = defaultvectorbool;
				//printf("%d ", start);
				if (start > -1){
					itList->second.work[start] = true;
					itList->second.work[start - 1] = true;
					itList->second.work[start - 2] = true;
				}

				for (int i = 0; i < 96; i++){
					defaultvector[i] = 0;
				}
				for (std::map<int, node>::iterator itList2 = list4.begin(); itList2 != list4.end(); ++itList2){
					for (int i = 0; i < 96; i++){
						if (itList2->second.work[i]){
							defaultvector[i]++;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 96; i++){
		defaultvector[i] = 0;
	}

	for (std::map<int, node>::iterator itList = list5.begin(); itList != list5.end(); ++itList){
		for (int i = 0; i < 96; i++){
			if (itList->second.work[i]){
				defaultvector[i]++;
			}
		}
	}

	for (std::map<int, node>::iterator itList = list5.begin(); itList != list5.end(); ++itList){
		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		for (int i = 1; i < startVector.size(); i++){
			int previousstart = 95 - startVector[0].first;
			int start = 95 - startVector[i].first;
			int currentend = start - 2;

			//printf("defaultvector[previousstart]:%d, >= , defaultvector[currentend]:%d", defaultvector[previousstart], defaultvector[currentend]);
			if (defaultvector[previousstart] >= defaultvector[currentend]){
				itList->second.work.clear();
				itList->second.work = defaultvectorbool;
				//printf("%d ", start);
				if (start > -1){
					itList->second.work[start] = true;
					itList->second.work[start - 1] = true;
					itList->second.work[start - 2] = true;
				}

				for (int i = 0; i < 96; i++){
					defaultvector[i] = 0;
				}
				for (std::map<int, node>::iterator itList2 = list5.begin(); itList2 != list5.end(); ++itList2){
					for (int i = 0; i < 96; i++){
						if (itList2->second.work[i]){
							defaultvector[i]++;
						}
					}
				}
			}
		}
	}*/
}

bool game::iterate(int iterationPoint){
	std::vector<bool> defaultvectorbool(96, false);

	for (std::map<int, node>::iterator itList = list1.begin(); itList != list1.end(); ++itList){

		std::vector<int> defaultvector(96, 0);
		for (std::map<int, node>::iterator itList1 = list1.begin(); itList1 != list1.end(); ++itList1){
			for (int i = 0; i < 96; i++){
				if (itList1->second.work[i]){
					defaultvector[i]++;
				}
			}
		}
		/*for (int i = 0; i < 96; i++){
		printf("%d , %d \n", i, defaultvector[i]);
		}*/


		std::vector<std::pair<int, double>> startVector = utilityFunctionForIterations(itList->second.temperature, itList->second.capacity, itList->second.firstWorkingHourCalculated);
		if ((startVector[iterationPoint-1].second + 0.25) >= startVector[iterationPoint].second){
			int start = 95 - startVector[iterationPoint].first;

			itList->second.work.clear();
			itList->second.work = defaultvectorbool;

			if (start > -1){
				itList->second.work[start] = true;
				itList->second.work[start - 1] = true;
				itList->second.work[start - 2] = true;
			}
			std::vector<int> compairevector(96, 0);
			for (std::map<int, node>::iterator itListTemp = list1.begin(); itListTemp != list1.end(); ++itListTemp){
				for (int i = 0; i < 96; i++){
					if (itListTemp->second.work[i]){
						compairevector[i]++;
					}
				}
			}
			int previousSum = 0;
			int currentSum = 0;
			int previousStart = 95 - startVector[iterationPoint-1].first;
			for (int i = 0; i < 3; i++){
				previousSum = previousSum + defaultvector[previousStart - i];
			}
			for (int i = 0; i < 3; i++){
				currentSum = currentSum + compairevector[start - i];
			}
			printf("previousSum = %d, currentSum = %d \n", previousSum, currentSum);
			if (currentSum < previousSum){
				printf("I changed working time from %d to %d \n", startVector[iterationPoint - 1].first, startVector[iterationPoint].first);
			}
			else{
				printf("I go back to previous working time %d \n", startVector[iterationPoint-1].first);
				int start = 95 - startVector[iterationPoint-1].first;
				itList->second.work.clear();
				itList->second.work = defaultvectorbool;
				//printf("%d ", start);
				if (start > -1){
					itList->second.work[start] = true;
					itList->second.work[start - 1] = true;
					itList->second.work[start - 2] = true;
				}
			}

		}
		
	}
}