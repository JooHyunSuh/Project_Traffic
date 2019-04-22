
// SerialCommView.h : CSerialCommView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "SerialFunction.h"
#include "afxwin.h"
#include "afxbutton.h"

class CSerialCommView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CSerialCommView();
	DECLARE_DYNCREATE(CSerialCommView)

public:
	enum{ IDD = IDD_SERIALCOMM_FORM };

// 특성입니다.
public:
	CSerialCommDoc* GetDocument() const;

// 작업입니다.
public:
	CSerialFunction* m_Comm;
	CSerialCommData* m_pParam;

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CSerialCommView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonConnect();							// 연결버튼 함수
	afx_msg void OnClickedButtonDisconnect();						// 끊기버튼 함수
	afx_msg void OnClickedButtonTx();								// 보내기버튼 함수
	LRESULT OnThreadClosed(WPARAM length, LPARAM lpara);	// 쓰레드종료 메세지 함수
	LRESULT OnReceive(WPARAM length, LPARAM lpara);			// 데이터수신 메세지 함수
	void SerialWindowEnable(BOOL bValue);
// 사용자 정의
public:
	CEdit		m_rxData;	// 수신 메세지 에디터		
	CComboBox	m_cbCOM;	// COM 포트		콤보박스
	CComboBox	m_cbBAUD;	// Baud Rate	콤보박스
	CComboBox	m_cbPARITY; // Parity		콤보박스
	CComboBox	m_cbDATA;	// Data bit		콤보박스
	CComboBox	m_cbSTOP;	// Stop bit		콤보박스
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

#ifndef _DEBUG  // SerialCommView.cpp의 디버그 버전
inline CSerialCommDoc* CSerialCommView::GetDocument() const
   { return reinterpret_cast<CSerialCommDoc*>(m_pDocument); }
#endif

