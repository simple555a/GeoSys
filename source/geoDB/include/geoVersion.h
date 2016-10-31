

/*****************************************************************************
*  GeoSysWare	Basic tool library											 *
*  Copyright (C) 2016 George.Kuo  guooujie@163.com.							 *
*                                                                            *
*  This file is part of GeoSys.							                   *
*                                                                            *
*  @file     geoVersion.h                                                     *
*  @brief    �̴߳�����                                                      *
*																			 *
*  GeoSys�ı���汾
*
*  @author   George.Kuo                                                      *
*  @email    guooujie@163.com												 *
*  @version  1.0.0.1(�汾��)                                                 *
*  @date     2016.8															 *
*  @license																	 *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : ������֧��C++11�����±���ͨ��                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2016/08/24 | 1.0.0.1   | George.Kuo      | Create file                    *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#ifndef GEOSYS_VERSIONED_H
#define GEOSYS_VERSIONED_H


#define GEOSYS_MAJOR_VERSION 1
#define GEOSYS_MINOR_VERSION 0
#define GEOSYS_BETA_VERSION  1
#define GEOSYS_VERSION_STR "1.0.1"
#define GEOSYS_VERSION 0x010001



#define GEODB_MAJOR_VERSION 1
#define GEODB_MINOR_VERSION 0
#define GEODB_BETA_VERSION  1

#define GEODB_VERSION_STR "1.0.1"

#define GEODB_VERSION 0x010001



/**
*@brief NICSYS��ǰ�İ汾�Ƚ�
���Բ����� #if (GEOSYS_VERSION >= GEOSYS_VERSION_CHECK(4, 4, 0))
*/
#define GEOSYS_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

/**
*@brief GEOSYS������ʵʱ�ಥ���߰汾
*/
#define GEOSYS_VBUS_VERSION 0x0201

#define STR_VBUS_VERSION "2.1"

#endif