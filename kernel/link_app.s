
    .align 3
    .section .data
    .global _num_app
_num_app:
    .quad 4
    .quad app_0_start
    .quad app_1_start
    .quad app_2_start
    .quad app_3_start
    .quad app_3_end

    .align 3
    .section .data
    .global app_0_start
    .global app_0_end
app_0_start:
    .incbin "../user/target/00t.bin"
app_0_end:

    .align 3
    .section .data
    .global app_1_start
    .global app_1_end
app_1_start:
    .incbin "../user/target/01t.bin"
app_1_end:

    .align 3
    .section .data
    .global app_2_start
    .global app_2_end
app_2_start:
    .incbin "../user/target/02t.bin"
app_2_end:

    .align 3
    .section .data
    .global app_3_start
    .global app_3_end
app_3_start:
    .incbin "../user/target/03t.bin"
app_3_end:
