		.h8300s
		.section .vects
		.global	 _vector_table

		.extern	_start
                .extern _gdb_unhandled_isr

_vector_table:
                jmp     @_start          ; power-on reset
                jmp     @_start          ; manual reset

                .space  (3 * 4)          ; reserved

                jmp     @_gdb_unhandled_isr      ; Trace
                jmp     @_gdb_unhandled_isr      ; Direct Transition
                jmp     @_gdb_unhandled_isr      ; NMI   (reserved for HDI-M)
                jmp     @_gdb_unhandled_isr      ; TRAP0 (reserved for HDI-M)

                jmp     @_gdb_unhandled_isr      ; TRAP1
                jmp     @_gdb_trapa2_isr         ; TRAP2  (compiled-in break)
                jmp     @_gdb_trapa3_isr         ; TRAP3  (gdb/step breakpoint)

                .space  (4 * 4)          ; reserved

                jmp     @_gdb_unhandled_isr      ; IRQ0
                jmp     @_gdb_unhandled_isr      ; IRQ1
                jmp     @_gdb_unhandled_isr      ; IRQ2
                jmp     @_gdb_unhandled_isr      ; IRQ3
                jmp     @_gdb_unhandled_isr      ; IRQ4
                jmp     @_gdb_unhandled_isr      ; IRQ5
                jmp     @_gdb_unhandled_isr      ; IRQ6
                jmp     @_gdb_unhandled_isr      ; IRQ7

                jmp     @_gdb_unhandled_isr      ; SWDTEND
                jmp     @_gdb_unhandled_isr      ; WatchDog0
                jmp     @_gdb_unhandled_isr      ; CMI
                jmp     @_gdb_unhandled_isr      ; PC break
                jmp     @_gdb_unhandled_isr      ; A/D end
                jmp     @_gdb_unhandled_isr      ; WatchDog1

                .space  (2 * 4)          ; reserved

                jmp     @_gdb_unhandled_isr      ; TGI0A (compare/capture)
                jmp     @_gdb_unhandled_isr      ; TGI0B     -   "   -
                jmp     @_gdb_unhandled_isr      ; TGI0C     -   "   -
                jmp     @_gdb_unhandled_isr      ; TGI0D     -   "   -
                jmp     @_gdb_unhandled_isr      ; TCI0V (overflow)

                .space  (3 * 4)          ; reserved

                jmp     @_gdb_unhandled_isr      ; 40 TGI1A (compare/capture)
                jmp     @_gdb_unhandled_isr      ; TGI1B     -   "   -
                jmp     @_gdb_unhandled_isr      ; TCI1V (overflow)
                jmp     @_gdb_unhandled_isr      ; TCI1U (underflow)

                jmp     @_gdb_unhandled_isr      ; TGI2A (compare/capture)
                jmp     @_gdb_unhandled_isr      ; TGI2B     -   "   -
                jmp     @_gdb_unhandled_isr      ; TCI2V (overflow)
                jmp     @_gdb_unhandled_isr      ; TCI2U (underflow)

                jmp     @_gdb_unhandled_isr      ; TGI3A (compare/capture)
                jmp     @_gdb_unhandled_isr      ; TGI3B     -   "   -
                jmp     @_gdb_unhandled_isr      ; TGI3C     -   "   -
                jmp     @_gdb_unhandled_isr      ; TGI3D     -   "   -
                jmp     @_gdb_unhandled_isr      ; TCI3V (overflow)

                .space  (3 * 4)          ; reserved

                jmp     @_gdb_unhandled_isr      ; TGI4A (compare/capture)
                jmp     @_gdb_unhandled_isr      ; TGI4B     -   "   -
                jmp     @_gdb_unhandled_isr      ; TCI4V (overflow)
                jmp     @_gdb_unhandled_isr      ; TCI4U (underflow)

                jmp     @_gdb_unhandled_isr      ; TGI5A (compare/capture)
                jmp     @_gdb_unhandled_isr      ; TGI5B     -   "   -
                jmp     @_gdb_unhandled_isr      ; TCI5V (overflow)
                jmp     @_gdb_unhandled_isr      ; TCI5U (underflow)

                jmp     @_gdb_unhandled_isr      ; CMIA0 (compare)
                jmp     @_gdb_unhandled_isr      ; CMIB0 (compare)
                jmp     @_gdb_unhandled_isr      ; OVI0 (overflow)

                .space  4                ; reserved

                jmp     @_gdb_unhandled_isr      ; CMIA1 (compare)
                jmp     @_gdb_unhandled_isr      ; CMIB1 (compare)
                jmp     @_gdb_unhandled_isr      ; OVI1 (overflow)

                .space  4                ; reserved

                jmp     @_gdb_unhandled_isr      ; DEND0A
                jmp     @_gdb_unhandled_isr      ; DEND0B
                jmp     @_gdb_unhandled_isr      ; DEND1A
                jmp     @_gdb_unhandled_isr      ; DEND1B

                .space  (4*4)            ; reserved

                jmp     @_gdb_unhandled_isr      ; ERI0
                jmp     @_gdb_unhandled_isr      ; RXI0
                jmp     @_gdb_unhandled_isr      ; TXI0
                jmp     @_gdb_unhandled_isr      ; TEI0


                jmp     @_gdb_unhandled_isr      ; ERI1
                jmp     @_gdb_unhandled_isr      ; RXI1
                jmp     @_gdb_unhandled_isr      ; TXI1
                jmp     @_gdb_unhandled_isr      ; TEI1

                jmp     @_gdb_unhandled_isr      ; ERI2
                jmp     @_gdb_unhandled_isr      ; RXI2
                jmp     @_gdb_unhandled_isr      ; TXI2
                jmp     @_gdb_unhandled_isr      ; TEI2

                jmp     @_gdb_unhandled_isr      ; CMIA0 (compare)
                jmp     @_gdb_unhandled_isr      ; CMIB0 ( - " - )
                jmp     @_gdb_unhandled_isr      ; OVI0 (overflow)

                .space  4                ; reserved

                jmp     @_gdb_unhandled_isr      ; CMIA1 (compare)
                jmp     @_gdb_unhandled_isr      ; CMIB1 ( - " - )
                jmp     @_gdb_unhandled_isr      ; OVI1 (overflow)

                .space  4                ; reserved

                jmp     @_gdb_unhandled_isr      ; IICI0    (volitelne!)
                jmp     @_gdb_unhandled_isr      ; DDCSW1   (volitelne!) 

                jmp     @_gdb_unhandled_isr      ; IICI1    (volitelne!)
                .space  4                ; reserved

                .space  (4 * 4 * 4)      ; reserved

                jmp     @_gdb_unhandled_isr      ; ERI3
                jmp     @_gdb_unhandled_isr      ; RXI3
                jmp     @_gdb_unhandled_isr      ; TXI3
                jmp     @_gdb_unhandled_isr      ; TEI3  

                jmp     @_gdb_unhandled_isr      ; ERI4
                jmp     @_gdb_unhandled_isr      ; RXI4
                jmp     @_gdb_unhandled_isr      ; TXI4
                jmp     @_gdb_unhandled_isr      ; TEI4


