// Machine Learning Scratch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>

using namespace std;


int getNumLines(string);						// Counts and returns the number of lines in a given file stream
void printSex(double sex[][2]);
double ageMean(vector<double>);
double ageVar(vector<double>, double);

int main()
{
	ifstream inFS;
	string line;

	// Opening the titanic project data file
	cout << "Opening file titanic_project.csv" << endl << endl;

	inFS.open("titanic_project.csv");

	// If we couldn't open the file
	if (!inFS.is_open())
	{
		cout << "Couldn't open the file :(" << endl;
	}	// File successfully opened
	else
	{
		// Get the file size (number of lines)
		const int fileSize = getNumLines("titanic_project.csv");
		cout << "File number of lines: " << fileSize << endl;

		// Initialize our vectors, strings, and variables for getting our data
		string person_in, pclass_in, survived_in, sex_in, age_in; // Used to temporarily hold values from getline
		const int trainSize = 800;
		const int testSize = (fileSize - 800);
		vector<double> personIDTrain(trainSize);
		vector<double> pclassTrain(trainSize);
		vector<double> survivedTrain(trainSize);
		vector<double> sexTrain(trainSize);
		vector<double> ageTrain(trainSize);
		vector<double> personIDTest(testSize);
		vector<double> pclassTest(testSize);
		vector<double> survivedTest(testSize);
		vector<double> sexTest(testSize);
		vector<double> ageTest(testSize);
		vector<double> numOneTrain(trainSize);		// Used for multiplying our vectors later
		vector<double> weights(2);
		weights.assign(2,1);


		// Used to track the current entry in our while loop below
		int numEntry = 0;	
		int numEntry2 = 0;	

		// Read in the first line to skip it (just headers)
		getline(inFS, line);
		// cout << "heading: " << line << endl;

		// While there is more to read from the file
		while (inFS.good())
		{
			// Read in the input seperated by commas
			getline(inFS, person_in, ',');		// Get the person ID input (int)
			getline(inFS, pclass_in, ',');		// Get the person's class input (int, 1 to 3)
			getline(inFS, survived_in, ',');	// Get the survived input (int, binary)
			getline(inFS, sex_in, ',');			// Get the person's sex input (int, binary)
			getline(inFS, age_in, '\n');		// Get the person's age input (int)

			// If numEntry is in range of the file size
			if (numEntry < fileSize-1) {
				// If the data entry is in the first 80% of the data set, add it to train vectors
				if (numEntry < trainSize) {
					personIDTrain.insert(personIDTrain.begin() + numEntry, stod(person_in));		// Add the person's ID entry to the person ID train vector
					pclassTrain.insert(pclassTrain.begin() + numEntry, stod(pclass_in));			// Add the person's class entry to the person class train vector
					survivedTrain.insert(survivedTrain.begin() + numEntry, stod(survived_in));		// Add the survived entry to the survived train vector
					sexTrain.insert(sexTrain.begin() + numEntry, stod(sex_in));				// Add the person's sex entry to the sex train vector
					ageTrain.insert(ageTrain.begin() + numEntry, stod(age_in));				// Add the person's age entry to the age train vector
					numOneTrain.insert(numOneTrain.begin() + 1, 1);						// Add the 1 for the number one vector
				}
				else {	// Otherwise it is in the last 20% of the data set, add it to the test vectors
					personIDTest.insert(personIDTest.begin() + numEntry2, stod(person_in));		// Add the person's ID entry to the person ID test vector
					pclassTest.insert(pclassTest.begin() + numEntry2, stod(pclass_in));		// Add the person's class entry to the person class test vector
					survivedTest.insert(survivedTest.begin() + numEntry2, stod(survived_in));	// Add the survived entry to the survived test vector
					sexTest.insert(sexTest.begin() + numEntry2, stod(sex_in));				// Add the person's sex entry to the sex test vector
					ageTest.insert(ageTest.begin() + numEntry2, stod(age_in));				// Add the person's age entry to the age test vector
					numEntry2++;										// Increment the numEntry 2 (used for test vectors index)			
				}

			}
			numEntry++;		// Increment the numEntry (used for the train vectors index)
		}

		cout << "Closing file titanic_project.csv" << endl << endl;
		inFS.close();
		
		double rawProbSex[2][2]; //sex 0 is female sex 1 male
								/*
										female died   female lived
										male died	  male lived
								*/
		double rawProbPclass[3][2];
								/*
										class1 died    class1 lived
										class2 died    class2 lived
										class3 died    class3 lived
								*/

		double numDiedM, totalM;
		double numDiedF, totalF;
		double totalOne, totalTwo, totalThree;
		double oneDied, twoDied, threeDied;

		for(int i = 0; i < 800; i++){
				if(sexTrain.at(i) == 1){
					totalM++;
					if(survivedTrain.at(i) == 0)
						numDiedM++;
				}
				else if(sexTrain.at(i) == 0){
					totalF++;
					if(survivedTrain.at(i) == 0)
						numDiedF++;
				}

				if(pclassTrain.at(i) == 1){
					totalOne++;
					if(survivedTrain.at(i) == 0)
						oneDied++;
				}
				else if(pclassTrain.at(i) == 2){
					totalTwo++;
					if(survivedTrain.at(i) == 0)
						twoDied++;
				}
				else if(pclassTrain.at(i) == 3){
					totalThree++;
					if(survivedTrain.at(i) == 0)
						threeDied++;
				}
		}

		rawProbSex[0][0] = (numDiedF/totalF);
		rawProbSex[0][1] = ( 1 - (numDiedF/totalF));
		rawProbSex[1][0] = (numDiedM/totalM);
		rawProbSex[1][1] = ( 1 - (numDiedM/totalM));

		rawProbPclass[0][0] = (oneDied/totalOne);
		rawProbPclass[0][1] = 1-(oneDied/totalOne);
		rawProbPclass[1][0] = (twoDied/totalTwo);
		rawProbPclass[1][1] = 1-(twoDied/totalTwo);
		rawProbPclass[2][0] = (threeDied/totalThree);
		rawProbPclass[2][1] = 1-(threeDied/totalThree);

		printSex(rawProbSex);
		printPclass(rawProbPclass);

		double averageAge = ageMean(ageTrain);
		double varianceAge = ageVar(ageTrain, averageAge);

	}

	cout << endl << "Exiting..." << endl;
}

