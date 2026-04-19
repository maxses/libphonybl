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
#include <biwak/flash_spi.hpp>

#include <phonybl/phonybl.hpp>


/*--- Declarations ---------------------------------------------------------*/


class CPhonyBL:public CPhonyBLCore
{
   private:
      CIOStream &m_io;
      CFlashSpi &m_flash;
       
   public:
      CPhonyBL(CIOStream &io, CFlashSpi &flash)
         :m_io( io )
         ,m_flash( flash )
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
         
         m_flash.readData(addr, dest, size);
      }
      virtual void writeFlashData(int addr, void *src, size_t size) override
      {
         m_flash.writeData(addr, src, size);
      }
      virtual void eraseFlash() override
      {
         m_flash.chipErase();
         m_flash.waitForReady();
      }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! PHONY_BL_BIWAK_HPP
