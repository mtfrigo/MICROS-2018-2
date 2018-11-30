/*
  rotary.c: Reads the voltage of a potenciometer on Galileo Gen2 ADC_AD0
  in one-shot mode.

  Copyright (c) 2016 Walter Fetter Lages <w.fetter@ieee.org>
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    You can also obtain a copy of the GNU General Public License
    at <http://www.gnu.org/licenses>.
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <galileo2io.h>

int main(int argc,char * argv[])
{
    int fd;
    int fd_LED;
    char data_str[80];
    double scale;
    int raw;
    char state = '0';

    if((fd=open("/sys/bus/iio/devices/iio:device0/in_voltage0_raw",O_RDONLY)) < 0)
    {
            perror("Opening in_voltage0raw:");
            return -1;
    }

    if((fd_LED=open("/sys/class/gpio/gpio62/value",O_WRONLY)) < 0)
    {
            perror("Opening GPIO 62:");
            return -1;
    }

    pgets(data_str,sizeof data_str,"/sys/bus/iio/devices/iio:device0/in_voltage0_scale");
    scale=atof(data_str)/1000.0;

    for(;;)
    {
            lseek(fd,0,SEEK_SET);
            read(fd,data_str,sizeof data_str);
            raw=atoi(data_str);

            if(raw > 2000)
            {
                state = '1';
                write(fd_LED,&state,sizeof state);
                printf("PARABENS SEU GRITO EH FORTE");
            }
            else
            {
                state = '0';
                write(fd_LED,&state,sizeof state);
                printf("GRITA MAIS PORRA FDP");
            }


            printf("Raw=%d\tScale=%f\tVoltage=%fV\n",raw,scale,raw*scale);
    sleep(1);
    }

    close(fd);

    return 0;
}
