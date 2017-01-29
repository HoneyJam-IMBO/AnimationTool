#pragma once
//#include "Camera.h"
#include "SingleTon.h"
#include "FileImporter.h"
#include "TxtImporter.h"
#include <fstream>

class CImporter : public CSingleTonBase<CImporter> {
public:
	bool Begin(wstring inputPath);//file open하구
	virtual bool End();//file close

	void ReadWCHAR(WCHAR* data, int size);
	void ReadCHAR(char* data, int size);
	void ReadUINT(UINT& data);
	void ReadInt(int& data);
	void ReadFloat(float& data);
	void ReadFloat4x4(XMFLOAT4X4& xmf4x4);
	void ReadXMMatrix(XMMATRIX& xmMtx);
	//txt file 사용할 경우 각 인자의 경계
	void ReadSpace();
	void ReadEnter();
private:
	wstring m_inputPath;
	wifstream m_in;
	
	CFileImporter* m_pFileImporter{ nullptr };
public:
	CImporter() : CSingleTonBase<CImporter>("importersingleton") {}

};
