// HMProject.cpp: implementation of the CHMProject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../SDDElecMap.h"
#include "../../resource.h"
#include "../../ElecMapDoc.h"
#include "../../ElecMapView.h"
#include "../../SDDElecMapDlg.h"

#include "canvas/huamian.h"
#include "canvas/HMProject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CHMProject G_HMProject;		//画面工程对象
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHMProject* CHMProject::m_pInstance =NULL;

CHMProject::CHMProject()
{
	m_strMainHM.Empty();
	m_pCurActiveHM = NULL;
}

CHMProject::~CHMProject()
{
	delete m_pCurActiveHM;
	if (!m_pCurActiveHM)
	{
		m_pCurActiveHM =NULL;
	}
}
//add by wx-------------------------------down

CHMProject* CHMProject::getInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CHMProject();
	return m_pInstance;
}

CHuaMian* CHMProject::getCurActiveHuamianInstance()
{
	return m_pCurActiveHM;
}

void	CHMProject::setCurActiveHuamianInstance(CHuaMian* pHMInstance)
{
	m_pCurActiveHM = pHMInstance;
}

BOOL	CHMProject::updateEntityInAHuamian( char *pRealData, int nCount )
{
	if (!m_pCurActiveHM){
		return FALSE;
	}
	
	if ( !(m_pCurActiveHM->updateEntityInAHuamian(pRealData,nCount)) ){
		TRACE0("HM update entity failed.\n");
		return FALSE;
	}
	
	return TRUE;
}

//add by wx-------------------------------up

void CHMProject::Load()
{

}

void CHMProject::Save()
{

}

LPCSTR CHMProject::GetMainHMName()
{
	return m_strMainHM;
}

int CHMProject::SetMainHMName(LPCSTR strname)
{
	if (_stricmp(strname,m_strMainHM)==0) 
		return 1;
	m_strMainHM=strname;	
	SetModifiedFlag();
	return 1;
}

CHuaMian*  CHMProject::GetHMObjectByID(tagHMID* pHMID)
{
	CString strName;
	strName.Format("%s",pHMID->strHMName);
	std::map<CString,CHuaMian*>::iterator itr;
	itr = map_IDHuamian.find(strName);
	if (itr != map_IDHuamian.end())
	{
		return (*itr).second;
	}
	return NULL;
}

CHuaMian * CHMProject::GetHMObject(LPCSTR strname)
{
	std::map<CString,CHuaMian*>::iterator itr =map_IDHuamian.begin();
	
	for ( ; itr != map_IDHuamian.end() ;++itr)
	{
		if ( strcmp( (*itr).first, strname ) ==0 )
			return (*itr).second;
	}
	return NULL;
}

BOOL CHMProject::HitTestOnline(LPCSTR strname)
{
	return TRUE;
}

BOOL CHMProject::OpenHM(BOOL changeScale,LPCSTR strname,LPCSTR layername)
{
	CHuaMian * pHM=NULL;

	if (g_pMainDlg ==NULL){
		return FALSE;
	}

	CElecMapDoc* pDoc = g_pMainDlg->getCurrentDocInstance();

	CString strFileFullPath = theApp.getCurrentAppPath();
	strFileFullPath.Format("%sglobal\\mapfiles\\%s.pic",theApp.getCurrentAppPath(),strname);

	pHM = GetHMObject(strname);
	if (pHM!=NULL){
		setCurActiveHuamianInstance(pHM);
	}
	else{
		//add new huamian instance
		pHM = pDoc->getHuamianInstance(strname,pDoc);
		setCurActiveHuamianInstance(pHM);
		
		tagHMID* pHMID = new tagHMID;
		ZeroMemory(pHMID,sizeof(tagHMID));
		sprintf_s(pHMID->strHMName,sizeof(pHMID->strHMName),strname);
		ReadHuaMianFileHead(strFileFullPath,pHMID);
		
		//add CHuamian instance and HMID
		AddHuaMian(pHMID);

		for (int i=0;i<pHMID->wLayerNum;i++){
			pHM->AddHMLayer(&(pHMID->ArrHMLayer[i]));
		}
		
		pDoc->OnOpenDocument(strFileFullPath);
	}

	return TRUE;
}