double ageMean(vector<double> age){
	int sum = 0;

	for(int i = 0; i < 800; i++){
		sum += age.at(i);
	}

	return(sum/800);
}

double ageVar(vector<double> age, double average){
	int ave = ageMean(age);

	int sum = 0;

	for(int i = 0; i < 800; i++){
		sum += ((age.at(i) - ave) * (age.at(i) - ave))
	}

	return((sum/799.0));
}

void printSex(double sex[][2]){
	cout << "Probabilities for survival based on sex:" << endl << endl;
	cout << "female died   female lived" << endl << "male died   male lived" << endl << endl;

	for(int i = 0; i < 2; i++)
			cout << sex[i][0] << ", " <<  sex[i][1] << endl;

	cout << endl;
}

void printPclass(double class[][2]){
	cout << "Probabilities for survival based on passenger class:" << endl << endl;
	cout << "class1 died    class1 lived" << endl << "class2 died    class2 lived" << endl << "class3 died    class3 lived" << endl << endl;

	for(int i = 0; i < 3; i++)
			cout << class[i][0] << ", " <<  class[i][1] << endl;

	cout << endl;
}

// Counts and returns the number of lines in a given file stream
int getNumLines(string filename)
{
	ifstream inFile;		// The input stream to read the given file
	inFile.open(filename);	// Open the file

	// If we couldn't open the file
	if (!inFile.is_open()) {
		cout << "Couldn't open file: " << filename << endl;
		return 0;	// Return number of lines as 0

	}	// Otherwise we could
	else {
		int numOfLines = 0;		// The number of lines
		string line;			// The current line of the file read (won't be used)

		// While we can read lines from the file, count it as a line
		while(getline(inFile, line)) {
			numOfLines++;	// Add one to the number of lines
		}

		// Return the number of lines found
		return numOfLines;
	}
}
