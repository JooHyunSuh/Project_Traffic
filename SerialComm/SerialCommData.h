#pragma once

#include "stdafx.h"
#include <vector>

// Ŭ���� �̸� : SerialCommData
// Ŭ���� ���� : Entity Ŭ����
// �ø��� ��� �Ķ����

class CSerialCommData
{
public:
	std::vector<CString>	listCom;		// COM ��Ʈ ���
	std::vector<CString>	listBaud;		// Baud Rate �޺��ڽ� ���
	std::vector<DWORD>		storeBaud;		// Baud Rate ���尪 ���
	std::vector<CString>	listParity;		// Parity �޺��ڽ� ���
	std::vector<BYTE>		storeParity;	// Parity ���尪 ���
	std::vector<CString>	listData;		// Data Bit �޺��ڽ� ���
	std::vector<BYTE>		storeData;		// Data Bit ���尪 ���
	std::vector<CString>	listStop;		// Stop Bit �޺��ڽ� ���
	std::vector<BYTE>		storeStop;		// Stop Bit ���尪 ���

	enum { SIZE_COM = 21, SIZE_BAUD = 21, SIZE_PARITY = 3, SIZE_DATA = 2, SIZE_STOP = 3 };

public:
	CSerialCommData();
	~CSerialCommData();
	void InitListCom();
	void InitListBaud();
	void InitStoreBaud();
	void InitListParity();
	void InitStoreParity();
	void InitListData();
	void InitStoreData();
	void InitListStop();
	void InitStoreStop();
};