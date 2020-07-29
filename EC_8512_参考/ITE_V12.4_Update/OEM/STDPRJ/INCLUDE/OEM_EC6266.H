/*-----------------------------------------------------------------------------
 * Filename: OEM_EC6266.H
 * Function: OEM_EC6266.C Include Definition
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
#ifndef OEM_EC6266_H
#define OEM_EC6266_H

/*-----------------------------------------------------------------------------
 * External Calling Process
 *---------------------------------------------------------------------------*/
extern BYTE Read_MapECSpace( BYTE MapIndex );
extern void Write_MapECSpace( BYTE MapIndex,BYTE data1 );
extern BYTE OEM_GetPort62Data(void);
extern void Read_External_Ram(void);
extern void Write_External_Ram(void);

/*-----------------------------------------------------------------------------
 * Internal Process Definition
 *---------------------------------------------------------------------------*/
void EC_Cmd_0X(void);
void EC_Cmd_1X(void);
void EC_Cmd_2X(void);
void EC_Cmd_3X(void);
void EC_Cmd_4X(void);
void EC_Cmd_5X(void);
void EC_Cmd_6X(void);
void EC_Cmd_7X(void);
void EC_Cmd_9X(void);
void EC_Cmd_AX(void);
void EC_Cmd_BX(void);
void EC_Cmd_CX(void);
void EC_Cmd_DX(void);
void EC_Cmd_EX(void);
void EC_Cmd_FX(void);

void EC6266_CMD_85(void);
void EC6266_CMD_86(void);
void EC6266_CMD_87(void);
void EC6266_CMD_88(void);
void EC6266_CMD_89(void);
void EC6266_CMD_8A(void);
void EC6266_CMD_8B(void);
void EC6266_CMD_8C(void);
void EC6266_CMD_8D(void);
void EC6266_CMD_8E(void);
void EC6266_CMD_8F(void);

void EC62_DATA_STEP0(void);
void EC62_DATA_STEP4(void);
void EC62_DATA_STEP5(void);
void EC62_DATA_STEP6(void);
void EC62_DATA_STEP7(void);


void EC6266_CMD_90(void);
void EC6266_CMD_91(void);
void EC6266_CMD_92(void);
void EC6266_CMD_93(void);
void EC6266_CMD_94(void);
void EC6266_CMD_95(void);
void EC6266_CMD_96(void);
void EC6266_CMD_97(void);
void EC6266_CMD_98(void);
void EC6266_CMD_99(void);
void EC6266_CMD_9A(void);
void EC6266_CMD_9B(void);
void EC6266_CMD_9C(void);
void EC6266_CMD_9D(void);
void EC6266_CMD_9E(void);
void EC6266_CMD_9F(void);

void EC6266_CMD_A0(void);
void EC6266_CMD_A1(void);
void EC6266_CMD_A2(void);
void EC6266_CMD_A3(void);
void EC6266_CMD_A4(void);
void EC6266_CMD_A5(void);
void EC6266_CMD_A6(void);
void EC6266_CMD_A7(void);
void EC6266_CMD_A8(void);
void EC6266_CMD_A9(void);
void EC6266_CMD_AA(void);
void EC6266_CMD_AB(void);
void EC6266_CMD_AC(void);
void EC6266_CMD_AD(void);
void EC6266_CMD_AE(void);
void EC6266_CMD_AF(void);

void EC6266_CMD_B0(void);
void EC6266_CMD_B1(void);
void EC6266_CMD_B2(void);
void EC6266_CMD_B3(void);
void EC6266_CMD_B4(void);
void EC6266_CMD_B5(void);
void EC6266_CMD_B6(void);
void EC6266_CMD_B7(void);
void EC6266_CMD_B8(void);
void EC6266_CMD_B9(void);
void EC6266_CMD_BA(void);
void EC6266_CMD_BB(void);
void EC6266_CMD_BC(void);
void EC6266_CMD_BD(void);
void EC6266_CMD_BE(void);
void EC6266_CMD_BF(void);

void EC6266_CMD_C0(void);
void EC6266_CMD_C1(void);
void EC6266_CMD_C2(void);
void EC6266_CMD_C3(void);
void EC6266_CMD_C4(void);
void EC6266_CMD_C5(void);
void EC6266_CMD_C6(void);
void EC6266_CMD_C7(void);
void EC6266_CMD_C8(void);
void EC6266_CMD_C9(void);
void EC6266_CMD_CA(void);
void EC6266_CMD_CB(void);
void EC6266_CMD_CC(void);
void EC6266_CMD_CD(void);
void EC6266_CMD_CE(void);
void EC6266_CMD_CF(void);

void EC6266_CMD_D0(void);
void EC6266_CMD_D1(void);
void EC6266_CMD_D2(void);
void EC6266_CMD_D3(void);
void EC6266_CMD_D4(void);
void EC6266_CMD_D5(void);
void EC6266_CMD_D6(void);
void EC6266_CMD_D7(void);
void EC6266_CMD_D8(void);
void EC6266_CMD_D9(void);
void EC6266_CMD_DA(void);
void EC6266_CMD_DB(void);
void EC6266_CMD_DC(void);
void EC6266_CMD_DD(void);
void EC6266_CMD_DE(void);
void EC6266_CMD_DF(void);

void EC6266_CMD_E0(void);
void EC6266_CMD_E1(void);
void EC6266_CMD_E2(void);
void EC6266_CMD_E3(void);
void EC6266_CMD_E4(void);
void EC6266_CMD_E5(void);
void EC6266_CMD_E6(void);
void EC6266_CMD_E7(void);
void EC6266_CMD_E8(void);
void EC6266_CMD_E9(void);
void EC6266_CMD_EA(void);
void EC6266_CMD_EB(void);
void EC6266_CMD_EC(void);
void EC6266_CMD_ED(void);
void EC6266_CMD_EE(void);
void EC6266_CMD_EF(void);

void EC6266_CMD_F0(void);
void EC6266_CMD_F1(void);
void EC6266_CMD_F2(void);
void EC6266_CMD_F3(void);
void EC6266_CMD_F4(void);
void EC6266_CMD_F5(void);
void EC6266_CMD_F6(void);
void EC6266_CMD_F7(void);
void EC6266_CMD_F8(void);
void EC6266_CMD_F9(void);
void EC6266_CMD_FA(void);
void EC6266_CMD_FB(void);
void EC6266_CMD_FC(void);
void EC6266_CMD_FD(void);
void EC6266_CMD_FE(void);
void EC6266_CMD_FF(void);

//-----------------------------------------------------------------------------
#endif

