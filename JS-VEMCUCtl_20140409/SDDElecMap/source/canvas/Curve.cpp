// Curve.cpp: implementation of the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sp_draw.h"
#include "canvas/Curve.h"
#include "dialog/CurConfig.h"
#include "canvas/TyTool.h"
#include "canvas/TyBase.h"
#include "dialog/FirstCurConfig.h"
#include "dialog/SecondCurConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CCurve,CLinkObjectBase,0)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurve::CCurve()
:CLinkObjectBase(lnkCur,"")
{
	ZeroMemory(&m_curvegroupinfo,sizeof(curvegroupinfo));
	m_curvegroupinfo.coordinatestyle = 0;
	strcpy(m_curvegroupinfo.curvegroupname,"曲线组名称");
	m_curvegroupinfo.curvegroupstyle = 0;
	m_curvegroupinfo.curstyle = 0;
	m_curvegroupinfo.firstcount = 0;
	m_curvegroupinfo.firstleftinterval = 1;
	m_curvegroupinfo.firstleftmax = 10;
	m_curvegroupinfo.firstleftmin = 0;
	strcpy(m_curvegroupinfo.firstleftunit,"单位名称");
	m_curvegroupinfo.firstrightinterval = 1;
	m_curvegroupinfo.firstrightmax = 10;
	m_curvegroupinfo.firstrightmin = 0;
	strcpy(m_curvegroupinfo.firstrightunit,"单位名称");
//	strcpy(m_curvegroupinfo.firstname,"第一组曲线名称");
	m_curvegroupinfo.secondcount = 0;
	m_curvegroupinfo.secondleftinterval = 1;
	m_curvegroupinfo.secondleftmax = 10;
	m_curvegroupinfo.secondleftmin = 0;
	strcpy(m_curvegroupinfo.secondleftunit,"单位名称");
	m_curvegroupinfo.secondrightinterval = 1;
	m_curvegroupinfo.secondrightmax = 10;
	m_curvegroupinfo.secondrightmin = 0;
	strcpy(m_curvegroupinfo.secondrightunit,"单位名称");
//	strcpy(m_curvegroupinfo.secondname,"第二组曲线名称");
}

CCurve::~CCurve()
{

}

