#include "stdafx.h"
#include "SerialFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSerialFunction, CCmdTarget)


CSerialFunction::CSerialFunction(CString port, CString baudrate, CString parity, CString databit, CString stopbit)
{
	m_sComPort		=port;						// COM ��Ʈ
	m_sBaudRate		=baudrate;					// Baud rate
	m_sParity		=parity;					// Parity bit
	m_sDataBit		=databit;					// Data bit
	m_sStopBit		=stopbit;					// Stop bit
	m_bFlowChk		=1;							// Flow Check
	m_bIsOpenned = FALSE;						// �����Ʈ ���� �ִ���
	m_nLength		=0;							// �޴� ������ ����

	memset(m_sInBuf, 0, MAXBUF * 2);			// ���Ź���
	m_pEvent = new CEvent(FALSE, TRUE);	// �����忡�� ����� �̺�Ʈ
}

CSerialFunction::~CSerialFunction()
{
	if (m_bIsOpenned)
		Close();
	delete m_pEvent;
}

BEGIN_MESSAGE_MAP(CSerialFunction, CCmdTarget)
END_MESSAGE_MAP()

BOOL CSerialFunction::Create(HWND hWnd)
{
	m_hWnd = hWnd;																		// �޼����� ������ ���
	m_hComDev = CreateFile(m_sComPort, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);	// �ø��� ��Ʈ ����

	if (m_hComDev != INVALID_HANDLE_VALUE)		// ��Ʈ�� ���������� ������
		m_bIsOpenned = TRUE;					// ����
	else										// �׷��� ������
		return FALSE;							// ����

	InitSerialParam();							// �ø��� ��Ʈ ������ �ʱ�ȭ
	if (!InitOverlapped())						// Overlapped ����ü �ʱ�ȭ �� �̺�Ʈ ����
		return FALSE;

	AfxBeginThread(CommThread, (LPVOID)this);	// ������ ������ ���� ������ ����
	EscapeCommFunction(m_hComDev,SETDTR);		// DTR(Data Transfer Ready) ��ȣ�� ����

	return TRUE;
}

void CSerialFunction::Clear()
{
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	memset(m_sInBuf, 0, MAXBUF * 2);
	m_nLength = 0;
}

void CSerialFunction::Close()
{
	if (!m_bIsOpenned)
		return;

	m_bIsOpenned = FALSE;
	SetCommMask(m_hComDev, 0);
	EscapeCommFunction(m_hComDev, CLRDTR);
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	Sleep(500);
}

BOOL CSerialFunction::Send(LPCTSTR outbuf, int len)
{
	BOOL	bRet = TRUE;
	DWORD	errorFlags		= 0;
	COMSTAT	comStat;

	DWORD	bytesWritten	= 0;
	DWORD	bytesSend		= 0;

	char		cSendData[128];	// Buffer of multibyte string
	TCHAR		tSendData[128];	// Buffer of Unicode string
	int			nSendSize = 0;
	wsprintf(tSendData, _T("%s"), outbuf);					// tSendData�� outbuf�� ���� ����
	memset((void*)&cSendData[0],0x0, sizeof(cSendData));

	WideCharToMultiByte(				// Wide char�� Multibyte�� ��ȯ
		_AtlGetConversionACP(),						// Code page
		0,											// Performance and mapping flags
		tSendData,									// Wide-character string (Unicode string)
		sizeof(tSendData) / sizeof(tSendData[0]),   // Number of charactors in string
		cSendData,									// Buffer for new string (Multibyte string)
		sizeof(cSendData),							// Size of buffer
		NULL,										// Default for unmappable characters
		NULL);										// Set when default character used

	nSendSize = strlen(cSendData);

	ClearCommError(m_hComDev, &errorFlags, &comStat);					// �����Ʈ ���� �ʱ�ȭ

	if (!WriteFile(m_hComDev, cSendData, nSendSize, &bytesWritten, &m_OLW))		// Overlapped I/O�� �̿��Ͽ� ������ ����
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			if (WaitForSingleObject(m_OLW.hEvent, 1000) != WAIT_OBJECT_0)
				bRet = FALSE;
			else
				GetOverlappedResult(m_hComDev, &m_OLW, &bytesWritten, FALSE);
		}
		else
		{
			bRet = FALSE;
		}
	}
	ClearCommError(m_hComDev, &errorFlags, &comStat);					// �����Ʈ ���� �ʱ�ȭ

	return bRet;
}

