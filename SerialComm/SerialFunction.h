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
	CSerialCommData	m_cParam;		// Entity 클래스

	HWND	m_hWnd;					// 메세지를 보낼때 사용
	HANDLE	m_hComDev;				// 시리얼포트 핸들

	BOOL	m_bIsOpenned;			// 통신포트 열려 있는지
	CString m_sComPort;				// COM 포트
	CString m_sBaudRate;			// Baud rate
	CString m_sParity;				// Parity bit
	CString m_sDataBit;				// Data bit
	CString m_sStopBit;				// Stop bit
	BOOL	m_bFlowChk;				// Flow Check
	
	OVERLAPPED m_OLR, m_OLW;
	char	m_sInBuf[MAXBUF * 2];	// 수신버퍼
	int		m_nLength;				// 받는 데이터 길이
	
	CEvent* m_pEvent;				// 쓰레드에서 사용할 이벤트	

public:
	CSerialFunction(CString port, CString baudrate, CString parity, CString databit, CString stopbit);
	BOOL	Create		(HWND hWnd					);	// GUI call : 연결버튼
	void	Clear		();								// GUI call : 연결버튼, 끊기버튼
	void	Close		();								// GUI call	: 끊기 버튼
	BOOL	Send		(LPCTSTR outbuf	, int len	);	// GUI call : 보내기 버튼	
	int		Receive		(LPSTR inbuf, int len);			// GUI call : 수신 메세지 콜백
	void	HandleClose	();								// GUI call : 쓰레드 종료 메세지 콜백
	void	InitSerialParam();	// in Create()
	BOOL	InitOverlapped();	// in Create() 

protected:
	DECLARE_MESSAGE_MAP()
};

UINT CommThread(LPVOID lpData); // in Create()

#endif