BOOL CCurve::OnEditProperties()
{	
	int i;
	CPropertySheet sheet( _T("曲线信息配置..") );
	CCurConfig dlg1;

	dlg1.m_coordinate = m_curvegroupinfo.coordinatestyle;
	dlg1.m_curgroupname = m_curvegroupinfo.curvegroupname;
	dlg1.m_firstleftinterval = m_curvegroupinfo.firstleftinterval;
	dlg1.m_firstleftmax = m_curvegroupinfo.firstleftmax;
	dlg1.m_firstleftmin = m_curvegroupinfo.firstleftmin;
	dlg1.m_firstleftunit = m_curvegroupinfo.firstleftunit;
	dlg1.m_firstrightinterval = m_curvegroupinfo.firstrightinterval;
	dlg1.m_firstrightmax = m_curvegroupinfo.firstrightmax;
	dlg1.m_firstrightmin = m_curvegroupinfo.firstrightmin;
	dlg1.m_firstrightunit = m_curvegroupinfo.firstrightunit;
	dlg1.m_levelcur = m_curvegroupinfo.curstyle;
	dlg1.m_secondleftinterval = m_curvegroupinfo.secondleftinterval;
	dlg1.m_secondleftmax = m_curvegroupinfo.secondleftmax;
	dlg1.m_secondleftmin = m_curvegroupinfo.secondleftmin;
	dlg1.m_secondleftunit = m_curvegroupinfo.secondleftunit;
	dlg1.m_secondrightinterval = m_curvegroupinfo.secondrightinterval;
	dlg1.m_secondrightmax = m_curvegroupinfo.secondrightmax;
	dlg1.m_secondrightmin = m_curvegroupinfo.secondrightmin;
	dlg1.m_secondrightunit = m_curvegroupinfo.secondrightunit;
	dlg1.m_singlecur = m_curvegroupinfo.curvegroupstyle;
	dlg1.SetCurParam(m_name);

	CFirstCurConfig dlg2;
	dlg2.m_curcount = m_curvegroupinfo.firstcount;
   
	for(i = 0;i < m_curvegroupinfo.firstcount;i++)
	{
		for(int j = 0;j < 33;j++)
		{
       		dlg2.m_curinfolist[i].cname[j] = m_curvegroupinfo.firstinfolist[i].cname[j];                	    
		}

		for(int j = 0;j < 17;j++)
		{
			dlg2.m_curinfolist[i].ename[j] = m_curvegroupinfo.firstinfolist[i].ename[j];
		}

   	    dlg2.m_curinfolist[i].color = m_curvegroupinfo.firstinfolist[i].color;
   	    dlg2.m_curinfolist[i].compare = m_curvegroupinfo.firstinfolist[i].compare;
   	    dlg2.m_curinfolist[i].precission = m_curvegroupinfo.firstinfolist[i].precission;
        dlg2.m_curinfolist[i].reference = m_curvegroupinfo.firstinfolist[i].reference;
   	    dlg2.m_curinfolist[i].scale = m_curvegroupinfo.firstinfolist[i].scale;
   	    dlg2.m_curinfolist[i].swing = m_curvegroupinfo.firstinfolist[i].swing;
   	    dlg2.m_curinfolist[i].width = m_curvegroupinfo.firstinfolist[i].width;	
	}

	CSecondCurConfig dlg3;
	dlg3.m_curcount = m_curvegroupinfo.secondcount;

	for(i = 0;i < m_curvegroupinfo.secondcount;i++)
	{		
		for(int j = 0;j < 33;j++)
		{
       		dlg3.m_curinfolist[i].cname[j] = m_curvegroupinfo.secondinfolist[i].cname[j];                	    
		}

		for(int j = 0;j < 17;j++)
		{
			dlg3.m_curinfolist[i].ename[j] = m_curvegroupinfo.secondinfolist[i].ename[j];
		}

   	    dlg3.m_curinfolist[i].color = m_curvegroupinfo.secondinfolist[i].color;
   	    dlg3.m_curinfolist[i].compare = m_curvegroupinfo.secondinfolist[i].compare;
   	    dlg3.m_curinfolist[i].precission = m_curvegroupinfo.secondinfolist[i].precission;
        dlg3.m_curinfolist[i].reference = m_curvegroupinfo.secondinfolist[i].reference;
   	    dlg3.m_curinfolist[i].scale = m_curvegroupinfo.secondinfolist[i].scale;
   	    dlg3.m_curinfolist[i].swing = m_curvegroupinfo.secondinfolist[i].swing;
   	    dlg3.m_curinfolist[i].width = m_curvegroupinfo.secondinfolist[i].width;	
	}

	LoadExt(m_curvegroupinfo.curvegroupname);

	dlg2.m_bChecked = m_CurveExtInfo[0].flag;
	dlg2.m_color = m_CurveExtInfo[0].color;
	dlg2.m_xcolor = m_CurveExtInfo[0].mincolor;
	dlg2.m_nIndex = m_CurveExtInfo[0].tpye;
	dlg2.m_nMax = m_CurveExtInfo[0].maxValue;
	dlg2.m_nMin = m_CurveExtInfo[0].minValue;
	dlg2.m_nWidth = m_CurveExtInfo[0].width;

	dlg3.m_bChecked = m_CurveExtInfo[1].flag;
	dlg3.m_color = m_CurveExtInfo[1].color;
	dlg3.m_color3 = m_CurveExtInfo[1].mincolor;
	dlg3.m_nIndex = m_CurveExtInfo[1].tpye;
	dlg3.m_nMax = m_CurveExtInfo[1].maxValue;
	dlg3.m_nMin = m_CurveExtInfo[1].minValue;
	dlg3.m_nWidth = m_CurveExtInfo[1].width;

	sheet.AddPage(&dlg1);
	sheet.AddPage(&dlg2);
	sheet.AddPage(&dlg3);

	dlg1.p_page=&dlg3;
	dlg1.p_sheet=&sheet;

	if (sheet.DoModal() == IDOK)
	{
        dlg1.GetCurParam(m_name);
		m_curvegroupinfo.coordinatestyle = dlg1.m_coordinate;
		m_curvegroupinfo.curstyle = dlg1.m_levelcur;
		strcpy(m_curvegroupinfo.curvegroupname,dlg1.m_curgroupname);
		m_curvegroupinfo.curvegroupstyle = dlg1.m_singlecur;
		m_curvegroupinfo.firstleftinterval = dlg1.m_firstleftinterval;
		m_curvegroupinfo.firstleftmax = dlg1.m_firstleftmax;
		m_curvegroupinfo.firstleftmin = dlg1.m_firstleftmin;
		strcpy(m_curvegroupinfo.firstleftunit,dlg1.m_firstleftunit);
		m_curvegroupinfo.firstrightinterval = dlg1.m_firstrightinterval;
		m_curvegroupinfo.firstrightmax = dlg1.m_firstrightmax;
		m_curvegroupinfo.firstrightmin = dlg1.m_firstrightmin;
		strcpy(m_curvegroupinfo.firstrightunit,dlg1.m_firstrightunit);
		m_curvegroupinfo.secondleftinterval = dlg1.m_secondleftinterval;
		m_curvegroupinfo.secondleftmax = dlg1.m_secondleftmax;
		m_curvegroupinfo.secondleftmin = dlg1.m_secondleftmin;
		strcpy(m_curvegroupinfo.secondleftunit,dlg1.m_secondleftunit);
		m_curvegroupinfo.secondrightinterval = dlg1.m_secondrightinterval;
		m_curvegroupinfo.secondrightmax = dlg1.m_secondrightmax;
		m_curvegroupinfo.secondrightmin = dlg1.m_secondrightmin;
		strcpy(m_curvegroupinfo.secondrightunit,dlg1.m_secondrightunit);

		m_curvegroupinfo.firstcount=dlg2.m_curcount;

		for(i = 0;i < m_curvegroupinfo.firstcount;i++)
		{
			int j;
			for( j= 0;j < 33;j++)
			{
    			m_curvegroupinfo.firstinfolist[i].cname[j] = dlg2.m_curinfolist[i].cname[j];	    		
			}

			for(j = 0;j < 17;j++)
			{
				m_curvegroupinfo.firstinfolist[i].ename[j] = dlg2.m_curinfolist[i].ename[j];
			}

			m_curvegroupinfo.firstinfolist[i].color = dlg2.m_curinfolist[i].color;
			m_curvegroupinfo.firstinfolist[i].compare = dlg2.m_curinfolist[i].compare;
			m_curvegroupinfo.firstinfolist[i].width = dlg2.m_curinfolist[i].width;
			m_curvegroupinfo.firstinfolist[i].scale = dlg2.m_curinfolist[i].scale;
			m_curvegroupinfo.firstinfolist[i].reference = dlg2.m_curinfolist[i].reference;
			m_curvegroupinfo.firstinfolist[i].precission = dlg2.m_curinfolist[i].precission;
			m_curvegroupinfo.firstinfolist[i].swing = dlg2.m_curinfolist[i].swing;			
		}

		m_curvegroupinfo.secondcount = dlg3.m_curcount;

		for(i = 0;i < m_curvegroupinfo.secondcount;i++)
		{
			int j;
			for( j= 0;j < 33;j++)
			{
    			m_curvegroupinfo.secondinfolist[i].cname[j] = dlg3.m_curinfolist[i].cname[j];	    		
			}

			for(j = 0;j < 17;j++)
			{
				m_curvegroupinfo.secondinfolist[i].ename[j] = dlg3.m_curinfolist[i].ename[j];
			}

			m_curvegroupinfo.secondinfolist[i].color = dlg3.m_curinfolist[i].color;
			m_curvegroupinfo.secondinfolist[i].compare = dlg3.m_curinfolist[i].compare;
			m_curvegroupinfo.secondinfolist[i].width = dlg3.m_curinfolist[i].width;
			m_curvegroupinfo.secondinfolist[i].scale = dlg3.m_curinfolist[i].scale;
			m_curvegroupinfo.secondinfolist[i].reference = dlg3.m_curinfolist[i].reference;
			m_curvegroupinfo.secondinfolist[i].precission = dlg3.m_curinfolist[i].precission;
			m_curvegroupinfo.secondinfolist[i].swing = dlg3.m_curinfolist[i].swing;			
		}

		m_CurveExtInfo[0].flag = dlg2.m_bChecked;
		m_CurveExtInfo[0].color = dlg2.m_color;
		m_CurveExtInfo[0].mincolor = dlg2.m_xcolor;
		m_CurveExtInfo[0].tpye = dlg2.m_nIndex;
		m_CurveExtInfo[0].maxValue = dlg2.m_nMax;
		m_CurveExtInfo[0].minValue = dlg2.m_nMin;
		m_CurveExtInfo[0].width = dlg2.m_nWidth;

		m_CurveExtInfo[1].flag = dlg3.m_bChecked;
		m_CurveExtInfo[1].color = dlg3.m_color;
		m_CurveExtInfo[1].mincolor = dlg3.m_color3;
		m_CurveExtInfo[1].tpye = dlg3.m_nIndex;
		m_CurveExtInfo[1].maxValue = dlg3.m_nMax;
		m_CurveExtInfo[1].minValue = dlg3.m_nMin;
		m_CurveExtInfo[1].width = dlg3.m_nWidth;

		SaveExt(dlg1.m_curgroupname, dlg1.m_singlecur);

		return TRUE;
	}

  return FALSE;

}

