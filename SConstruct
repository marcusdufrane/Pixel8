import SCons
from SCons.Warnings import VisualCMissingWarning
import os

SCons.Warnings.suppressWarningClass(VisualCMissingWarning)
env = Environment(ENV = os.environ, tools=['mingw'])

uC = 'atmega328'
Target = 'pixel8'

env.Replace(CC = 'avr-gcc -mmcu='+uC+' -O2')
env.Replace(CPPPATH = '#/inc/')
env.Replace(CPPPREIX = '*/src/')
env.Replace(CPPDEFINES = 'F_CPU=14745600')
env.Replace(PROGPREFIX = '#/lib/')
env.Replace(PROGSUFFIX = '.elf')
env.Replace(OBJPREFIX = '#/lib/')

source_files = ['bbtUart.c', 'main.c', 'memoryFunctions.c', 'spi.c',
				'timer_1.c', 'usrLed.c', 'utils.c']
source_files = ['src/'+x for x in source_files]

env.Program(Target, Flatten(source_files))

#generate .hex file
env.Command('lib/'+Target+'.hex', 'lib/'+Target+'.elf', 'avr-objcopy -j .text -j .data -O ihex $SOURCE $TARGET')
if int(ARGUMENTS.get('send', 0)):
	#program controller
	env.Command(None, 'lib/'+Target+'.hex', 'avrdude -p m328p -c avrispmkII -P usb -U flash:w:$SOURCE -F -V')