# ef
tiny eventflag library

ef is a very simple runtime library for ARM cortex-m embedded system which gives limited event flag and threading functions.  Currently supports STM32F446 MCU only, though an experimental support for LPC11C24 is there.  
All timeout, interrupt and software signals are handled as event which has a uniue id.  A thread can wait a set of these events, i.e. one can easily write a routine driven by I/O interrupts or signals from other thread with timeout function.  
The basic idea comes from Chopstx written by gNiibe.  For Chopstx, see  
<http://git.gniibe.org/gitweb/?p=chopstx/chopstx.git;a=summary>

ef has strong limitations compared with the "real" embedded OSs like Chosptx, ChibiOS and so on.  Less than 64 unique ids for events.  No priority control and other things that modern systems have.  One will easily make dead lock or bad behavior happen on ef.  ef stands for "bare metal plus epsilon".  Although ef is written by C++ and some abstructions are done with class, they simply give services and syntax sugar for the bare metal systems in the end.  
I hope that it will help rapid test/proto-typing for various peripherals of MCUs despite its limitations and drawbacks.  

Examples:

* Wait interrupt with 5sec timeout

```c++
      bitset flags;

      // wait interrupt with timeout function
      thread::poll_section ();
      flags.clear ();
      id_t to_id = eventflag::timeout_event (5000*1000);
      flags.add (to_id);
      id_t irq_id = eventflag::irq_event (INTR_REQ_USART2);
      flags.add (irq_id);
      id_t id = thread::poll (flags);

```


