#include "trainer.h"
#include <stdlib.h>
#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <dlib/svm.h>
/*
Include directly the different
headers from cppconn/ and mysql_driver.h + mysql_util.h
(and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

using namespace std;
using namespace dlib;

/* Queries with tod NOT with ids
std::vector<double> makePrediction(int previousday, int previoustod, int day, int tod)
{
	std::vector<double> values;

	try {
		sql::Driver *driver;
		sql::Connection *con;

		sql::Statement *stmtprice;
		sql::ResultSet *resprice;

		sql::Statement *stmtweather;
		sql::ResultSet *resweather;

		sql::Statement *stmtpreviousweather;
		sql::ResultSet *respreviousweather;

		sql::Statement *stmtpreviousprice;
		sql::ResultSet *respreviousprice;

		// Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		// Connect to the MySQL test database
		con->setSchema("gridlabd");

		stmtprice = con->createStatement();
		stmtweather = con->createStatement();
		stmtpreviousweather = con->createStatement();
		stmtpreviousprice = con->createStatement();

		string daystring = std::to_string(day);
		string todstring = std::to_string(tod);

		string previousdaystring = std::to_string(previousday);
		string previoustodstring = std::to_string(previoustod);

		string queryprice = "SELECT * from gridlabd.clearing where day=" + daystring + " and time=" + todstring;
		string queryweather = "SELECT * from gridlabd.weather where day=" + daystring + " and time=" + todstring;
		string querypreviousweather = "SELECT * from gridlabd.weather where day=" + previousdaystring + " and time=" + previoustodstring;
		string querypreviousprice = "SELECT * from gridlabd.clearing where day=" + previousdaystring + " and time=" + previoustodstring;

		resprice = stmtprice->executeQuery(queryprice);
		resweather = stmtweather->executeQuery(queryweather);
		respreviousweather = stmtpreviousweather->executeQuery(querypreviousweather);
		respreviousprice = stmtpreviousprice->executeQuery(querypreviousprice);

		while (resprice->next() && respreviousprice->next()) {

			//string clearingType = resprice->getString("clearingtype");
		    //string seller ("MARGINAL_SELLER");
			//string buyer ("MARGINAL_BUYER");
			//if (clearingType == seller || clearingType == buyer){

			// Access column fata by numeric offset, 1 is the first column
			// Access column data by alias or column name
			double price = resprice->getDouble("clearingprice");
			double previousprice = respreviousprice->getDouble("clearingprice");
			double resultprice = price - previousprice;
			values.push_back(resultprice);
			while (resweather->next() && respreviousweather->next()) {
				double wind = resweather->getDouble("windspeed");
				double previouswind = respreviousweather->getDouble("windspeed");
				double resultwind = wind - previouswind;
				values.push_back(resultwind);

				cout << "resultprice = " << resultprice << " and resultwind = " << resultwind << endl;
			}

			//}

		}
		delete resweather;
		delete stmtweather;
		delete resprice;
		delete stmtprice;
		delete con;

	}
	catch (std::exception &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
	}


	return values;
}*/

