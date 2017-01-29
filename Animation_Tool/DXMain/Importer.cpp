#include "stdafx.h"
#include "Importer.h"

bool CImporter::Begin(wstring outputPath) {
	m_inputPath = outputPath;
	m_in = wifstream(m_inputPath, ios::out);

	m_pFileImporter = new CTxtImporter();

	return true;
}

bool CImporter::End() {
	m_in.close();

	delete m_pFileImporter;
	m_pFileImporter = nullptr;

	return true;
}

void CImporter::ReadWCHAR(WCHAR * data, int size) {
	m_pFileImporter->ReadWCHAR(m_in, data, size);
}

void CImporter::ReadCHAR(char * data, int size) {
	m_pFileImporter->ReadCHAR(m_in, data, size);
}

void CImporter::ReadUINT(UINT& data) {
	m_pFileImporter->ReadUINT(m_in, data);
}

void CImporter::ReadInt(int& data) {
	m_pFileImporter->ReadInt(m_in, data);
}

void CImporter::ReadFloat(float& data) {
	m_pFileImporter->ReadFloat(m_in, data);
}

void CImporter::ReadFloat4x4(XMFLOAT4X4& xmf4x4) {
	ReadFloat(xmf4x4._11); ReadFloat(xmf4x4._12); ReadFloat(xmf4x4._13); ReadFloat(xmf4x4._14);
	ReadFloat(xmf4x4._21); ReadFloat(xmf4x4._22); ReadFloat(xmf4x4._23); ReadFloat(xmf4x4._24);
	ReadFloat(xmf4x4._31); ReadFloat(xmf4x4._32); ReadFloat(xmf4x4._33); ReadFloat(xmf4x4._34);
	ReadFloat(xmf4x4._41); ReadFloat(xmf4x4._42); ReadFloat(xmf4x4._43); ReadFloat(xmf4x4._44);
}

void CImporter::ReadXMMatrix(XMMATRIX& xmMtx) {
	XMFLOAT4X4 xmf4x4;
	XMStoreFloat4x4(&xmf4x4, xmMtx);
	ReadFloat4x4(xmf4x4);
}

void CImporter::ReadSpace() {
	m_pFileImporter->ReadSpace(m_in);
}

void CImporter::ReadEnter() {
	m_pFileImporter->ReadEnter(m_in);
}
