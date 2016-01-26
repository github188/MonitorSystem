// HuaMian.cpp: implementation of the CHuaMian class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\SDDElecMap.h"
#include "canvas/PicFileException.h"
#include "..\..\ElecMapDoc.h"
#include "..\..\ElecMapView.h"
#include "canvas/HuaMian.h"

#include "gloable/FileRead.h"
#include "canvas/YX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CHuaMian, CObject, 0)

WORD CHuaMian::m_wPicFlag=0x7F6E;
WORD CHuaMian::m_wMVersion=6;
WORD CHuaMian::m_wEVersion=0;

CHuaMian::CHuaMian(CElecMapDoc* pdoc)
{
	m_pDoc=pdoc;
}

CHuaMian::CHuaMian(LPCSTR strName,CElecMapDoc* pdoc,DWORD style,WORD picwidth,WORD picheight,WORD wresolution,
									 WORD hresolution,float scale,int preLayerID)
{
	m_dwPICStyle=style;
	m_wPicWidth=picwidth;
	m_wPicHeight=picheight;
	m_wResolution=wresolution;
	m_hResolution=hresolution;
	m_fDefaultScale=scale;
	m_pDoc=pdoc;
	m_strName=strName;
	m_iLayerIdDef=preLayerID;
}

CHuaMian::~CHuaMian()
{
	POSITION pos = m_HMLayerList.GetHeadPosition();
	while (pos != NULL)
		delete m_HMLayerList.GetNext(pos);
}

void CHuaMian::OnAddTyEvent(CTyBase *pTy)
{
	if (m_pDoc!=NULL)
		m_pDoc->SetModifiedFlag();
}

BOOL CHuaMian::GetScaleProp()
{
	return ((m_dwPICStyle&HMT_SCALE)!=0);
}

BOOL CHuaMian::GetTempleProp()
{
	return ((m_dwPICStyle&HMT_TEMPLE)!=0);
}

void CHuaMian::Serialize(CArchive &ar)
{

}