/* Training with wind variation
std::vector<double> makePrediction(int x)
{
	std::vector<double> values;

	try {
		sql::Driver *driver;
		sql::Connection *con;

		sql::Statement *stmtprice;
		sql::ResultSet *resprice;

		sql::Statement *stmtweather;
		sql::ResultSet *resweather;

		sql::Statement *stmtpreviousweather;
		sql::ResultSet *respreviousweather;

		sql::Statement *stmtprepreviousweather;
		sql::ResultSet *resprepreviousweather;

		sql::Statement *stmtpreviousprice;
		sql::ResultSet *respreviousprice;

		// Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		// Connect to the MySQL test database
		con->setSchema("gridlabd");

		stmtprice = con->createStatement();
		stmtweather = con->createStatement();
		stmtpreviousweather = con->createStatement();
		stmtprepreviousweather = con->createStatement();
		stmtpreviousprice = con->createStatement();

		string prepreviousid = std::to_string(x - 2);
		string previousid = std::to_string(x - 1);
		string id = std::to_string(x);
		string nextid = std::to_string(x+1);

		string queryprice = "SELECT * from gridlabd.market where id=" + id;
		string querypreviousprice = "SELECT * from gridlabd.market where id=" + previousid;
		string queryweather = "SELECT * from gridlabd.weather where id=" + id;
		string querypreviousweather = "SELECT * from gridlabd.weather where id=" + previousid;
		string queryprepreviousweather = "SELECT * from gridlabd.weather where id=" + prepreviousid;

		resprice = stmtprice->executeQuery(queryprice);
		respreviousprice = stmtpreviousprice->executeQuery(querypreviousprice);
		resweather = stmtweather->executeQuery(queryweather);
		respreviousweather = stmtpreviousweather->executeQuery(querypreviousweather);
		resprepreviousweather = stmtprepreviousweather->executeQuery(queryprepreviousweather);

		while (resprice->next() && respreviousprice->next()) {

			// Access column fata by numeric offset, 1 is the first column
			// Access column data by alias or column name
			double price = resprice->getDouble("clearingprice");
			double previousprice = respreviousprice->getDouble("clearingprice");
			double resultprice = price - previousprice;
			
			values.push_back(resultprice);
			while (resweather->next() && respreviousweather->next() && resprepreviousweather->next()) {
				double wind = resweather->getDouble("windspeed");
				double previouswind = respreviousweather->getDouble("windspeed");
				double prepreviouswind = resprepreviousweather->getDouble("windspeed");
				double resultwind = wind - previouswind;
				double resultprevwind = previouswind - prepreviouswind;

				values.push_back(previouswind);
				values.push_back(resultprevwind);
				//values.push_back(resultprevwind);
				//values.push_back(wind);
			}

			//}

		}
		delete resweather;
		delete stmtweather;
		delete respreviousweather;
		delete stmtpreviousweather;
		delete resprepreviousweather;
		delete stmtprepreviousweather;
		delete resprice;
		delete stmtprice;
		delete respreviousprice;
		delete stmtpreviousprice;
		delete con;

	}
	catch (std::exception &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
	}


	return values;
}
*/

/* Training with wind history*/
std::vector<double> makePrediction(int x)
{
	std::vector<double> values;

	try {
		sql::Driver *driver;
		sql::Connection *con;

		sql::Statement *stmtprice;
		sql::ResultSet *resprice;

		sql::Statement *stmtweather;
		sql::ResultSet *resweather;

		sql::Statement *stmtpreviousweather;
		sql::ResultSet *respreviousweather;

		sql::Statement *stmtprepreviousweather;
		sql::ResultSet *resprepreviousweather;

		sql::Statement *stmtpreviousprice;
		sql::ResultSet *respreviousprice;

		// Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		// Connect to the MySQL test database
		con->setSchema("gridlabd");

		stmtprice = con->createStatement();
		stmtweather = con->createStatement();
		stmtpreviousweather = con->createStatement();
		stmtprepreviousweather = con->createStatement();
		stmtpreviousprice = con->createStatement();

		string prepreviousid = std::to_string(x - 2);
		string previousid = std::to_string(x - 1);
		string id = std::to_string(x);

		string queryprice = "SELECT * from gridlabd.marketsolar where id=" + id;
		string queryweather = "SELECT * from gridlabd.weather where id=" + id;
		string querypreviousweather = "SELECT * from gridlabd.weather where id=" + previousid;
		string queryprepreviousweather = "SELECT * from gridlabd.weather where id=" + prepreviousid;

		resprice = stmtprice->executeQuery(queryprice);
		resweather = stmtweather->executeQuery(queryweather);
		respreviousweather = stmtpreviousweather->executeQuery(querypreviousweather);
		resprepreviousweather = stmtprepreviousweather->executeQuery(queryprepreviousweather);

		while (resprice->next()) {

			double price = resprice->getDouble("sellertotalquantity");
			price = price - 2400;
			//double price = resprice->getDouble("clearingprice");
			int tod = resprice->getInt("time");

			values.push_back(price);
			while (resweather->next() && respreviousweather->next() && resprepreviousweather->next()) {
				double wind = resweather->getDouble("windspeed");
				double solar = resweather->getDouble("solarflux");
				double previouswind = respreviousweather->getDouble("windspeed");
				double previoussolar = respreviousweather->getDouble("solarflux");
				double prepreviouswind = resprepreviousweather->getDouble("windspeed");
				double preprevioussolar = resprepreviousweather->getDouble("solarflux");

				//values.push_back(wind);
				values.push_back(previouswind);
				values.push_back(previoussolar);
			}

		}
		delete resweather;
		delete stmtweather;
		delete resprice;
		delete stmtprice;
		delete con;

	}
	catch (std::exception &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
	}


	return values;
}

