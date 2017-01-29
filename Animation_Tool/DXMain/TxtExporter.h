#pragma once
#include "FileExporter.h"

//File Exporter�� ����ϴ� ��ü
class CTxtExporter :public CFileExporter{
public:
	void WriteWCHAR(wofstream& out, const WCHAR* data, int size);
	void WriteCHAR(wofstream& out, const char* data, int size);
	void WriteUINT(wofstream& out, const UINT data);
	void WriteInt(wofstream& out, const int data);
	void WriteFloat(wofstream& out, const float data);

	//txt file ����� ��� �� ������ ���
	void WriteSpace(wofstream& out);
	void WriteEnter(wofstream& out);
private:
	
public:
	CTxtExporter();
	~CTxtExporter();
};
