
/*

-------------------------------------------------
-- 版本 : 1.2
-- 作者 : Craftor
-- 时间 : 2011-11-03
-- 说明 : 重新设定指令定义，请使用v1.4以上固件
-------------------------------------------------

-------------------------------------------------
-- 版本 : 1.1
-- 作者 : Craftor
-- 时间 : 2011-10-07
-- 说明 : 加入对单字节地址的I2C从设备的读写支持
-------------------------------------------------

-------------------------------------------------
-- 版本 : 1.0
-- 作者 : Craftor
-- 时间 : 2011-09-08
-- 说明 : 
-------------------------------------------------

*/

// usb2iic.h: interface for the usb2iic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USB2IIC_H__C236E7A8_9D0C_4504_8FD0_204DE8CB8E8B__INCLUDED_)
#define AFX_USB2IIC_H__C236E7A8_9D0C_4504_8FD0_204DE8CB8E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CyAPI.h"

#define LEN_INT_EP      64
#define MAX_DATA_LENGTH 32

// 一般I2C储存设备，如EEPROM，地址为两个字节，即0x0000，使用 OP_R2N/OP_W2N 进行读写
// 有些I2C接口的芯片，寄存器地址只有一个字节，即0x00，使用 OP_R1N/OP_W1N 进行读写

#define OP_R2N 0x21  // 2 Byte Address Slave, Read N Byte
#define OP_W2N 0x20  // 2 Byte Address Slave, Write N Byte

#define OP_R1N 0x11  // 1 Byte Address Slave, Read N Byte
#define OP_W1N 0x10  // 1 Byte Address Slave, Write N Byte

class CUSB2IIC  
{
public:
	CUSB2IIC();
	virtual ~CUSB2IIC();

	CCyUSBDevice *USBDevice;
	
	BYTE* pPkgOut ;
	BYTE* pDataOut ;
	BYTE* pPkgIn  ;
	BYTE* pDataIn ;
	
	BYTE cBuf ;
	BYTE cOperation ;
	BYTE cI2C_Addr ;
	BYTE cData_Addr_High ;
	BYTE cData_Addr_Low ;
	BYTE cLength ;

	// 带调试信息传输数据
	BOOL D_XferData(	BYTE* pData, 
						BYTE cOperation,
						BYTE cI2C_Addr, 
						BYTE cData_Addr_High, 
						BYTE cData_Addr_Low, 
						BYTE cLength);

	// 不带调试信息传输数据
	BOOL XferData(	BYTE* pData, 
					BYTE cOperation,
					BYTE cI2C_Addr, 
					BYTE cData_Addr_High, 
					BYTE cData_Addr_Low, 
					BYTE cLength);
	
private:
	
	BOOL DebugPkg ; // Print the Debug Information to info.txt
	FILE *fp;

	// EP1 is for IIC Bus Control
	// EP2,EP6 is for Slave FIFO with FPGA
	BOOL IIC_PkgIn(BYTE* pData, long cLength);    // EP1 In
	BOOL IIC_PkgOut(BYTE* pData, long cLength);   // EP1 Out

	BOOL WriteFIFO(BYTE* pData, long cLength); // EP2 Out
	BOOL ReadFIFO(BYTE* pData, long cLength); // EP6 In

	// Create Command Package
	BYTE MakeCRC(BYTE* pData, BYTE cLength);
	BOOL MakePackage(	BYTE* pData, 
						BYTE cType,
						BYTE cI2C_Addr, 
						BYTE cData_Addr_High, 
						BYTE cData_Addr_Low,
						BYTE cLength);
};

#endif // !defined(AFX_USB2IIC_H__C236E7A8_9D0C_4504_8FD0_204DE8CB8E8B__INCLUDED_)
