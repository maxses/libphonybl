#ifndef PHONY_BL_HPP
#define PHONY_BL_HPP
/**---------------------------------------------------------------------------
 *
 * @brief   Phony bootloader
 *
 * Simulate the ROM Uart bootloader from STM32 device.
 * Reference:
 * https://www.st.com/resource/en/application_note/an3155-usart-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf
 * 
 * This headers 
 *
 * @date   20221112
 * @author Maximilian Seesslen <code@seesslen.net>
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <phonybl/phonyblCore.hpp>

#if defined( HOST ) || defined( __x86_64__ )
   #include <phonybl/phonybl_host.hpp>
#else
   #if defined USE_BIWAK
      #include <phonybl/phonybl_biwak.hpp>
   #else
      #error Could not figure out which platform is used.
   #endif
#endif



/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! PHONY_BL_HPP