BOOL CHMProject::CloseHM(LPCSTR strname)
{
	return TRUE;
}


BOOL CHMProject::DeleteHM(LPCSTR strname)
{
	POSITION pos1,pos2;
	tagHMID* phmid;

	if (HitTestOnline(strname)) 
			CloseHM(strname);

	pos2=pos1=m_HMIDList.GetHeadPosition();
	while (pos1!=NULL)
	{
		phmid=m_HMIDList.GetNext(pos1);
		if (_stricmp(strname,phmid->strHMName)==0)
		{
		//	m_HMIDList.GetPrev(pos);
			m_HMIDList.RemoveAt(pos2);
			delete phmid;
			SetModifiedFlag();
			break;
		}
		pos2=pos1;
	}

	return TRUE;
}


void CHMProject::ActiveHM(LPCSTR strname)
{

}

tagHMIDList* CHMProject::GetHuaMianNameList()
{
	return &m_HMIDList;
}

void CHMProject::InitMemory()
{
	m_strMainHM.Empty();
	POSITION pos=m_HMIDList.GetHeadPosition();
	while (pos!=NULL) 
		delete m_HMIDList.GetNext(pos);
	m_HMIDList.RemoveAll();
}

BOOL CHMProject::LoadFile(LPCSTR filepath)
{
	CString str;
	CStdioFile file;

	if (file.Open(filepath,CFile::modeRead|CFile::typeText)==FALSE)
	{
		str="不能打开工程定义文件: ";
		str=str+filepath;
		AfxMessageBox(str);
		return FALSE;
	}

	ReadHuaMianList(&file);
	ReadMainHuaMian(&file);
	ReadHMLayer();

	return TRUE;
}

BOOL CHMProject::SaveFile(LPCSTR filepath)
{
	CString str;
	CStdioFile file;

	UpdateAllHMLayer();

	if (file.Open(filepath,CFile::modeCreate|CFile::modeReadWrite|CFile::typeText)==FALSE)
	{
		str="不能写工程定义文件: ";
		str=str+filepath;
		AfxMessageBox(str);
		return FALSE;
	}

	WriteHuaMianList(&file);
	WriteProjectSection(&file);

	return TRUE;
}

BOOL CHMProject::ReadHuaMianList(CStdioFile *pfile)
{
	CString str;
	BOOL bl1,bl2;
	tagHMID hmid;
	tagHMID* phmid;
	int iret;

	//移动读指针到画面段落	
	if (SeekSection(pfile,"HuaMian")==FALSE) 
		return FALSE;
	try
	{
		//读取下一行字符串
		bl1=pfile->ReadString(str);
		if (!bl1) return FALSE;

		str.TrimLeft();
		str.TrimRight();
		//判断字符串是否是段落名
		bl2=HitTestSectionID(str);
		if (bl2) return FALSE;
		while ((!bl2)&&(bl1))
		{
			//读入画面名称和画面标志
			iret=sscanf(str,"%x=%[^;]33s",&hmid.dwflag,hmid.strHMName);//%[^=]s
			if (iret==2)
			{
				phmid=new tagHMID;
				if (phmid==NULL) return FALSE;
				ZeroMemory(phmid,sizeof(*phmid));
				strcpy(phmid->strHMName,hmid.strHMName);
				phmid->dwflag=hmid.dwflag;
				m_HMIDList.AddTail(phmid);
			}

			//读取下一行字符串
			bl1=pfile->ReadString(str);
			if (!bl1) break;

			str.TrimLeft();
			str.TrimRight();
			//判断字符串是否是段落名
			bl2=HitTestSectionID(str);
			if (bl2) break;
		}
		return TRUE;
	}
	catch(CFileException* e)
	{
	  e->Delete();
		return FALSE;
	}

}


