.macro  gfx_data
        ;; \1 = label name
        ;; \2 = file name
        .phrase
 	.globl	\1
\1:
        incbin  \2
.endm

.macro  gfx_data_packed
        ;; \1 = label name
        ;; \2 = file name
        .long
 	.globl	\1
\1:
        incbin  \2
.endm

.macro	animation_begin
	;; \1 = label name
	.long
 	.globl	\1
\1:
.endm

.macro	animation_chunk
	;; \1 = speed
	;; \2 = gfx data address
	dc.l	\2
	dc.w	\1
	dc.w	0
.endm
	
.macro	animation_end
	;; \?1 = loop index
	.if	\?1
	animation_chunk	\1,0
	.else
	animation_chunk	0,0
	.end
.endm
	
.macro  audio_data
        ;; \1 = label name
        ;; \2 = file name
        .phrase
 	.globl	\1
\1:
        incbin  \2
.endm
	
.macro  audio_data_packed
        ;; \1 = label name
        ;; \2 = file name
        .long
 	.globl	\1
\1:
        incbin  \2
.endm



