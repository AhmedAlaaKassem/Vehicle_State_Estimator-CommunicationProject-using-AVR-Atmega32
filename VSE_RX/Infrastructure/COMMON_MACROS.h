#ifndef _COMMON_MACROS_H
#define _COMMON_MACROS_H

//#define SET_BIT(REG,BIT)    (REG |=  (1<<BIT))
#define CLR_BIT(REG,BIT)  (REG &= ~(1<<BIT))
//#define TOGGLE_BIT(REG,BIT) (REG ^=  (1<<BIT))
#define READ_BIT(REG,BIT)   ((REG & (1<<BIT)) >> BIT)




//#define NULL	 0U
#define DUMMY	 0XFFU

#define SET_BIT(Byte,Pos)           ( Byte |=  (1U<<Pos))
#define CLEAR_BIT(Byte,Pos)         ( Byte &=~ (1U<<Pos))
#define TOGGLE_BIT(Byte,Pos)        ( Byte ^=  (1U<<Pos))
#define GET_BIT(Byte,Bit)           ((Byte >> Bit) & 1U )    //(Byte &(1<<Bit))>>Bit

#define SET_BYTE(Byte)              ( Byte |=  (0xFFU))
#define CLEAR_BYTE(Byte)            ( Byte &=~ (0xFFU))

#define SET_MASK(Byte, Mask)        ( Byte |=  (Mask))
#define CLEAR_MASK(Byte, Mask)      ( Byte &=~ (Mask))
#define TOGGLE_BYTE(Byte)           ( Byte ^=  (0xFF))

#define SET_NIBBLE_HIGH(Byte)       ( Byte |= (0xF0U))
#define SET_NIBBLE_LOW(Byte)        ( Byte |= (0x0FU))

#define CLEAR_NIBBLE_HIGH(Byte)     ( Byte &= (0x0FU))
#define CLEAR_NIBBLE_LOW(Byte)      ( Byte &= (0xF0U))

/* Rotate right the register value with specific number of rotates */
#define ROR(Byte,Pos) ( Byte = (Byte>>Pos) | (Byte<<(8U-Pos)) )

/* Rotate left the register value with specific number of rotates */
#define ROL(Byte,Pos) ( Byte = (Byte<<Pos) | (Byte>>(8U-Pos)) )



#endif



