
:SetUp
@echo off

rem 
rem Compiling a kernel base for x86 processors.
rem The binary format is PE. $.
rem This script generate the file KERNEL.BIN.
rem 
rem (c) Copyright 2005-2018 Fred Nora.
rem

:Start_Job0
rem Main.
echo ~{ ...
echo ~{ Compiling KERNEL.BIN ...
echo ~{ Main.
	gcc -c k/main.c -I./include -o main.o 
	
:Hal_Job1	
rem hal	
echo ~{ ...
echo ~{ ux4 /hal	
	gcc -c k/hal/hal.c -I./include -o hal.o	
	
	
	
:Microkernel_Job2	
echo ~{ ...
echo ~{ ux3 /mk
    
	rem head.s
	nasm  -I./k/mk/head/x86/  k/mk/head/x86/head.s  -f elf -o head.o
    rem c. 
    gcc -c  k/mk/mk.c -I./include  -o mk.o
	
	rem cpu
	gcc -c  k/mk/cpu/context.c -I./include  -o context.o
	gcc -c  k/mk/cpu/start.c -I./include  -o start.o
	
	rem
	rem /pc process control ( ipc, sched, mm )
	rem
	
	gcc -c  k/mk/pc/ts.c -I./include -o ts.o	
    gcc -c  k/mk/pc/dispatch.c  -I./include  -o dispatch.o	
	gcc -c  k/mk/pc/queue.c     -I./include  -o queue.o
	gcc -c  k/mk/pc/process.c   -I./include  -o process.o
    gcc -c  k/mk/pc/pheap.c     -I./include  -o pheap.o	
    gcc -c  k/mk/pc/thread.c    -I./include  -o thread.o		
    gcc -c  k/mk/pc/threadi.c   -I./include  -o threadi.o
    gcc -c  k/mk/pc/theap.c     -I./include  -o theap.o
    gcc -c  k/mk/pc/tstack.c    -I./include  -o tstack.o	
    gcc -c  k/mk/pc/tasks.c     -I./include  -o tasks.o	
	gcc -c  k/mk/pc/create.c    -I./include  -o create.o
	
	rem mk/pc/ipc 
	gcc -c  k/mk/pc/ipc/spawn.c  -I./include  -o spawn.o
	gcc -c  k/mk/pc/ipc/callout.c  -I./include  -o callout.o
	gcc -c  k/mk/pc/ipc/callfar.c  -I./include  -o callfar.o
    gcc -c  k/mk/pc/ipc/ipc.c  -I./include  -o ipc.o	
    gcc -c  k/mk/pc/ipc/sem.c  -I./include  -o sem.o
	rem ...
	
	rem mk/pc/sched
    gcc -c  k/mk/pc/sched/sched.c  -I./include -o sched.o
    gcc -c  k/mk/pc/sched/schedi.c -I./include -o schedi.o    	
	
	rem /mk/pc/mm (memory manager)
    gcc -c k/mk/pc/mm/memory.c  -I./include -o memory.o    
	rem /mk/pc/mm (memory manager)
    gcc -c k/mk/pc/mm/pages.c   -I./include -o pages.o 	
	
	gcc -c  k/mk/request.c -I./include -o request.o	
	gcc -c  k/mk/faults.c -I./include -o faults.o		
	
	
:Executive_Job3	
echo ~{ ...
echo ~{ ux2 /execve
	rem /execve
	gcc -c k/execve/execve.c  -I./include -o execve.o	
	
	rem services - System Call Interface.  
    gcc -c k/execve/sci/services.c  -I./include -o services.o		
	
    rem execve/dd
		
	rem sm
	rem /execve/init
    gcc -c k/execve/dd/sm/init/init.c -I./include -o init.o
	rem /execve/sm/ob
	gcc -c k/execve/dd/sm/ob/object.c -I./include -o object.o		
	rem /execve/network
	gcc -c k/execve/dd/sm/network/nic.c     -I./include -o nic.o
	gcc -c k/execve/dd/sm/network/network.c -I./include -o network.o
	gcc -c k/execve/dd/sm/network/socket.c  -I./include -o socket.o
	rem System Management.
	
	rem /execve/sm/sys
    rem system
    gcc -c k/execve/dd/sm/sys/modules.c  -I./include -o modules.o
    rem procedure
    gcc -c k/execve/dd/sm/sys/proc.c  -I./include -o proc.o	
    rem abort
    gcc -c k/execve/dd/sm/sys/abort.c  -I./include -o abort.o		
    rem info
    gcc -c k/execve/dd/sm/sys/info.c  -I./include -o info.o	
    rem server manager
    gcc -c k/execve/dd/sm/sys/sm.c  -I./include -o sm.o
    rem channel
    gcc -c k/execve/dd/sm/sys/channel.c  -I./include -o channel.o	
	rem signal
    gcc -c k/execve/dd/sm/sys/signal.c  -I./include -o signal.o	
    rem system
    gcc -c k/execve/dd/sm/sys/system.c  -I./include -o system.o
	rem io
	gcc -c k/execve/dd/sm/sys/io.c -I./include -o io.o	
	
    rem sm
	rem /execve/rt (runtime)
	gcc -c k/execve/dd/sm/rt/runtime.c -I./include -o runtime.o
	
	rem tty
    gcc -c k/execve/dd/tty/tty.c -I./include -o tty.o		
	
	rem uitm
	rem /execve/uitm/libk
    gcc -c k/execve/dd/uitm/libk/misc.c   -I./include -o misc.o	
	
	rem uitm
	rem /execve/uitm/libc
	gcc -c k/execve/dd/uitm/libc/stdlib.c -I./include -o stdlib.o
	gcc -c k/execve/dd/uitm/libc/stdio.c  -I./include -o stdio.o
    gcc -c k/execve/dd/uitm/libc/string.c -I./include -o string.o
	
	rem uigm Nothing for now.	
		
	rem sm
	rem /execve/sm/disk
    gcc -c k/execve/dd/sm/disk/disk.c   -I./include -o disk.o						
    gcc -c k/execve/dd/sm/disk/volume.c -I./include -o volume.o	
	
  	rem sm
    gcc -c k/execve/dd/sm/install/install.c -I./include -o install.o    
	
	rem sm
	rem /execve/debug
	gcc -c k/execve/dd/sm/debug/debug.c -I./include -o debug.o	
	
	gcc -c k/execve/dd/device/device.c -I./include -o device.o
	
	rem (unblocked) legados (K2).
	gcc -c k/execve/dd/unb/video.c   -I./include -o video.o  
	gcc -c k/execve/dd/unb/screen.c  -I./include -o screen.o 	
	gcc -c k/execve/dd/unb/keyboard.c  -I./include -o keyboard.o
    gcc -c k/execve/dd/unb/mouse.c  -I./include -o mouse.o  	
	gcc -c k/execve/dd/unb/ldisc.c  -I./include -o ldisc.o    	
	gcc -c k/execve/dd/unb/hdd.c    -I./include -o hdd.o	
	gcc -c k/execve/dd/unb/pic.c   -I./include -o pic.o 
    gcc -c k/execve/dd/unb/timer.c     -I./include -o timer.o	
	gcc -c k/execve/dd/unb/rtc.c    -I./include -o rtc.o
	gcc -c k/execve/dd/unb/vsync.c  -I./include -o vsync.o 
	gcc -c k/execve/dd/unb/arch/x86/syscall.c  -I./include -o syscall.o		
	gcc -c k/execve/dd/unb/arch/x86/cpux86.c  -I./include -o cpux86.o 
    gcc -c k/execve/dd/unb/arch/x86/x86.c     -I./include -o x86.o	
	gcc -c k/execve/dd/unb/arch/x86/ports.c   -I./include -o ports.o 	
	gcc -c k/execve/dd/unb/arch/amd/cpuamd.c  -I./include -o cpuamd.o 	    
	
	rem (blocked) (K1).
	gcc -c k/execve/dd/b/pci.c -I./include -o pci.o
	gcc -c k/execve/dd/b/usb.c -I./include -o usb.o
	gcc -c k/execve/dd/b/apic.c -I./include -o apic.o 

	rem File system
	
	rem /execve/fs
    gcc -c k/execve/fs/fs.c     -I./include -o fs.o						
    gcc -c k/execve/fs/read.c   -I./include -o read.o			
    gcc -c k/execve/fs/write.c  -I./include -o write.o			
    gcc -c k/execve/fs/cf.c     -I./include -o cf.o			
    gcc -c k/execve/fs/search.c -I./include -o search.o			
    gcc -c k/execve/fs/format.c -I./include -o format.o	
    gcc -c k/execve/fs/vfs.c    -I./include -o vfs.o		
	
echo ~{ ...
echo ~{ ux1 /gramado \o/	

	rem k\gramado\gui	
    rem char
    gcc -c k/gramado/gui/char.c -I./include -o char.o
    rem pixel
    gcc -c k/gramado/gui/pixel.c -I./include -o pixel.o 
    rem line
    gcc -c k/gramado/gui/line.c -I./include -o line.o	
    rem rect
    gcc -c k/gramado/gui/rect.c -I./include -o rect.o 
    rem draw
    gcc -c k/gramado/gui/dtext.c -I./include -o dtext.o
    rem draw
    gcc -c k/gramado/gui/font.c -I./include -o font.o	
    rem createw
    gcc -c k/gramado/gui/createw.c -I./include -o createw.o    
    rem grid
    gcc -c k/gramado/gui/grid.c -I./include -o grid.o 
    rem statusbar
    gcc -c k/gramado/gui/sbar.c -I./include -o sbar.o 
    rem menubar
    gcc -c k/gramado/gui/menubar.c -I./include -o menubar.o 
    rem menu
    gcc -c k/gramado/gui/menu.c -I./include -o menu.o 
    rem bmp
    gcc -c k/gramado/gui/bmp.c -I./include -o bmp.o 
    rem button
    gcc -c k/gramado/gui/button.c -I./include -o button.o 
    rem messagebox
    gcc -c k/gramado/gui/mbox.c -I./include -o mbox.o
    rem dialogboxbox
    gcc -c k/gramado/gui/dbox.c -I./include -o dbox.o
	rem background
    gcc -c k/gramado/gui/bg.c -I./include -o bg.o  
    rem window
    gcc -c k/gramado/gui/window.c -I./include -o window.o    
    rem desktop
    gcc -c k/gramado/gui/desktop.c -I./include -o desktop.o  
    rem window station
    gcc -c k/gramado/gui/wstation.c -I./include -o wstation.o    
    rem user section
    gcc -c k/gramado/gui/usession.c -I./include -o usession.o        
    
	rem gramado logon e logoff
	
	rem /gramado/logon
    gcc -c k/gramado/logon/logon.c -I./include -o logon.o 
	rem /gramado/logoff
    gcc -c k/gramado/logoff/logoff.c -I./include -o logoff.o 

	rem gramado/user
	gcc -c k/gramado/user/userenv.c -I./include -o userenv.o 	

	rem *gui (GRAMADO)
    gcc -c k/gramado/gramado.c -I./include -o gramado.o    	
	

rem 
rem ## Objects ##
rem 
	
:Objects	
set myObjects=head.o ^
main.o ^
io.o ^
hal.o ^
mk.o ^
execve.o ^
video.o ^
screen.o ^
runtime.o ^
ports.o ^
x86.o ^
stdio.o ^
stdlib.o ^
string.o ^
tty.o ^
init.o ^
logon.o ^
logoff.o ^
cpux86.o ^
cpuamd.o ^
memory.o ^
pages.o ^
apic.o ^
pic.o ^
timer.o ^
rtc.o ^
keyboard.o ^
mouse.o ^
ldisc.o ^
hdd.o ^
pci.o ^
usb.o ^
device.o ^
spawn.o ^
callout.o ^
callfar.o ^
process.o ^
pheap.o ^
thread.o ^
threadi.o ^
theap.o ^
tstack.o ^
create.o ^
start.o ^
context.o ^
tasks.o ^
ts.o ^
queue.o ^
sched.o ^
schedi.o ^
dispatch.o ^
sem.o ^
modules.o ^
nic.o ^
network.o ^
socket.o ^
sm.o ^
channel.o ^
ipc.o ^
read.o ^
write.o ^
cf.o ^
search.o ^
format.o ^
disk.o ^
volume.o ^
fs.o ^
vfs.o ^
misc.o ^
request.o ^
info.o ^
signal.o ^
system.o ^
bg.o ^
grid.o ^
sbar.o ^
menubar.o ^
menu.o ^
mbox.o ^
dbox.o ^
button.o ^
char.o ^
pixel.o ^
line.o ^
rect.o ^
dtext.o ^
font.o ^
vsync.o ^
bmp.o ^
createw.o ^
window.o ^
desktop.o ^
wstation.o ^
usession.o ^
gramado.o ^
object.o ^
userenv.o ^
proc.o ^
syscall.o ^
install.o ^
debug.o ^
abort.o ^
faults.o ^
services.o  



:Linking_Job4 
echo ~{ ...
echo ~{ Linking objects ...
   ld -T link.ld -o KERNEL.BIN %myObjects%  -Map kmap.s
   
:Moving_Job5  
echo ~{ ...
echo ~{ Moving ...   
    move KERNEL.BIN bin\KERNEL.BIN
	
:Clean_Job6	
echo ~{ ...
echo ~{ Clean.
    del *.o
 
:End 
echo End!
	pause
	