void CHuaMian::Save(CFile *file, BOOL Yn)
{
	tagHuaMianFileHead filehead;
	POSITION pos;
	CHMLayer * pLayer;
	int i=0;

	ZeroMemory(&filehead,sizeof(filehead));

	/*
	CString tmp;
	tmp.Format("%d",sizeof(tagHMLayerID));
	AfxMessageBox(tmp);
*/

	if(Yn)	//如果是在进行保存
	{
		ZhenLi();
		filehead.recBase.wPicFlag=m_wPicFlag;
		filehead.recBase.wMVersion=m_wMVersion;
		filehead.recBase.wEVersion=m_wEVersion;
		filehead.recBase.dwPICStyle=m_dwPICStyle;
		filehead.recBase.wPicWidth=m_wPicWidth;
		filehead.recBase.wPicHeight=m_wPicHeight;
		filehead.recBase.wResolution=m_wResolution;
		filehead.recBase.hResolution=m_hResolution;
		filehead.recBase.fDefaultScale=m_fDefaultScale;
		filehead.recBase.dwBackColor=m_dwBackColor;
		filehead.recBase.dwBackImageNo=m_dwBackImageNo;
		filehead.iLayerIdDef=m_iLayerIdDef;
		filehead.bLayerNum=(byte)m_HMLayerList.GetCount();

		file->Write((byte *)&filehead,sizeof(filehead));

		
		pos=m_HMLayerList.GetHeadPosition();
		while (pos!=NULL)
		{
		  pLayer=m_HMLayerList.GetNext(pos);
			ASSERT(pLayer!=NULL);
			filehead.arrLayer[i].iLayerNo=pLayer->m_iLayerNo;
			lstrcpy((LPSTR)filehead.arrLayer[i].strLayerName,(LPCTSTR)pLayer->m_CName.GetBuffer());
			filehead.arrLayer[i].fStartScale=pLayer->m_startViewBL;
			filehead.arrLayer[i].fEndScale=pLayer->m_endViewBL;
			filehead.arrLayer[i].fScale=pLayer->m_DefaultBL;
			filehead.arrLayer[i].iOffset=(int)file->GetPosition();
			filehead.arrLayer[i].iStartNumber=0;//pLayer->m_iStartNumber;
			filehead.arrLayer[i].iEndNumber=1;//pLayer->m_iEndNumber;
			filehead.arrLayer[i].IDDef=pLayer->m_IDDef;
			filehead.arrLayer[i].bHide=pLayer->m_bHide;

			pLayer->Save(file,Yn);
			++i;
		}

		file->SeekToBegin();
		file->Write((byte *)&filehead,sizeof(filehead));

		//写画面文件头信息

	}
	else //装载文件
	{
		UINT ret;
		int errno;
		CPicFileException *pException;

		//读画面文件头信息
		ret=file->Read((byte *)&filehead,sizeof(filehead));
		if (ret<sizeof(filehead))
		{
			pException=new CPicFileException();
			pException->m_errno=CPicFileException::BADFILE;
			throw(pException);
		}

		//比较画面文件特定标志
		if (filehead.recBase.wPicFlag!=m_wPicFlag)
		{
			pException=new CPicFileException();
			pException->m_errno=CPicFileException::UNKNOWN;
			throw(pException);
		}

		//比较画面文件版本号
		if ((filehead.recBase.wMVersion>m_wMVersion)||((filehead.recBase.wMVersion==m_wMVersion)
				&&((filehead.recBase.wEVersion>m_wEVersion))))
		{
			pException=new CPicFileException();
			pException->m_errno=CPicFileException::HIGHVERSION;
			throw(pException);
		} 
		else if ((filehead.recBase.wMVersion<m_wMVersion)||((filehead.recBase.wMVersion==m_wMVersion)
							&&((filehead.recBase.wEVersion<m_wEVersion))))
		{
			pException=new CPicFileException();
			pException->m_errno=CPicFileException::LOWVERSION;
			throw(pException);
		}

		m_wPicFlag=filehead.recBase.wPicFlag;
		m_wMVersion=filehead.recBase.wMVersion;
		m_wEVersion=filehead.recBase.wEVersion;
		m_dwPICStyle=filehead.recBase.dwPICStyle;
		m_wPicWidth=filehead.recBase.wPicWidth;
		m_wPicHeight=filehead.recBase.wPicHeight;
		m_wResolution=filehead.recBase.wResolution;
		m_hResolution=filehead.recBase.hResolution;
		m_fDefaultScale=filehead.recBase.fDefaultScale;
		m_dwBackColor=filehead.recBase.dwBackColor;
		m_dwBackImageNo=filehead.recBase.dwBackImageNo;
		m_iLayerIdDef=filehead.iLayerIdDef;

		pos = m_HMLayerList.GetHeadPosition();
		for (i=0;i<filehead.bLayerNum;i++)
		{
			pLayer= m_HMLayerList.GetNext(pos);//new CHMLayer(this);
			pLayer->m_iLayerNo=filehead.arrLayer[i].iLayerNo;
			pLayer->m_CName=filehead.arrLayer[i].strLayerName;
			pLayer->m_startViewBL=filehead.arrLayer[i].fStartScale;
			pLayer->m_endViewBL=filehead.arrLayer[i].fEndScale;
			pLayer->m_DefaultBL=filehead.arrLayer[i].fScale;
			//pLayer->m_iStartNumber=filehead.arrLayer[i].iStartNumber;
			pLayer->m_iStartNumber=0;
			//pLayer->m_iEndNumber=filehead.arrLayer[i].iEndNumber;
			pLayer->m_iEndNumber=0;
			//pLayer->m_IDDef=filehead.arrLayer[i].IDDef;
			pLayer->m_IDDef=0;
			pLayer->m_bHide=filehead.arrLayer[i].bHide;

			pLayer->OnHMLayerDataChanged();
			//m_HMLayerList.AddTail(pLayer);
			pLayer->Save(file,Yn);
		}
		if (m_pDoc!=NULL)
			m_pDoc->UpdateScale(m_fDefaultScale);
	}
}

CString CHuaMian::GetHMName()
{
  return m_strName;
}

void CHuaMian::SetHMID(tagHMID *pHMID)
{
	ASSERT(pHMID!=NULL);
	if (pHMID==NULL) return;
	if (strlen(pHMID->strHMName)<=32)
			m_strName=pHMID->strHMName;
	m_dwPICStyle=pHMID->dwStyle;
	m_fDefaultScale=pHMID->fScale;

	if (m_pDoc!=NULL)
	{
		m_pDoc->SetTitle(m_strName);
		m_pDoc->SetModifiedFlag(TRUE);
		
		//=========================================
		//modified by wx 2015-08-31......update later
		CString filepath;
		filepath="D:\\work\\code\\elecMap\\MDI_DIALOG\\SDDElecMap\\SDDElecMap";//theApp.GetDirectory();
	    //filepath=filepath+PICPATH;
		//filepath=filepath+m_strName+".pic";
		//=========================================

	    m_pDoc->SetPathName(filepath);
	}

	if ((m_wPicWidth!=pHMID->wWidth)||(m_wPicHeight!=pHMID->wHeight)) 
	{
		m_wPicWidth=pHMID->wWidth;
		m_wPicHeight=pHMID->wHeight;
		
		OnHMSizeChanged();
	}

	if (m_pDoc!=NULL)
		m_pDoc->UpdateScale(pHMID->fScale);
}


