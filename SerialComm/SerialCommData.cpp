#include "stdafx.h"
#include "SerialCommData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSerialCommData::CSerialCommData( )
{
	InitListCom();
	InitListBaud();
	InitStoreBaud();
	InitListParity();
	InitStoreParity();
	InitListData();
	InitStoreData();
	InitListStop();
	InitStoreStop();
}

CSerialCommData::~CSerialCommData()
{
	listCom.clear();
	listBaud.clear();
	storeBaud.clear();
	listParity.clear();
	storeParity.clear();
	listData.clear();
	storeData.clear();
	listStop.clear();
	storeStop.clear();
}
void CSerialCommData::InitListCom()
{
	listCom.reserve(SIZE_COM);

	CString tempStr;
	for (int idx = 0; idx < SIZE_COM; idx++)
	{
		tempStr.Format(_T("COM%d"), idx);
		listCom.push_back(tempStr);
	}
}

void CSerialCommData::InitListBaud( )
{
	listBaud.reserve(SIZE_BAUD);
	listBaud.push_back(_T("300"));				
	listBaud.push_back(_T("600"));				
	listBaud.push_back(_T("1200"));				
	listBaud.push_back(_T("2400"));				
	listBaud.push_back(_T("4800"));				
	listBaud.push_back(_T("9600"));				
	listBaud.push_back(_T("14400"));			
	listBaud.push_back(_T("19200"));
	listBaud.push_back(_T("28800"));
	listBaud.push_back(_T("33600"));
	listBaud.push_back(_T("38400"));
	listBaud.push_back(_T("56000"));
	listBaud.push_back(_T("57600"));
	listBaud.push_back(_T("115200"));
	listBaud.push_back(_T("128000"));
	listBaud.push_back(_T("256000"));
	listBaud.push_back(_T("PCI_9600"));
	listBaud.push_back(_T("PCI_19200"));
	listBaud.push_back(_T("PCI_38400"));
	listBaud.push_back(_T("PCI_57600"));
	listBaud.push_back(_T("PCI_500K"));
}

void CSerialCommData::InitStoreBaud()
{
	storeBaud.reserve(SIZE_BAUD);
	storeBaud.push_back(CBR_300);				
	storeBaud.push_back(CBR_600);				
	storeBaud.push_back(CBR_1200);				
	storeBaud.push_back(CBR_2400);				
	storeBaud.push_back(CBR_4800);				
	storeBaud.push_back(CBR_9600);				
	storeBaud.push_back(CBR_14400);			
	storeBaud.push_back(CBR_19200);
	storeBaud.push_back(CBR_38400);
	storeBaud.push_back(CBR_38400);
	storeBaud.push_back(CBR_38400);
	storeBaud.push_back(CBR_56000);
	storeBaud.push_back(CBR_57600);
	storeBaud.push_back(CBR_115200);
	storeBaud.push_back(CBR_128000);
	storeBaud.push_back(CBR_256000);
	storeBaud.push_back(1075);
	storeBaud.push_back(2212);
	storeBaud.push_back(4300);
	storeBaud.push_back(6450);
	storeBaud.push_back(56000);
}

void CSerialCommData::InitListParity()
{
	listParity.reserve(SIZE_PARITY);
	listParity.push_back(_T("None"));
	listParity.push_back(_T("Even"));
	listParity.push_back(_T("Odd"));
}

void CSerialCommData::InitStoreParity()
{
	storeParity.reserve(SIZE_PARITY);
	storeParity.push_back(NOPARITY);
	storeParity.push_back(EVENPARITY);
	storeParity.push_back(ODDPARITY);
}

void CSerialCommData::InitListData() 
{
	listData.reserve(SIZE_DATA);
	listData.push_back(_T("7 Bit"));
	listData.push_back(_T("8 Bit"));
}

void CSerialCommData::InitStoreData()
{
	storeData.reserve(SIZE_DATA);
	storeData.push_back(7);
	storeData.push_back(8);
}

void CSerialCommData::InitListStop()
{
	listStop.reserve(SIZE_STOP);
	listStop.push_back(_T("1 Bit"));
	listStop.push_back(_T("1.5 Bit"));
	listStop.push_back(_T("2 Bit"));
}

void CSerialCommData::InitStoreStop()
{
	storeStop.reserve(SIZE_STOP);
	storeStop.push_back(ONESTOPBIT);
	storeStop.push_back(ONE5STOPBITS);
	storeStop.push_back(TWOSTOPBITS);
}