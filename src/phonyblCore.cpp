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


#undef LEPTO_LOG_DEBUG


/*--- Includes -------------------------------------------------------------*/


#include <phonybl/phonybl.hpp>
#include <lepto/log.h>
#include <lepto/lepto.h>   // arraySize
#include <unistd.h>        // usleep


/*--- Implementation -------------------------------------------------------*/


constexpr char CPhonyBLCore::ACK;
constexpr char CPhonyBLCore::NACK;


// This functions are described in the document from ST mentioned in the header
CPhonyBLCore::SFunction CPhonyBLCore::functions[] {
   { 0x00, 0xFF, &CPhonyBLCore::commandInfo, "info" },
   { 0x02, 0xFD, &CPhonyBLCore::commandVersion, "version" },
   { 0x11, 0xEE, &CPhonyBLCore::commandReadMemory, "read memory" },
   { 0x43, 0xBC, &CPhonyBLCore::commandEraseMemory, "erase memory" },
   { 0x31, 0xCE, &CPhonyBLCore::commandWriteMemory, "write memory" },
   { 0x44, 0xBB, &CPhonyBLCore::commandExtEraseMemory, "extended erase memory" },
   { 0x50, 0xAF, &CPhonyBLCore::commandSpecial, "special command" },
};


void CPhonyBLCore::eventLoop()
{
   systemEventLoop();
   int c1 = getChar();
   
   // No data, just skip
   if(c1 == -1)
   {
      return;
   }
   
   // Don't spam debug output
   // lDebug( "[ 0x%X ]", c1 );

   int c2=0;
   bool acknowledged=false;
   switch(c1)
   {
      // Start frame
      case 0x7F:
         ack();
         break;
      default:
      {
         c2 = getInput();
         if( c2 == -1 )
         {
            lWarning( "Timeout for c2" );
         }
         bool found=false;
         for(int i1=0; i1 < arraySize(functions); i1++ )
         {
            if(functions[i1].ch1==c1)
            {
               lDebug("Function found: c1=0x%X c2=0x%X", c1, c2);
               found=true;
               if(functions[i1].ch2==c2)
               {
                  ack();
                  ( (*this).*(functions[i1].func) )();
                  lDebug("Function '%s' called", functions[i1].name);
                  acknowledged=true;
                  break;
               }
               else
               {
                  lWarning("Protocol Error c1=0x%X c2=0x%X", c1, c2);
                  nack();
                  acknowledged=true;
                  break;
               }
            }
         }
         if(!found)
         {
            lWarning("Function not found: c1=0x%X c2=0x%X", c1, c2);
            nack();
         }
         else if(!acknowledged)
         {
            lWarning("Protocol Error; c1=0x%X c2=0x%X", c1, c2);
            nack();
         }
         break;
      }
   }
}


int CPhonyBLCore::getInput()
{
   int input;
   int counter=-1;
   do
   {
      input=getChar();
      counter++;
      systemEventLoop();
      if( input == -1 )
      {
         usleep(100);
      }
   }while( ( input == -1 )
           && ( counter < ( TIMEOUT_MS * 10 ) ) );

   static_assert( (int)(uint8_t)-1 == 0xff, "");

   if( input == -1 )
   {
      lWarning( "Input timeout" );
   }

   return(input);
}


void CPhonyBLCore::ack()
{
   write( &ACK, 1 );
}


void CPhonyBLCore::nack()
{
   lWarning("NACK");
   write( &NACK, 1 );
}


bool CPhonyBLCore::checkXor( char start, const char *buf, int size, char targetXor )
{
   char val=start;
   for(int i1=0; i1<size; i1++)
   {
      val=val^buf[i1];
   }
   if(val==targetXor)
   {
      return(true);
   }
   lWarning("XOR does not match: target: %X; calculated: %X", targetXor, val);
   return(false);
}


