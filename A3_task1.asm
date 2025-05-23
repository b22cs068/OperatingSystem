.data
  str1: .asciiz "Computer "
  str2: .asciiz "Architecture"
  result: .space 64
  


.text
.globl main
 main:
   la $t0, str1
   la $t1, str2
   la $t2, result
   
   
 copy_str1:
  lb $t3, 0($t0)
  beq $t3, $zero, copy_str2
  sb $t3, 0($t2)
  addi $t2, $t2, 1
  addi $t0, $t0, 1
  #addi $t3, $t3, 1
  j copy_str1
  
 copy_str2:
  
  lb $t4, 0($t1)
  beq $t4, $zero, end
  sb  $t4, 0($t2)
  addi $t1,$t1, 1
  addi $t2, $t2, 1
  j copy_str2
  
    
  end:
  sb $zero, 0($t2)
 
  la $a0, result
  li $v0, 4
  syscall
   
  li $v0, 10
  syscall

