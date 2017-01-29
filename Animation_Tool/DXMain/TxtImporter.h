#pragma once
#include "FileImporter.h"

//File Exporter�� ����ϴ� ��ü
class CTxtImporter :public CFileImporter {
public:
	virtual void ReadWCHAR(wifstream& in, WCHAR* data, int size);
	virtual void ReadCHAR(wifstream& in, char* data, int size);
	virtual void ReadUINT(wifstream& in, UINT& data);
	virtual void ReadInt(wifstream& in, int& data);
	virtual void ReadFloat(wifstream& in, float& data);

	//txt file ����� ��� �� ������ ���
	virtual void ReadSpace(wifstream& in);
	virtual void ReadEnter(wifstream& in);
private:

public:
	CTxtImporter();
	~CTxtImporter();
};