void CPhonyBLCore::commandInfo()
{
   constexpr uint8_t data[]{
      11,      // N; Number of chars - 1
      0x10,    // BL Protocol Version; 0x10=1.0
      0x00,    // Get command
      0x01,    // Get Version and Read Protection Status
      0x02,    // Get ID
      0x11,    // Read Memory command
      0x21,    // Go command
      0x31,    // Write Memory command
      0x43,   // Erase command or Extended Erase command
      // 0x44,    // Erase command or Extended Erase command
      0x63,    // Write Protect command
      0x73,    // Write Unprotect comman
      0x82,    // Readout Protect command
      0x92,    // Readout Unprotect command
      ACK
   };
   write( (const char*)data, sizeof(data));
}


void CPhonyBLCore::commandVersion()
{
   constexpr char data[]{
         0x01,    // N; Number of chars - 1
         0x04,    // STM32
         0x10,    // 0x10: STM32F10x Medium-density; 0x68: STM32G443
         ACK
   };
   write(data, sizeof(data));
}


void CPhonyBLCore::commandReadMemory()
{
   uint32_t  addr=0;
   uint8_t  _xor=0;
   uint8_t  num=0;
   for(int i1=0; i1<4; i1++)
   {
      addr |= getInput() << ((3 - i1)*8);
   }

   _xor=getInput();
   if ( ! checkXor( 0, (char *)&addr, 4, _xor) )
   {
      nack();
      return;
   }

   ack();
   num=getInput();

   _xor=getInput();
   if ( ! checkXor( 0xFF, (char *)&num, 1, _xor) )
   {
      nack();
      return;
   }

   ack();
   readFlashData( addr, m_data, num+1);
   for(int i1=0; i1<num+1; i1++)
   {
      write( (const char *)&m_data[i1],1  );
   }
}


void CPhonyBLCore::commandEraseMemory()
{
   int number=getInput();
   static char value[0x100];
   int _xor=0;
   bool massErase=false;

   if(number==0xFF)
   {
      value[0] = getInput();
      massErase = true;
   }
   else
   {
      lFatal("Not implemented");
      lAssert( number+1 <= 0x100 );
      for(int i1=0; i1<number+1; i1++)
      {
         value[i1] = getInput();
      }
      _xor=getInput();
      if ( ! checkXor( number, (char *)value, number+1, _xor) )
      {
         nack();
         return;
      }
   }
   if(massErase)
   {
      eraseFlash();
   }

   ack();

   writeSignal.emitSignal(0);
}


void CPhonyBLCore::commandWriteMemory()
{
   uint32_t  addr=0;
   uint8_t  _xor=0;
   uint8_t  num=0;
   int readValue;

   for(int i1=0; i1<4; i1++)
   {
      addr |= getInput() << ((3 - i1)*8);
   }
   _xor=getInput();
   if ( ! checkXor( 0, (char *)&addr, 4, _xor) )
   {
      nack();
      return;
   }

   ack();
   readValue=getInput();
   if(readValue==-1)
   {
      lWarning("Error reading num");
      return;
   }
   num=readValue;

   for(int i1=0; i1<num+1; i1++)
   {
      int i2=getInput();
      m_data[i1]=i2;
      if(i2==-1)
      {
         lWarning("Error reading data %d of num=%d", i1, num);
         return;
      }
   }
   readValue=getInput();
   _xor=readValue;
   if ( ! checkXor( num, (char *)&m_data, num+1, _xor) )
   {
      nack();
      return;
   }
   
   lDebug( "Writing flash memory, %d bytes", num+1 );
   writeFlashData( addr, m_data, num+1);
   ack();

   writeSignal.emitSignal(addr);
}


void CPhonyBLCore::commandExtEraseMemory( void )
{
   // No support yet
}


void CPhonyBLCore::commandSpecial( void )
{
   uint32_t  addr=0;
   uint8_t  _xor=0;
   for(int i1=0; i1<2; i1++)
   {
      addr |= getInput() << ((1 - i1)*8);
   }
   _xor=getInput();
   if ( ! checkXor( 0, (char *)&addr, 4, _xor) )
   {
      nack();
      return;
   }

   ack();
   
   // Nothing implemented yet. Not needed for flashing only.

   lDebug("Special command: 0x%lX", addr);
   return;
}


/*--- Fin ------------------------------------------------------------------*/