BOOL CHMProject::ReadString(CStdioFile *pfile, LPCSTR strSection, LPCSTR strKey, LPSTR buffer,int bufsize)
{
	CString str,str2;
	BOOL bl1,bl2;
	char buf[64],strft[32];
	char strRet[256];
	int iret;
 
	//移动读指针到画面段落	
	if (SeekSection(pfile,strSection)==FALSE) 
		return NULL;
	
	try
	{
		//读取下一行字符串
		bl1=pfile->ReadString(str);
		if (!bl1) return FALSE;

		str.TrimLeft();
		str.TrimRight();

		//判断字符串是否是段落名
		bl2=HitTestSectionID(str);
		if (bl2) return FALSE;
		while ((!bl2)&&(bl1))
		{
			//读入关键字和关键字包含的内容		
			ZeroMemory(strRet,sizeof(*strRet));
			sprintf(strft,"%d",strlen(strSection)-1);
			str2="%";
			str2=str2+strft+"s=%[^;]255s";
			iret=sscanf(str,str2,buf,strRet);
			if (iret>0)
			{
				if (_stricmp(buf,strKey)==0) 
				{
					strncpy(buffer,strRet, bufsize>256?255:bufsize-1);
					return TRUE;
				}
			}

			//读取下一行字符串
			bl1=pfile->ReadString(str);
			if (!bl1) break;

			str.TrimLeft();
			str.TrimRight();
			//判断字符串是否是段落名
			bl2=HitTestSectionID(str);
			if (bl2) break;
		}
		return FALSE;
	}
	catch(CFileException* e)
	{
	  e->Delete();
		return FALSE;
	}
}

BOOL CHMProject::ReadMainHuaMian(CStdioFile *pfile)
{
	char buf[33];

	m_strMainHM.Empty();
	ZeroMemory(buf,sizeof(buf));

	if (ReadString(pfile,"project","MainHM",buf,33)&& (strlen(buf)>0))
	{
		m_strMainHM=buf;
		return TRUE;
	}

	return FALSE;
}

BOOL CHMProject::ReadHMLayer()
{
	return TRUE;
}

BOOL CHMProject::ReadHuaMianFileHead(LPCSTR filepath, tagHMID *pHMID)
{
	CFile file;
	CString str;
	tagHuaMianFileHead filehead;
	memset(&filehead,0,sizeof(tagHuaMianFileHead));
	ASSERT(pHMID!=NULL);
	if (file.Open(filepath,CFile::modeRead|CFile::typeBinary)==FALSE)
	{
		str="不能打开画面文件: ";
		str=str+filepath;
		AfxMessageBox(str);
		return FALSE;
	}
	if (file.Read((char *)&filehead,sizeof(filehead))<sizeof(filehead)) return FALSE;
	pHMID->dwStyle=filehead.recBase.dwPICStyle;
	pHMID->wWidth=filehead.recBase.wPicWidth;
	pHMID->wHeight=filehead.recBase.wPicHeight;
	pHMID->fScale=filehead.recBase.fDefaultScale;
	pHMID->wLayerNum=filehead.bLayerNum%(HMC_MAXNUM+1);
	for (int i=0;i<pHMID->wLayerNum;i++)
		pHMID->ArrHMLayer[i]=filehead.arrLayer[i];

	file.Close();
	return TRUE;
}


BOOL CHMProject::HitTestSectionID(LPCSTR str)
{
	CString s;
	ASSERT(str!=NULL);
	s=str;
	s.TrimLeft();
	if (s.IsEmpty()) return FALSE; 
	if ((s[0]=='[')&&(s.FindOneOf("]")>0))
		return TRUE;
	return FALSE;
}

BOOL CHMProject::HitTestSectionName(LPCSTR strSection, LPCSTR strName)
{
	ASSERT(strSection!=NULL);
	ASSERT(strName!=NULL);

	CString str="[";
	str=str+strName+"]";

	if (_strnicmp(strSection,str,str.GetLength())==0)
		 return TRUE;

	else return FALSE;
}

//更新所有的画面层
void CHMProject::UpdateAllHMLayer()
{

}

