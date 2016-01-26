#pragma once

#include "VMHeaderCtrl.h"

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};

class CVMListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CVMListCtrl)

public:
	CVMListCtrl();
	virtual ~CVMListCtrl();

public:
	void SetHeaderBKColor(int R, int G, int B, int Gradient);// Gradient - 渐变系数，立体背景用,不用渐变设为0
	void SetHeaderHeight(float Height);// 设置表头高度
	bool FindRowColor(int row ,COLORREF &color); //查找行颜色
	bool FindRowByColor(int &row ,COLORREF color); //查找行颜色
	bool FindColColor(int col ,COLORREF &color); //查找列颜色
	bool FindItemColor(int col,int row,COLORREF &color);
	bool FindRowTextColor(int row,COLORREF &color); //查找行字体颜色
	bool FindRowByTextColor(int &row,COLORREF color); //查找行字体颜色
	bool FindColTextColor(int col,COLORREF &color); //查找列字体颜色
	bool FindItemTextColor(int col,int row,COLORREF &color);
	bool ModifyRowColor(int row,COLORREF color);//修改行颜色
	bool ModifyRowColor(int row,int new_row,COLORREF &color);//修改行颜色
	bool ModifyColColor(int col,COLORREF color);//修改列颜色
	bool ModifyItemColor(int col,int row,COLORREF color);//修改item颜色
	bool ModifyRowTextColor(int row,COLORREF color);//修改行文字颜色
	bool ModifyRowTextColor(int row,int new_row,COLORREF &color);//修改行文字颜色
	bool ModifyColTextColor(int col,COLORREF color);//修改列文字颜色
	bool ModifyItemTextColor(int col,int row,COLORREF color);//修改item文字颜色
	void SetRowColor(int row,COLORREF color);  //设置行颜色
	void SetColColor(int col,COLORREF color);  //设置列颜色
	void SetItemColor(int col,int row,COLORREF color); //设置Item颜色
	void SetRowTextColor(int row,COLORREF color);   //设置行文本颜色
	void SetColTextColor(int col,COLORREF color);   //设置列文本颜色
	void SetItemTextColor(int col,int row,COLORREF color);//设置列文字颜色
	bool DeleteRowColor(int row,COLORREF &color);
	bool DeleteAllRowColor();
	bool DeleteRowTextColor(int row,COLORREF &color);
	bool DeleteAllRowTextColor();
	void SetRowHeigt(int nHeight); //设置行高
	void SetHeaderFontHW(int nHeight,int nWidth); //设置表头字体大小
	void SetHeaderTextColor(COLORREF color);
	BOOL SetTextColor(COLORREF cr);
	void SetFontHW(int nHeight,int nWidth);  //设置字体的高和宽
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	BOOL DeleteColumn(int nCol);

public:
	CVMHeaderCtrl m_Header;
	CPtrList m_ptrListCol;  //保存列颜色
	CPtrList m_ptrListItem; //保存Item颜色表
	CPtrList m_ptrListRow; //保存行颜色 
	CPtrList m_rowTextColor; //保存行字体颜色 
	CPtrList m_colTextColor; //保存列字体颜色
	CPtrList m_ItemTextColor; //保存单元格字体颜色

public:
	int m_nFontHeight;     // 字体高度
	int m_nFontWidth;	     // 字体宽度
	int m_nRowHeight;     // 行高
	COLORREF m_color;


protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLvnInsertitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteallitems(NMHDR *pNMHDR, LRESULT *pResult);
};


