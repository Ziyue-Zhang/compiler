.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

main:
  move $fp, $sp
  addi $sp, $sp, -76
  addi $sp, $sp, -8
  addi $sp, $sp, -8
  li $t0, 1
  sw $t0, -20($fp)
  addi $t0, $fp, -8
  sw $t0, -24($fp)
  lw $t0, -20($fp)
  lw $t1, -24($fp)
  sw $t0, 0($t1)
  li $t0, 70
  sw $t0, -28($fp)
  addi $t0, $fp, -8
  li $t1, 4
  add $t2, $t0, $t1
  sw $t2, -32($fp)
  lw $t0, -28($fp)
  lw $t1, -32($fp)
  sw $t0, 0($t1)
  li $t0, 2
  sw $t0, -36($fp)
  addi $t0, $fp, -16
  sw $t0, -40($fp)
  lw $t0, -36($fp)
  lw $t1, -40($fp)
  sw $t0, 0($t1)
  li $t0, 90
  sw $t0, -44($fp)
  addi $t0, $fp, -16
  li $t1, 4
  add $t2, $t0, $t1
  sw $t2, -48($fp)
  lw $t0, -44($fp)
  lw $t1, -48($fp)
  sw $t0, 0($t1)
  addi $t0, $fp, -16
  sw $t0, -64($fp)
  lw $t0, -64($fp)
  lw $t0, 0($t0)
  sw $t0, -56($fp)
  addi $t0, $fp, -8
  li $t1, 4
  add $t2, $t0, $t1
  sw $t2, -68($fp)
  lw $t0, -68($fp)
  lw $t0, 0($t0)
  sw $t0, -60($fp)
  lw $t0, -56($fp)
  lw $t1, -60($fp)
  mul $t2, $t0, $t1
  sw $t2, -52($fp)
  lw $a0, -52($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t0, 0
  sw $t0, -72($fp)
  li $v0, 0
  jr $ra
