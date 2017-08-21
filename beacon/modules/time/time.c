/*
 * time.c
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina
 * 
 * This file is part of FloripaSat-Beacon.
 * 
 * FloripaSat-Beacon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-Beacon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-Beacon. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file time.c
 * 
 * \brief Time control variables implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 10/06/2017
 * 
 * \addtogroup time
 * \{
 */

#include "time.h"

void time_reset(Time *time)
{
    time->millisecond   = 0x0000;
    time->second        = 0x00;
    time->minute        = 0x00;
    time->hour          = 0x00;
    time->day           = 0x00;
    time->week          = 0x00;
    time->month         = 0x00;
    time->year          = 0x00;
}

uint8_t time_compare(Time *time_ref, Time *time_cmp)
{
    if (time_cmp->day < time_ref->day)
    {
        return TIME_REF_GREATER_THAN_CMP;
    }
    else if (time_cmp->day > time_ref->day)
    {
        return TIME_CMP_GREATER_THAN_REF;
    }
    
    if (time_cmp->hour < time_ref->hour)
    {
        return TIME_REF_GREATER_THAN_CMP;
    }
    else if (time_cmp->hour > time_ref->hour)
    {
        return TIME_CMP_GREATER_THAN_REF;
    }
    
    if (time_cmp->minute < time_ref->minute)
    {
        return TIME_REF_GREATER_THAN_CMP;
    }
    else if (time_cmp->minute > time_ref->minute)
    {
        return TIME_CMP_GREATER_THAN_REF;
    }
    
    if (time_cmp->second < time_ref->second)
    {
        return TIME_REF_GREATER_THAN_CMP;
    }
    else if (time_cmp->second > time_ref->second)
    {
        return TIME_CMP_GREATER_THAN_REF;
    }
    
    return TIME_CMP_EQUAL_REF;
}

void time_diff(Time time_a, Time time_b, Time *diff)
{    
    
}

void time_copy(Time *time_a, Time *time_b)
{
    time_b->millisecond = time_a->millisecond;
    time_b->second      = time_a->second;
    time_b->minute      = time_a->minute;
    time_b->hour        = time_a->hour;
    time_b->day         = time_a->day;
    time_b->week        = time_a->week;
    time_b->month       = time_a->month;
    time_b->year        = time_a->year;
}

//! \} End of time group