void CCurve::Serialize_Ty(CArchive &ar)
{
	if(ar.IsStoring())  
	{
		ar.Write((unsigned char *)m_curvegroupinfo.curvegroupname,sizeof(char)*33);

		ar.Write((unsigned char *)&m_curvegroupinfo.curvegroupstyle,sizeof(m_curvegroupinfo.curvegroupstyle));
		ar.Write((unsigned char *)&m_curvegroupinfo.coordinatestyle,sizeof(m_curvegroupinfo.coordinatestyle));
		ar.Write((unsigned char *)&m_curvegroupinfo.curstyle,sizeof(m_curvegroupinfo.curstyle));
		ar.Write((unsigned char *)&m_curvegroupinfo.firstleftmax,sizeof(m_curvegroupinfo.firstleftmax));
		ar.Write((unsigned char *)&m_curvegroupinfo.firstleftmin,sizeof(m_curvegroupinfo.firstleftmin));
		ar.Write((unsigned char *)&m_curvegroupinfo.firstleftinterval,sizeof(m_curvegroupinfo.firstleftinterval));
		ar.Write((unsigned char *)m_curvegroupinfo.firstleftunit,sizeof(char)*9);
		ar.Write((unsigned char *)&m_curvegroupinfo.firstrightmax,sizeof(m_curvegroupinfo.firstrightmax));		
		ar.Write((unsigned char *)&m_curvegroupinfo.firstrightmin,sizeof(m_curvegroupinfo.firstrightmin));		
		ar.Write((unsigned char *)&m_curvegroupinfo.firstrightinterval,sizeof(m_curvegroupinfo.firstrightinterval));
		ar.Write((unsigned char *)m_curvegroupinfo.firstrightunit,sizeof(char)*9);
		ar.Write((unsigned char *)&m_curvegroupinfo.secondleftmax,sizeof(m_curvegroupinfo.secondleftmax));
		ar.Write((unsigned char *)&m_curvegroupinfo.secondleftmin,sizeof(m_curvegroupinfo.secondleftmin));
		ar.Write((unsigned char *)&m_curvegroupinfo.secondleftinterval,sizeof(m_curvegroupinfo.secondleftinterval));
		ar.Write((unsigned char *)m_curvegroupinfo.secondleftunit,sizeof(char)*9);
		ar.Write((unsigned char *)&m_curvegroupinfo.secondrightmax,sizeof(m_curvegroupinfo.secondrightmax));
		ar.Write((unsigned char *)&m_curvegroupinfo.secondrightmin,sizeof(m_curvegroupinfo.secondrightmin));
		ar.Write((unsigned char *)&m_curvegroupinfo.secondrightinterval,sizeof(m_curvegroupinfo.secondrightinterval));
		ar.Write((unsigned char *)m_curvegroupinfo.secondrightunit,sizeof(char)*9);

	}
	else
	{
		ar.Read((unsigned char *)m_curvegroupinfo.curvegroupname,sizeof(char)*33);
	
		ar.Read((unsigned char *)&m_curvegroupinfo.curvegroupstyle,sizeof(m_curvegroupinfo.curvegroupstyle));
		ar.Read((unsigned char *)&m_curvegroupinfo.coordinatestyle,sizeof(m_curvegroupinfo.coordinatestyle));
		ar.Read((unsigned char *)&m_curvegroupinfo.curstyle,sizeof(m_curvegroupinfo.curstyle));
		ar.Read((unsigned char *)&m_curvegroupinfo.firstleftmax,sizeof(m_curvegroupinfo.firstleftmax));
		ar.Read((unsigned char *)&m_curvegroupinfo.firstleftmin,sizeof(m_curvegroupinfo.firstleftmin));
		ar.Read((unsigned char *)&m_curvegroupinfo.firstleftinterval,sizeof(m_curvegroupinfo.firstleftinterval));
		ar.Read((unsigned char *)m_curvegroupinfo.firstleftunit,sizeof(char)*9);
		ar.Read((unsigned char *)&m_curvegroupinfo.firstrightmax,sizeof(m_curvegroupinfo.firstrightmax));
		ar.Read((unsigned char *)&m_curvegroupinfo.firstrightmin,sizeof(m_curvegroupinfo.firstrightmin));
		ar.Read((unsigned char *)&m_curvegroupinfo.firstrightinterval,sizeof(m_curvegroupinfo.firstrightinterval));
		ar.Read((unsigned char *)m_curvegroupinfo.firstrightunit,sizeof(char)*9);
		ar.Read((unsigned char *)&m_curvegroupinfo.secondleftmax,sizeof(m_curvegroupinfo.secondleftmax));
		ar.Read((unsigned char *)&m_curvegroupinfo.secondleftmin,sizeof(m_curvegroupinfo.secondleftmin));
		ar.Read((unsigned char *)&m_curvegroupinfo.secondleftinterval,sizeof(m_curvegroupinfo.secondleftinterval));
		ar.Read((unsigned char *)m_curvegroupinfo.secondleftunit,sizeof(char)*9);
		ar.Read((unsigned char *)&m_curvegroupinfo.secondrightmax,sizeof(m_curvegroupinfo.secondrightmax));
		ar.Read((unsigned char *)&m_curvegroupinfo.secondrightmin,sizeof(m_curvegroupinfo.secondrightmin));
		ar.Read((unsigned char *)&m_curvegroupinfo.secondrightinterval,sizeof(m_curvegroupinfo.secondrightinterval));
		ar.Read((unsigned char *)m_curvegroupinfo.secondrightunit,sizeof(char)*9);
		
		m_curvegroupinfo.firstcount = 0;
		m_curvegroupinfo.secondcount = 0;
	}
}

