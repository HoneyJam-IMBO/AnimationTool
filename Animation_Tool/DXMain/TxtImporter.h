#pragma once
#include "FileImporter.h"

//File Exporter가 사용하는 객체
class CTxtImporter :public CFileImporter {
public:
	virtual void ReadWCHAR(wifstream& in, WCHAR* data, int size);
	virtual void ReadCHAR(wifstream& in, char* data, int size);
	virtual void ReadUINT(wifstream& in, UINT& data);
	virtual void ReadInt(wifstream& in, int& data);
	virtual void ReadFloat(wifstream& in, float& data);

	//txt file 사용할 경우 각 인자의 경계
	virtual void ReadSpace(wifstream& in);
	virtual void ReadEnter(wifstream& in);
private:

public:
	CTxtImporter();
	~CTxtImporter();
};
