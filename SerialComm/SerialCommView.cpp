
// SerialCommView.cpp : CSerialCommView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "SerialComm.h"
#endif

#include "SerialCommDoc.h"
#include "SerialCommView.h"
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSerialCommView

IMPLEMENT_DYNCREATE(CSerialCommView, CFormView)

BEGIN_MESSAGE_MAP(CSerialCommView, CFormView)
	ON_BN_CLICKED	(IDC_BUTTON_CONNECT,	&CSerialCommView::OnClickedButtonConnect	)
	ON_BN_CLICKED	(IDC_BUTTON_DISCONNECT, &CSerialCommView::OnClickedButtonDisconnect	)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CSerialCommView::OnClickedButtonTx)
	ON_MESSAGE(WM_MYCLOSE, OnThreadClosed)
	ON_MESSAGE(WM_MYRECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_RXCLEAR, &CSerialCommView::OnBnClickedButtonRxclear)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_CNTRAFFIC, &CSerialCommView::OnBnClickedBtCntraffic)
	ON_BN_CLICKED(IDC_BT_CHECK, &CSerialCommView::OnBnClickedBtCheck)
END_MESSAGE_MAP()

// CSerialCommView ����/�Ҹ�

CSerialCommView::CSerialCommView()
	: CFormView(CSerialCommView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CSerialCommView::~CSerialCommView()
{
	delete m_pParam;
}

void CSerialCommView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RX, m_rxData);
	DDX_Control(pDX, IDC_COMBO_COM, m_cbCOM);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_cbBAUD);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_cbPARITY);
	DDX_Control(pDX, IDC_COMBO_DATA, m_cbDATA);
	DDX_Control(pDX, IDC_COMBO_STOP, m_cbSTOP);
	DDX_Control(pDX, IDC_MFC_BLESTA, m_blecolor);
	DDX_Control(pDX, IDC_MFC_LightSTA, m_lightcolor);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}

BOOL CSerialCommView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CSerialCommView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	SerialWindowEnable(TRUE);
	m_pParam = new CSerialCommData;
	for (int idx = 0; idx < m_pParam->SIZE_COM;		idx++)	m_cbCOM.	AddString(m_pParam->listCom		[idx]);
	for (int idx = 0; idx < m_pParam->SIZE_BAUD;	idx++)	m_cbBAUD.	AddString(m_pParam->listBaud	[idx]);
	for (int idx = 0; idx < m_pParam->SIZE_PARITY;	idx++)	m_cbPARITY.	AddString(m_pParam->listParity	[idx]);
	for (int idx = 0; idx < m_pParam->SIZE_DATA;	idx++)	m_cbDATA.	AddString(m_pParam->listData	[idx]);
	for (int idx = 0; idx < m_pParam->SIZE_STOP;	idx++)	m_cbSTOP.	AddString(m_pParam->listStop	[idx]);

	m_cbCOM.	SetCurSel(5	);	// COM ��Ʈ		: COM 5
	m_cbBAUD.	SetCurSel(5);	// Baud rate	: 9600
	m_cbPARITY.	SetCurSel(0	);	// Parity		: None
	m_cbDATA.	SetCurSel(1	);	// Data bit		: 8 bit
	m_cbSTOP.	SetCurSel(0	);	// Stop bit		: 1 bit

	//CSize size;
	//size = GetTotalSize();
	//size.cx = 0;
	//size.cy = 0;
	//SetScrollSizes(MM_TEXT, size);
	

	m_blecolor.SetFaceColor(RGB(255, 0, 0));
	m_lightcolor.SetFaceColor(RGB(255, 0, 0));

	//����ó���� 
	capture = new VideoCapture(0);
	if (!capture->isOpened())
	{
		MessageBox(_T("ķ�� ���� �����ϴ�. \n"));
	}

	//��ķ ũ�⸦  400x600���� ����    
	capture->set(CAP_PROP_FRAME_WIDTH, 400);
	capture->set(CAP_PROP_FRAME_HEIGHT, 600);

	SetTimer(1000, 30, NULL);

}


// CSerialCommView ����

#ifdef _DEBUG
void CSerialCommView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSerialCommView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSerialCommDoc* CSerialCommView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSerialCommDoc)));
	return (CSerialCommDoc*)m_pDocument;
}
#endif //_DEBUG


// CSerialCommView �޽��� ó����


void CSerialCommView::OnClickedButtonConnect()
{
	CString curCOM;
	CString curBAUD;
	CString curPARITY;
	CString curDATA;
	CString curSTOP;

	this->m_cbCOM.		GetLBText(this->m_cbCOM.GetCurSel(),	curCOM		);
	this->m_cbBAUD.		GetLBText(this->m_cbBAUD.GetCurSel(),	curBAUD		);
	this->m_cbPARITY.	GetLBText(this->m_cbPARITY.GetCurSel(),	curPARITY	);
	this->m_cbDATA.		GetLBText(this->m_cbDATA.GetCurSel(),	curDATA		);
	this->m_cbSTOP.		GetLBText(this->m_cbSTOP.GetCurSel(),	curSTOP		);

	m_Comm = new CSerialFunction(_T("\\\\.\\") + curCOM, curBAUD, curPARITY, curDATA, curSTOP);
	if (m_Comm->Create(GetSafeHwnd()) == TRUE)
	{
		m_blecolor.SetFaceColor(RGB(0, 255, 0));
		m_Comm->Clear();
		SerialWindowEnable(FALSE);
	}
	else
	{
		
		SerialWindowEnable(TRUE);
	
	}
}

