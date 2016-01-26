#pragma once

// CListBase

class CListBase : public CListCtrl
{
	DECLARE_DYNAMIC(CListBase)

public:
	CListBase();
	virtual ~CListBase();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	
};


