// McCurveData.cpp : 实现文件
//

#include "stdafx.h"
#include "sp_draw.h"
#include "McCurveData.h"


// CMcCurveData

CMcCurveData::CMcCurveData()
{
	bonlyrealline=nIprecision=nVprecision=bNeedsave=fleftcoord=ftopcoord=frightcoord=fbottomcoord=fImin=fVmin=nNode=nLine=nRtu=0;
	bgtmode=1;
	fVmax=300;
	fImax=30;
	m_realtime_style=0;

	strfilepath="C:\\sunpac\\par\\global\\pic\\McCurveLine.ini";
}

CMcCurveData::~CMcCurveData()
{

}

void CMcCurveData::SetPath(LPCTSTR strpath)
{
	strfilepath=strpath;
}

void CMcCurveData::WriteCurveData()
{
	TCurve tempcurve;
	CString strnewsection;
	CString strsiglecurve;
	strnewsection=strMcename;
	if (strnewsection.IsEmpty())
	{
		return;
	}
	if (strnewsection!=stroldsection&&(!stroldsection.IsEmpty()))
	{
		WritePrivateProfileSection(stroldsection,"",strfilepath);
	}
	else
	{
		WritePrivateProfileSection(strnewsection,"",strfilepath);
	}
	
	CString strkey;
	strkey.Format("%d",nIprecision);
	WritePrivateProfileString(strnewsection,"Iprecision",strkey,strfilepath);
	strkey.Format("%d",nVprecision);
	WritePrivateProfileString(strnewsection,"Vprecision",strkey,strfilepath);
	strkey.Format("%f",fImin);
	WritePrivateProfileString(strnewsection,"Imin",strkey,strfilepath);
	strkey.Format("%f",fImax);
	WritePrivateProfileString(strnewsection,"Imax",strkey,strfilepath);
	strkey.Format("%f",fVmin);
	WritePrivateProfileString(strnewsection,"Vmin",strkey,strfilepath);
	strkey.Format("%f",fVmax);
	WritePrivateProfileString(strnewsection,"Vmax",strkey,strfilepath);
	strkey.Format("%d",bgtmode);
	WritePrivateProfileString(strnewsection,"gtmode",strkey,strfilepath);
	strkey.Format("%d",bonlyrealline);
	WritePrivateProfileString(strnewsection,"onlyrealline",strkey,strfilepath);
	strkey.Format("%d",m_realtime_style);
	WritePrivateProfileString(strnewsection,"realtime_style",strkey,strfilepath);
	strkey.Format("%d",nNode);
	WritePrivateProfileString(strnewsection,"node",strkey,strfilepath);
	strkey.Format("%d",nLine);
	WritePrivateProfileString(strnewsection,"line",strkey,strfilepath);
	strkey.Format("%d",nRtu);
	WritePrivateProfileString(strnewsection,"rtu",strkey,strfilepath);
	WritePrivateProfileString(strnewsection,"UpName",UpName,strfilepath);
	WritePrivateProfileString(strnewsection,"DownName",DownName,strfilepath);
	CString strgtIcount;
	strgtIcount.Format("%d",gtIcurvearray.GetSize());
	WritePrivateProfileString(strnewsection,"gtIlinecount",strgtIcount,strfilepath);
	for (int i=0; i<gtIcurvearray.GetSize(); i++)
	{
		tempcurve=gtIcurvearray.GetAt(i);
		tempcurve.unit[7]=0x0;
		strsiglecurve.Format("%d,%d,%d,%d,%d,%d,%0.3f,%d,%s",tempcurve.ncurvecolor,tempcurve.BcurvechannelNO,tempcurve.nlinenode,tempcurve.nlineline,tempcurve.nlinertu,tempcurve.Bdot,tempcurve.fcurveratio,tempcurve.Bprecision,tempcurve.unit);
		CString strgtIkey;
		strgtIkey.Format("gtIline%d",i);
		WritePrivateProfileString(strnewsection,strgtIkey,strsiglecurve,strfilepath);
	}

	CString strgtVcount;
	strgtVcount.Format("%d",gtVcurvearray.GetSize());
	WritePrivateProfileString(strnewsection,"gtVlinecount",strgtVcount,strfilepath);
	for (int i=0; i<gtVcurvearray.GetSize(); i++)
	{
		tempcurve=gtVcurvearray.GetAt(i);
		tempcurve.unit[7]=0x0;
		strsiglecurve.Format("%d,%d,%d,%d,%d,%d,%0.3f,%d,%s",tempcurve.ncurvecolor,tempcurve.BcurvechannelNO,tempcurve.nlinenode,tempcurve.nlineline,tempcurve.nlinertu,tempcurve.Bdot,tempcurve.fcurveratio,tempcurve.Bprecision,tempcurve.unit);
		CString strgtVkey;
		strgtVkey.Format("gtVline%d",i);
		WritePrivateProfileString(strnewsection,strgtVkey,strsiglecurve,strfilepath);
	}
	if (bgtmode)
	{
		CString strzbIcount;
		strzbIcount.Format("%d",zbIcurvearray.GetSize());
		WritePrivateProfileString(strnewsection,"zbIlinecount",strzbIcount,strfilepath);
		for (int i=0; i<zbIcurvearray.GetSize(); i++)
		{
			tempcurve=zbIcurvearray.GetAt(i);
			tempcurve.unit[7]=0x0;
			strsiglecurve.Format("%d,%d,%d,%d,%d,%d,%0.3f,%d,%s",tempcurve.ncurvecolor,tempcurve.BcurvechannelNO,tempcurve.nlinenode,tempcurve.nlineline,tempcurve.nlinertu,tempcurve.Bdot,tempcurve.fcurveratio,tempcurve.Bprecision,tempcurve.unit);
			CString strzbIkey;
			strzbIkey.Format("zbIline%d",i);
			WritePrivateProfileString(strnewsection,strzbIkey,strsiglecurve,strfilepath);
		}

		CString strzbVcount;
		strzbVcount.Format("%d",zbVcurvearray.GetSize());
		WritePrivateProfileString(strnewsection,"zbVlinecount",strzbVcount,strfilepath);
		for (int i=0; i<zbVcurvearray.GetSize(); i++)
		{
			tempcurve=zbVcurvearray.GetAt(i);
			tempcurve.unit[7]=0x0;
			strsiglecurve.Format("%d,%d,%d,%d,%d,%d,%0.3f,%d,%s",tempcurve.ncurvecolor,tempcurve.BcurvechannelNO,tempcurve.nlinenode,tempcurve.nlineline,tempcurve.nlinertu,tempcurve.Bdot,tempcurve.fcurveratio,tempcurve.Bprecision,tempcurve.unit);
			CString strzbVkey;
			strzbVkey.Format("zbVline%d",i);
			WritePrivateProfileString(strnewsection,strzbVkey,strsiglecurve,strfilepath);
		}
	}
}

