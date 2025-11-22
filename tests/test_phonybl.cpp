/**---------------------------------------------------------------------------
 *
 * @file       test_phonybl.cpp
 * @brief      Test phonybl
 *
 * @date       20220619
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#if defined ( CATCH_V3 )
#include <catch2/catch_test_macros.hpp>
#elif defined ( CATCH_V2 )
#include <catch2/catch.hpp>
#elif defined ( CATCH_V1 )
#include <catch/catch.hpp>
#else
#error "Either 'catch' or 'catch2' has to be installed"
#endif

#include <phonybl/phonybl.hpp>

#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "Phony-BL", "[default]" )
{
   SECTION( "Phony-BL" )
   {
      CPhonyBL bl;
      
      // No idea what to test yet.
   }
}


/*--- Fin ------------------------------------------------------------------*/
