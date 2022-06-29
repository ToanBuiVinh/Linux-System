//////////////////////////////////////////////////////////////
//
//Copyright 2014 Matthias Fuchs
//Copyright 2016 Volansys Technologies
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS, 
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
//
// Description:
/// \file gpio.c
/// \brief control GPIO from user space via /sys
//
/// This file provides apis to control the GPIO from userspace
//
//////////////////////////////////////////////////////////////
//
// History
//
// May/17/2015, Dhvanil P, Created (description)
//
//////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "gpio.h"


///////////////////////////////////////////////////////////////////////////////
// Function Definations
///////////////////////////////////////////////////////////////////////////////

/// Export GPIO pin 
int32_t GPIOExport( int32_t pin_ )
{
        char buffer[BUFFER_MAX];
        ssize_t bytes_written;
        int32_t fd;
        fd = open( "/sys/class/gpio/export", O_WRONLY );
        if ( -1 == fd )
        {
                fprintf( stderr, "Failed to open export for writing!\n" );
                return( -1 );
        }

        bytes_written = snprintf( buffer, BUFFER_MAX, "%d", pin_ );
        write( fd, buffer, bytes_written );
        close( fd );
        return( 0 );
}

/// Set GPIO direction
int32_t GPIODirection( int32_t pin_, int32_t dir_ )
{
        static const char s_directions_str[]  = "in\0out";

        char path[DIRECTION_MAX];
        int32_t fd;
        snprintf( path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin_ );
        fd = open( path, O_WRONLY );
        if ( -1 == fd )
        {
                fprintf( stderr, "Failed to open gpio direction for writing!\n" );
                return( -1 );
        }

        if ( -1 == write( fd, &s_directions_str[IN == dir_ ? 0 : 3], IN == dir_ ? 2 : 3 ) )
        {
                fprintf( stderr, "Failed to set direction!\n" );
                return( -1 );
        }

        close( fd );
        return( 0 );
}

/// Write value on GPIO pin
int32_t GPIOWrite( int32_t pin_, int32_t value_ )
{
        static const char s_values_str[] = "01";

        char path[VALUE_MAX];
        int32_t fd;
        snprintf( path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin_ );
        fd = open( path, O_WRONLY );
        if ( -1 == fd )
        {
                fprintf( stderr, "Failed to open gpio value for writing!\n" );
                return( -1 );
        }

        if ( 1 != write( fd, &s_values_str[LOW == value_ ? 0 : 1], 1 ) )
        {
                fprintf( stderr, "Failed to write value!\n" );
                return( -1 );
        }

        close( fd );
        return( 0 );
}
