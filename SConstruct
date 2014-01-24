import SCons
from SCons.Warnings import VisualCMissingWarning
import os

SCons.Warnings.suppressWarningClass(VisualCMissingWarning)
env = Environment(ENV = os.environ, tools=['mingw'], variant_dir='#/lib/obj')

uC = 'atmega328'
Target = 'pixel8'

env.Replace(CC = 'avr-gcc -mmcu='+uC+' -O2')
env.Replace(CPPPATH = '#/inc/')
env.Replace(CPPDEFINES = 'F_CPU=14745600')
env.Replace(PROGSUFFIX = '.elf')

elf = env.Program('#/lib/'+Target, env.Glob('#/src/*.c'))

#generate .hex file
hex = env.Command('#/lib/'+Target+'.hex', elf, 'avr-objcopy -j .text -j .data -O ihex $SOURCE $TARGET')
env.Default(hex)
if int(ARGUMENTS.get('send', 0)):
	#program controller
	env.Command(None, hex, 'avrdude -p m328p -c avrispmkII -P usb -U flash:w:$SOURCE -F -V')
