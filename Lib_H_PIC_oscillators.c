/** 
 * File:   Lib_H_PIC_oscillators.c
 * Author: Isaev
 *
 * Created on 8 октября 2017 г., 17:00
 */

//******************************************************************************
// Секция include: здесь подключается заголовочный файл к модулю
#include "Lib_H_PIC_oscillators.h"
//******************************************************************************


//******************************************************************************
//------------------------------------------------------------------------------
// Глобальные переменные
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Локальные переменные
//------------------------------------------------------------------------------
//******************************************************************************


//******************************************************************************
// Секция прототипов локальных функций
//******************************************************************************


//******************************************************************************
// Секция описания функций (сначала глобальных, потом локальных)
//==============================================================================
//------------------------------------------------------------------------------
// Функции для микроконтроллера серии "PIC24H"
#if defined (__PIC24H__)

/**
 *  @brief  Функция конфигурирует тактовый генератор микроконтроллера со следующими 
 *          параметрами:
 *              HS - 8 MHz (веншний тактовый генератор как источник импульсов);
 *              FOSC - 64 MHz;
 *              FCY - 32 MIPS;
 */
void PIC_Init_Oscillator_HS_8MHz_FOSC_64MHz_FCY_32MIPS(void)
{
    CLKDIVbits.ROI = 0; //              1 = Interrupts will clear the DOZEN bit and the processor clock/peripheral clock ratio is set to 1:1
    //                                  0 = Interrupts have no effect on the DOZEN bit 

    //  DOZE<2:0>: Processor Clock Reduction Select bits
    CLKDIVbits.DOZE = 0; //             111 = FCY/128
    //                                  110 = FCY/64
    //                                  101 = FCY/32
    //                                  100 = FCY/16
    //                                  011 = FCY/8 (default)
    //                                  010 = FCY/4
    //                                  001 = FCY/2
    //                                  000 = FCY/1

    //  DOZEN: DOZE Mode Enable bit
    //    CLKDIVbits.DOZEN = 0; //            1 = DOZE<2:0> field specifies the ratio between the peripheral clocks and the processor clocks
    //                                  0 = Processor clock/peripheral clock ratio forced to 1:1

    //  PLLPOST<1:0>: PLL VCO Output Divider Select bits (also denoted as ‘N2’, PLL postscaler)
    CLKDIVbits.PLLPOST = 0; //          00 = Output/2
    //                                  01 = Output/4 (default)
    //                                  10 = Reserved
    //                                  11 = Output/8

    //  PLLPRE<4:0>: PLL Phase Detector Input Divider bits (also denoted as ‘N1’, PLL prescaler)
    CLKDIVbits.PLLPRE = 0; //           00000 = Input/2 (default)
    //                                  00001 = Input/3
    //                                  •••
    //                                  11111 = Input/33

    //  PLL Feedback Divisor bits (also denoted as ‘M’, PLL multiplier)
    PLLFBDbits.PLLDIV = 0b000001110; // 000000000 = 2
    //                                  000000001 = 3
    //                                  000000010 = 4
    //                                  •
    //                                  •
    //                                  •
    //                                  000110000 = 50 (default)
    //                                  •
    //                                  •
    //                                  •
    //                                  111111111 = 513

    while (OSCCONbits.LOCK != 1);

    RCONbits.SWDTEN = 0; //             Disable Watch Dog Timer
}
#endif //   (__PIC24H__)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Функции для микроконтроллера серии "PIC24E"
#if defined (__PIC24E__) 
#endif //   (__PIC24E__)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Функции для микроконтроллера серии "dsPIC33E"
#if defined (__dsPIC33E__)

/**
 *  @brief  Функция конфигурирует тактовый генератор микроконтроллера со следующими
 *          параметрами:
 *              FRC - 7.23 MHz (вунтренняя RC цепочка как источник импульсов);
 *		Подстройка FRC до 8,0056625 MHz
 *              FVCO - 256,18 MHz;
 *		FOSC - 128,09 MHz;
 *              FCY - 64,045 MIPS;
 */
