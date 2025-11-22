#ifndef PHONY_BL_HOST_HPP
#define PHONY_BL_HOST_HPP
/**---------------------------------------------------------------------------
 *
 * @brief   Phony bootloader
 *
 * Inherit "CPhonyBLCore" and implement interface functions for UART on host PC.
 *
 * @date   20221112
 * @author Maximilian Seesslen <code@seesslen.net>
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <phonybl/phonybl.hpp>
#include <QtSerialPort/QSerialPort>
#include <QCoreApplication>


/*--- Declarations ---------------------------------------------------------*/


class CPhonyBL:public CPhonyBLCore
{
   private:
      QSerialPort *pSerial;

   public:
      CPhonyBL( const QString &device )
      {
         pSerial = new QSerialPort( );

         pSerial->setPortName( device );
         pSerial->setBaudRate( QSerialPort::Baud115200 );

         if( !pSerial->open( QIODevice::ReadWrite ) )
         {
            qInfo( qPrintable( pSerial->errorString() ) );
            qFatal( "Cloud not open serial device" );
         }
      };
      // For tests only
      CPhonyBL( )
      {
         pSerial=nullptr;
      }
      ~CPhonyBL()
      {
         printf("Quit...\n");
         pSerial->close();
      }
      virtual void systemEventLoop() override
      {
         QCoreApplication::processEvents();
      }
      virtual int getChar() override
      {
         char ch=0;
         int cnt;
         if( pSerial->bytesAvailable() == 0 )
         {
            return( -1 );
         }

         cnt=pSerial->read( &ch, sizeof(ch) );

         return( ( cnt == 0 ) ? -1 : (uint8_t)ch );
      }
      virtual int write(const char *data, int size) override
      {
         return( pSerial->write( data, size ) );
      }
      virtual void readFlashData(int addr, void *dest, size_t size) override
      {
         // Dummy, don't do anything
      }
      virtual void writeFlashData(int addr, void *src, size_t size) override
      {
         // Dummy, don't do anything
      }
      virtual void eraseFlash() override
      {
         // Dummy, don't do anything
      }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! PHONY_BL_HOST_HPP