void CSerialCommView::OnClickedButtonDisconnect()
{
	if (m_Comm)
	{
		m_Comm->Close();
		m_Comm = NULL;
		m_blecolor.SetFaceColor(RGB(255, 0, 0));
		SerialWindowEnable(TRUE);
		
		
	}
}

void CSerialCommView::OnClickedButtonTx()
{
	CString str;
	GetDlgItem(IDC_EDIT_TX)->GetWindowTextW(str);
	str += _T("\r\n");
	m_Comm->Send(str, str.GetLength());
}

LRESULT CSerialCommView::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	((CSerialFunction*)lpara)->HandleClose();
	delete ((CSerialFunction*)lpara);
	return 0;
}

LRESULT CSerialCommView::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[20000];
	if (m_Comm)
	{
		m_Comm->Receive(data, length);
		data[length] = '\0';
		for (int idx = 0; idx < (int)length; idx++)
			str += data[idx];

		m_rxData.ReplaceSel(str);
		//GetDlgItem(IDC_EDIT_RX)->SetWindowTextW(str);
		str = _T("");
	}
	return 0;
}

void CSerialCommView::SerialWindowEnable(BOOL bValue)
{
	GetDlgItem(IDC_BUTTON_CONNECT)	->EnableWindow(bValue);		// �����ư
	GetDlgItem(IDC_COMBO_COM)		->EnableWindow(bValue);		// COM ��Ʈ		�޺��ڽ�
	GetDlgItem(IDC_COMBO_BAUD)		->EnableWindow(bValue);		// Baud Rate	�޺��ڽ�
	GetDlgItem(IDC_COMBO_PARITY)	->EnableWindow(bValue);		// Parity		�޺��ڽ�
	GetDlgItem(IDC_COMBO_DATA)		->EnableWindow(bValue);		// Data bit		�޺��ڽ�
	GetDlgItem(IDC_COMBO_STOP)		->EnableWindow(bValue);		// Stop bit		�޺��ڽ�

	GetDlgItem(IDC_BUTTON_DISCONNECT)	->EnableWindow(!bValue);	// �����ư
	GetDlgItem(IDC_BUTTON_TX)			->EnableWindow(!bValue);	// �������ư
	GetDlgItem(IDC_BUTTON_RXCLEAR)		->EnableWindow(!bValue);	// ����â ����� ��ư
	GetDlgItem(IDC_EDIT_TX)				->EnableWindow(!bValue);	// �۽� �޼��� ������
	GetDlgItem(IDC_EDIT_RX)				->EnableWindow(!bValue);	// ���� �޼��� ������
	GetDlgItem(IDC_BT_CHECK)			->EnableWindow(!bValue);	// AT��ɾ�
	GetDlgItem(IDC_BT_CNTRAFFIC)		->EnableWindow(!bValue);	// �Ƶ��̳� ����
	GetDlgItem(IDC_BT_STATUS)			->EnableWindow(!bValue);	// ����� ��� ��ɾ�
}

void CSerialCommView::OnBnClickedButtonRxclear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_EDIT_RX)->SetWindowText(_T(" "));
}


void CSerialCommView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CSerialCommView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//mat_frame�� �Է� �̹����Դϴ�. 
	capture->read(mat_frame);


	//�̰��� OpenCV �Լ����� �����մϴ�.

	//���⿡���� �׷��̽����� �̹����� ��ȯ�մϴ�.
	//cvtColor(mat_frame, mat_frame, COLOR_BGR2GRAY);



	//ȭ�鿡 �����ֱ� ���� ó���Դϴ�.
	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_frame.cols % 4);
	}



	Mat mat_temp;
	if (border > 0 || mat_frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_frame;
	}


	RECT r;
	m_picture.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);


	BITMAPINFO *bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//�׷��̽����� �ΰ�� �ȷ�Ʈ�� �ʿ�
	if (bpp == 8)
	{
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}


	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect

	if (mat_temp.cols == winSize.width  && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}


	HDC dc = ::GetDC(m_picture.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);


	::ReleaseDC(m_picture.m_hWnd, dc);

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();



	CFormView::OnTimer(nIDEvent);
}




void CSerialCommView::OnBnClickedBtCntraffic()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	str += _T("AT+CON3403DE3E7B1C\r\n");
	m_Comm->Send(str, str.GetLength());
}


void CSerialCommView::OnBnClickedBtCheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	str += _T("AT\r\n");
	m_Comm->Send(str, str.GetLength());
}






