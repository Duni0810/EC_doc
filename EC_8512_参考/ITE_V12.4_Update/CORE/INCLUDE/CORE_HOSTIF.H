/*-----------------------------------------------------------------------------
 * TITLE: CORE_HOSTIF.H - Header File for Host Interface Handler
 *
 * The manufacturer identification (uses Translated key codes) and the
 * controller type.
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/
#ifndef CORE_HOSTIF_H
#define CORE_HOSTIF_H

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern void Service_SEND(void);
extern void Service_Send_PS2(void);

extern BYTE KBC_DataToHost(BYTE nKBData);
extern void KBC_DataPending(BYTE nPending);
extern BYTE GetKB_PendingData(void);
extern void Data_To_Host(BYTE data_byte);
extern void Aux_Data_To_Host(BYTE data_byte);
extern void InitSio(void);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
static BYTE send_to_pc(BYTE data_word, BYTE break_prefix_flag);
static BYTE common_send_to_pc(BYTE data_word, BYTE break_prefix_flag);
static BYTE translate_to_pc(BYTE data_word, BYTE break_prefix_flag);


//-----------------------------------------------------------------------------
#endif