BOOL CHMProject::WriteHuaMianList(CStdioFile *pfile)
{
	tagHMID* phmid;
	POSITION pos;
	CString str="[HuaMian]\n";
	char buf[1024];

	pfile->WriteString(str);
	
	pos=m_HMIDList.GetHeadPosition();
	while (pos!=NULL)
	{
		phmid=m_HMIDList.GetNext(pos);
		ASSERT(phmid!=NULL);
		ZeroMemory(buf,sizeof(*buf));
		sprintf(buf,"%x=%s\n",phmid->dwflag,phmid->strHMName);
		str=buf;
		pfile->WriteString(str);
	}
	return TRUE;
}


BOOL CHMProject::WriteProjectSection(CStdioFile *pfile)
{
	CString str="[project]\n";
	char buf[1024];

	pfile->WriteString(str);
	ZeroMemory(buf,sizeof(*buf));
	sprintf(buf,"%s=%s\n","MainHM",m_strMainHM);
	str=buf;
	pfile->WriteString(str);
	return TRUE;
}


BOOL CHMProject::SeekSection(CStdioFile *pfile, LPCSTR strSection)
{
	CString str;
	BOOL bret;

	ASSERT(strSection!=NULL);
	ASSERT(pfile!=NULL);
	try
	{
			pfile->SeekToBegin();
			bret=pfile->ReadString(str);
			while (bret)
			{	
				if (HitTestSectionID(str)==TRUE)
				{
				  if ((HitTestSectionName(str,strSection))==TRUE)
						  return TRUE;
				}
				bret=pfile->ReadString(str);

			}
			return FALSE;
	}
	catch(CFileException* e)
	{
		e->Delete();
		return FALSE;
	}
}

void CHMProject::SetModifiedFlag(BOOL bModified)
{
  if (m_modifyflag==bModified) return;
	m_modifyflag=bModified;
}

const tagHMLayerID&  tagHMLayerID::operator =( const tagHMLayerID& srcHMLayerID)
{
	iLayerNo=srcHMLayerID.iLayerNo;
	strcpy(strLayerName,srcHMLayerID.strLayerName);
	fScale=srcHMLayerID.fScale;
	fStartScale=srcHMLayerID.fStartScale;
	fEndScale=srcHMLayerID.fEndScale;
	iStartNumber=srcHMLayerID.iStartNumber;
	iEndNumber=srcHMLayerID.iEndNumber;
	IDDef=srcHMLayerID.IDDef;	
	iOffset=srcHMLayerID.iOffset;
	bHide=srcHMLayerID.bHide;
	return *this;
}

const tagHMID& tagHMID::operator =(const tagHMID &srcHMID)
{
	strcpy(strHMName,srcHMID.strHMName);
	dwflag=srcHMID.dwflag;
	dwStyle=srcHMID.dwStyle;
	wWidth=srcHMID.wWidth;
	wHeight=srcHMID.wHeight;
	wLayerNum=srcHMID.wLayerNum % (HMC_MAXNUM+1);
	fScale=srcHMID.fScale;
	iLayerIdDef=srcHMID.iLayerIdDef;
	
	for (int i=0;i<=wLayerNum-1;i++)
		ArrHMLayer[i]=srcHMID.ArrHMLayer[i];

	return *this;
}

BOOL CHMProject::AddHuaMian(tagHMID *pHMID)
{
	ASSERT(pHMID!=NULL);

	m_HMIDList.AddTail(pHMID);
	SetModifiedFlag(TRUE); 

	CString strName;
	strName.Format("%s",pHMID->strHMName);

	map_IDHuamian.insert(make_pair(strName,m_pCurActiveHM));

	return TRUE;
}

BOOL CHMProject::Get_HmLayer_Names(LPCSTR hmname, CStringArray& layerList)
{
	BOOL ret=FALSE;
	tagHMID * pHMID=FindHMID(hmname);
	if (pHMID==NULL)  return ret;
	int loop=pHMID->wLayerNum;
	CString str;
	for (int i=0;i<=loop-1;i++)
	{
		str=pHMID->ArrHMLayer[i].strLayerName;
		if (!str.IsEmpty())
		  layerList.Add(str);
	}
	ret=TRUE;
	return ret;
}


