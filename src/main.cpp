/**---------------------------------------------------------------------------
 *
 * @brief   Main file for libphonybl test application 'phony'
 *
 * Dummy application which can interact with STM32CubeProgrammer.
 *
 * @date   20240908
 * @author Maximilian Seesslen <code@seesslen.net>
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QFile>
#include <QDebug>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QDir>
#include <signal.h>

#include <phonybl/phonybl_host.hpp>


/*--- Implementation -------------------------------------------------------*/


int main(int argc, char *argv[])
{
   QCoreApplication qapp(argc, argv);
   int sta=0;
   CPhonyBL *bl=nullptr;

   QCoreApplication::setApplicationName("Phony-BL");
   QCoreApplication::setApplicationVersion( "0.0.0" );
   
   QCommandLineParser parser;
   parser.setApplicationDescription("Simulates STM32 bootloader");
   parser.addHelpOption();
   parser.addVersionOption();
   
   QCommandLineOption oDevice(QStringList() << "d" << "device",
                                  "Device file for serial interface", "device");
   
   parser.addOptions( { oDevice } );
   parser.process(qapp);
   
   if( !parser.isSet( oDevice ) )
   {
      parser.showHelp(22);
   }
   
   bl=new CPhonyBL( parser.value( oDevice ) );

   qInfo("Entering event loop...");
   while(1)
   {
      bl->eventLoop();
   }

   return(0);
}


/*--- Fin ------------------------------------------------------------------*/