void CCurve::Save_Ty(CFile *file, BOOL Yn)
{
	int i,loop;
  
	if(Yn)//如果是在进行保存
	{
		file->Write((unsigned char *)m_curvegroupinfo.curvegroupname,sizeof(char)*33);
		file->Write((unsigned char *)&m_curvegroupinfo.curvegroupstyle,sizeof(m_curvegroupinfo.curvegroupstyle));
		file->Write((unsigned char *)&m_curvegroupinfo.coordinatestyle,sizeof(m_curvegroupinfo.coordinatestyle));
		file->Write((unsigned char *)&m_curvegroupinfo.curstyle,sizeof(m_curvegroupinfo.curstyle));
		file->Write((unsigned char *)&m_curvegroupinfo.firstleftmax,sizeof(m_curvegroupinfo.firstleftmax));
		file->Write((unsigned char *)&m_curvegroupinfo.firstleftmin,sizeof(m_curvegroupinfo.firstleftmin));
		file->Write((unsigned char *)&m_curvegroupinfo.firstleftinterval,sizeof(m_curvegroupinfo.firstleftinterval));
		file->Write((unsigned char *)m_curvegroupinfo.firstleftunit,sizeof(char)*9);
		file->Write((unsigned char *)&m_curvegroupinfo.firstrightmax,sizeof(m_curvegroupinfo.firstrightmax));		
		file->Write((unsigned char *)&m_curvegroupinfo.firstrightmin,sizeof(m_curvegroupinfo.firstrightmin));		
		file->Write((unsigned char *)&m_curvegroupinfo.firstrightinterval,sizeof(m_curvegroupinfo.firstrightinterval));
		file->Write((unsigned char *)m_curvegroupinfo.firstrightunit,sizeof(char)*9);
		file->Write((unsigned char *)&m_curvegroupinfo.secondleftmax,sizeof(m_curvegroupinfo.secondleftmax));
		file->Write((unsigned char *)&m_curvegroupinfo.secondleftmin,sizeof(m_curvegroupinfo.secondleftmin));
		file->Write((unsigned char *)&m_curvegroupinfo.secondleftinterval,sizeof(m_curvegroupinfo.secondleftinterval));
		file->Write((unsigned char *)m_curvegroupinfo.secondleftunit,sizeof(char)*9);
		file->Write((unsigned char *)&m_curvegroupinfo.secondrightmax,sizeof(m_curvegroupinfo.secondrightmax));
		file->Write((unsigned char *)&m_curvegroupinfo.secondrightmin,sizeof(m_curvegroupinfo.secondrightmin));
		file->Write((unsigned char *)&m_curvegroupinfo.secondrightinterval,sizeof(m_curvegroupinfo.secondrightinterval));
		file->Write((unsigned char *)m_curvegroupinfo.secondrightunit,sizeof(char)*9);

		loop = m_curvegroupinfo.firstcount;
		
		ASSERT(loop <= 8);
		
		if (loop  >8) 
			loop=0;

		file->Write((unsigned char *)&loop,sizeof(int));

		for (i = 0;i < loop;i++)
		{
			file->Write((unsigned char *)m_curvegroupinfo.firstinfolist[i].ename,sizeof(char)*17);
			file->Write((unsigned char *)m_curvegroupinfo.firstinfolist[i].cname,sizeof(char)*33);
			file->Write((unsigned char *)&m_curvegroupinfo.firstinfolist[i].scale,sizeof(m_curvegroupinfo.firstinfolist[i].scale));
			file->Write((unsigned char *)&m_curvegroupinfo.firstinfolist[i].precission,sizeof(m_curvegroupinfo.firstinfolist[i].precission));
			file->Write((unsigned char *)&m_curvegroupinfo.firstinfolist[i].color,sizeof(m_curvegroupinfo.firstinfolist[i].color));
			file->Write((unsigned char *)&m_curvegroupinfo.firstinfolist[i].width,sizeof(m_curvegroupinfo.firstinfolist[i].width));
			file->Write((unsigned char *)&m_curvegroupinfo.firstinfolist[i].reference,sizeof(m_curvegroupinfo.firstinfolist[i].reference));
			file->Write((unsigned char *)&m_curvegroupinfo.firstinfolist[i].compare,sizeof(m_curvegroupinfo.firstinfolist[i].compare));
			file->Write((unsigned char *)&m_curvegroupinfo.firstinfolist[i].swing,sizeof(m_curvegroupinfo.firstinfolist[i].swing));
		}

		loop = m_curvegroupinfo.secondcount;
		ASSERT(loop <= 8);

		if (loop > 8)
			loop = 0; 

		file->Write((unsigned char *)&loop,sizeof(int));

		for (i = 0;i < loop;i++)
		{			
			file->Write((unsigned char *)m_curvegroupinfo.secondinfolist[i].ename,sizeof(char)*17);
			file->Write((unsigned char *)m_curvegroupinfo.secondinfolist[i].cname,sizeof(char)*33);
			file->Write((unsigned char *)&m_curvegroupinfo.secondinfolist[i].scale,sizeof(m_curvegroupinfo.secondinfolist[i].scale));
			file->Write((unsigned char *)&m_curvegroupinfo.secondinfolist[i].precission,sizeof(m_curvegroupinfo.secondinfolist[i].precission));
			file->Write((unsigned char *)&m_curvegroupinfo.secondinfolist[i].color,sizeof(m_curvegroupinfo.secondinfolist[i].color));
			file->Write((unsigned char *)&m_curvegroupinfo.secondinfolist[i].width,sizeof(m_curvegroupinfo.secondinfolist[i].width));
			file->Write((unsigned char *)&m_curvegroupinfo.secondinfolist[i].reference,sizeof(m_curvegroupinfo.secondinfolist[i].reference));
			file->Write((unsigned char *)&m_curvegroupinfo.secondinfolist[i].compare,sizeof(m_curvegroupinfo.secondinfolist[i].compare));
			file->Write((unsigned char *)&m_curvegroupinfo.secondinfolist[i].swing,sizeof(m_curvegroupinfo.secondinfolist[i].swing));
		}
	}
	else
	{
		file->Read((unsigned char *)m_curvegroupinfo.curvegroupname,sizeof(char)*33);
		file->Read((unsigned char *)&m_curvegroupinfo.curvegroupstyle,sizeof(m_curvegroupinfo.curvegroupstyle));
		file->Read((unsigned char *)&m_curvegroupinfo.coordinatestyle,sizeof(m_curvegroupinfo.coordinatestyle));
		file->Read((unsigned char *)&m_curvegroupinfo.curstyle,sizeof(m_curvegroupinfo.curstyle));
		file->Read((unsigned char *)&m_curvegroupinfo.firstleftmax,sizeof(m_curvegroupinfo.firstleftmax));
		file->Read((unsigned char *)&m_curvegroupinfo.firstleftmin,sizeof(m_curvegroupinfo.firstleftmin));
		file->Read((unsigned char *)&m_curvegroupinfo.firstleftinterval,sizeof(m_curvegroupinfo.firstleftinterval));
		file->Read((unsigned char *)m_curvegroupinfo.firstleftunit,sizeof(char)*9);
		file->Read((unsigned char *)&m_curvegroupinfo.firstrightmax,sizeof(m_curvegroupinfo.firstrightmax));
		file->Read((unsigned char *)&m_curvegroupinfo.firstrightmin,sizeof(m_curvegroupinfo.firstrightmin));
		file->Read((unsigned char *)&m_curvegroupinfo.firstrightinterval,sizeof(m_curvegroupinfo.firstrightinterval));
		file->Read((unsigned char *)m_curvegroupinfo.firstrightunit,sizeof(char)*9);
		file->Read((unsigned char *)&m_curvegroupinfo.secondleftmax,sizeof(m_curvegroupinfo.secondleftmax));
		file->Read((unsigned char *)&m_curvegroupinfo.secondleftmin,sizeof(m_curvegroupinfo.secondleftmin));
		file->Read((unsigned char *)&m_curvegroupinfo.secondleftinterval,sizeof(m_curvegroupinfo.secondleftinterval));
		file->Read((unsigned char *)m_curvegroupinfo.secondleftunit,sizeof(char)*9);
		file->Read((unsigned char *)&m_curvegroupinfo.secondrightmax,sizeof(m_curvegroupinfo.secondrightmax));
		file->Read((unsigned char *)&m_curvegroupinfo.secondrightmin,sizeof(m_curvegroupinfo.secondrightmin));
		file->Read((unsigned char *)&m_curvegroupinfo.secondrightinterval,sizeof(m_curvegroupinfo.secondrightinterval));
		file->Read((unsigned char *)m_curvegroupinfo.secondrightunit,sizeof(char)*9);

		file->Read((unsigned char *)&loop,sizeof(int));
		ASSERT(loop <= 8);

		if (loop > 8)
		{
			loop = 0; 
			return;
		}

		m_curvegroupinfo.firstcount = loop;

		for (i = 0;i < m_curvegroupinfo.firstcount;i++)
		{
			file->Read((unsigned char *)m_curvegroupinfo.firstinfolist[i].ename,sizeof(char)*17);
			file->Read((unsigned char *)m_curvegroupinfo.firstinfolist[i].cname,sizeof(char)*33);
			file->Read((unsigned char *)&m_curvegroupinfo.firstinfolist[i].scale,sizeof(m_curvegroupinfo.firstinfolist[i].scale));
			file->Read((unsigned char *)&m_curvegroupinfo.firstinfolist[i].precission,sizeof(m_curvegroupinfo.firstinfolist[i].precission));
			file->Read((unsigned char *)&m_curvegroupinfo.firstinfolist[i].color,sizeof(m_curvegroupinfo.firstinfolist[i].color));
			file->Read((unsigned char *)&m_curvegroupinfo.firstinfolist[i].width,sizeof(m_curvegroupinfo.firstinfolist[i].width));
			file->Read((unsigned char *)&m_curvegroupinfo.firstinfolist[i].reference,sizeof(m_curvegroupinfo.firstinfolist[i].reference));
			file->Read((unsigned char *)&m_curvegroupinfo.firstinfolist[i].compare,sizeof(m_curvegroupinfo.firstinfolist[i].compare));
			file->Read((unsigned char *)&m_curvegroupinfo.firstinfolist[i].swing,sizeof(m_curvegroupinfo.firstinfolist[i].swing));
		}

		file->Read((unsigned char *)&loop,sizeof(int));
		ASSERT(loop <= 8);

		if (loop > 8)
		{
			loop = 0; 
			return;
		}

		m_curvegroupinfo.secondcount = loop;

		for (i = 0;i < m_curvegroupinfo.secondcount;i++)
		{			
			file->Read((unsigned char *)m_curvegroupinfo.secondinfolist[i].ename,sizeof(char)*17);
			file->Read((unsigned char *)m_curvegroupinfo.secondinfolist[i].cname,sizeof(char)*33);
			file->Read((unsigned char *)&m_curvegroupinfo.secondinfolist[i].scale,sizeof(m_curvegroupinfo.secondinfolist[i].scale));
			file->Read((unsigned char *)&m_curvegroupinfo.secondinfolist[i].precission,sizeof(m_curvegroupinfo.secondinfolist[i].precission));
			file->Read((unsigned char *)&m_curvegroupinfo.secondinfolist[i].color,sizeof(m_curvegroupinfo.secondinfolist[i].color));
			file->Read((unsigned char *)&m_curvegroupinfo.secondinfolist[i].width,sizeof(m_curvegroupinfo.secondinfolist[i].width));
			file->Read((unsigned char *)&m_curvegroupinfo.secondinfolist[i].reference,sizeof(m_curvegroupinfo.secondinfolist[i].reference));
			file->Read((unsigned char *)&m_curvegroupinfo.secondinfolist[i].compare,sizeof(m_curvegroupinfo.secondinfolist[i].compare));
			file->Read((unsigned char *)&m_curvegroupinfo.secondinfolist[i].swing,sizeof(m_curvegroupinfo.secondinfolist[i].swing));
		}
	}
}