//保存/画面文件头信息

void CHuaMian::SavePicHead(CFile *file, tagHuaMianFileHead* filehead,BOOL Yn)
{
	ASSERT(file!=NULL);
	ASSERT(filehead!=NULL);
	
	if (Yn)
	{
		file->Write((char*)&filehead->recBase.wPicFlag,sizeof(filehead->recBase.wPicFlag));
		file->Write((char*)&filehead->recBase.wMVersion,sizeof(filehead->recBase.wMVersion));
		file->Write((char*)&filehead->recBase.wEVersion,sizeof(filehead->recBase.wEVersion));
		file->Write((char*)&filehead->recBase.dwPICStyle,sizeof(filehead->recBase.dwPICStyle));
		file->Write((char*)&filehead->recBase.wPicWidth,sizeof(filehead->recBase.wPicWidth));
		file->Write((char*)&filehead->recBase.wPicHeight,sizeof(filehead->recBase.wPicHeight));
		file->Write((char*)&filehead->recBase.wResolution,sizeof(filehead->recBase.wResolution));
		file->Write((char*)&filehead->recBase.hResolution,sizeof(filehead->recBase.hResolution));
		file->Write((char*)&filehead->recBase.fDefaultScale,sizeof(filehead->recBase.fDefaultScale));
	}
	else 
	{
		file->Read((char*)&filehead->recBase.wPicFlag,sizeof(filehead->recBase.wPicFlag));
		file->Read((char*)&filehead->recBase.wMVersion,sizeof(filehead->recBase.wMVersion));
		file->Read((char*)&filehead->recBase.wEVersion,sizeof(filehead->recBase.wEVersion));
		file->Read((char*)&filehead->recBase.dwPICStyle,sizeof(filehead->recBase.dwPICStyle));
		file->Read((char*)&filehead->recBase.wPicWidth,sizeof(filehead->recBase.wPicWidth));
		file->Read((char*)&filehead->recBase.wPicHeight,sizeof(filehead->recBase.wPicHeight));
		file->Read((char*)&filehead->recBase.wResolution,sizeof(filehead->recBase.wResolution));
		file->Read((char*)&filehead->recBase.hResolution,sizeof(filehead->recBase.hResolution));
		file->Read((char*)&filehead->recBase.fDefaultScale,sizeof(filehead->recBase.fDefaultScale));
	}
}

CSize CHuaMian::GetHMSize()
{
	CSize sz;
	sz.cx=m_wPicWidth;
	sz.cy=m_wPicHeight;
	return sz;
}

void CHuaMian::DrawDC(CDC *pDC, CElecMapView *pView)
{
	CHMLayer* pLayer;
	ASSERT(pView!=NULL);
	ASSERT(pDC!=NULL);
	POSITION pos=m_HMLayerList.GetHeadPosition();
	while (pos!=NULL)
	{
		pLayer=m_HMLayerList.GetNext(pos);
		ASSERT(pLayer!=NULL);
		if (pLayer->IsInViewBL(pView->getCurrentScale()));
			pLayer->DrawDC(pDC,pView);
	}
}

void CHuaMian::OnHMSizeChanged()
{
	//视图环境初始化	
	if (m_pDoc!=NULL)
	{
		CElecMapView *pView = m_pDoc->getCurrentView();
		ASSERT(pView!=NULL);
		pView->OnHMSizeChanged();
	}
	
	//画面层处理画面大小的变化
	CHMLayer* pLayer;
	POSITION pos=m_HMLayerList.GetHeadPosition();
	while (pos!=NULL)
	{
	  pLayer=m_HMLayerList.GetNext(pos);
		ASSERT(pLayer!=NULL);
		pLayer->OnHMLayerDataChanged();
	}
}

CHMLayer * CHuaMian::AddHMLayer(tagHMLayerID *pLayer)
{
	ASSERT(pLayer!=NULL);
	CHMLayer* p=FindLayer(pLayer->strLayerName);
	if (p!=NULL) return NULL;
	if (m_HMLayerList.GetCount()==8) return NULL;
	//float startBL,float endBL,float defBL, int layerID,LPCSTR pStr,CHuaMian *pHuaMian)
	pLayer->iLayerNo=++m_iLayerIdDef;
	p=new CHMLayer(pLayer->fStartScale,pLayer->fEndScale,pLayer->fScale,pLayer->iLayerNo,pLayer->strLayerName,this);
	if (p==NULL) return NULL;
	p->m_bHide=pLayer->bHide;
	m_HMLayerList.AddTail(p);
	return p;
}

