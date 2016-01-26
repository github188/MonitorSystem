/*
	MFC Tree-controlÊ÷¿Ø¼þµÄ²Ù×÷
*/
#ifndef _ASSTREEOPERATE_H_
#define _ASSTREEOPERATE_H_

std::vector<HTREEITEM> GetCheckItem(HTREEITEM hItem);
void GlobalInitStationTree(CTreeCtrl *pTreeCtrl);
void GlobalSetCameraNodeInfo(CTreeCtrl *pTreeCtrl, HTREEITEM hItem, char* ItemName, char* Itemnum, int StationOrCameraID,int ItemStatus,int Streamless,float ItemLongitude, float ItemLatitude, int ItemDecodeTag, int Itemtype, char* stationname);
int GlobalCheckStationCurrentStatus2(int nStationId);
void GlobalDeleteCameraChildTreeListInfo(CTreeCtrl *pTreeCtrl, HTREEITEM hItem);
void GlobalDeleteCameraTreeList(CTreeCtrl *pTreeCtrl);
#endif
