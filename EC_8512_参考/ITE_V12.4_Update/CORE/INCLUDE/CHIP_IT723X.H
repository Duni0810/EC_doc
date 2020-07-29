/*-----------------------------------------------------------------------------
 * Filename: CHIP_IT7230.H     For Chipset: ITE.IT7230
 *
 * Function: Register Definition for all code base reference
 *
 * Copyright(c) 2008-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Parameter & Register Defination
//-----------------------------------------------------------------------------
#define IT723X_ADDR         0x8C
#define IT723X_PAGE_0       0x00    // Define page number 0
#define IT723X_PAGE_1       0x01    // Define page number 1
#define IT723X_PAGE_2       0x02    // Define page number 2
//-----------------------------------------------------------------------------
#define IT723XP0_PSR        0x00    //Page Setting Register
#define IT723XP0_SIR        0x01    //Sensor Information Register
#define IT723XP0_SXPOSR     0x02    //Stage X Proximity On Status Register
#define IT723XP0_SXCHSR     0x03    //Stage X Contact High Status Register
#define IT723XP0_SXCLSR     0x04    //Stage X Contact Low Status Register
#define IT723XP0_SXDSR      0x05    //Stage X Drift Status Register
#define IT723XP0_SXCSR      0x06    //Stage X Complete Status Register
#define IT723XP0_SLR        0x07    //Slider Level Register
//-----------------------------------------------------------------------------
#define IT723XP0_ISR        0x08    //Interrupt Status Register
#define IT723XP0_SXCHAIER   0x09    //Stage X Contact High Assert Interrupt Enable Register
#define IT723XP0_SXCHRIER   0x0A    //Stage X Contact High Release Interrupt Enable Register
#define IT723XP0_SXCLAIER   0x0B    //Stage X Contact Low Assert Interrupt Enable Register
#define IT723XP0_SXCLRIER   0x0C    //Stage X Contact Low Release Interrupt Enable Register
#define IT723XP0_SXCIER     0x0D    //Stage X Conversion Interrupt Enable Register
//-----------------------------------------------------------------------------
#define IT723XP0_S0CCDCVR   0x20    //Stage 0 Conversion CDC Value Register
#define IT723XP0_S1CCDCVR   0x21    //Stage 1 Conversion CDC Value Register
#define IT723XP0_S2CCDCVR   0x22    //Stage 2 Conversion CDC Value Register
#define IT723XP0_S3CCDCVR   0x23    //Stage 3 Conversion CDC Value Register
#define IT723XP0_S4CCDCVR   0x24    //Stage 4 Conversion CDC Value Register
#define IT723XP0_S5CCDCVR   0x25    //Stage 5 Conversion CDC Value Register
#define IT723XP0_S6CCDCVR   0x26    //Stage 6 Conversion CDC Value Register
#define IT723XP0_S7CCDCVR   0x27    //Stage 7 Conversion CDC Value Register
#define IT723XP0_S8CCDCVR   0x28    //Stage 8 Conversion CDC Value Register
#define IT723XP0_S9CCDCVR   0x29    //Stage 9 Conversion CDC Value Register
#define IT723XP0_S10CCDCVR  0x2A    //Stage 10 Conversion CDC Value Register
#define IT723XP0_S11CCDCVR  0x2B    //Stage 11 Conversion CDC Value Register
#define IT723XP0_S12CCDCVR  0x2C    //Stage 12 Conversion CDC Value Register
//-----------------------------------------------------------------------------
#define IT723XP0_S0FR0      0x30    //Stage 0 FIFO Register 0
#define IT723XP0_S0FR1      0x31    //Stage 0 FIFO Register 1
#define IT723XP0_S0FR2      0x32    //Stage 0 FIFO Register 2
#define IT723XP0_S0DLR      0x33    //Stage 0 Device Level Register
#define IT723XP0_S0CVR      0x34    //Stage 0 Compensation Value Register
#define IT723XP0_S0OHCR     0x35    //Stage 0 Offset High Clamp Register
#define IT723XP0_S0OLCR     0x36    //Stage 0 Offset Low Clamp Register
#define IT723XP0_S0OHR      0x37    //Stage 0 Offset High Register
#define IT723XP0_S0OLR      0x38    //Stage 0 Offset Low Register
#define IT723XP0_S0OHUR     0x39    //Stage 0 Offset High Updated Register
#define IT723XP0_S0OLUCR    0x3A    //Stage 0 Offset Low Updated Register
#define IT723XP0_S0HTR      0x3B    //Stage 0 High Threshold Register
#define IT723XP0_S0LTR      0x3C    //Stage 0 Low Threshold Register
#define IT723XP0_S0SR       0x3D    //Stage 0 Sensitivity Register
#define IT723XP0_S0CDC_CMP  0x3E    //Stage 0 Compensated CDC Value Register
#define IT723XP0_S0CSR      0x3F    //Stage 0 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S1FR0      0x40    //Stage 1 FIFO Register 0
#define IT723XP0_S1FR1      0x41    //Stage 1 FIFO Register 1
#define IT723XP0_S1FR2      0x42    //Stage 1 FIFO Register 2
#define IT723XP0_S1DLR      0x43    //Stage 1 Device Level Register
#define IT723XP0_S1CVR      0x44    //Stage 1 Compensation Value Register
#define IT723XP0_S1OHCR     0x45    //Stage 1 Offset High Clamp Register
#define IT723XP0_S1OLCR     0x46    //Stage 1 Offset Low Clamp Register
#define IT723XP0_S1OHR      0x47    //Stage 1 Offset High Register
#define IT723XP0_S1OLR      0x48    //Stage 1 Offset Low Register
#define IT723XP0_S1OHUR     0x49    //Stage 1 Offset High Updated Register
#define IT723XP0_S1OLUCR    0x4A    //Stage 1 Offset Low Updated Register
#define IT723XP0_S1HTR      0x4B    //Stage 1 High Threshold Register
#define IT723XP0_S1LTR      0x4C    //Stage 1 Low Threshold Register
#define IT723XP0_S1SR       0x4D    //Stage 1 Sensitivity Register
#define IT723XP0_S1CDC_CMP  0x4E    //Stage 1 Compensated CDC Value Register
#define IT723XP0_S1CSR      0x4F    //Stage 1 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S2FR0      0x50    //Stage 2 FIFO Register 0
#define IT723XP0_S2FR1      0x51    //Stage 2 FIFO Register 1
#define IT723XP0_S2FR2      0x52    //Stage 2 FIFO Register 2
#define IT723XP0_S2DLR      0x53    //Stage 2 Device Level Register
#define IT723XP0_S2CVR      0x54    //Stage 2 Compensation Value Register
#define IT723XP0_S2OHCR     0x55    //Stage 2 Offset High Clamp Register
#define IT723XP0_S2OLCR     0x56    //Stage 2 Offset Low Clamp Register
#define IT723XP0_S2OHR      0x57    //Stage 2 Offset High Register
#define IT723XP0_S2OLR      0x58    //Stage 2 Offset Low Register
#define IT723XP0_S2OHUR     0x59    //Stage 2 Offset High Updated Register
#define IT723XP0_S2OLUCR    0x5A    //Stage 2 Offset Low Updated Register
#define IT723XP0_S2HTR      0x5B    //Stage 2 High Threshold Register
#define IT723XP0_S2LTR      0x5C    //Stage 2 Low Threshold Register
#define IT723XP0_S2SR       0x5D    //Stage 2 Sensitivity Register
#define IT723XP0_S2CDC_CMP  0x5E    //Stage 2 Compensated CDC Value Register
#define IT723XP0_S2CSR      0x5F    //Stage 2 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S3FR0      0x60    //Stage 3 FIFO Register 0
#define IT723XP0_S3FR1      0x61    //Stage 3 FIFO Register 1
#define IT723XP0_S3FR2      0x62    //Stage 3 FIFO Register 2
#define IT723XP0_S3DLR      0x63    //Stage 3 Device Level Register
#define IT723XP0_S3CVR      0x64    //Stage 3 Compensation Value Register
#define IT723XP0_S3OHCR     0x65    //Stage 3 Offset High Clamp Register
#define IT723XP0_S3OLCR     0x66    //Stage 3 Offset Low Clamp Register
#define IT723XP0_S3OHR      0x67    //Stage 3 Offset High Register
#define IT723XP0_S3OLR      0x68    //Stage 3 Offset Low Register
#define IT723XP0_S3OHUR     0x69    //Stage 3 Offset High Updated Register
#define IT723XP0_S3OLUCR    0x6A    //Stage 3 Offset Low Updated Register
#define IT723XP0_S3HTR      0x6B    //Stage 3 High Threshold Register
#define IT723XP0_S3LTR      0x6C    //Stage 3 Low Threshold Register
#define IT723XP0_S3SR       0x6D    //Stage 3 Sensitivity Register
#define IT723XP0_S3CDC_CMP  0x6E    //Stage 3 Compensated CDC Value Register
#define IT723XP0_S3CSR      0x6F    //Stage 3 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S4FR0      0x70    //Stage 4 FIFO Register 0
#define IT723XP0_S4FR1      0x71    //Stage 4 FIFO Register 1
#define IT723XP0_S4FR2      0x72    //Stage 4 FIFO Register 2
#define IT723XP0_S4DLR      0x73    //Stage 4 Device Level Register
#define IT723XP0_S4CVR      0x74    //Stage 4 Compensation Value Register
#define IT723XP0_S4OHCR     0x75    //Stage 4 Offset High Clamp Register
#define IT723XP0_S4OLCR     0x76    //Stage 4 Offset Low Clamp Register
#define IT723XP0_S4OHR      0x77    //Stage 4 Offset High Register
#define IT723XP0_S4OLR      0x78    //Stage 4 Offset Low Register
#define IT723XP0_S4OHUR     0x79    //Stage 4 Offset High Updated Register
#define IT723XP0_S4OLUCR    0x7A    //Stage 4 Offset Low Updated Register
#define IT723XP0_S4HTR      0x7B    //Stage 4 High Threshold Register
#define IT723XP0_S4LTR      0x7C    //Stage 4 Low Threshold Register
#define IT723XP0_S4SR       0x7D    //Stage 4 Sensitivity Register
#define IT723XP0_S4CDC_CMP  0x7E    //Stage 4 Compensated CDC Value Register
#define IT723XP0_S4CSR      0x7F    //Stage 4 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S5FR0      0x80    //Stage 5 FIFO Register 0
#define IT723XP0_S5FR1      0x81    //Stage 5 FIFO Register 1
#define IT723XP0_S5FR2      0x82    //Stage 5 FIFO Register 2
#define IT723XP0_S5DLR      0x83    //Stage 5 Device Level Register
#define IT723XP0_S5CVR      0x84    //Stage 5 Compensation Value Register
#define IT723XP0_S5OHCR     0x85    //Stage 5 Offset High Clamp Register
#define IT723XP0_S5OLCR     0x86    //Stage 5 Offset Low Clamp Register
#define IT723XP0_S5OHR      0x87    //Stage 5 Offset High Register
#define IT723XP0_S5OLR      0x88    //Stage 5 Offset Low Register
#define IT723XP0_S5OHUR     0x89    //Stage 5 Offset High Updated Register
#define IT723XP0_S5OLUCR    0x8A    //Stage 5 Offset Low Updated Register
#define IT723XP0_S5HTR      0x8B    //Stage 5 High Threshold Register
#define IT723XP0_S5LTR      0x8C    //Stage 5 Low Threshold Register
#define IT723XP0_S5SR       0x8D    //Stage 5 Sensitivity Register
#define IT723XP0_S5CDC_CMP  0x8E    //Stage 5 Compensated CDC Value Register
#define IT723XP0_S5CSR      0x8F    //Stage 5 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S6FR0      0x90    //Stage 6 FIFO Register 0
#define IT723XP0_S6FR1      0x91    //Stage 6 FIFO Register 1
#define IT723XP0_S6FR2      0x92    //Stage 6 FIFO Register 2
#define IT723XP0_S6DLR      0x93    //Stage 6 Device Level Register
#define IT723XP0_S6CVR      0x94    //Stage 6 Compensation Value Register
#define IT723XP0_S6OHCR     0x95    //Stage 6 Offset High Clamp Register
#define IT723XP0_S6OLCR     0x96    //Stage 6 Offset Low Clamp Register
#define IT723XP0_S6OHR      0x97    //Stage 6 Offset High Register
#define IT723XP0_S6OLR      0x98    //Stage 6 Offset Low Register
#define IT723XP0_S6OHUR     0x99    //Stage 6 Offset High Updated Register
#define IT723XP0_S6OLUCR    0x9A    //Stage 6 Offset Low Updated Register
#define IT723XP0_S6HTR      0x9B    //Stage 6 High Threshold Register
#define IT723XP0_S6LTR      0x9C    //Stage 6 Low Threshold Register
#define IT723XP0_S6SR       0x9D    //Stage 6 Sensitivity Register
#define IT723XP0_S6CDC_CMP  0x9E    //Stage 6 Compensated CDC Value Register
#define IT723XP0_S6CSR      0x9F    //Stage 6 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S7FR0      0xA0    //Stage 7 FIFO Register 0
#define IT723XP0_S7FR1      0xA1    //Stage 7 FIFO Register 1
#define IT723XP0_S7FR2      0xA2    //Stage 7 FIFO Register 2
#define IT723XP0_S7DLR      0xA3    //Stage 7 Device Level Register
#define IT723XP0_S7CVR      0xA4    //Stage 7 Compensation Value Register
#define IT723XP0_S7OHCR     0xA5    //Stage 7 Offset High Clamp Register
#define IT723XP0_S7OLCR     0xA6    //Stage 7 Offset Low Clamp Register
#define IT723XP0_S7OHR      0xA7    //Stage 7 Offset High Register
#define IT723XP0_S7OLR      0xA8    //Stage 7 Offset Low Register
#define IT723XP0_S7OHUR     0xA9    //Stage 7 Offset High Updated Register
#define IT723XP0_S7OLUCR    0xAA    //Stage 7 Offset Low Updated Register
#define IT723XP0_S7HTR      0xAB    //Stage 7 High Threshold Register
#define IT723XP0_S7LTR      0xAC    //Stage 7 Low Threshold Register
#define IT723XP0_S7SR       0xAD    //Stage 7 Sensitivity Register
#define IT723XP0_S7CDC_CMP  0xAE    //Stage 7 Compensated CDC Value Register
#define IT723XP0_S7CSR      0xAF    //Stage 7 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S8FR0      0xB0    //Stage 8 FIFO Register 0
#define IT723XP0_S8FR1      0xB1    //Stage 8 FIFO Register 1
#define IT723XP0_S8FR2      0xB2    //Stage 8 FIFO Register 2
#define IT723XP0_S8DLR      0xB3    //Stage 8 Device Level Register
#define IT723XP0_S8CVR      0xB4    //Stage 8 Compensation Value Register
#define IT723XP0_S8OHCR     0xB5    //Stage 8 Offset High Clamp Register
#define IT723XP0_S8OLCR     0xB6    //Stage 8 Offset Low Clamp Register
#define IT723XP0_S8OHR      0xB7    //Stage 8 Offset High Register
#define IT723XP0_S8OLR      0xB8    //Stage 8 Offset Low Register
#define IT723XP0_S8OHUR     0xB9    //Stage 8 Offset High Updated Register
#define IT723XP0_S8OLUCR    0xBA    //Stage 8 Offset Low Updated Register
#define IT723XP0_S8HTR      0xBB    //Stage 8 High Threshold Register
#define IT723XP0_S8LTR      0xBC    //Stage 8 Low Threshold Register
#define IT723XP0_S8SR       0xBD    //Stage 8 Sensitivity Register
#define IT723XP0_S8CDC_CMP  0xBE    //Stage 8 Compensated CDC Value Register
#define IT723XP0_S8CSR      0xBF    //Stage 8 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S9FR0      0xC0    //Stage 9 FIFO Register 0
#define IT723XP0_S9FR1      0xC1    //Stage 9 FIFO Register 1
#define IT723XP0_S9FR2      0xC2    //Stage 9 FIFO Register 2
#define IT723XP0_S9DLR      0xC3    //Stage 9 Device Level Register
#define IT723XP0_S9CVR      0xC4    //Stage 9 Compensation Value Register
#define IT723XP0_S9OHCR     0xC5    //Stage 9 Offset High Clamp Register
#define IT723XP0_S9OLCR     0xC6    //Stage 9 Offset Low Clamp Register
#define IT723XP0_S9OHR      0xC7    //Stage 9 Offset High Register
#define IT723XP0_S9OLR      0xC8    //Stage 9 Offset Low Register
#define IT723XP0_S9OHUR     0xC9    //Stage 9 Offset High Updated Register
#define IT723XP0_S9OLUCR    0xCA    //Stage 9 Offset Low Updated Register
#define IT723XP0_S9HTR      0xCB    //Stage 9 High Threshold Register
#define IT723XP0_S9LTR      0xCC    //Stage 9 Low Threshold Register
#define IT723XP0_S9SR       0xCD    //Stage 9 Sensitivity Register
#define IT723XP0_S9CDC_CMP  0xCE    //Stage 9 Compensated CDC Value Register
#define IT723XP0_S9CSR      0xCF    //Stage 9 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S10FR0      0xD0    //Stage 10 FIFO Register 0
#define IT723XP0_S10FR1     0xD1    //Stage 10 FIFO Register 1
#define IT723XP0_S10FR2     0xD2    //Stage 10 FIFO Register 2
#define IT723XP0_S10DLR     0xD3    //Stage 10 Device Level Register
#define IT723XP0_S10CVR     0xD4    //Stage 10 Compensation Value Register
#define IT723XP0_S10OHCR    0xD5    //Stage 10 Offset High Clamp Register
#define IT723XP0_S10OLCR    0xD6    //Stage 10 Offset Low Clamp Register
#define IT723XP0_S10OHR     0xD7    //Stage 10 Offset High Register
#define IT723XP0_S10OLR     0xD8    //Stage 10 Offset Low Register
#define IT723XP0_S10OHUR    0xD9    //Stage 10 Offset High Updated Register
#define IT723XP0_S10OLUCR   0xDA    //Stage 10 Offset Low Updated Register
#define IT723XP0_S10HTR     0xDB    //Stage 10 High Threshold Register
#define IT723XP0_S10LTR     0xDC    //Stage 10 Low Threshold Register
#define IT723XP0_S10SR      0xDD    //Stage 10 Sensitivity Register
#define IT723XP0_S10CDC_CMP 0xDE    //Stage 10 Compensated CDC Value Register
#define IT723XP0_S10CSR     0xDF    //Stage 10 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S11FR0     0xE0    //Stage 11 FIFO Register 0
#define IT723XP0_S11FR1     0xE1    //Stage 11 FIFO Register 1
#define IT723XP0_S11FR2     0xE2    //Stage 11 FIFO Register 2
#define IT723XP0_S11DLR     0xE3    //Stage 11 Device Level Register
#define IT723XP0_S11CVR     0xE4    //Stage 11 Compensation Value Register
#define IT723XP0_S11OHCR    0xE5    //Stage 11 Offset High Clamp Register
#define IT723XP0_S11OLCR    0xE6    //Stage 11 Offset Low Clamp Register
#define IT723XP0_S11OHR     0xE7    //Stage 11 Offset High Register
#define IT723XP0_S11OLR     0xE8    //Stage 11 Offset Low Register
#define IT723XP0_S11OHUR    0xE9    //Stage 11 Offset High Updated Register
#define IT723XP0_S11OLUCR   0xEA    //Stage 11 Offset Low Updated Register
#define IT723XP0_S11HTR     0xEB    //Stage 11 High Threshold Register
#define IT723XP0_S11LTR     0xEC    //Stage 11 Low Threshold Register
#define IT723XP0_S11SR      0xED    //Stage 11 Sensitivity Register
#define IT723XP0_S11CDC_CMP 0xEE    //Stage 11 Compensated CDC Value Register
#define IT723XP0_S11CSR     0xEF    //Stage 11 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP0_S12FR0     0xF0    //Stage 12 FIFO Register 0
#define IT723XP0_S12FR1     0xF1    //Stage 12 FIFO Register 1
#define IT723XP0_S12FR2     0xF2    //Stage 12 FIFO Register 2
#define IT723XP0_S12DLR     0xF3    //Stage 12 Device Level Register
#define IT723XP0_S12CVR     0xF4    //Stage 12 Compensation Value Register
#define IT723XP0_S12OHCR    0xF5    //Stage 12 Offset High Clamp Register
#define IT723XP0_S12OLCR    0xF6    //Stage 12 Offset Low Clamp Register
#define IT723XP0_S12OHR     0xF7    //Stage 12 Offset High Register
#define IT723XP0_S12OLR     0xF8    //Stage 12 Offset Low Register
#define IT723XP0_S12OHUR    0xF9    //Stage 12 Offset High Updated Register
#define IT723XP0_S12OLUCR   0xFA    //Stage 12 Offset Low Updated Register
#define IT723XP0_S12HTR     0xFB    //Stage 12 High Threshold Register
#define IT723XP0_S12LTR     0xFC    //Stage 12 Low Threshold Register
#define IT723XP0_S12SR      0xFD    //Stage 12 Sensitivity Register
#define IT723XP0_S12CDC_CMP 0xFE    //Stage 12 Compensated CDC Value Register
#define IT723XP0_S12CSR     0xFF    //Stage 12 Calculation Status Register
//-----------------------------------------------------------------------------
#define IT723XP1_PSR        0x00    //Page Setting Register
#define IT723XP1_PCR        0x01    //Power Control Register
#define IT723XP1_PMR        0x02    //Power Mode Register
#define IT723XP1_CFER       0x03    //Calibration Function Enable Register
#define IT723XP1_RTR        0x04    //Recalibration Timeout Register
#define IT723XP1_CTR        0x05    //Calibration Timeout Register
#define IT723XP1_CRMR       0x06    //Contact Release Margin Register
#define IT723XP1_PDR        0x07    //Proximity Detection Register
#define IT723XP1_DR         0x08    //Drift Register (DR)
#define IT723XP1_S0UBR      0x09    //Slider 0 Upper Boundary Register
#define IT723XP1_S0LBR      0x0A    //Slider 0 Lower Boundary Register
#define IT723XP1_S0OSR      0x0B    //Slider 0 Operational Setting Register
#define IT723XP1_S1UBR      0x0C    //Slider 1 Upper Boundary Register
#define IT723XP1_S1LBR      0x0D    //Slider 1 Lower Boundary Register
#define IT723XP1_S1OSR      0x0E    //Slider 1 Operational Setting Register
//-----------------------------------------------------------------------------
#define IT723XP1_S0CR       0x10    //Stage 0 Connection Register
#define IT723XP1_S1CR       0x11    //Stage 1 Connection Register
#define IT723XP1_S2CR       0x12    //Stage 2 Connection Register
#define IT723XP1_S3CR       0x13    //Stage 3 Connection Register
#define IT723XP1_S4CR       0x14    //Stage 4 Connection Register
#define IT723XP1_S5CR       0x15    //Stage 5 Connection Register
#define IT723XP1_S6CR       0x16    //Stage 6 Connection Register
#define IT723XP1_S7CR       0x17    //Stage 7 Connection Register
#define IT723XP1_S8CR       0x18    //Stage 8 Connection Register
#define IT723XP1_S9CR       0x19    //Stage 9 Connection Register
#define IT723XP1_S10CR      0x1A    //Stage 10 Connection Register
#define IT723XP1_S11CR      0x1B    //Stage 11 Connection Register
#define IT723XP1_S12CR      0x1C    //Stage 12 Connection Register
//-----------------------------------------------------------------------------
#define IT723XP1_C0COR      0x20    //CIN 0 Cstray Offset Register
#define IT723XP1_C1COR      0x21    //CIN 1 Cstray Offset Register
#define IT723XP1_C2COR      0x22    //CIN 2 Cstray Offset Register
#define IT723XP1_C3COR      0x23    //CIN 3 Cstray Offset Register
#define IT723XP1_C4COR      0x24    //CIN 4 Cstray Offset Register
#define IT723XP1_C5COR      0x25    //CIN 5 Cstray Offset Register
#define IT723XP1_C6COR      0x26    //CIN 6 Cstray Offset Register
#define IT723XP1_C7COR      0x27    //CIN 7 Cstray Offset Register
#define IT723XP1_C8COR      0x28    //CIN 8 Cstray Offset Register
#define IT723XP1_C9COR      0x29    //CIN 9 Cstray Offset Register
#define IT723XP1_C10COR     0x2A    //CIN 10 Cstray Offset Register
#define IT723XP1_C11COR     0x2B    //CIN 11 Cstray Offset Register
#define IT723XP1_C12COR     0x2C    //CIN 12 Cstray Offset Register
#define IT723XP1_ICR0       0x2D    //CIN7~CIN0 Input Configurations Register
#define IT723XP1_ICR1       0x2E    //CIN12~CIN8 Input Configurations Register
#define IT723XP1_COER0      0x2F    //CIN7~CIN0 Cstray Offset Enable Register
#define IT723XP1_COER1      0x30    //CIN12~CIN8 Cstray Offset Enable Register
#define IT723XP1_PDCR       0x31    //Power Down Control Register
#define IT723XP1_CGCR       0x32    //CDC Gain Control Register
//-----------------------------------------------------------------------------
#define IT723XP1_GODCR0     0x40    //GPIO Output Driving Capability Register 0
#define IT723XP1_GODCR1     0x41    //GPIO Output Driving Capability Register 1
#define IT723XP1_I2CODCR    0x42    //I2C Driving Capability Register
#define IT723XP1_IODCR      0x43    //INT Output Driving Capability Register
#define IT723XP1_GPIOOSRR   0x44    //GPIO Output Slew Rate Register
#define IT723XP1_GPIOICR    0x45    //GPIO Input Characteristic Register
//-----------------------------------------------------------------------------
#define IT723XP1_GPIOMSR    0x50    //GPIO Mode Select Register
#define IT723XP1_GPIODR     0x51    //GPIO Direction Register
#define IT723XP1_GPIOIR     0x52    //GPIO Input Register
#define IT723XP1_GPIOOR     0x53    //GPIO Output Register
#define IT723XP1_GPIOMR     0x54    //GPIO Mode Register
#define IT723XP1_GPIOLR     0x55    //GPIO Level Register
#define IT723XP1_GPIOER     0x56    //GPIO Edge Register
#define IT723XP1_GPIOISR    0x57    //GPIO Interrupt Status Register
#define IT723XP1_GPIOIMR    0x58    //GPIO Interrupt Mask Register
#define IT723XP1_GPIOPCR    0x59    //GPIO Pull Control Register
#define IT723XP1_GPIONPCR   0x5A    //GPIO Non Pull Control Register
#define IT723XP1_LEDCMR0    0x5B    //LED Stage Mapping Register 0
#define IT723XP1_LEDCMR1    0x5C    //LED Stage Mapping Register 1
#define IT723XP1_LEDCMR2    0x5D    //LED Stage Mapping Register 2
#define IT723XP1_LEDCMR3    0x5E    //LED Stage Mapping Register 3
#define IT723XP1_LEDRPR     0x5F    //LED Ramp Period Register
#define IT723XP1_LEDBR      0x60    //LED Brightness Register
#define IT723XP1_LEDCGCR    0x61    //LED Current Gain Control Register 0
#define IT723XP1_LEDPR0     0x62    //LED Pattern Register 0
#define IT723XP1_LEDPR1     0x63    //LED Pattern Register 1
#define IT723XP1_LEDPR2     0x64    //LED Pattern Register 2
#define IT723XP1_LEDPR3     0x65    //LED Pattern Register 3
//-----------------------------------------------------------------------------

/*****************************************************************************/
// END DEFINITION
/*****************************************************************************/
