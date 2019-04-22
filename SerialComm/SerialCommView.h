
// SerialCommView.h : CSerialCommView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "SerialFunction.h"
#include "afxwin.h"
#include "afxbutton.h"

class CSerialCommView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CSerialCommView();
	DECLARE_DYNCREATE(CSerialCommView)

public:
	enum{ IDD = IDD_SERIALCOMM_FORM };

// Ư���Դϴ�.
public:
	CSerialCommDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CSerialFunction* m_Comm;
	CSerialCommData* m_pParam;

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CSerialCommView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonConnect();							// �����ư �Լ�
	afx_msg void OnClickedButtonDisconnect();						// �����ư �Լ�
	afx_msg void OnClickedButtonTx();								// �������ư �Լ�
	LRESULT OnThreadClosed(WPARAM length, LPARAM lpara);	// ���������� �޼��� �Լ�
	LRESULT OnReceive(WPARAM length, LPARAM lpara);			// �����ͼ��� �޼��� �Լ�
	void SerialWindowEnable(BOOL bValue);
// ����� ����
public:
	CEdit		m_rxData;	// ���� �޼��� ������		
	CComboBox	m_cbCOM;	// COM ��Ʈ		�޺��ڽ�
	CComboBox	m_cbBAUD;	// Baud Rate	�޺��ڽ�
	CComboBox	m_cbPARITY; // Parity		�޺��ڽ�
	CComboBox	m_cbDATA;	// Data bit		�޺��ڽ�
	CComboBox	m_cbSTOP;	// Stop bit		�޺��ڽ�
	afx_msg void OnBnClickedButtonRxclear();
	CMFCButton m_blecolor;
	CMFCButton m_lightcolor;
	CStatic m_picture;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	VideoCapture *capture;
	Mat mat_frame;
	CImage cimage_mfc;
	afx_msg void OnBnClickedBtCntraffic();
	afx_msg void OnBnClickedBtCheck();
};

#ifndef _DEBUG  // SerialCommView.cpp�� ����� ����
inline CSerialCommDoc* CSerialCommView::GetDocument() const
   { return reinterpret_cast<CSerialCommDoc*>(m_pDocument); }
#endif

