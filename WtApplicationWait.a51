;#include <rtx51tny.h>
;unsigned char wt_application_wait(unsigned char type, unsigned int count, unsigned char tick)
;{
;	unsigned char ret = TMO_EVENT;
;	while(count-- && ret == TMO_EVENT)
;	{
;		ret = os_wait2(type, tick);
;	}
;	return ret;
;}

NAME	WTAPPLICATIONWAIT
?PR?_wt_application_wait?WTAPPLICATIONWAIT       SEGMENT CODE 
	EXTRN	CODE (_os_wait2)
	PUBLIC	_wt_application_wait 
; unsigned char wt_application_wait(unsigned char type, unsigned int count, unsigned char tick)
	RSEG  ?PR?_wt_application_wait?WTAPPLICATIONWAIT
_wt_application_wait:
	USING	0		
	MOV	R0,AR3;tick
	MOV R3,AR7;type
	MOV	R1,AR4;count msb
	MOV	R2,AR5;count lsb	
; {			
; 	unsigned char ret = TMO_EVENT;
	MOV  	R7,#08H
?C0001:
; 	while(count-- && ret == TMO_EVENT)
	MOV  	A,R2
	DEC  	R2
	MOV  	R4,AR1
	JNZ  	?C0004
	DEC  	R1
?C0004:
	ORL  	A,R4
	JZ   	?C0002
	MOV  	A,R7
	XRL  	A,#08H
	JNZ  	?C0002
; 	{			
; 		ret = os_wait2(type, tick);
	PUSH	AR0;
	PUSH	AR1
	PUSH	AR2
	PUSH 	AR3
	MOV  	R5,AR0;tick
	MOV  	R7,AR3;type
	LCALL	_os_wait2
	POP		AR3
	POP		AR2
	POP		AR1
	POP		AR0
; 	}			
	SJMP 	?C0001
?C0002:
; 	return ret;			
; }			
	RET  	
; END OF _wt_application_wait
	END