int CSerialFunction::Receive(LPSTR inbuf, int len)
{
	CSingleLock lockObj((CSyncObject*)m_pEvent, FALSE);

	if (len == 0)
		return -1;
	else if (len > MAXBUF)
		return -1;

	if (m_nLength == 0)
	{
		inbuf[0] = '\0';
		return 0;
	}
	else if (m_nLength <= len)
	{
		lockObj.Lock();
		memcpy(inbuf, m_sInBuf, m_nLength);
		memset(m_sInBuf, 0, MAXBUF * 2);
		int temp = m_nLength;
		m_nLength = 0;
		lockObj.Unlock();
		return temp;
	}
	else
	{
		lockObj.Lock();
		memcpy(inbuf, m_sInBuf, len);
		memmove(m_sInBuf, m_sInBuf + len, MAXBUF * 2 - len);
		m_nLength -= len;
		lockObj.Unlock();
		return len;
	}
}

void CSerialFunction::HandleClose()
{
	CloseHandle(m_hComDev);
	CloseHandle(m_OLR.hEvent);
	CloseHandle(m_OLW.hEvent);
}

void CSerialFunction::InitSerialParam()
{
	DCB	dcb;
	DWORD DErr;
	COMMTIMEOUTS CommTimeOuts;

	if (!m_bIsOpenned)
		return;

	ClearCommError(m_hComDev, &DErr, NULL);					// �����Ʈ ���� �ʱ�ȭ
	SetupComm(m_hComDev, InBufSize, OutBufSize);			// �����Ʈ Input/Output ���� ũ�� ����
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	//Set up for overlapped I/O (MSDN ����)
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;	// ��ż��� ��, 1Byte ���۽ð�
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;	// Read ���۽�, Timeout ��� ����
	CommTimeOuts.ReadTotalTimeoutConstant = 0;		// Read ���۽�, Timeout ��� ����
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(m_hComDev, &CommTimeOuts);

	//DCB �ʱ�ȭ
	memset(&dcb, 0, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);							// DCBlength ����
	GetCommState(m_hComDev, &dcb);							// �����Ʈ�� DCB�� ����
	dcb.fBinary = TRUE;										// fBinary ����
	dcb.fParity = TRUE;										// fParity ����
	for (int idx = 0; idx < m_cParam.SIZE_BAUD; idx++)		// Baud Rate ����
	{
		if (m_sBaudRate == m_cParam.listBaud[idx])
			dcb.BaudRate = m_cParam.storeBaud[idx];
	}
	for (int idx = 0; idx < m_cParam.SIZE_PARITY; idx++)	// Parity ����
	{
		if (m_sParity == m_cParam.listParity[idx])
			dcb.Parity = m_cParam.storeParity[idx];
	}
	for (int idx = 0; idx < m_cParam.SIZE_DATA; idx++)		// Data bit ����
	{
		if (m_sDataBit == m_cParam.listData[idx])
			dcb.ByteSize = m_cParam.storeData[idx];
	}
	for (int idx = 0; idx < m_cParam.SIZE_STOP; idx++)		// Stop bit ����
	{
		if (m_sStopBit == m_cParam.listStop[idx])
			dcb.StopBits = m_cParam.storeStop[idx];
	}
	dcb.fRtsControl = RTS_CONTROL_ENABLE;					// fRtsControl ����
	dcb.fDtrControl = DTR_CONTROL_ENABLE;					// fDtrControl ����
	dcb.fOutxDsrFlow = FALSE;								// fOutxDsrFlow ����
	if (m_bFlowChk)
	{
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		dcb.XonLim = 2048;
		dcb.XoffLim = 1024;
	}
	else
	{
		dcb.fOutxCtsFlow = TRUE;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
	SetCommState(m_hComDev, &dcb);					// ������ dcb�� �����Ʈ�� �Ķ���� ����
	SetCommMask(m_hComDev, EV_RXCHAR);				// �Է¹��ۿ� ������ ������ ���, �̺�Ʈ �߻��ϵ��� ����
}

BOOL CSerialFunction::InitOverlapped()
{
	m_OLW.Offset = 0;		// Write Overlapped structure �ʱ�ȭ
	m_OLW.OffsetHigh = 0;
	m_OLR.Offset = 0;		// Read Overlapped structure �ʱ�ȭ
	m_OLR.OffsetHigh = 0;
	m_OLR.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_OLR.hEvent == NULL)
	{
		CloseHandle(m_OLR.hEvent);
		return FALSE;
	}
	m_OLW.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_OLW.hEvent == NULL)
	{
		CloseHandle(m_OLW.hEvent);
		return FALSE;
	}
	return TRUE;
}

