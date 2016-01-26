#pragma once


// CVMHeaderCtrl
class CVMHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CVMHeaderCtrl)

public:
	CVMHeaderCtrl();
	virtual ~CVMHeaderCtrl();

public:
	COLORREF m_color;
	CStringArray m_strHeadArray;
	CString m_strFormat; //表示对齐类型的整型数组,0表示左对齐，1表示中间对齐，2表示右对齐

public:
	int m_R;
	int m_G;
	int m_B;
	int m_Gradient;	// 画立体背景，渐变系数
	float m_fHeight;  //表头高度，这是倍数,
	int m_nFontHeight; //字体高度
	int m_nFontWith;   //字体宽度

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );
};


