
// MfcStartDlg.h: 헤더 파일
//

#pragma once
#include <vector>
#include <atlimage.h>

// CMfcStartDlg 대화 상자
class CMfcStartDlg : public CDialogEx
{
// 생성입니다.
public:
	CMfcStartDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CPoint m_ptClick[3];
	int m_nClickCount = 0;

	CPoint m_ptCenter;
	double m_dRadius = 0.0;
	bool m_bIsCircleReady = false;

	bool m_bIsDragging = false;
	int m_nDragIndex = -1;

	CImage m_image;
	void CircleImage();
	void drawDot(int x, int y, unsigned char colorIdx);
	void DrawCircleCustom(int xc, int yc, int r, unsigned char colorIdx);
	void DrawCircleFilled(int xc, int yc, int r, unsigned char colorIdx);
	void DrawCircleBorder(int xc, int yc, int r, int thickness, unsigned char colorIdx);
	void CalculateCircle(CPoint p1, CPoint p2, CPoint p3);

	int m_nPointRadius;
	int m_nThickness;

	CButton BTN_RESET;
	CButton BTN_RANDOM;

	static UINT ThreadRandomMove(LPVOID pParam);
	bool m_bIsThreadRunning = false;
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSTART_DIALOG };
#endif

	protected:

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRandom();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
