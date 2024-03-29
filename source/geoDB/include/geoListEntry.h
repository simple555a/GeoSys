/*****************************************************************************
*  GeoSysWare	Basic tool library											 *
*  Copyright (C) 2016 George.Kuo  guooujie@163.com.							 *
*                                                                            *
*  This file is part of GeoSys.						                         *
*                                                                            *
*  @file     geoListEntry.h                                            *
*  @brief    宏定义的链表，直接引用指针串接结构体                              *
*																			 *
*
*
*  @author   George.Kuo                                                      *
*  @email    guooujie@163.com												 *
*  @version  1.0.0.1(版本号)                                                 *
*  @date     2016.8															 *
*  @license																	 *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : 必须在支持C++11环境下编译通过                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2016/10/24 | 1.0.0.1   | George.Kuo      | Create file                    *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#ifndef  GEOSYS_OS_LISTENTRY_H__
#define  GEOSYS_OS_LISTENTRY_H__


//
//	Singly linked list structure. Can be used as either a list head, or
//	as link words.
//

typedef struct _RTK_SINGLE_LIST_ENTRY {
	struct _RTK_SINGLE_LIST_ENTRY *Next;
} RTK_SINGLE_LIST_ENTRY, *PRTK_SINGLE_LIST_ENTRY;

//
//	Doubly linked list structure.  Can be used as either a list head, or
//	as link words.
//

typedef struct _RTK_LIST_ENTRY {
	struct _RTK_LIST_ENTRY *Flink;
	struct _RTK_LIST_ENTRY *Blink;
}RTK_LIST_ENTRY, *PRTK_LIST_ENTRY;

#ifdef RTK_FIELD_OFFSET
#undef RTK_FIELD_OFFSET
#endif
//
// Calculate the byte offset of a field in a structure of type type.
//

#define RTK_FIELD_OFFSET(type, field)	 ((int)(int*)&(((type *)0)->field))

#ifndef RTK_CONTAINING_RECORD
#define RTK_CONTAINING_RECORD(address, type, field) \
	((type *)( (char *)(address) - RTK_FIELD_OFFSET(type, field)))
#endif

//
// Calculate the address of the base of the structure given its type, and an
// address of a field within the structure.
//

#define SAFE_CONTAINING_RECORD(address, type, field) \
((address)? ((type *)( (char *)(address) - RTK_FIELD_OFFSET(type, field))) : 0)
//
//	Doubly-linked list manipulation routines.  Implemented as macros
//	but logically these are procedures.
//

//
//	VOID
//	InitializeListHead(
//		PRTK_LIST_ENTRY ListHead
//		);
//

#define RtkInitializeListHead(ListHead) (\
	(ListHead)->Flink = (ListHead)->Blink = (ListHead))


#define RtkMergeList(List1, List2) {\
	PRTK_LIST_ENTRY Tail_1;\
	PRTK_LIST_ENTRY Tail_2;\
	Tail_1 = (List1)->Blink;\
	Tail_2 = (List2)->Blink;\
	Tail_1->Flink = (List2);\
	(List2)->Blink = Tail_1;\
	Tail_2->Flink = (List1);\
	(List1)->Blink = Tail_2;\
	}


//
//	BOOLEAN
//	IsListEmpty(
//		PRTK_LIST_ENTRY ListHead
//		);
//

#define RtkIsListEmpty(ListHead) \
	((ListHead)->Flink == (ListHead))

//
//	PRTK_LIST_ENTRY
//	RemoveHeadList(
//		PRTK_LIST_ENTRY ListHead
//		);
//

#define RtkRemoveHeadList(ListHead) \
	(ListHead)->Flink;\
	{RtkRemoveEntryList((ListHead)->Flink)}

//
//	PRTK_LIST_ENTRY
//	RemoveTailList(
//		PRTK_LIST_ENTRY ListHead
//		);
//

#define RtkRemoveTailList(ListHead) \
	(ListHead)->Blink;\
	{RtkRemoveEntryList((ListHead)->Blink)}

//
//	VOID
//	RemoveEntryList(
//		PRTK_LIST_ENTRY Entry
//		);
//

#define RtkRemoveEntryList(Entry) {\
	PRTK_LIST_ENTRY _EX_Blink;\
	PRTK_LIST_ENTRY _EX_Flink;\
	_EX_Flink = (Entry)->Flink;\
	_EX_Blink = (Entry)->Blink;\
	_EX_Blink->Flink = _EX_Flink;\
	_EX_Flink->Blink = _EX_Blink;\
	}

//
//	VOID
//	InsertTailList(
//		PRTK_LIST_ENTRY ListHead,
//		PRTK_LIST_ENTRY Entry
//		);
//

#define RtkInsertTailList(ListHead,Entry) {\
	PRTK_LIST_ENTRY _EX_Blink;\
	PRTK_LIST_ENTRY _EX_ListHead;\
	_EX_ListHead = (ListHead);\
	_EX_Blink = _EX_ListHead->Blink;\
	(Entry)->Flink = _EX_ListHead;\
	(Entry)->Blink = _EX_Blink;\
	_EX_Blink->Flink = (Entry);\
	_EX_ListHead->Blink = (Entry);\
	}

//
//	VOID
//	InsertHeadList(
//		PRTK_LIST_ENTRY ListHead,
//		PRTK_LIST_ENTRY Entry
//		);
//

#define RtkInsertHeadList(ListHead,Entry) {\
	PRTK_LIST_ENTRY _EX_Flink;\
	PRTK_LIST_ENTRY _EX_ListHead;\
	_EX_ListHead = (ListHead);\
	_EX_Flink = _EX_ListHead->Flink;\
	(Entry)->Flink = _EX_Flink;\
	(Entry)->Blink = _EX_ListHead;\
	_EX_Flink->Blink = (Entry);\
	_EX_ListHead->Flink = (Entry);\
	}


//
//
//	PRTK_SINGLE_LIST_ENTRY
//	PopEntryList(
//		PRTK_SINGLE_LIST_ENTRY ListHead
//		);
//

#define RtkPopEntryList(ListHead) \
	(ListHead)->Next;\
	{\
		PRTK_SINGLE_LIST_ENTRY FirstEntry;\
		FirstEntry = (ListHead)->Next;\
		if (FirstEntry != NULL) {	  \
			(ListHead)->Next = FirstEntry->Next;\
		}							  \
	}


//
//	VOID
//	PushEntryList(
//		PRTK_SINGLE_LIST_ENTRY ListHead,
//		PRTK_SINGLE_LIST_ENTRY Entry
//		);
//

#define RtkPushEntryList(ListHead,Entry) \
	(Entry)->Next = (ListHead)->Next; \
	(ListHead)->Next = (Entry)

//
//
//
//
//					   Triply List Entry
// ---------------------------------------------------------------------------
//
//
//

//	Triply linked list structure.  Can be used as either a list head, or
//	as link words.
//

typedef struct _TRIPLE_LIST_ENTRY {
	struct _TRIPLE_LIST_ENTRY *Flink;
	struct _TRIPLE_LIST_ENTRY *Blink;
	struct _TRIPLE_LIST_ENTRY *Head;
}TRIPLE_LIST_ENTRY, *PTRIPLE_LIST_ENTRY;

//
//	Triply-linked list manipulation routines.  Implemented as macros
//	but logically these are procedures.
//

//
//	VOID
//	InitializeTripleListHead(
//		PTRIPLE_LIST_ENTRY ListHead
//		);
//

#define RtkInitializeTripleListHead(TripleListHead) (\
	(TripleListHead)->Flink = (TripleListHead)->Blink = (TripleListHead)->Head = (TripleListHead))

//
//	BOOLEAN
//	IsTripleListEmpty(
//		PRTK_LIST_ENTRY TripleListHead
//		);
//

#define RtkIsTripleListEmpty(TripleListHead) \
	((TripleListHead)->Flink == (TripleListHead))

//
//	PRTK_LIST_ENTRY
//	RemoveHeadTripleList(
//		PRTK_LIST_ENTRY TripleListHead
//		);
//

#define RtkRemoveHeadTripleList(TripleListHead) \
	(TripleListHead)->Flink;\
	{RtkRemoveEntryTripleList((TripleListHead)->Flink)}

//
//	PRTK_LIST_ENTRY
//	RemoveTailTripleList(
//		PTRIPLE_LIST_ENTRY TripleListHead
//		);
//

#define RtkRemoveTailTripleList(TripleListHead) \
	(TripleListHead)->Blink;\
	{RtkRemoveEntryTripleList((TripleListHead)->Blink)}

//
//	VOID
//	RemoveEntryTripleList(
//		PTRIPLE_LIST_ENTRY Entry
//		);
//
//you cannot remove the head entry once initialzed

#define RtkRemoveEntryTripleList(Entry) {\
	if( (Entry) != (Entry)->Head){\
		PTRIPLE_LIST_ENTRY _EX_Blink;\
		PTRIPLE_LIST_ENTRY _EX_Flink;\
		_EX_Flink = (Entry)->Flink;\
		_EX_Blink = (Entry)->Blink;\
		_EX_Blink->Flink = _EX_Flink;\
		_EX_Flink->Blink = _EX_Blink;\
		(Entry)->Head = (Entry);\
		(Entry)->Flink = (Entry);\
		(Entry)->Blink = (Entry);\
	}\
}

//
//	VOID
//	InsertTailTripleList(
//		PTRIPLE_LIST_ENTRY TripleListHead,
//		PTRIPLE_LIST_ENTRY Entry
//		);
//

#define RtkInsertTailTripleList(TripleListHead,Entry) {\
	PTRIPLE_LIST_ENTRY _EX_Tail;\
	_EX_Tail = (TripleListHead)->Blink;\
	(Entry)->Flink = (TripleListHead);\
	(Entry)->Blink = _EX_Tail;\
	_EX_Tail->Flink = (Entry);\
	(TripleListHead)->Blink = (Entry);\
	(Entry)->Head = (TripleListHead);\
	}

//
//	VOID
//	InsertHeadTripleList(
//		PTRIPLE_LIST_ENTRY TripleListHead,
//		PTRIPLE_LIST_ENTRY Entry
//		);

//
// VOID
// PivotHeadTripleList(
//	PTRIPLE_LIST_ENTRY NewHead
// );

#endif