tagHMID * CHMProject::FindHMID(LPCSTR strname)
{
	POSITION pos;
	tagHMID* phmid;

	pos=m_HMIDList.GetHeadPosition();
	while (pos!=NULL)
	{
		phmid=m_HMIDList.GetNext(pos);
		ASSERT(phmid!=NULL);
		if (_stricmp(strname,phmid->strHMName)==0)
		  return phmid;
	}

	return NULL;
}

BOOL CHMProject::ModifyHMprop(LPCSTR strname, tagHMID *pHMID)
{
	ASSERT(strname!=NULL);
	ASSERT(pHMID!=NULL);
	tagHMID * p=FindHMID(strname);
	if (p!=NULL)  *p=*pHMID;

	CHuaMian* pHuaMian;
	pHuaMian=GetHMObject(strname);
	if (pHuaMian==NULL) return TRUE;
	pHuaMian->SetHMID(pHMID);
	SetModifiedFlag(TRUE);
	return TRUE;
}

BOOL CHMProject::IsModified()
{
  return (m_modifyflag!=0);
}

void tagHMID::AddHMLayer(tagHMLayerID *pLayer)
{
	 if (wLayerNum==HMC_MAXNUM) return;
	 ASSERT(pLayer!=NULL);
	 if (FindHMLayerID(pLayer->strLayerName)>=0) return;
	 ArrHMLayer[wLayerNum++]=*pLayer; 
}

void tagHMID::SetHMLayer(LPCSTR strname, tagHMLayerID *pLayer)
{
	 int id=FindHMLayerID(pLayer->strLayerName);
	 if (id<0) return;
   ArrHMLayer[id]=*pLayer; 
}	 		

void tagHMID::DeleteHMLayer(LPCSTR strname)
{
	 int id=FindHMLayerID(strname);
	 if (id<0) return;
	 for (int i=id;i<wLayerNum-1;i++)
     ArrHMLayer[i]=ArrHMLayer[i+1]; 
}

int tagHMID::FindHMLayerID(LPCSTR strname)
{
	 for (int i=0;i<wLayerNum;i++)
     if(_stricmp(ArrHMLayer[i].strLayerName,strname)==0) return i;
	 return -1;	 
}

void CHMProject::AddHMLayer(LPCSTR strName, tagHMLayerID *pLayer)
{
	ASSERT(pLayer!=NULL);
	ASSERT(strName!=NULL);

	tagHMID * pHMID= CHMProject::FindHMID(strName);
	if (pHMID==NULL) return;
	pHMID->AddHMLayer(pLayer);

	CHuaMian *pHM=GetHMObject(strName);
	ASSERT(pHM!=NULL);
	pHM->AddHMLayer(pLayer);
}

void CHMProject::SetHMLayer(LPCSTR pHM, LPCSTR pLayer, tagHMLayerID *pLayerID)
{
	tagHMID * pHMID= CHMProject::FindHMID(pHM);
	if (pHMID==NULL) return;
	pHMID->SetHMLayer(pLayer,pLayerID);
	CHuaMian* pHMObj=GetHMObject(pHM);
	if (pHMObj==NULL) return;
	pHMObj->ModifyLayerProp(pLayer,pLayerID);
}


int CHMProject::FindHMLayerID(LPCSTR pHM, LPCSTR pLayer)
{
	tagHMID * pHMID= CHMProject::FindHMID(pHM);
	if (pHMID==NULL) return -1;
	return pHMID->FindHMLayerID(pLayer);
}

void CHMProject::DeleteHMLayer(LPCSTR pHM, LPCSTR pLayer)
{
	ASSERT(pHM!=NULL);
	ASSERT(pLayer!=NULL);

	tagHMID * pHMID= CHMProject::FindHMID(pHM);
	if (pHMID==NULL) return;
	pHMID->DeleteHMLayer(pLayer);

	CHuaMian *pobj=GetHMObject(pHM);
	ASSERT(pobj!=NULL);
	pobj->DeleteLayer(pLayer);
}


tagHMLayerID * CHMProject::GetHMLayerID(LPCSTR pHM, LPCSTR pLayer)
{
	tagHMID * pHMID= CHMProject::FindHMID(pHM);
	if (pHMID==NULL) return NULL;
	int id=pHMID->FindHMLayerID(pLayer);
	if (id<0) return NULL;
	return (tagHMLayerID*)(&(pHMID->ArrHMLayer[id]));
}

