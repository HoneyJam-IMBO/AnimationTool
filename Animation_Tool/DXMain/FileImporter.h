#pragma once
#include <fstream>

//File Exporter가 사용하는 객체
class CFileImporter {
public:
	virtual void ReadWCHAR(wifstream& in, WCHAR* data, int size) = 0;
	virtual void ReadCHAR(wifstream& in, char* data, int size) = 0;
	virtual void ReadUINT(wifstream& in, UINT& data) = 0;
	virtual void ReadInt(wifstream& in, int& data) = 0;
	virtual void ReadFloat(wifstream& in, float& data) = 0;

	//txt file 사용할 경우 각 인자의 경계
	virtual void ReadSpace(wifstream& in) = 0;
	virtual void ReadEnter(wifstream& in) = 0;
private:

public:
	CFileImporter() {};
	~CFileImporter() {};
};

