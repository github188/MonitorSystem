#pragma once

// CPrecitionEdit

class CPrecitionEdit : public CEdit
{
	DECLARE_DYNAMIC(CPrecitionEdit)

public:
	CPrecitionEdit();
	virtual ~CPrecitionEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


