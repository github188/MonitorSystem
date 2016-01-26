#pragma once


// CColorList

class CColorList : public CListCtrl
{
	DECLARE_DYNAMIC(CColorList)

public:
	CColorList();
	virtual ~CColorList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


class CNewColorList : public CListCtrl
{
	DECLARE_DYNAMIC(CNewColorList)
public:
	CNewColorList();
	virtual ~CNewColorList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};