void PIC_Init_Oscillator_FRC_8MHz_FOSC_128MHz_FCY_64MIPS(void)
{
    //--------------------------------------------------------------------------
    /* CLKDIV: Clock Divisor Register */

    //  Recover on Interrupt bit
    CLKDIVbits.ROI = 0; //              Interrupts have no effect on the DOZEN bit

    //  Processor Clock Reduction Select bits
    CLKDIVbits.DOZE = 0b000; //         FCY divided by 1

    //  Doze Mode Enable bit
    CLKDIVbits.DOZEN = 0; //            Processor clock and peripheral clock ratio are forced to 1:1

    //  Internal Fast RC Oscillator Postscaler bits
    CLKDIVbits.FRCDIV = 0b000; //       FRC divided by 1 (default)

    //  PLL VCO Output Divider Select bits (also denoted as СN2Т, PLL postscaler)
    CLKDIVbits.PLLPOST = 0b00; //       Output divided by 2

    //  PLL Phase Detector Input Divider Select bits (also denoted as СN1Т, PLL prescaler)
    CLKDIVbits.PLLPRE = 0b00000; //     Input divided by 2 (default)

    //  PLLFBD: PLL Feedback Divisor Register
    PLLFBDbits.PLLDIV = 62; //          PLL Feedback Divisor bits (also denoted as СMТ, PLL multiplier)

    //  OSCTUN: FRC OSCILLATOR TUNING REGISTER
    OSCTUNbits.TUN = 23; //             FRC Oscillator Tuning bits
    //                                  Подстройка FRC до 8,0056625 MHz
    //--------------------------------------------------------------------------


    // LOCK: PLL Lock Status bit (read-only)
    while (OSCCONbits.LOCK != 1); //    1 = Indicates that PLL is in lock or PLL start-up timer is satisfied
    //                                  0 = Indicates that PLL is out of lock, start-up timer is in progress or PLL is disabled

    RCONbits.SWDTEN = 0; //             Отключаем сторожевой таймер
}

/**
 *  @brief  Функция конфигурирует тактовый генератор микроконтроллера со следующими
 *          параметрами:
 *              HS - 16 MHz (внешний кварцевый генератор)
 *              FVCO - 160 MHz;
 *		FOSC - 80 MHz;
 *              FCY - 40 MIPS;
 */
void PIC_Init_Oscillator_HS_16MHz_FOSC_80MHz_FCY_40MIPS(void)
{
    //  Recover on Interrupt bit
    CLKDIVbits.ROI = 0; //              Interrupts have no effect on the DOZEN bit

    //  Processor Clock Reduction Select bits
    CLKDIVbits.DOZE = 0b000; //         FCY divided by 1

    //  Doze Mode Enable bit
    CLKDIVbits.DOZEN = 0; //            Processor clock and peripheral clock ratio are forced to 1:1

    //  PLL VCO Output Divider Select bits (also denoted as СN2Т, PLL postscaler)
    CLKDIVbits.PLLPOST = 0b00; //       Output divided by 2

    //  PLL Phase Detector Input Divider Select bits (also denoted as СN1Т, PLL prescaler)
    CLKDIVbits.PLLPRE = 0b00000; //     Input divided by 2 (default)

    //  PLLFBD: PLL Feedback Divisor Register
    PLLFBDbits.PLLDIV = 18; //          PLL Feedback Divisor bits (also denoted as СMТ, PLL multiplier)

    //настройка источника тактирования
    //__builtin_write_OSCCONH(0x03);
    //__builtin_write_OSCCONL(0x01);    
    OSCCONbits.NOSC = 0x03; //выбор источика тактирования - Primary Oscillator (POSC) with PLL (XTPLL, HSPLL, ECPLL)
    OSCCONbits.OSWEN = 0x1; //задействовать биты NOSC<2:0>

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0x03);

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);

    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;
}