double trainer::windVariation(int x)
{
	double resultwind = 0;
	try {
		sql::Driver *driver;
		sql::Connection *con;

		sql::Statement *stmtweather;
		sql::ResultSet *resweather;

		sql::Statement *stmtpreviousweather;
		sql::ResultSet *respreviousweather;
		
		// Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		// Connect to the MySQL test database
		con->setSchema("gridlabd");

		stmtweather = con->createStatement();
		stmtpreviousweather = con->createStatement();

		string id = std::to_string(x);
		string previd = std::to_string(x -1);

		string queryweather = "SELECT * from gridlabd.weather where id=" + id;
		string querynextweather = "SELECT * from gridlabd.weather where id=" + previd;

		resweather = stmtweather->executeQuery(queryweather);
		respreviousweather = stmtpreviousweather->executeQuery(querynextweather);

				
		while (resweather->next() && respreviousweather->next()) {
			double wind = resweather->getDouble("windspeed");
			double previouswind = respreviousweather->getDouble("windspeed");
			resultwind = wind - previouswind;
			
		}
	
		delete resweather;
		delete stmtweather; 
		delete respreviousweather;
		delete stmtpreviousweather;
		delete con;

	}
	catch (std::exception &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
	}

	return resultwind;
}

double trainer::wind(int x)
{
	double resultwind = 0;
	try {
		sql::Driver *driver;
		sql::Connection *con;

		sql::Statement *stmtweather;
		sql::ResultSet *resweather;

		// Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		// Connect to the MySQL test database
		con->setSchema("gridlabd");

		stmtweather = con->createStatement();

		string id = std::to_string(x);

		string queryweather = "SELECT * from gridlabd.weather where id=" + id;

		resweather = stmtweather->executeQuery(queryweather);


		while (resweather->next()) {
			resultwind = resweather->getDouble("windspeed");
		}

		delete resweather;
		delete stmtweather;
		delete con;

	}
	catch (std::exception &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
	}

	return resultwind;
}

double trainer::solar(int x)
{
	double resultwind = 0;
	try {
		sql::Driver *driver;
		sql::Connection *con;

		sql::Statement *stmtweather;
		sql::ResultSet *resweather;

		// Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		// Connect to the MySQL test database
		con->setSchema("gridlabd");

		stmtweather = con->createStatement();

		string id = std::to_string(x);

		string queryweather = "SELECT * from gridlabd.weather where id=" + id;

		resweather = stmtweather->executeQuery(queryweather);


		while (resweather->next()) {
			resultwind = resweather->getDouble("solarflux");
		}

		delete resweather;
		delete stmtweather;
		delete con;

	}
	catch (std::exception &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
	}

	return resultwind;
}

trainer::trainer()
{
	
}

trainer::~trainer()
{
}

