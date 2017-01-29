#include "stdafx.h"
#include "TxtImporter.h"


void CTxtImporter::ReadWCHAR(wifstream& in, WCHAR * data, int size) {
	in >> data;
}

void CTxtImporter::ReadCHAR(wifstream& in, char * data, int size) {
	//in >> data;
}

void CTxtImporter::ReadUINT(wifstream& in, UINT& data) {
	in >> data;
}

void CTxtImporter::ReadInt(wifstream& in, int& data) {
	in >> data;
}

void CTxtImporter::ReadFloat(wifstream& in, float& data) {
	in >> data;
}

void CTxtImporter::ReadSpace(wifstream& in) {
	//in >> L" ";
}

void CTxtImporter::ReadEnter(wifstream& in) {
	//in >> L"\n";
}

CTxtImporter::CTxtImporter() {
}

CTxtImporter::~CTxtImporter() {

}
