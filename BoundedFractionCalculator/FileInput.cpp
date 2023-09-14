#include "pch.h"
#include "FileInput.h"

FileInput::FileInput() {
    Initialize();
}

FileInput::~FileInput() {

}

void FileInput::Initialize() {
    _vecXCoordThatYBelowThreshold.clear();
    _vecUnbounded.clear();
    _threshold = 128;
    _countOfPore = 0;
    _lengthOfX = 0;
    _lengthOfY = 0;
}

bool FileInput::ReadFile(String^ filePath) {
    Initialize();
    //將filePath(String^)轉換成char*
    const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filePath)).ToPointer();
    _filePath = chars;
    ifstream myFile(_filePath);
    string temp;
    _vecValueInFile.clear();

    //將file中的數據存入_vecValueInFile中
    if (!myFile) {
        cout << "File not found!" << endl;
        return false;
    }
    else {
        cout << "Reading File" << endl;
        myFile >> temp;
        while (!myFile.eof()) {
            temp = "";
            myFile >> temp;

            string xString = temp.substr(0, temp.find(","));
            temp.erase(0, (int)temp.find(",") + 1);
            string yString = temp.substr(0, temp.find(","));
            temp.erase(0, (int)temp.find(",") + 1);
            string valueString = temp.substr(0, temp.find(","));

            if (xString == "")
                break;

            int x = stoi(xString);
            int y = stoi(yString);
            int value = stoi(valueString);

            vector<int> tempVec{ x, y, value };
            _vecValueInFile.push_back(tempVec);
        }
    }
    myFile.close();
    return true;
}

void FileInput::SetThreshold(String^ threshold) {
    Initialize();
    //將threshold(String^)轉換成char*
    const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(threshold)).ToPointer();
    _threshold = stoi(chars);

    //限制_threshold不能大於255
    if (_threshold > 255)
        _threshold = 255;

    ComputeBoundedFraction();
}

void FileInput::ComputeBoundedFraction() {
    for (int i = 0; i < _vecValueInFile.size(); i++) {

        //記錄出現的最大X值即為該圖長度
        if (_lengthOfX < _vecValueInFile[i].at(0))
            _lengthOfX = _vecValueInFile[i].at(0);

        //記錄出現的最大Y值即為該圖高度
        if (_lengthOfY < _vecValueInFile[i].at(1))
            _lengthOfY = _vecValueInFile[i].at(1);

        //threshold = 0的時候不應該有點被檢查到
        if (_threshold != 0 && _vecValueInFile[i].at(2) <= _threshold) {
            _vecXCoordThatYBelowThreshold.push_back(_vecValueInFile[i].at(0));
            vector<int> tempVec{ _vecValueInFile[i].at(0), _vecValueInFile[i].at(1), _vecValueInFile[i].at(2) };
            _vecUnbounded.push_back(tempVec);
        }
    }

    //_vecXCoordThatYBelowThreshold收集有任一Y值小於threshold的X座標軸，代表該個X座標軸上有孔洞(Pore)，故只需收集一次X座標的值
    sort(_vecXCoordThatYBelowThreshold.begin(), _vecXCoordThatYBelowThreshold.end());
    _vecXCoordThatYBelowThreshold.erase(unique(_vecXCoordThatYBelowThreshold.begin(), _vecXCoordThatYBelowThreshold.end()), _vecXCoordThatYBelowThreshold.end());

    for (int p = 0; p < _vecXCoordThatYBelowThreshold.size(); p++)
        _countOfPore++;

    //X, Y值從0開始，所以需補上1
    _lengthOfX += 1;
    _lengthOfY += 1;
}

int FileInput::GetThreshold() {
    return _threshold;
}

vector<vector<int>> FileInput::GetUnboundedPoint() {
    return _vecUnbounded;
}

int FileInput::GetLengthOfX() {
    return _lengthOfX;
}

int FileInput::GetLengthOfY() {
    return _lengthOfY;
}

//在特定threshold下非孔洞的比例
float FileInput::GetContactFraction() {
    if (_countOfPore == 0)
        return 1;
    else
        return 1 - ((float)_countOfPore / (float)_lengthOfX);
}

//在特定threshold下孔洞的比例
float FileInput::GetUnboundedFraction() {
    if (_countOfPore == 0)
        return 0;
    else
        return ((float)_countOfPore / (float)_lengthOfX);
}