void trainer::training(){
	// Here we declare that our samples will be 1 dimensional column vectors.  
	//typedef matrix<double, 3, 1> sample_type;
	typedef matrix<double, 2, 1> sample_type;

	// Now sample some points from the sinc() function
	sample_type m;
	std::vector<sample_type> samples;
	std::vector<double> labels;
	
	for (int x = 3; x <= 768; x += 1){
		std::vector<double> values = makePrediction(x);
		if (values.size() > 0){
			m(0) = values[1];
			m(1) = values[2];
			samples.push_back(m);
			labels.push_back(values[0]);
		}
	}

	// Now we are making a typedef for the kind of kernel we want to use.  I picked the
	// radial basis kernel because it only has one parameter and generally gives good
	// results without much fiddling.
	typedef radial_basis_kernel<sample_type> kernel_type;

	// Here we declare an instance of the rvm_regression_trainer object.  This is the
	// object that we will later use to do the training.
	rvm_regression_trainer<kernel_type> trainer;

	// Here we set the kernel we want to use for training.   The radial_basis_kernel 
	// has a parameter called gamma that we need to determine.  As a rule of thumb, a good 
	// gamma to try is 1.0/(mean squared distance between your sample points).  So 
	// below we are using a similar value.   Note also that using an inappropriately large
	// gamma will cause the RVM training algorithm to run extremely slowly.  What
	// "large" means is relative to how spread out your data is.  So it is important
	// to use a rule like this as a starting point for determining the gamma value
	// if you want to use the RVM.  It is also probably a good idea to normalize your
	// samples as shown in the rvm_ex.cpp example program.
	const double gamma = 0.05;
		//2.0 / compute_mean_squared_distance(samples);
	cout << "using gamma of " << gamma << endl;
	trainer.set_kernel(kernel_type(gamma));

	// One thing you can do to reduce the RVM training time is to make its
	// stopping epsilon bigger.  However, this might make the outputs less
	// reliable.  But sometimes it works out well.  0.001 is the default.
	trainer.set_epsilon(0.001);

	// now train a function based on our sample points
	decision_function<kernel_type> test = trainer.train(samples, labels);
	
	// Another thing that is worth knowing is that just about everything in dlib is serializable.
	// So for example, you can save the test object to disk and recall it later like so:
	serialize("saved_function_solar.dat") << test;
	cout << "serialized!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

	// Now let's open that file back up and load the function object it contains.
	deserialize("saved_function_solar.dat") >> test;
}

void trainer::loadTraining(){
	typedef matrix<double, 2, 1> sample_type;

	sample_type m;
	std::vector<sample_type> samples;
	std::vector<double> labels;

	for (int x = 3; x <= 768; x += 1){
		std::vector<double> values = makePrediction(x);
		if (values.size() > 0){
			m(0) = values[1];
			m(1) = values[2];
			samples.push_back(m);
			labels.push_back(values[0]);
		}
	}

	typedef radial_basis_kernel<sample_type> kernel_type;

	rvm_regression_trainer<kernel_type> trainer;

	const double gamma = 0.05;
	
	cout << "using gamma of " << gamma << endl;
	trainer.set_kernel(kernel_type(gamma));

	trainer.set_epsilon(0.001);

	decision_function<kernel_type> test = trainer.train(samples, labels);

	serialize("saved_function_loadsolar.dat") << test;
	cout << "serialized!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	system("pause");
	// Now let's open that file back up and load the function object it contains.
	//deserialize("saved_function_loadwind.dat") >> test;
}

double trainer::getresult(){
	//typedef matrix<double, 3, 1> sample_type;
	typedef matrix<double, 1, 1> sample_type;
	sample_type m;
	typedef radial_basis_kernel<sample_type> kernel_type;
	decision_function<kernel_type> test;
	deserialize("saved_function_wind.dat") >> test;

	double windresult = wind(simulationId-1);
	//double solarresult = solar(simulationId - 1);
	m(0) = windresult;
	//m(1) = solarresult;

	double enthousiasm = test(m);
	return enthousiasm;

	/*ofstream myfile("enthousiasm.txt", std::ios_base::app | std::ios_base::out);
	for (int i = 769; i < 1825; i++){

		double windresult = wind(i - 1);
		double solarresult = solar(i - 1);

		m(0) = windresult;
		m(1) = solarresult;

		double enthousiasm = test(m);
		//cout << "enthousiasm = " << enthousiasm << endl;
		myfile << enthousiasm << endl;
		
	}
	myfile.close();
	system("pause");
	return 0;*/
}

void trainer::setNextSimulationId(int x){
	simulationId = x;
}