void CCurve::LoadExt(CString name)
{
	CString strFilePath;

	WIN32_FIND_DATA FileData; 
	HANDLE hSearch = FindFirstFile(strFilePath, &FileData);
	if(hSearch == INVALID_HANDLE_VALUE)
	{
		::MoveFile("c:\\sunpac\\par\\global\\Curve.ini", strFilePath);
	}
	else
	{
		FindClose(hSearch);
	}

	memset(&m_CurveExtInfo[0],0,sizeof(CURVEEXTINFO));
	memset(&m_CurveExtInfo[1],0,sizeof(CURVEEXTINFO));

	m_CurveExtInfo[0].flag = GetPrivateProfileInt(name, "enable0", 0, strFilePath);
	m_CurveExtInfo[0].color = GetPrivateProfileInt(name, "color0", 0, strFilePath);
	m_CurveExtInfo[0].mincolor = GetPrivateProfileInt(name, "mincolor0", 0, strFilePath);
	m_CurveExtInfo[0].tpye = GetPrivateProfileInt(name, "tpye0", 0, strFilePath);

	char temp[64];
	memset(temp,0,sizeof(char)*64);
	GetPrivateProfileString(name,"maxValue0","",temp,64,strFilePath);

	float ftemp = 0;
	sscanf(temp,"%f",&ftemp);
	m_CurveExtInfo[0].maxValue = ftemp;
	memset(temp,0,sizeof(char)*64);
	GetPrivateProfileString(name,"minValue0","",temp,64,strFilePath);

	ftemp = 0;
	sscanf(temp,"%f",&ftemp);
	m_CurveExtInfo[0].minValue = ftemp;
    memset(temp,0,sizeof(char)*64);
	GetPrivateProfileString(name,"width0","",temp,64,strFilePath);

	ftemp = 0;
	sscanf(temp,"%f",&ftemp);
	m_CurveExtInfo[0].width = ftemp;
	if(m_curvegroupinfo.curvegroupstyle == 0)
	{
		return;
	}

	m_CurveExtInfo[1].flag = GetPrivateProfileInt(name, "enable1", 0, strFilePath);
	
	memset(temp,0,sizeof(char)*64);
	GetPrivateProfileString(name,"maxValue1","",temp,64,strFilePath);
	
	sscanf(temp,"%f",&ftemp);
	m_CurveExtInfo[1].maxValue = ftemp;
	memset(temp,0,sizeof(char)*64);
	GetPrivateProfileString(name,"minValue1","",temp,64,strFilePath);

	ftemp=0;
	sscanf(temp,"%f",&ftemp);
	m_CurveExtInfo[1].minValue = ftemp;
	m_CurveExtInfo[1].color = GetPrivateProfileInt(name, "color1", 0, strFilePath);
	m_CurveExtInfo[1].mincolor = GetPrivateProfileInt(name, "mincolor1", 0, strFilePath);
	m_CurveExtInfo[1].tpye = GetPrivateProfileInt(name, "tpye1", 0, strFilePath);

	memset(temp,0,sizeof(char)*64);
	GetPrivateProfileString(name,"width1","",temp,64,strFilePath);
	
	ftemp=0;
	sscanf(temp,"%f",&ftemp);
	m_CurveExtInfo[1].width = ftemp;
}

