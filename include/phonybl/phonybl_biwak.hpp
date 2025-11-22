#ifndef PHONY_BL_BIWAK_HPP
#define PHONY_BL_BIWAK_HPP
/**---------------------------------------------------------------------------
 *
 * @brief   Phony bootloader
 *
 * Inherit "CPhonyBLCore" and implement interface functions for UART for STM32
 * device using libbiwak.
 *
 * @date   20221112
 * @author Maximilian Seesslen <code@seesslen.net>
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <biwak/biwak.h>
#include <biwak/iostream.h>

#include <phonybl/phonybl.hpp>


/*--- Declarations ---------------------------------------------------------*/


// A little bit 'quick and dirty'. Signals would be more clean.
void readFlashData(int addr, void *dest, size_t size);
void writeFlashData(int addr, void *src, size_t size);
void eraseFlash();

class CPhonyBL:public CPhonyBLCore
{
   private:
   CIOStream& m_io;
       
   public:
      CPhonyBL(CIOStream &io)
         :m_io(io)
      {};

      virtual int getChar() override
      {
         return( m_io.getch() );
      }

      virtual void systemEventLoop() override
      {
         m_io.eventLoop();
         biwakEventLoop();
      }
      
      virtual int write(const char *data, int size) override
      {
         return( m_io.write(data, size) );
      }
      virtual void readFlashData(int addr, void *dest, size_t size) override
      {
         ::readFlashData( addr, dest, size);
      }
      virtual void writeFlashData(int addr, void *src, size_t size) override
      {
         ::writeFlashData( addr, src, size);
      }
      virtual void eraseFlash() override
      {
         ::eraseFlash();
      }
      
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! PHONY_BL_BIWAK_HPP
