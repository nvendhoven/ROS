; Author: Daniel Versluis
; Email: VersD@hr.nl
; Description: Contains all neccesary low-level functions
; for pushing and popping CPU/Stack

        .global pushRegistersToCurrentPSP
        .global popRegistersFromCurrentPSP
        .global readPSP
        .global readMSP
        .global writePSP
        .global writeMSP
		
pushRegistersToCurrentPSP:
		MRS 	R0	, psp			;Move PSP to register R0 (the function argument)
		STMDB 	R0!	, {r4-r11}		;Store multiple registers decrement address before each access,
									;when done write back last used address to %0 (new SP location)
		MSR 	psp	, R0			;Move register %0 to psp to update to the new SP, output to scratch
		MOV		PC	, LR			;return by moving link-register to program counter


popRegistersFromCurrentPSP:
		MRS 	R0	, psp			;Move PSP to register R0
		LDMFD 	R0!	, {r4-r11}		;Load multiple registers increment address after each access,
									;when done write back last used address to R0 (new SP location)
		MSR 	psp	, R0			;Move register R0 to psp to update to the new SP, output to scratch
		MOV		PC	, LR			;return by moving link-register to program counter

readPSP:
		MRS 	R0 , psp			;move psp to r0
		MOV		PC	, LR			;return by moving link-register to program counter

readMSP:
		MRS 	R0 , msp			;move msp to r0
		MOV		PC	, LR			;return by moving link-register to program counter

writePSP:
    	MSR 	psp	, R0			;Move r0 to psp.
		MOV		PC	, LR			;return by moving link-register to program counter

writeMSP:
    	MSR 	msp	, R0			;Move r0 to msp.
		MOV		PC	, LR			;return by moving link-register to program counter