BOOL CMcCurveData::ReadCurveData()
{
	WIN32_FIND_DATA fd;
	HANDLE handle=FindFirstFile(strfilepath,&fd);
	if (handle==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	FindClose(handle);
	//stroldsection.Format("mc_n%02dl%03du%03d",nNode,nLine,nRtu);
	stroldsection=strMcename;
	
	char szsection[20];
	ZeroMemory(szsection,20);
	GetPrivateProfileSection(stroldsection,szsection,20,strfilepath);
	if (strlen(szsection)==0)
	{
		return FALSE;
	}
	gtIcurvearray.RemoveAll();
	gtVcurvearray.RemoveAll();
	zbIcurvearray.RemoveAll();
	zbVcurvearray.RemoveAll();
	char szrec[80];
	char sz0[20];
	char sz1[20];
	char sz2[20];
	char sz3[20];
	char sz4[20];
	char sz5[20];
	char sz6[20];
	char sz7[20];
	char sz8[20];

	char nUpName[33];
	char nDownName[33];

	strcpy(nUpName,"");
	strcpy(nDownName,"");
	
	
	nIprecision=GetPrivateProfileInt(stroldsection,"Iprecision",0,strfilepath);
	nVprecision=GetPrivateProfileInt(stroldsection,"Vprecision",0,strfilepath);
	//fImin=GetPrivateProfileInt(stroldsection,"Imin",0,strfilepath);
	GetPrivateProfileString(stroldsection,"Imin",0,szrec,80,strfilepath);
	fImin=atof(szrec);
	//fImax=GetPrivateProfileInt(stroldsection,"Imax",30,strfilepath);
	GetPrivateProfileString(stroldsection,"Imax",0,szrec,80,strfilepath);
	fImax=atof(szrec);
	//fVmin=GetPrivateProfileInt(stroldsection,"Vmin",0,strfilepath);
	GetPrivateProfileString(stroldsection,"Vmin",0,szrec,80,strfilepath);
	fVmin=atof(szrec);
	//fVmax=GetPrivateProfileInt(stroldsection,"Vmax",300,strfilepath);
	GetPrivateProfileString(stroldsection,"Vmax",0,szrec,80,strfilepath);
	fVmax=atof(szrec);
	bgtmode=GetPrivateProfileInt(stroldsection,"gtmode",1,strfilepath);

	bonlyrealline=GetPrivateProfileInt(stroldsection,"onlyrealline",0,strfilepath);
	m_realtime_style=GetPrivateProfileInt(stroldsection,"realtime_style",0,strfilepath);
	nNode=GetPrivateProfileInt(stroldsection,"node",0,strfilepath);
	nLine=GetPrivateProfileInt(stroldsection,"line",0,strfilepath);
	nRtu=GetPrivateProfileInt(stroldsection,"rtu",0,strfilepath);
	
	GetPrivateProfileString(stroldsection,"UpName","",nUpName,sizeof(char)*32,strfilepath);
	GetPrivateProfileString(stroldsection,"DownName","",nDownName,sizeof(char)*32,strfilepath);
	UpName=nUpName;
	DownName=nDownName;

	TCurve addTcurve;
	CString singlecurvekey;
	
	int curvecount=GetPrivateProfileInt(stroldsection,"gtIlinecount",0,strfilepath);
	for (int i=0; i<curvecount; i++)
	{

		singlecurvekey.Format("gtIline%d",i);
		GetPrivateProfileString(stroldsection,singlecurvekey,0,szrec,79,strfilepath);
		
		extractstring(szrec,sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8);
		memset(&addTcurve,0,sizeof(TCurve));
		addTcurve.ncurvecolor=atoi(sz0);
		addTcurve.BcurvechannelNO=atoi(sz1);
		addTcurve.nlinenode=atoi(sz2);
		addTcurve.nlineline=atoi(sz3);
		addTcurve.nlinertu=atoi(sz4);
		addTcurve.Bdot=atoi(sz5);
		addTcurve.fcurveratio=atof(sz6);
		addTcurve.Bprecision=atoi(sz7);
		if (strlen(sz8)>0)
			strcpy(addTcurve.unit,sz8);
		gtIcurvearray.Add(addTcurve);
	}

	curvecount=GetPrivateProfileInt(stroldsection,"gtVlinecount",0,strfilepath);
	for (int i=0; i<curvecount; i++)
	{
		singlecurvekey.Format("gtVline%d",i);
		GetPrivateProfileString(stroldsection,singlecurvekey,0,szrec,79,strfilepath);
		extractstring(szrec,sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8);
	    memset(&addTcurve,0,sizeof(TCurve)); 
		addTcurve.ncurvecolor=atoi(sz0);
		addTcurve.BcurvechannelNO=atoi(sz1);
		addTcurve.nlinenode=atoi(sz2);
		addTcurve.nlineline=atoi(sz3);
		addTcurve.nlinertu=atoi(sz4);
		addTcurve.Bdot=atoi(sz5);
		addTcurve.fcurveratio=atof(sz6);
		addTcurve.Bprecision=atoi(sz7);
		if (strlen(sz8)>0)
			strcpy(addTcurve.unit,sz8);
		gtVcurvearray.Add(addTcurve);
	}

	if (bgtmode)
	{
		curvecount=GetPrivateProfileInt(stroldsection,"zbIlinecount",0,strfilepath);
		for (int i=0; i<curvecount; i++)
		{
			singlecurvekey.Format("zbIline%d",i);
			GetPrivateProfileString(stroldsection,singlecurvekey,0,szrec,80,strfilepath);
			extractstring(szrec,sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8);
			memset(&addTcurve,0,sizeof(TCurve));
			addTcurve.ncurvecolor=atoi(sz0);
			addTcurve.BcurvechannelNO=atoi(sz1);
			addTcurve.nlinenode=atoi(sz2);
			addTcurve.nlineline=atoi(sz3);
			addTcurve.nlinertu=atoi(sz4);
			addTcurve.Bdot=atoi(sz5);
			addTcurve.fcurveratio=atof(sz6);
			addTcurve.Bprecision=atoi(sz7);
			if (strlen(sz8)>0)
				strcpy(addTcurve.unit,sz8);
			zbIcurvearray.Add(addTcurve);
		}

		curvecount=GetPrivateProfileInt(stroldsection,"zbVlinecount",0,strfilepath);
		for (int i=0; i<curvecount; i++)
		{
			singlecurvekey.Format("zbVline%d",i);
			GetPrivateProfileString(stroldsection,singlecurvekey,0,szrec,80,strfilepath);
			extractstring(szrec,sz0,sz1,sz2,sz3,sz4,sz5,sz6,sz7,sz8);
			memset(&addTcurve,0,sizeof(TCurve));
			addTcurve.ncurvecolor=atoi(sz0);
			addTcurve.BcurvechannelNO=atoi(sz1);
			addTcurve.nlinenode=atoi(sz2);
			addTcurve.nlineline=atoi(sz3);
			addTcurve.nlinertu=atoi(sz4);
			addTcurve.Bdot=atoi(sz5);
			addTcurve.fcurveratio=atof(sz6);
			addTcurve.Bprecision=atoi(sz7);
			if (strlen(sz8)>0)
				strcpy(addTcurve.unit,sz8);

			zbVcurvearray.Add(addTcurve);
		}
	}
	return TRUE;
}

void CMcCurveData::Duplicate(CMcCurveData* pObject)
{
	ASSERT(pObject);
	fbottomcoord=pObject->fbottomcoord;
	nIprecision=pObject->nIprecision;
	nVprecision=pObject->nVprecision;
	fleftcoord=pObject->fleftcoord;
	ftopcoord=pObject->ftopcoord;
	frightcoord=pObject->frightcoord;
	fbottomcoord=pObject->fbottomcoord;
	fImin=pObject->fImin;
	fImax=pObject->fImax;
	fVmin=pObject->fVmin;
	fVmax=pObject->fVmax;
	nNode=pObject->nNode;
	nLine=pObject->nLine;
	nRtu=pObject->nRtu;
	bgtmode=pObject->bgtmode;
	bNeedsave=pObject->bNeedsave;
	UpName=pObject->UpName;
	DownName=pObject->DownName;
	m_realtime_style=pObject->m_realtime_style;
	
	gtIcurvearray.RemoveAll();
	gtVcurvearray.RemoveAll();
	zbIcurvearray.RemoveAll();
	zbVcurvearray .RemoveAll();
	int i;
	for (i=0; i<pObject->gtIcurvearray.GetSize(); i++)
	{
		gtIcurvearray.Add(pObject->gtIcurvearray.GetAt(i));
	}
	
	for (i=0; i<pObject->gtVcurvearray.GetSize(); i++)
	{
		gtVcurvearray.Add(pObject->gtVcurvearray.GetAt(i));
	}
	for (i=0; i<pObject->zbIcurvearray.GetSize(); i++)
	{
		zbIcurvearray.Add(pObject->zbIcurvearray.GetAt(i));
	}
	for (i=0; i<pObject->zbVcurvearray.GetSize(); i++)
	{
		zbVcurvearray.Add(pObject->zbVcurvearray.GetAt(i));
	}
	
}

void CMcCurveData::extractstring(PCHAR strextract,PTSTR str0,PTSTR str1,PTSTR str2,PTSTR str3,PTSTR str4,PTSTR str5,PTSTR str6,PTSTR str7,PSTR str8)
{
	char tempchar[80];
	char* token;
	strcpy(tempchar,strextract);

	ZeroMemory(str0,sizeof(char)*20);
	ZeroMemory(str1,sizeof(char)*20);
	ZeroMemory(str2,sizeof(char)*20);
	ZeroMemory(str3,sizeof(char)*20);
	ZeroMemory(str4,sizeof(char)*20);
	ZeroMemory(str5,sizeof(char)*20);
	ZeroMemory(str6,sizeof(char)*20);
	ZeroMemory(str7,sizeof(char)*20);
	ZeroMemory(str8,sizeof(char)*20);


	char seps[] = ",";
	token=strtok(tempchar,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		ZeroMemory(str5,sizeof(*str4));
		ZeroMemory(str6,sizeof(*str4));
		ZeroMemory(str7,sizeof(*str4));
		return;
	}
	strcpy(str0,token);
	
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		ZeroMemory(str5,sizeof(*str4));
		ZeroMemory(str6,sizeof(*str4));
		ZeroMemory(str7,sizeof(*str4));
		return;
	}
	strcpy(str1,token);
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		return;
	}
	strcpy(str2,token);
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		ZeroMemory(str5,sizeof(*str4));
		ZeroMemory(str6,sizeof(*str4));
		ZeroMemory(str7,sizeof(*str4));
		return;
	}
	strcpy(str3,token);
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		ZeroMemory(str5,sizeof(*str4));
		ZeroMemory(str6,sizeof(*str4));
		ZeroMemory(str7,sizeof(*str4));
		return;
	}
	strcpy(str4,token);
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		ZeroMemory(str5,sizeof(*str4));
		ZeroMemory(str6,sizeof(*str4));
		ZeroMemory(str7,sizeof(*str4));
		return;
	}
	strcpy(str5,token);
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		ZeroMemory(str5,sizeof(*str4));
		ZeroMemory(str6,sizeof(*str4));
		ZeroMemory(str7,sizeof(*str4));
		return;
	}
	strcpy(str6,token);
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		ZeroMemory(str0,sizeof(*str0));
		ZeroMemory(str1,sizeof(*str1));
		ZeroMemory(str2,sizeof(*str2));
		ZeroMemory(str3,sizeof(*str3));
		ZeroMemory(str4,sizeof(*str4));
		ZeroMemory(str5,sizeof(*str4));
		ZeroMemory(str6,sizeof(*str4));
		ZeroMemory(str7,sizeof(*str4));
		return;
	}
	strcpy(str7,token);
	token=strtok(NULL,seps);
	if (token==NULL)
	{
		return;
	}
	strncpy(str8,token,min(7,strlen(token)));

}

