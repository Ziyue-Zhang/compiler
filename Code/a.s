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

_Swap:
  move $fp, $sp
  addi $sp, $sp, -80
  lw $t0, 8($fp)
  sw $t0, -4($fp)
  lw $t0, 12($fp)
  sw $t0, -8($fp)
  lw $t0, 16($fp)
  sw $t0, -12($fp)
  li $t0, 0
  sw $t0, -24($fp)
  lw $t0, -8($fp)
  sw $t0, -28($fp)
  lw $t0, -28($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -28($fp)
  lw $t0, -24($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -24($fp)
  lw $t0, -24($fp)
  lw $t1, -28($fp)
  add $t2, $t0, $t1
  sw $t2, -24($fp)
  lw $t0, -4($fp)
  lw $t1, -24($fp)
  add $t2, $t0, $t1
  sw $t2, -32($fp)
  lw $t0, -32($fp)
  lw $t0, 0($t0)
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  sw $t0, -16($fp)
  li $t0, 0
  sw $t0, -36($fp)
  lw $t0, -8($fp)
  sw $t0, -40($fp)
  lw $t0, -40($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -40($fp)
  lw $t0, -36($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -36($fp)
  lw $t0, -36($fp)
  lw $t1, -40($fp)
  add $t2, $t0, $t1
  sw $t2, -36($fp)
  li $t0, 0
  sw $t0, -48($fp)
  lw $t0, -12($fp)
  sw $t0, -52($fp)
  lw $t0, -52($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -52($fp)
  lw $t0, -48($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -48($fp)
  lw $t0, -48($fp)
  lw $t1, -52($fp)
  add $t2, $t0, $t1
  sw $t2, -48($fp)
  lw $t0, -4($fp)
  lw $t1, -48($fp)
  add $t2, $t0, $t1
  sw $t2, -56($fp)
  lw $t0, -56($fp)
  lw $t0, 0($t0)
  sw $t0, -44($fp)
  lw $t0, -4($fp)
  lw $t1, -36($fp)
  add $t2, $t0, $t1
  sw $t2, -60($fp)
  lw $t0, -44($fp)
  lw $t1, -60($fp)
  sw $t0, 0($t1)
  li $t0, 0
  sw $t0, -64($fp)
  lw $t0, -12($fp)
  sw $t0, -68($fp)
  lw $t0, -68($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -68($fp)
  lw $t0, -64($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -64($fp)
  lw $t0, -64($fp)
  lw $t1, -68($fp)
  add $t2, $t0, $t1
  sw $t2, -64($fp)
  lw $t0, -16($fp)
  sw $t0, -72($fp)
  lw $t0, -4($fp)
  lw $t1, -64($fp)
  add $t2, $t0, $t1
  sw $t2, -76($fp)
  lw $t0, -72($fp)
  lw $t1, -76($fp)
  sw $t0, 0($t1)
  li $t0, 0
  sw $t0, -80($fp)
  lw $v0, -80($fp)
  jr $ra
_Partition:
  move $fp, $sp
  addi $sp, $sp, -152
  lw $t0, 8($fp)
  sw $t0, -4($fp)
  lw $t0, 12($fp)
  sw $t0, -8($fp)
  lw $t0, 16($fp)
  sw $t0, -12($fp)
  li $t0, 0
  sw $t0, -20($fp)
  lw $t0, -8($fp)
  sw $t0, -24($fp)
  lw $t0, -24($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -24($fp)
  lw $t0, -20($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -20($fp)
  lw $t0, -20($fp)
  lw $t1, -24($fp)
  add $t2, $t0, $t1
  sw $t2, -20($fp)
  lw $t0, -4($fp)
  lw $t1, -20($fp)
  add $t2, $t0, $t1
  sw $t2, -28($fp)
  lw $t0, -28($fp)
  lw $t0, 0($t0)
  sw $t0, -16($fp)
label1:
  lw $t0, -8($fp)
  sw $t0, -32($fp)
  lw $t0, -12($fp)
  sw $t0, -36($fp)
  lw $t0, -32($fp)
  lw $t1, -36($fp)
  blt $t0, $t1, label2
  j label3
label2:
label4:
  lw $t0, -8($fp)
  sw $t0, -40($fp)
  lw $t0, -12($fp)
  sw $t0, -44($fp)
  lw $t0, -40($fp)
  lw $t1, -44($fp)
  blt $t0, $t1, label7
  j label6
label7:
  li $t0, 0
  sw $t0, -56($fp)
  lw $t0, -12($fp)
  sw $t0, -60($fp)
  lw $t0, -60($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -60($fp)
  lw $t0, -56($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -56($fp)
  lw $t0, -56($fp)
  lw $t1, -60($fp)
  add $t2, $t0, $t1
  sw $t2, -56($fp)
  lw $t0, -4($fp)
  lw $t1, -56($fp)
  add $t2, $t0, $t1
  sw $t2, -64($fp)
  lw $t0, -64($fp)
  lw $t0, 0($t0)
  sw $t0, -48($fp)
  lw $t0, -16($fp)
  sw $t0, -52($fp)
  lw $t0, -48($fp)
  lw $t1, -52($fp)
  bge $t0, $t1, label5
  j label6
label5:
  lw $t0, -12($fp)
  sw $t0, -72($fp)
  li $t0, 1
  sw $t0, -76($fp)
  lw $t0, -72($fp)
  lw $t1, -76($fp)
  sub $t2, $t0, $t1
  sw $t2, -68($fp)
  lw $t0, -68($fp)
  sw $t0, -12($fp)
  j label4
label6:
  lw $t0, -4($fp)
  sw $t0, -80($fp)
  lw $t0, -8($fp)
  sw $t0, -84($fp)
  lw $t0, -12($fp)
  sw $t0, -88($fp)
  lw $t0, -88($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -84($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -80($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal _Swap
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 20
  sw $v0, -92($fp)
label8:
  lw $t0, -8($fp)
  sw $t0, -96($fp)
  lw $t0, -12($fp)
  sw $t0, -100($fp)
  lw $t0, -96($fp)
  lw $t1, -100($fp)
  blt $t0, $t1, label11
  j label10
label11:
  li $t0, 0
  sw $t0, -112($fp)
  lw $t0, -8($fp)
  sw $t0, -116($fp)
  lw $t0, -116($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -116($fp)
  lw $t0, -112($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -112($fp)
  lw $t0, -112($fp)
  lw $t1, -116($fp)
  add $t2, $t0, $t1
  sw $t2, -112($fp)
  lw $t0, -4($fp)
  lw $t1, -112($fp)
  add $t2, $t0, $t1
  sw $t2, -120($fp)
  lw $t0, -120($fp)
  lw $t0, 0($t0)
  sw $t0, -104($fp)
  lw $t0, -16($fp)
  sw $t0, -108($fp)
  lw $t0, -104($fp)
  lw $t1, -108($fp)
  ble $t0, $t1, label9
  j label10
label9:
  lw $t0, -8($fp)
  sw $t0, -128($fp)
  li $t0, 1
  sw $t0, -132($fp)
  lw $t0, -128($fp)
  lw $t1, -132($fp)
  add $t2, $t0, $t1
  sw $t2, -124($fp)
  lw $t0, -124($fp)
  sw $t0, -8($fp)
  j label8
label10:
  lw $t0, -4($fp)
  sw $t0, -136($fp)
  lw $t0, -8($fp)
  sw $t0, -140($fp)
  lw $t0, -12($fp)
  sw $t0, -144($fp)
  lw $t0, -144($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -140($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -136($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal _Swap
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 20
  sw $v0, -148($fp)
  j label1
label3:
  lw $t0, -8($fp)
  sw $t0, -152($fp)
  lw $v0, -152($fp)
  jr $ra
_QuickSort:
  move $fp, $sp
  addi $sp, $sp, -88
  lw $t0, 8($fp)
  sw $t0, -4($fp)
  lw $t0, 12($fp)
  sw $t0, -8($fp)
  lw $t0, 16($fp)
  sw $t0, -12($fp)
  lw $t0, -8($fp)
  sw $t0, -16($fp)
  lw $t0, -12($fp)
  sw $t0, -20($fp)
  lw $t0, -16($fp)
  lw $t1, -20($fp)
  blt $t0, $t1, label12
  j label13
label12:
  lw $t0, -4($fp)
  sw $t0, -28($fp)
  lw $t0, -8($fp)
  sw $t0, -32($fp)
  lw $t0, -12($fp)
  sw $t0, -36($fp)
  lw $t0, -36($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -32($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -28($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal _Partition
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 20
  sw $v0, -24($fp)
  lw $t0, -4($fp)
  sw $t0, -40($fp)
  lw $t0, -8($fp)
  sw $t0, -44($fp)
  lw $t0, -24($fp)
  sw $t0, -52($fp)
  li $t0, 1
  sw $t0, -56($fp)
  lw $t0, -52($fp)
  lw $t1, -56($fp)
  sub $t2, $t0, $t1
  sw $t2, -48($fp)
  lw $t0, -48($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -44($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -40($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal _QuickSort
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 20
  sw $v0, -60($fp)
  lw $t0, -4($fp)
  sw $t0, -64($fp)
  lw $t0, -24($fp)
  sw $t0, -72($fp)
  li $t0, 1
  sw $t0, -76($fp)
  lw $t0, -72($fp)
  lw $t1, -76($fp)
  add $t2, $t0, $t1
  sw $t2, -68($fp)
  lw $t0, -12($fp)
  sw $t0, -80($fp)
  lw $t0, -80($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -68($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -64($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal _QuickSort
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 20
  sw $v0, -84($fp)
label13:
  li $t0, 0
  sw $t0, -88($fp)
  lw $v0, -88($fp)
  jr $ra
main:
  move $fp, $sp
  addi $sp, $sp, -148
  li $t0, 8
  sw $t0, -4($fp)
  li $t0, 0
  sw $t0, -40($fp)
label14:
  lw $t0, -40($fp)
  sw $t0, -44($fp)
  lw $t0, -4($fp)
  sw $t0, -48($fp)
  lw $t0, -44($fp)
  lw $t1, -48($fp)
  blt $t0, $t1, label15
  j label16
label15:
  li $t0, 0
  sw $t0, -52($fp)
  lw $t0, -40($fp)
  sw $t0, -56($fp)
  lw $t0, -56($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -56($fp)
  lw $t0, -52($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -52($fp)
  lw $t0, -52($fp)
  lw $t1, -56($fp)
  add $t2, $t0, $t1
  sw $t2, -52($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  sw $v0, -60($fp)
  addi $t0, $fp, -36
  lw $t1, -52($fp)
  add $t2, $t0, $t1
  sw $t2, -64($fp)
  lw $t0, -60($fp)
  lw $t1, -64($fp)
  sw $t0, 0($t1)
  lw $t0, -40($fp)
  sw $t0, -72($fp)
  li $t0, 1
  sw $t0, -76($fp)
  lw $t0, -72($fp)
  lw $t1, -76($fp)
  add $t2, $t0, $t1
  sw $t2, -68($fp)
  lw $t0, -68($fp)
  sw $t0, -40($fp)
  j label14
label16:
  addi $t0, $fp, -36
  sw $t0, -80($fp)
  li $t0, 0
  sw $t0, -84($fp)
  lw $t0, -4($fp)
  sw $t0, -92($fp)
  li $t0, 1
  sw $t0, -96($fp)
  lw $t0, -92($fp)
  lw $t1, -96($fp)
  sub $t2, $t0, $t1
  sw $t2, -88($fp)
  lw $t0, -88($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -84($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -80($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -8
  sw $ra, 4($sp)
  sw $fp, 0($sp)
  jal _QuickSort
  move $sp, $fp
  lw $fp, 0($sp)
  lw $ra, 4($sp)
  addi $sp, $sp, 20
  sw $v0, -100($fp)
  li $t0, 0
  sw $t0, -104($fp)
  lw $t0, -104($fp)
  sw $t0, -40($fp)
label17:
  lw $t0, -40($fp)
  sw $t0, -108($fp)
  lw $t0, -4($fp)
  sw $t0, -112($fp)
  lw $t0, -108($fp)
  lw $t1, -112($fp)
  blt $t0, $t1, label18
  j label19
label18:
  li $t0, 0
  sw $t0, -120($fp)
  lw $t0, -40($fp)
  sw $t0, -124($fp)
  lw $t0, -124($fp)
  li $t1, 4
  mul $t2, $t0, $t1
  sw $t2, -124($fp)
  lw $t0, -120($fp)
  li $t1, 0
  add $t2, $t0, $t1
  sw $t2, -120($fp)
  lw $t0, -120($fp)
  lw $t1, -124($fp)
  add $t2, $t0, $t1
  sw $t2, -120($fp)
  addi $t0, $fp, -36
  lw $t1, -120($fp)
  add $t2, $t0, $t1
  sw $t2, -128($fp)
  lw $t0, -128($fp)
  lw $t0, 0($t0)
  sw $t0, -116($fp)
  lw $a0, -116($fp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $t0, 0
  sw $t0, -132($fp)
  lw $t0, -40($fp)
  sw $t0, -140($fp)
  li $t0, 1
  sw $t0, -144($fp)
  lw $t0, -140($fp)
  lw $t1, -144($fp)
  add $t2, $t0, $t1
  sw $t2, -136($fp)
  lw $t0, -136($fp)
  sw $t0, -40($fp)
  j label17
label19:
  li $t0, 0
  sw $t0, -148($fp)
  lw $v0, -148($fp)
  jr $ra