/**
 *  @brief  Функция конфигурирует тактовый генератор микроконтроллера со следующими
 *          параметрами:
 *              HS - 16 MHz (внешний кварцевый генератор)
 *              FVCO - 256 MHz;
 *		FOSC - 128 MHz;
 *              FCY - 64 MIPS;
 */
void PIC_Init_Oscillator_HS_16MHz_FOSC_128MHz_FCY_64MIPS(void)
{
    //  Recover on Interrupt bit
    CLKDIVbits.ROI = 0; //              Interrupts have no effect on the DOZEN bit

    //  Processor Clock Reduction Select bits
    CLKDIVbits.DOZE = 0b000; //         FCY divided by 1

    //  Doze Mode Enable bit
    CLKDIVbits.DOZEN = 0; //            Processor clock and peripheral clock ratio are forced to 1:1

    //  PLL VCO Output Divider Select bits (also denoted as СN2Т, PLL postscaler)
    CLKDIVbits.PLLPOST = 0b00; //       Output divided by 2

    //  PLL Phase Detector Input Divider Select bits (also denoted as СN1Т, PLL prescaler)
    CLKDIVbits.PLLPRE = 2; //           Input divided by 4;

    //  PLLFBD: PLL Feedback Divisor Register
    PLLFBDbits.PLLDIV = 62; //          PLL Feedback Divisor bits (also denoted as СMТ, PLL multiplier)

    //настройка источника тактирования
    //__builtin_write_OSCCONH(0x03);
    //__builtin_write_OSCCONL(0x01);    
    OSCCONbits.NOSC = 0x03; //выбор источика тактирования - Primary Oscillator (POSC) with PLL (XTPLL, HSPLL, ECPLL)
    OSCCONbits.OSWEN = 0x1; //задействовать биты NOSC<2:0>

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0x03);

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);

    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;
}

/**
 *  @brief  Функция конфигурирует тактовый генератор микроконтроллера со следующими
 *          параметрами:
 *              HS - 16 MHz (внешний кварцевый генератор)
 *              FVCO - 320 MHz;
 *		FOSC - 160 MHz;
 *              FCY - 80 MIPS;
 */
void PIC_Init_Oscillator_HS_16MHz_FOSC_160MH_FCY_80MIPS (void)
{
       //  Recover on Interrupt bit
    CLKDIVbits.ROI = 0; //              Interrupts have no effect on the DOZEN bit

    //  Processor Clock Reduction Select bits
    CLKDIVbits.DOZE = 0b000; //         FCY divided by 1

    //  Doze Mode Enable bit
    CLKDIVbits.DOZEN = 0; //            Processor clock and peripheral clock ratio are forced to 1:1

    //  PLL VCO Output Divider Select bits (also denoted as СN2Т, PLL postscaler)
    CLKDIVbits.PLLPOST = 0b00; //       Output divided by 2

    //  PLL Phase Detector Input Divider Select bits (also denoted as СN1Т, PLL prescaler)
    CLKDIVbits.PLLPRE = 2; //           Input divided by 4;

    //  PLLFBD: PLL Feedback Divisor Register
    PLLFBDbits.PLLDIV = 78; //          PLL Feedback Divisor bits (also denoted as СMТ, PLL multiplier)

    //настройка источника тактирования
    //__builtin_write_OSCCONH(0x03);
    //__builtin_write_OSCCONL(0x01);    
    OSCCONbits.NOSC = 0x03; //выбор источика тактирования - Primary Oscillator (POSC) with PLL (XTPLL, HSPLL, ECPLL)
    OSCCONbits.OSWEN = 0x1; //задействовать биты NOSC<2:0>

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0x03);

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);

    // Disable Watch Dog Timer
    RCONbits.SWDTEN = 0;
}
#endif //   (__dsPIC33E__)
//------------------------------------------------------------------------------
//==============================================================================
//******************************************************************************


////////////////////////////////////////////////////////////////////////////////
// END OF FILE
////////////////////////////////////////////////////////////////////////////////