void CHMProject::MovePre(LPCSTR hmname,LPCSTR layername)
{
	tagHMID* phmid1,*phmid2;
	ASSERT(hmname!=NULL);

	if (layername==NULL)
	{
		POSITION pos,pre;

		pos=m_HMIDList.GetHeadPosition();
		pre=pos;
		phmid1=NULL;
		while (pos!=NULL)
		{
			phmid2=phmid1;
			phmid1=m_HMIDList.GetAt(pos);
			ASSERT(phmid1!=NULL);
			if (_stricmp(hmname,phmid1->strHMName)!=0)
			{
				pre=pos;
				m_HMIDList.GetNext(pos);
		    continue;
			}
			m_HMIDList.SetAt(pre,phmid1);
			m_HMIDList.SetAt(pos,phmid2);
			break;
		}
	}
	else 
	{
		phmid1=FindHMID(hmname);
		ASSERT(phmid1!=NULL);
		int id=phmid1->FindHMLayerID(layername);
		ASSERT((id>0)&&(id<phmid1->wLayerNum)); 
		phmid1->Exchange(id,id-1);
		
		CHuaMian* pHM=GetHMObject(hmname);
		if(pHM==NULL) return;
		pHM->MovePre(layername);		
	}
}

void CHMProject::MoveNext(LPCSTR hmname, LPCSTR layername)
{
	tagHMID* phmid1,*phmid2;

	ASSERT(hmname!=NULL);

	if (layername==NULL)
	{
		POSITION pos,pre;

		pos=m_HMIDList.GetTailPosition();
		pre=pos;
		phmid1=NULL;
		while (pos!=NULL)
		{
			phmid2=phmid1;
			phmid1=m_HMIDList.GetAt(pos);
			ASSERT(phmid1!=NULL);
			if (_stricmp(hmname,phmid1->strHMName)!=0)
			{
				pre=pos;
				m_HMIDList.GetPrev(pos);
		    continue;
			}
			m_HMIDList.SetAt(pre,phmid1);
			m_HMIDList.SetAt(pos,phmid2);
			break;
		}
	}
	else 
	{
		phmid1=FindHMID(hmname);
		ASSERT(phmid1!=NULL);
		int id=phmid1->FindHMLayerID(layername);
		ASSERT((id>=0)&&(id<phmid1->wLayerNum-1)); 
		phmid1->Exchange(id,id+1);
		
		CHuaMian* pHM=GetHMObject(hmname);
		if(pHM==NULL) return;
		pHM->MoveNext(layername);		
	}
}

void tagHMID::Exchange(int i, int j)
{
	tagHMLayerID id;
	ASSERT((i>=0)&&(i<HMC_MAXNUM));
	ASSERT((j>=0)&&(j<HMC_MAXNUM));
	id=ArrHMLayer[i];
	ArrHMLayer[i]=ArrHMLayer[j];
	ArrHMLayer[j]=id;
}

void CHMProject::SaveHM(LPCSTR hmname)
{
	ASSERT(hmname!=NULL);
	if (hmname=="") 
		return;

	if (!HitTestOnline(hmname)) 
		return;

	CHuaMian * pHM=GetHMObject(hmname);
	if (pHM==NULL) 
		return;

	CSDDElecMapDlg* pDlg = (CSDDElecMapDlg*)(AfxGetApp()->m_pMainWnd);
	CElecMapDoc* pDoc = pDlg->getCurrentDocInstance();
	if (pDoc==NULL) {
		return;
	}

	CString strFileFullPath = theApp.getCurrentAppPath();
	CString strpath;
	strpath.Format("global\\mapfiles\\%s.PIC",hmname);
	strFileFullPath = strFileFullPath + strpath;

	pDoc->OnFileSave(strFileFullPath);
}

// 获取系统正在编辑的画面的数目
int CHMProject::GetOpenHmCount(void)
{
	int ret=0;
	return ret;
}

CString CHMProject::GetActiveComputer()
{
	return m_ComputerName;
}
