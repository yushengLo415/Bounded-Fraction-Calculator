#ifndef FILEINPUT_H
#define FILEINPUT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace System;
using namespace std;

class FileInput
{
public:
	FileInput();
	~FileInput();
	bool ReadFile(String^ filePath);
	float GetContactFraction();
	float GetUnboundedFraction();
	void SetThreshold(String^ threshold);
	vector<vector<int>> GetUnboundedPoint();
	int GetLengthOfX();
	int GetLengthOfY();
	int GetThreshold();
private:
	void ComputeBoundedFraction();
	void Initialize();
	string _filePath;
	vector<int> _vecXCoordThatYBelowThreshold;
	vector<vector<int>> _vecValueInFile;
	vector<vector<int>> _vecUnbounded;
	int _threshold = 128;
	int _countOfPore = 0;
	int _lengthOfX = 0;
	int _lengthOfY = 0;
};

#endif 