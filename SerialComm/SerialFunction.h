#if !defined(AFX_PYH_COMM_H__BC230D73_1ABD_4A40_813F_27B124A805C6__INCLUDED_)
#define AFX_PYH_COMM_H__BC230D73_1ABD_4A40_813F_27B124A805C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#pragma once


#include <afxmt.h>
#include "SerialCommData.h"

#define MAXBUF			50000
#define InBufSize		50000
#define OutBufSize		50000
#define WM_MYRECEIVE	(WM_USER+1)
#define WM_MYCLOSE		(WM_USER+2)

class CSerialFunction : public CCmdTarget
{
	
	DECLARE_DYNCREATE(CSerialFunction)	

	CSerialFunction() {};
	~CSerialFunction();

public:
	CSerialCommData	m_cParam;		// Entity Ŭ����

	HWND	m_hWnd;					// �޼����� ������ ���
	HANDLE	m_hComDev;				// �ø�����Ʈ �ڵ�

	BOOL	m_bIsOpenned;			// �����Ʈ ���� �ִ���
	CString m_sComPort;				// COM ��Ʈ
	CString m_sBaudRate;			// Baud rate
	CString m_sParity;				// Parity bit
	CString m_sDataBit;				// Data bit
	CString m_sStopBit;				// Stop bit
	BOOL	m_bFlowChk;				// Flow Check
	
	OVERLAPPED m_OLR, m_OLW;
	char	m_sInBuf[MAXBUF * 2];	// ���Ź���
	int		m_nLength;				// �޴� ������ ����
	
	CEvent* m_pEvent;				// �����忡�� ����� �̺�Ʈ	

public:
	CSerialFunction(CString port, CString baudrate, CString parity, CString databit, CString stopbit);
	BOOL	Create		(HWND hWnd					);	// GUI call : �����ư
	void	Clear		();								// GUI call : �����ư, �����ư
	void	Close		();								// GUI call	: ���� ��ư
	BOOL	Send		(LPCTSTR outbuf	, int len	);	// GUI call : ������ ��ư	
	int		Receive		(LPSTR inbuf, int len);			// GUI call : ���� �޼��� �ݹ�
	void	HandleClose	();								// GUI call : ������ ���� �޼��� �ݹ�
	void	InitSerialParam();	// in Create()
	BOOL	InitOverlapped();	// in Create() 

protected:
	DECLARE_MESSAGE_MAP()
};

UINT CommThread(LPVOID lpData); // in Create()

#endif