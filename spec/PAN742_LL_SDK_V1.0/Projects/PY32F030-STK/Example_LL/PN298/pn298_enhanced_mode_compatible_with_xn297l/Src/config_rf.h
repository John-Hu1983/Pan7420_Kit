/*
*********************************************************************************************************
*	Copyright (C), ����΢���� www.panchip.com
*	�ļ��� : config_pn298.h
*	��  �� : ������
*	˵  �� : ����PN298
*	��  �� : ��
*   ��  ־ ������ 2021/02/03 �������½� 
*	��  �� : V1.0
*********************************************************************************************************
*/
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
#ifndef __CONFIG_PN298_H
#define __CONFIG_PN298_H


/******************************************************************************/
//<h>RF����
//  <o>PAYLOAD <1-64>
#define __PN298_PAYLOAD             7

//  <o>��ʼͨ��(Ƶ��) <0-255>
#define __PN298_INIT_CHN             36

//    <h>��ʼ��ַ
//    <o>��ַ���� <3=>3�ֽ� <4=>4�ֽ� <5=>5�ֽ�
#define __PN298_INIT_ADDR_LEN        5
//    <o0>��ַ��1�ֽ� <0-255>
//    <o1>��ַ��2�ֽ� <0-255>
//    <o2>��ַ��3�ֽ� <0-255>
//    <o3>��ַ��4�ֽ� <0-255>
//    <o4>��ַ��5�ֽ� <0-255>
#define __PN298_INIT_ADDR_BYTE0      0xCC
#define __PN298_INIT_ADDR_BYTE1      0xCC
#define __PN298_INIT_ADDR_BYTE2      0xCC
#define __PN298_INIT_ADDR_BYTE3      0xCC
#define __PN298_INIT_ADDR_BYTE4      0xCC
#if   __PN298_INIT_ADDR_LEN == 3
    #define __PN298_INIT_ADDR        {__PN298_INIT_ADDR_BYTE0,__PN298_INIT_ADDR_BYTE1,__PN298_INIT_ADDR_BYTE2}
#elif __PN298_INIT_ADDR_LEN == 4                                                  
    #define __PN298_INIT_ADDR        {__PN298_INIT_ADDR_BYTE0,__PN298_INIT_ADDR_BYTE1,__PN298_INIT_ADDR_BYTE2,__PN298_INIT_ADDR_BYTE3}
#elif __PN298_INIT_ADDR_LEN == 5                                                                       
    #define __PN298_INIT_ADDR        {__PN298_INIT_ADDR_BYTE0,__PN298_INIT_ADDR_BYTE1,__PN298_INIT_ADDR_BYTE2,__PN298_INIT_ADDR_BYTE3,__PN298_INIT_ADDR_BYTE4}
#else
    #error rfͨ�ŵ�ַ���ȴ���
#endif
//  </h>

//  <o>RFģʽ
//	<0x00=>xn297L��ͨ��ģʽ 
//	<0x01=>xn297L��ǿ��ģʽ
//	<0x10=>bleģʽ 
//	<0x20=>nordic��ͨ��ģʽ
//	<0x21=>nordic��ǿ��ģʽ
#define __PN298_MODE                0x0001
	
//  <o>ͨ������ <0x00=>1M <0xC0=>250K
#define __PN298_RATE                 0x00

//  <o>PN298���� 
//	<0xc9ca=>10dBm
//	<0xc9c2=>6dBm 
//	<0x49c6=>3dBm
//	<0x49c2=>2Bm 
//	<0xc9c5=>1dBm
//	<0xc9c1=>0dBm
//	<0x49c1=>-8dBm	
#define __PN298_POWER                0xC9C2
	

//</h>

#endif //__CONFIG_PN298_H