UINT CommThread(LPVOID lpData)
{
	extern short	g_nRemoteStatus;
	DWORD			errorFlags	= 0;
	COMSTAT			comStat;
	DWORD			evtMask		= 0;
	char			buf[MAXBUF] = { 0, };
	DWORD			length		= 0;
	int				size		= 0;
	int				insize		= 0;

	CSerialFunction* Comm = (CSerialFunction*)lpData;
	while (Comm->m_bIsOpenned) // �����Ʈ�� ����������,
	{
		evtMask = 0;
		length = 0;
		insize = 0;
		memset(buf, '\0', MAXBUF);
		WaitCommEvent(Comm->m_hComDev, &evtMask, NULL);				// �̺�Ʈ ��ٸ�
		ClearCommError(Comm->m_hComDev, &errorFlags, &comStat);		// ���� �ʱ�ȭ
		if ((evtMask & EV_RXCHAR) && comStat.cbInQue)				// �̺�Ʈ �߻��ϸ�,
		{
			if (comStat.cbInQue > MAXBUF)
				size = MAXBUF;
			else
				size = comStat.cbInQue;
			do{
				ClearCommError(Comm->m_hComDev, &errorFlags, &comStat);		// ���� �ʱ�ȭ
				if (!ReadFile(Comm->m_hComDev, buf + insize, size, &length, &(Comm->m_OLR)))
				{
					if (GetLastError() == ERROR_IO_PENDING)
					{
						if (WaitForSingleObject(Comm->m_OLR.hEvent, 1000) != WAIT_OBJECT_0)
							length = 0;
						else
							GetOverlappedResult(Comm->m_hComDev, &(Comm->m_OLR), &length, FALSE);
					}
					else
						length = 0;
				}
				insize += length;
			} while ((length != 0) && (insize < size));
			ClearCommError(Comm->m_hComDev, &errorFlags, &comStat);		// ���� �ʱ�ȭ
			if (Comm->m_nLength + insize > MAXBUF * 2)
				insize = (Comm->m_nLength + insize) - MAXBUF * 2;
			Comm->m_pEvent->ResetEvent();								// �̺�Ʈ �߻� ��� �ߴ���, �Է¹��ۿ� ������ ����
			memcpy(Comm->m_sInBuf + Comm->m_nLength, buf, insize);
			Comm->m_nLength += insize;
			Comm->m_pEvent->SetEvent();									// ���� �Ϸ���, �̺�Ʈ Ȱ��ȭ
			LPARAM temp = (LPARAM)Comm;
			SendMessage(Comm->m_hWnd, WM_MYRECEIVE, Comm->m_nLength, temp);
		}
	}
	PurgeComm(Comm->m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	LPARAM temp = (LPARAM)Comm;
	SendMessage(Comm->m_hWnd, WM_MYCLOSE, 0, temp);
	return 0;
}