void trainer::setNextAuctionTime(char t[]){
	//cout << "set next auction time : " << t << endl;
	try {
		sql::Driver *driver;
		sql::Connection *con;

		sql::Statement *stmtprice;
		sql::ResultSet *resprice;

		// Create a connection
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		// Connect to the MySQL test database
		con->setSchema("gridlabd");

		stmtprice = con->createStatement();

		string timeString(t);
		string yearString = timeString.substr(0, 4);
		string monthString = timeString.substr(5, 2);
		string dayString = timeString.substr(8, 2);
		string todString = timeString.substr(11,5);
		todString.erase(2, 1);
		
		string queryprice = "SELECT * from gridlabd.market where day=" + dayString + " and month=" + monthString + " and year=" + yearString + " and time=" + todString;
		
		resprice = stmtprice->executeQuery(queryprice);
		while (resprice->next()){
			simulationId = resprice->getInt("id");
			simulationTod = resprice->getInt("time");
		}
		
		delete resprice;
		delete stmtprice;
		delete con;

	}
	catch (std::exception &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
	}
}

void trainer::record(double min, double max,double last_p, double last_q){
	if (previoussimulationId != simulationId){
		ofstream myfile("double_ramp_hvac.csv", std::ios_base::app | std::ios_base::out);
		myfile << "" << min << "," << max << "," << last_p << "," << last_q << endl;
		myfile.close();
		previoussimulationId = simulationId;
	}
	
	ofstream myfile("double_ramp_hvac_full.csv", std::ios_base::app | std::ios_base::out);
	myfile << "" << min << "," << max << "," << last_p << "," << last_q << endl;
	myfile.close();
	previoussimulationId = simulationId;
	
}

std::vector<double> trainer::getPredictions(char day[]){
	std::vector<double> predictions;
	setNextAuctionTime(day);
	string simulationDay(day);
	
	//printf("get predictions for day %s \n\n", simulationDay);
	typedef matrix<double, 1, 1> sample_type;
	//typedef matrix<double, 2, 1> sample_type;
	sample_type m;
	typedef radial_basis_kernel<sample_type> kernel_type;
	decision_function<kernel_type> test;
	deserialize("saved_function_wind.dat") >> test;
	//deserialize("saved_function_solar.dat") >> test;

	ofstream myfile("enthousiasm.txt", std::ios_base::app | std::ios_base::out);
	for (int i = 0; i < 96; i++)
	{
		double windresult = wind(simulationId - 1);
		//double solarflux = solar(simulationId-1);
		m(0) = windresult;
		//m(1) = solarflux;
		double enthousiasm = test(m);
		myfile << enthousiasm << endl;
		int scaled = enthousiasm * 100;
		enthousiasm = static_cast<double>(scaled) / 100.0;
		predictions.push_back(enthousiasm);
		simulationId++;
	}
	myfile.close();
	system("pause");
	return predictions;
}

double trainer::getSinglePrediction(char day[]){
	setNextAuctionTime(day);

	//printf("get predictions for day %s \n\n", simulationDay);
	typedef matrix<double, 1, 1> sample_type;
	//typedef matrix<double, 2, 1> sample_type;
	sample_type m;
	typedef radial_basis_kernel<sample_type> kernel_type;
	decision_function<kernel_type> test;
	deserialize("saved_function_wind.dat") >> test;
	//deserialize("saved_function_solar.dat") >> test;
	
	double windresult = wind(simulationId - 1);
	//double solarflux = solar(simulationId-1);
	m(0) = windresult;
	//m(1) = solarflux;
	double enthousiasm = test(m);
	
	return enthousiasm;
}

double trainer::getSingleLoadPrediction(char day[]){
	setNextAuctionTime(day);

	//printf("get predictions for day %s \n\n", simulationDay);
	//typedef matrix<double, 1, 1> sample_type;
	typedef matrix<double, 2, 1> sample_type;
	sample_type m;
	typedef radial_basis_kernel<sample_type> kernel_type;
	decision_function<kernel_type> test;
	//deserialize("saved_function_loadwind.dat") >> test;
	deserialize("saved_function_loadsolar.dat") >> test;

	double windresult = wind(simulationId - 1);
	double solarflux = solar(simulationId-1);
	m(0) = windresult;
	m(1) = solarflux;
	double enthousiasm = test(m);

	return enthousiasm;
}