import SCons
from SCons.Warnings import VisualCMissingWarning
import os

SCons.Warnings.suppressWarningClass(VisualCMissingWarning)
env = Environment(ENV = os.environ, tools=['mingw'], variant_dir='#/lib/obj')

uC = 'atmega328'
Target = 'pixel8'

env.Replace(CC = 'avr-gcc -mmcu='+uC)
env.Append(CFLAGS = ['-O2', '-std=gnu99'])
env.Replace(CPPPATH = '#/inc/')
env.Replace(CPPDEFINES = ['F_CPU=14745600'])
env.Replace(PROGSUFFIX = '.elf')

elf = env.Program('#/lib/'+Target, env.Glob('#/src/*.c'))

#generate .hex file
hex = env.Command('#/lib/'+Target+'.hex', elf, 'avr-objcopy -j .text -j .data -O ihex $SOURCE $TARGET')

burn = env.Command(None, hex, 'avrdude -p m328p -c avrispmkII -V -P usb -U flash:w:$SOURCE -F')
	
boot_env = env.Clone()
boot_env.Append(LINKFLAGS = ['-Wl,,--section-start=.text=0xC00'])
boot_loader_elf = boot_env.Program('#/lib/'+Target+'_boot', 'boot.c')
boot_loader_hex = boot_env.Command('#/lib/'+Target+'_boot.hex', boot_loader_elf, 'avr-objcopy -j .text -j .data -O ihex $SOURCE $TARGET')

env.Alias('boot', boot_loader_hex)
env.Alias('app', hex)
env.Alias('burn', burn, hex)
env.Default('app')