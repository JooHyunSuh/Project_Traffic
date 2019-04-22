#pragma once

#include "stdafx.h"
#include <vector>

// 클래스 이름 : SerialCommData
// 클래스 유형 : Entity 클래스
// 시리얼 통신 파라미터

class CSerialCommData
{
public:
	std::vector<CString>	listCom;		// COM 포트 목록
	std::vector<CString>	listBaud;		// Baud Rate 콤보박스 목록
	std::vector<DWORD>		storeBaud;		// Baud Rate 저장값 목록
	std::vector<CString>	listParity;		// Parity 콤보박스 목록
	std::vector<BYTE>		storeParity;	// Parity 저장값 목록
	std::vector<CString>	listData;		// Data Bit 콤보박스 목록
	std::vector<BYTE>		storeData;		// Data Bit 저장값 목록
	std::vector<CString>	listStop;		// Stop Bit 콤보박스 목록
	std::vector<BYTE>		storeStop;		// Stop Bit 저장값 목록

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