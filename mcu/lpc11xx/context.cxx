// Copyright (C) 2016 kaz Kojima
//
// This file is part of ef runtime library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the EF Runtime Library Exception.

// You should have received a copy of the GNU General Public License and
// a copy of the EF Runtime Library Exception along with this program;
// see the files COPYING and EXCEPTION respectively.

// LPC11xx dependant context and support routines

// Switch thread immediately
void __attribute__ ((naked))
ef::exc::sched ()
{
  // Emurate exception
  asm volatile ("mov	r1, lr\n\t"
		"mov	r2, r1\n\t"
		"mov	r3, #128\n\t"
		"lsl	r3, #17\n\t"
		"push	{r1, r2, r3}\n\t"
		"mov	r1, #0\n\t"
		"mov	r2, r1\n\t"
		"mov	r3, r1\n\t"
		"push	{r1, r2, r3}\n\t"
		"push	{r1, r2}"
		: /* no output*/
		: /* no input */
		: "r1", "r2", "r3", "memory");

  // Get ef::thread::running and save context
  asm volatile ("ldr	r1, =_ZN2ef6thread7runningE\n\t"
		"ldr	r0, [r1]\n\t"
		"stm	r0!, {r4, r5, r6, r7}\n\t"
		"mov	r2, r8\n\t"
		"mov	r3, r9\n\t"
		"mov	r4, r10\n\t"
		"mov	r5, r11\n\t"
		"mov	r6, sp\n\t"
		"mov	r7, #0\n\t"
		"sub	r7, #3\n\t"
		"stm	r0!, {r2, r3, r4, r5, r6, r7}"
		: /* no output */
		: /* no input */
		: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "memory");

  bitset r = thread::runnable;
  id_t id = r.pop ();
  thread *np = thread::pvec[id];
  thread::running = np;

  // Switch to new thread
  asm volatile ("ldm	r0!, {r4, r5, r6, r7}\n\t"
		"ldm	r0!, {r1, r2, r3}\n\t"
		"mov	r8, r1\n\t"
		"mov	r9, r2\n\t"
		"mov	r10, r3\n\t"
		"ldm	r0!, {r1, r2}\n\t"
		"mov	r11, r1\n\t"
		"mov	sp, r2\n\t"
		"ldm	r0!, {r1, r2}\n\t"
		"mov	lr, r1\n\t"
		"msr	PRIMASK, r2\n\t"
		"isb\n\t"
		"ldr	r0, [sp, #28]\n\t"
		"lsl	r1, r0, #23\n\t"
		"bcc	2f\n\t"
		//
		"ldr	r2, [sp, #24]\n\t"
		"mov	r1, #1\n\t"
		"orr	r2, r1\n\t"	// Ensure Thumb-mode
		"str	r2, [sp, #32]\n\t"
		"msr	APSR_nzcvq, r0\n\t"
		//
		"ldr	r0, [sp, #20]\n\t"
		"mov	lr, r0\n\t"
		"ldr	r0, [sp, #16]\n\t"
		"mov	r12, r0\n\t"
		"pop	{r0, r1, r2, r3}\n\t"
		"add	sp, #16\n\t"
		"pop	{pc}\n"
	"2:\n\t"
		"ldr	r2, [sp, #24]\n\t"
		"mov	r1, #1\n\t"
		"orr	r2, r1\n\t"	// Ensure Thumb-mode
		"str	r2, [sp, #28]\n\t"
		"msr	APSR_nzcvq, r0\n\t"
		//
		"ldr	r0, [sp, #20]\n\t"
		"mov	lr, r0\n\t"
		"ldr	r0, [sp, #16]\n\t"
		"mov	r12, r0\n\t"
		"pop	{r0, r1, r2, r3}\n\t"
		"add	sp, #12\n\t"
		"pop	{pc}"
		: /* no output */
		: "l" (np)
		: "r1", "r2", "r3", "r4", "r5", "r6", "r7", "memory");
}

// Exception handlers

// Support routine
void __attribute__ ((naked))
ef::context::restore (void)
{
  asm volatile ("ldm	r0!, {r4, r5, r6, r7}\n\t"
		"ldm	r0!, {r1, r2, r3}\n\t"
		"mov	r8, r1\n\t"
		"mov	r9, r2\n\t"
		"mov	r10, r3\n\t"
		"ldm	r0!, {r1, r2}\n\t"
		"mov	r11, r1\n\t"
		"msr	PSP, r2\n\t"
		"ldm	r0!, {r1, r2}\n\t"
		"mov	lr, r1\n\t"
		"msr	PRIMASK, r2\n\t"
		"isb\n\t"
		"bx	lr"
		: /* no output */
		: "l" (this)
		: "r1", "r2", "r3", "r4", "r5", "r6", "r7", "memory");
}

// PendSV
void __attribute__ ((naked))
ef::exc::preempt (void)
{
  register thread *tp asm ("r0");

  asm volatile (// Get ef::thread::running and save context
		"ldr	r1, =_ZN2ef6thread7runningE\n\t"
		"ldr	r0, [r1]\n\t"
		"stm	r0!, {r4, r5, r6, r7}\n\t"
		"mov	r2, r8\n\t"
		"mov	r3, r9\n\t"
		"mov	r4, r10\n\t"
		"mov	r5, r11\n\t"
		"mrs	r6, PSP\n\t"
		"mov	r7, lr\n\t"
		"stm	r0!, {r2, r3, r4, r5, r6, r7}"
		: "=r" (tp)
		: /* no input */
		: "r1", "r2", "r3", "r4", "r5", "r6", "r7", "memory");

  bitset r = thread::runnable;
  id_t id = r.pop ();
  thread *np = thread::pvec[id];
  thread::running = np;

  ((context *) np)->restore ();
}

// SysTick
void
ef::exc::systick (void)
{
  timers::update ();
}

// Generic IRQ handler
void
ef::exc::irq_handler (void)
{
  uint32_t n;

  asm ("mrs    %0, IPSR" : "=r" (n) : /* no input */ : "memory");
  // Exception # - 16 = interrupt number
  n -= 16;

  irq ir(n);
  ir.disable ();
  id_t id = ir.id ();
  if (irq::irqn (id) == n)
    eventflag::signal (id);
}
