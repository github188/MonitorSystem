// DashLine.h : main header file for the CDashLine
//

/////////////////////////////////////////////////////////////////////////////
// CDashLine

class CDashLine
{
public:
  CDashLine(CDC* pDC, COLORREF Color1, COLORREF Color2,BOOL blTranslate, int nWidth, int Type[4]);
  ~CDashLine();

protected:
  CDC* m_pDC;
  CPen *m_pPen[2];
  int m_Pos, m_Count[4], m_Type[4];
  int m_x, m_y;
  int m_Color;

  void Reset();
  void Inter(int x, int y);

public:
  void MoveTo(int x, int y);
  void LineTo(int x, int y);
};
