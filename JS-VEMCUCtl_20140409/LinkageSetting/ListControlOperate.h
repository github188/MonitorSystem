#ifndef _LISTCONTROLOPERATE_H
#define _LISTCONTROLOPERATE_H

void InitDeviceLedgerListColumn(CListCtrl *list);
void AddItemToDeviceLedgerList(CListCtrl *list, int nIndex, _T_ASS_DEVICE_LEDGER_* pDeviceLedger);
void FreeDeviceLedgerList(CListCtrl *list);

#endif