void CCurve::SaveExt(CString name, int bFlag)
{
	CString strFilePath;

	CString str;
	str.Format("%d", m_CurveExtInfo[0].flag);
	WritePrivateProfileString(name, "enable0", str, strFilePath);
	str.Format("%f", m_CurveExtInfo[0].maxValue);
	WritePrivateProfileString(name, "maxValue0", str, strFilePath);
	str.Format("%f", m_CurveExtInfo[0].minValue);
	WritePrivateProfileString(name, "minValue0", str, strFilePath);
	str.Format("%d", m_CurveExtInfo[0].color);
	WritePrivateProfileString(name, "color0", str, strFilePath);
	str.Format("%d", m_CurveExtInfo[0].mincolor);
	WritePrivateProfileString(name, "mincolor0", str, strFilePath);
	str.Format("%d", m_CurveExtInfo[0].tpye);
	WritePrivateProfileString(name, "tpye0", str, strFilePath);
	str.Format("%f", m_CurveExtInfo[0].width);
	WritePrivateProfileString(name, "width0", str, strFilePath);

	if(bFlag == 0)
	{
		return;
	}
		
	str.Format("%d", m_CurveExtInfo[1].flag);
	WritePrivateProfileString(name, "enable1", str, strFilePath);
	str.Format("%f", m_CurveExtInfo[1].maxValue);
	WritePrivateProfileString(name, "maxValue1", str, strFilePath);
	str.Format("%f", m_CurveExtInfo[1].minValue);
	WritePrivateProfileString(name, "minValue1", str, strFilePath);
	str.Format("%d", m_CurveExtInfo[1].color);
	WritePrivateProfileString(name, "color1", str, strFilePath);
	str.Format("%d", m_CurveExtInfo[1].mincolor);
	WritePrivateProfileString(name, "mincolor1", str, strFilePath);
	str.Format("%d", m_CurveExtInfo[1].tpye);
	WritePrivateProfileString(name, "tpye1", str, strFilePath);
	str.Format("%f", m_CurveExtInfo[1].width);
	WritePrivateProfileString(name, "width1", str, strFilePath);
}


void CCurve::ParamReplace_Ty(CStringList& ruleList)
{
	CString str;
	int ret,i;
	
	for (i = 0;i  <m_curvegroupinfo.firstcount;i++)
	{
		ret = CTyBase::GetEname_From_ReplacRule(m_curvegroupinfo.firstinfolist[i].ename,str,ruleList);
		if (ret>0)
		{
			SetModifiedFlag(true);
			strcpy(m_curvegroupinfo.firstinfolist[i].ename,str);
		}
	}

	for (i = 0;i < m_curvegroupinfo.secondcount;i++)
	{
		ret = CTyBase::GetEname_From_ReplacRule(m_curvegroupinfo.secondinfolist[i].ename,str,ruleList);
		if (ret > 0)
		{
			SetModifiedFlag(true);
			strcpy(m_curvegroupinfo.secondinfolist[i].ename,str);
		}
	}
}
