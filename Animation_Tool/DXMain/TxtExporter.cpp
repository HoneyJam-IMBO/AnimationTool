#include "stdafx.h"
#include "TxtExporter.h"


void CTxtExporter::WriteCHAR(wofstream & out, const char * data, int size){
	out << data;
}

void CTxtExporter::WriteWCHAR(wofstream& out, const WCHAR * data, int size) {
	out << data;
}

void CTxtExporter::WriteUINT(wofstream& out, const UINT data) {
	out << data;
}

void CTxtExporter::WriteInt(wofstream& out, const int data) {
	out << data;
}

void CTxtExporter::WriteFloat(wofstream& out, const float data) {
	out << data;
}

void CTxtExporter::WriteSpace(wofstream& out) {
	out << L" ";
}

void CTxtExporter::WriteEnter(wofstream& out) {
	out << L"\n";
}

CTxtExporter::CTxtExporter(){
}

CTxtExporter::~CTxtExporter(){

}