void CHuaMian::ModifyLayerProp(LPCSTR pLayer, tagHMLayerID *pLayerID)
{
	ASSERT(pLayer!=NULL);
	ASSERT(pLayerID!=NULL);
	ASSERT(m_pDoc!=NULL);
	CHMLayer* p=FindLayer(pLayer);
	if (p==NULL) return;
	p->ModifyProp(pLayerID);					
	m_pDoc->SetModifiedFlag(TRUE);
	m_pDoc->UpdateScale(pLayerID->fScale);
	m_pDoc->UpdateAllViews(NULL,HINT_UPDATE_WINDOW,NULL);
}

CHMLayer * CHuaMian::FindLayer(LPCSTR strLayer)
{
	CHMLayer* pLayer;
	POSITION pos=m_HMLayerList.GetHeadPosition();

	while (pos!=NULL)
	{
	  pLayer=m_HMLayerList.GetNext(pos);
		ASSERT(pLayer!=NULL);
		if (_stricmp((LPCSTR)pLayer->m_CName.GetBuffer(),strLayer)==0)
			 return pLayer; 
	}
	return NULL;
}

CHMLayer * CHuaMian::FindLayer(int LayerID)
{
	CHMLayer* pLayer;
	POSITION pos=m_HMLayerList.GetHeadPosition();

	while (pos!=NULL)
	{
	  pLayer=m_HMLayerList.GetNext(pos);
		ASSERT(pLayer!=NULL);
		if (pLayer->m_iLayerNo==LayerID)
			 return pLayer; 
	}
	return NULL;
}

void CHuaMian::DeleteLayer(LPCSTR strName)
{
	ASSERT(strName!=NULL);
	CHMLayer* pLayer=FindLayer(strName); 
	if (pLayer==NULL) return;
	OnDeleteLayer(pLayer);
	POSITION pos=m_HMLayerList.Find(pLayer);
	ASSERT(pos!=NULL);
	m_HMLayerList.RemoveAt(pos);
	delete pLayer;
}

void CHuaMian::OnDeleteLayer(CHMLayer *pLayer)
{
	ASSERT(pLayer!=NULL);
	if (m_pDoc!=NULL)
		m_pDoc->SetModifiedFlag();
}

void CHuaMian::OnRemoveTyEvent(CTyBase *pTy)
{
	if (m_pDoc!=NULL)
		m_pDoc->SetModifiedFlag();
}

BOOL CHuaMian::GetBmpBackGroundMode()
{
  return (HMT_BMP&m_dwPICStyle);
}

float CHuaMian::GetDefaultScale()
{
	return m_fDefaultScale;
}

void CHuaMian::ZhenLi()
{
	CHMLayer* pLayer;
	POSITION pos=m_HMLayerList.GetHeadPosition();
	while (pos!=NULL)
	{
		pLayer=m_HMLayerList.GetNext(pos);
		ASSERT(pLayer!=NULL);
		pLayer->ZhenLi();
	}
}

void CHuaMian::MovePre(LPCSTR layername)
{
	ASSERT(layername!=NULL);
	CHMLayer* pLayer1,*pLayer2;

	POSITION pos=m_HMLayerList.GetHeadPosition();
	POSITION pre=pos;
	pLayer1=NULL;
	while (pos!=NULL)
	{
		pLayer2=pLayer1;
		pLayer1=m_HMLayerList.GetAt(pos);
		ASSERT(pLayer1!=NULL);
		if( _stricmp((LPCSTR)pLayer1->m_CName.GetBuffer(),layername)!=0)
		{
			pre=pos;
			m_HMLayerList.GetNext(pos);
			continue;
		}
		ASSERT(pre!=NULL);
		m_HMLayerList.SetAt(pre,pLayer1);
		m_HMLayerList.SetAt(pos,pLayer2);
		break;
	}

	if (m_pDoc!=NULL)
	{
	  m_pDoc->UpdateAllViews(NULL,HINT_UPDATE_WINDOW,NULL);
		m_pDoc->SetModifiedFlag();
	}
}

