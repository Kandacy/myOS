
    .align 3

    .section .data
    .global app_0_start
    .global app_0_end
app_0_start:
    .incbin "../user/target/main.bin"
app_0_end:
