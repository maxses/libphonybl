#ifndef PHONY_BL_CORE_HPP
#define PHONY_BL_CORE_HPP
/**---------------------------------------------------------------------------
 *
 * @brief   Phony bootloader
 *
 * Simulate the ROM Uart bootloader from STM32 device.
 * Reference:
 * https://www.st.com/resource/en/application_note/an3155-usart-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf
 *
 * @date   20221112
 * @author Maximilian Seesslen <code@seesslen.net>
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>
#include <stddef.h>
#include <lepto/signal.hpp>


/*--- Declarations ---------------------------------------------------------*/


class CPhonyBLCore
{
   private:
      static constexpr char ACK  = 0x79;
      static constexpr char NACK = 0x1F;
      static constexpr int  TIMEOUT_MS = 2000;

      struct SFunction
      {
         uint8_t  ch1;
         uint8_t  ch2;
         void (CPhonyBLCore::*func)(void);
         const char *name;
      };
      static SFunction functions[];
      uint8_t m_pages[0x101];
      uint8_t m_data[0x101];

   public:
      /**
       * @brief Signal which can be used to let an LED blink according to traffic
       */
      CSignal <void, int> writeSignal;
       
      CPhonyBLCore()
      {};
      
      /**
       * @brief The event loop has to be called periodically
       * 
       * The code may run on an MCU so Qt signals are not used
       */
      void eventLoop();
      
      /**
       * @brief recevies an character from input, blocking with timeout
       * 
       * While blocking the systems event loop is called via systemEventLoop()
       * 
       * @return Character or '-1' when no data is avalable
       */
      int getInput();

      // Bootloader commands
      void commandInfo( void );
      void commandVersion( void );
      void commandReadMemory( void );
      void commandEraseMemory( void );
      void commandWriteMemory( void );
      void commandExtEraseMemory( void );
      void commandSpecial( void );
      
      /**
       * @brief Send Acknowledge to programmer
       */
      void ack( void );
      
      /**
       * @brief Send Not-Acknowledge to programmer
       */
      void nack( void );
      
      static bool checkXor( char start, const char *buf, int size, char targetXor );
      
      // These methods have to be implemented for specific target device
      virtual int getChar() = 0;
      virtual int write(const char *data, int size) = 0;
      virtual void systemEventLoop() = 0;
      virtual void readFlashData(int addr, void *dest, size_t size) = 0;
      virtual void writeFlashData(int addr, void *src, size_t size) = 0;
      virtual void eraseFlash() = 0;
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! PHONY_BL_CORE_HPP