void CHuaMian::MoveNext(LPCSTR layername)
{
	ASSERT(layername!=NULL);
	CHMLayer* pLayer1,*pLayer2;

	POSITION pos=m_HMLayerList.GetTailPosition();
	POSITION pre=pos;
	pLayer1=NULL;
	while (pos!=NULL)
	{
		pLayer2=pLayer1;
		pLayer1=m_HMLayerList.GetAt(pos);
		ASSERT(pLayer1!=NULL);
		if( _stricmp((LPCSTR)pLayer1->m_CName.GetBuffer(),layername)!=0)
		{
			pre=pos;
			m_HMLayerList.GetPrev(pos);
			continue;
		}
		ASSERT(pre!=NULL);
		m_HMLayerList.SetAt(pre,pLayer1);
		m_HMLayerList.SetAt(pos,pLayer2);
		break;
	}

	if (m_pDoc!=NULL)
	{
	  m_pDoc->UpdateAllViews(NULL,HINT_UPDATE_WINDOW,NULL);
		m_pDoc->SetModifiedFlag();
	}
}

void CHuaMian::GetScaleRange(float *pMinScale, float *pMaxScale)
{
	CHMLayer * pLayer;
	*pMinScale=1;
	*pMaxScale=1;
	POSITION pos=m_HMLayerList.GetHeadPosition();
	while (pos!=NULL)
	{
		pLayer=m_HMLayerList.GetNext(pos);
		ASSERT(pLayer!=NULL);
		if (pLayer->m_startViewBL<(*pMinScale))
			*pMinScale=pLayer->m_startViewBL;
		if (pLayer->m_endViewBL>(*pMaxScale))
			*pMaxScale=pLayer->m_endViewBL;
	}
}


void CHuaMian::ParamReplace(CStringList& ruleList)
{
	POSITION pos;
	CHMLayer * pLayer;
	pos=m_HMLayerList.GetHeadPosition();
	while (pos!=NULL)
	{
		pLayer=m_HMLayerList.GetNext(pos);
		ASSERT(pLayer!=NULL);
		if (pLayer!=NULL)
			pLayer->ParamReplace(ruleList);
	}
}

BOOL CHuaMian::updateEntityInAHuamian( char *pRealData, int nCount )
{
	//变电站实时消息结构
	/*
	typedef struct _push_data_response
	{
		int nRvuId;
		int nSmId;
		int nNodeId;
		float fValue;
		int nType;
		int nStatus;
	}PUSH_DATA_RESPONSE;
	*/

	//typedef std::vector<PUSH_DATA_RESPONSE*> _P_VECENTITYSTATUS;
	//_P_VECENTITYSTATUS vecEntityStatus;
	////get entity value.
	//for (int i=0; i<nCount; i++){
	//	PUSH_DATA_RESPONSE* stu_Data = pRealData[i];
	//	vecEntityStatus.push_back(stu_Data);
	//}
	PUSH_DATA_RESPONSE* stu_Data = (PUSH_DATA_RESPONSE*)(pRealData);
	if (stu_Data == NULL){
		return FALSE;
	}

	POSITION pos;
	CHMLayer * pLayer;
	pos=m_HMLayerList.GetHeadPosition();
	while (pos!=NULL)
	{

		pLayer=m_HMLayerList.GetNext(pos);
		ASSERT( pLayer !=NULL );
		if ( pLayer ==NULL ){
			continue;
		}

		CTyBase* pTy;
		POSITION posTybase;
		posTybase = pLayer->m_TyList.GetHeadPosition();
		while(posTybase !=NULL){
			//=======
			pTy = pLayer->m_TyList.GetNext(posTybase);
			
			CYX* pYX = dynamic_cast<CYX*>(pTy);
			if ( pYX !=NULL ){
				//========
				CString strEname;
				strEname.Format("%s",pYX->m_yx.ename);
				
				int rvuID(0);
				int smID(0);

				CString strSceneID;
				if (!CFileRead::getSceneIDBySceneName( m_strName, strSceneID ) ){
					TRACE0("get scene id failed.\n");
					continue;
				}

				if (!CFileRead::getRvuIDAndSmIDBySceneAndEname(strSceneID,strEname,rvuID,smID)){
					TRACE0("read yx rvu_id sm_id failed.\n");
					continue;
				}
				
				for (int i=0; i<nCount; i++){
					if ( (stu_Data[i].nRvuId ==rvuID) && (stu_Data[i].nSmId ==smID) ){
						pYX->setStatus(stu_Data[i].nStatus);
					}
				}
				//======
			}
			else{
				continue;
			}
			//=======
		}

	}

	return TRUE;
}