/*void CMcCurveData::arraybackup()
{
	int ncount;
	int i;
	TCurve copycurve;
	ncount=gtIcurvearray.GetSize();
	old_gtIcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=gtIcurvearray.GetAt(i);
		old_gtIcurvearray.Add(copycurve);
	}

	ncount=gtVcurvearray.GetSize();
	old_gtVcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=gtVcurvearray.GetAt(i);
		old_gtVcurvearray.Add(copycurve);
	}

	ncount=zbIcurvearray.GetSize();
	old_zbIcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=zbIcurvearray.GetAt(i);
		old_zbIcurvearray.Add(copycurve);
	}

	ncount=zbVcurvearray.GetSize();
	old_zbVcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=zbVcurvearray.GetAt(i);
		old_zbVcurvearray.Add(copycurve);
	}

}

void CMcCurveData::arrayrestore()
{
	int ncount;
	int i;
	TCurve copycurve;
	ncount=old_gtIcurvearray.GetSize();
	gtIcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=old_gtIcurvearray.GetAt(i);
		gtIcurvearray.Add(copycurve);
	}

	ncount=old_gtVcurvearray.GetSize();
	gtVcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=old_gtVcurvearray.GetAt(i);
		
		gtVcurvearray.Add(copycurve);
	}

	ncount=old_zbIcurvearray.GetSize();
	zbIcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=old_zbIcurvearray.GetAt(i);
		
		zbIcurvearray.Add(copycurve);
	}

	ncount=old_zbVcurvearray.GetSize();
	zbVcurvearray.RemoveAll();
	for (i=0; i<ncount; i++)
	{
		copycurve=old_zbVcurvearray.GetAt(i);
		
		zbVcurvearray.Add(copycurve);
	}
}*/

// CMcCurveData 成员函数

//void CMcCurveData::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())
//	{	ar<<ncurvecolor;
//		ar<< BcurvechannelNO;
//		 ar<<Bdot;
//		ar<<fcurveratio;
//		ar<<Bprecision;
//		ar<<nlinenode;
//		ar<<nlineline;
//		ar<<nlinertu;
//	
//	
//}
//	else
//	{	// loading code
//	}
//}
