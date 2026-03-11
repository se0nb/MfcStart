
// MfcStartDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MfcStart.h"
#include "MfcStartDlg.h"
#include "afxdialogex.h"
#include <cmath>
#include <ctime>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMfcStartDlg 대화 상자

CMfcStartDlg::CMfcStartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSTART_DIALOG, pParent)
	, m_nPointRadius(10)
	, m_nThickness(2)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_nPointRadius);
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_nThickness);
}

BEGIN_MESSAGE_MAP(CMfcStartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_RESET, &CMfcStartDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CMfcStartDlg::OnBnClickedBtnRandom)
END_MESSAGE_MAP()


// CMfcStartDlg 메시지 처리기

BOOL CMfcStartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	srand((unsigned int)time(NULL));
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMfcStartDlg::CircleImage()
{
	CRect rect;
	GetClientRect(&rect);
	int nWidth = rect.Width();
	int nHeight = rect.Height();

	if (m_image.IsNull() || m_image.GetWidth() != nWidth || abs(m_image.GetHeight()) != nHeight)
	{
		if (!m_image.IsNull()) m_image.Destroy();
		m_image.Create(nWidth, -nHeight, 8);

		RGBQUAD rgb[256];
		for (int i=0; i<256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = (BYTE)i;
			rgb[i].rgbReserved = 0;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();
	memset(fm, 255, abs(nPitch) * nHeight);
}

void CMfcStartDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMfcStartDlg::OnPaint()
{
	CPaintDC dc(this);
	CircleImage();

	for (int i=0; i<m_nClickCount; i++)
	{
		DrawCircleFilled(m_ptClick[i].x, m_ptClick[i].y, m_nPointRadius, 0);
	}

	if (m_bIsCircleReady)
	{
		DrawCircleBorder(m_ptCenter.x, m_ptCenter.y, (int)m_dRadius, m_nThickness, 0);
	}

	m_image.Draw(dc.GetSafeHdc(), 0, 0);

	dc.SetBkMode(TRANSPARENT);
	for (int i=0; i<m_nClickCount; i++)
	{
		CString s;
		s.Format(_T("(%d, %d)"), m_ptClick[i].x, m_ptClick[i].y);
		dc.TextOutW(m_ptClick[i].x + m_nPointRadius, m_ptClick[i].y, s);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMfcStartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

struct MyCircle
{
	CPoint centers[3];
	int count = 0;
	int pointRadius;
	int borderThickness;
};

void CMfcStartDlg::drawDot(int x, int y, unsigned char colorIdx)
{
	if (m_image.IsNull()) return;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	if (x < 0 || x >= nWidth || y < 0 || y >= nHeight) return;

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();

	fm[y * nPitch + x] = colorIdx;
}

void CMfcStartDlg::DrawCircleCustom(int xc, int yc, int r, unsigned char colorIdx) {
	int x = 0, y = r;
	int d = 3 - 2 * r;
	while (y >= x)
	{
		drawDot(xc + x, yc + y, colorIdx); drawDot(xc - x, yc + y, colorIdx);
		drawDot(xc + x, yc - y, colorIdx); drawDot(xc - x, yc - y, colorIdx);
		drawDot(xc + y, yc + x, colorIdx); drawDot(xc - y, yc + x, colorIdx);
		drawDot(xc + y, yc - x, colorIdx); drawDot(xc - y, yc - x, colorIdx);
		if (d < 0) d = d + 4 * x + 6;
		else
		{
			d = d + 4 * (x - y) + 10; y--;
		}
		x++;
	}
}

void CMfcStartDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);

	if (m_nPointRadius <= 0)
	{
		AfxMessageBox(_T("점의 반지름"));
		return;
	}

	if (m_nThickness <= 0)
	{
		AfxMessageBox(_T("정원의 두께"));
		return;
	}

	if (m_nClickCount == 3)
	{
		for (int i=0; i<3; i++)
		{
			double dist = sqrt(pow(m_ptClick[i].x - point.x, 2) + pow(m_ptClick[i].y - point.y, 2));
			if (dist <= m_nPointRadius)
			{
				m_bIsDragging = true;
				m_nDragIndex = i;
				SetCapture();
				return;
			}
		}
	}

	if (m_nClickCount < 3)
	{
		m_ptClick[m_nClickCount] = point;
		m_nClickCount++;

		if (m_nClickCount == 3)
		{
			CalculateCircle(m_ptClick[0], m_ptClick[1], m_ptClick[2]);
		}

		Invalidate(FALSE);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMfcStartDlg::OnMouseMove(UINT nFlags, CPoint point) {
	if (m_bIsDragging)
	{
		CRect rect; GetClientRect(&rect);
		if (rect.PtInRect(point))
		{
			m_ptClick[m_nDragIndex] = point;
			CalculateCircle(m_ptClick[0], m_ptClick[1], m_ptClick[2]);
			Invalidate(FALSE);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMfcStartDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	if (m_bIsDragging) 
	{
		m_bIsDragging = false;
		ReleaseCapture();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMfcStartDlg::OnBnClickedBtnReset()
{
	m_nClickCount = 0;
	m_bIsCircleReady = false;
	m_bIsDragging = false;
	m_nDragIndex = -1; 

	m_nPointRadius = 0;
	m_nThickness = 0;
	UpdateData(FALSE);

	Invalidate(TRUE);
}

void CMfcStartDlg::OnBnClickedBtnRandom()
{
	if (m_nClickCount < 3)
		return;
	if (m_bIsThreadRunning)
		return;

	m_bIsThreadRunning = true;
	AfxBeginThread(ThreadRandomMove, this);
}

UINT CMfcStartDlg::ThreadRandomMove(LPVOID pParam)
{
	CMfcStartDlg* p = (CMfcStartDlg*)pParam;
	CRect r;
	p -> GetClientRect(&r);

	for (int i=0; i<10; i++)
	{
		for (int j=0; j<3; j++)
		{
			p->m_ptClick[j].x = rand() % (r.Width() - 150) + 75;
			p->m_ptClick[j].y = rand() % (r.Height() - 150) + 75;
		}
		p->CalculateCircle(p->m_ptClick[0], p->m_ptClick[1], p->m_ptClick[2]);
		p->Invalidate(FALSE);
		Sleep(500);
	}

	p->m_bIsThreadRunning = false;

	return 0;
}

void CMfcStartDlg::DrawCircleFilled(int xc, int yc, int r, unsigned char colorIdx) {
	int r2 = r * r;

	for (int y=yc-r; y<=yc+r; y++)
	{
		for (int x=xc-r; x<=xc+r; x++)
		{

			int dist2 = (x - xc) * (x - xc) + (y - yc) * (y - yc);

			if (dist2 <= r2)
			{
				drawDot(x, y, colorIdx);
			}
		}
	}
}

void CMfcStartDlg::CalculateCircle(CPoint p1, CPoint p2, CPoint p3)
{
	double x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y, x3 = p3.x, y3 = p3.y;
	double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

	if (abs(D) < 0.0001) {
		m_bIsCircleReady = false;
		return;
	}

	m_ptCenter.x = (long)(((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D);
	m_ptCenter.y = (long)(((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D);
	m_dRadius = sqrt(pow(x1 - m_ptCenter.x, 2) + pow(y1 - m_ptCenter.y, 2));

	if (m_dRadius > 2000) {
		m_bIsCircleReady = false;
		return;
	}

	m_bIsCircleReady = true;
}

void CMfcStartDlg::DrawCircleBorder(int xc, int yc, int r, int thickness, unsigned char colorIdx) {
	if (m_image.IsNull() || thickness <= 0 || r <= 0)
		return;

	int outerRadius = r + thickness; 
	int outerRadius2 = outerRadius * outerRadius;
	int innerRadius = r;
	int innerRadius2 = innerRadius * innerRadius;

	for (int y = yc-outerRadius; y<=yc+outerRadius; y++)
	{
		for (int x=xc-outerRadius; x<=xc+outerRadius; x++)
		{
			int dist2 = (x - xc) * (x - xc) + (y - yc) * (y - yc);

			if (dist2 >= innerRadius2 && dist2 <= outerRadius2)
			{
				drawDot(x, y, colorIdx);
			}
		}
	}
}