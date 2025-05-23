.data
prompt_hours:      .asciiz "Enter the number of regular hours worked: "
prompt_ot_hours:   .asciiz "Enter the number of overtime hours worked: "
prompt_wage:       .asciiz "Enter the hourly wage: "
gross_msg:         .asciiz "\nGross Salary: $"
deductions_msg:    .asciiz "Total Deductions: $"
net_salary_msg:    .asciiz "Net Salary: $"
continue_msg:      .asciiz "Do you want to enter another employee? (1 for Yes, 0 for No): "
newline:           .asciiz "\n"

# Variables to store user input
hours:       .word 0
ot_hours:    .word 0
wage:        .word 0
gross:       .word 0
tax:         .word 0
insurance:   .word 0
deductions:  .word 0
net_salary:  .word 0
continue:    .word 1
.text
main:
    # Start loop for multiple employees
    loop_start:
        li $v0, 4                  # syscall to print string
        la $a0, prompt_hours       # load address of the prompt
        syscall

        li $v0, 5                  # syscall to read integer
        syscall
        sw $v0, hours              # store user input in hours

        li $v0, 4                  # print prompt for OT hours
        la $a0, prompt_ot_hours
        syscall

        li $v0, 5                  # read OT hours
        syscall
        sw $v0, ot_hours           # store in ot_hours

        li $v0, 4                  # print prompt for hourly wage
        la $a0, prompt_wage
        syscall

        li $v0, 5                  # read hourly wage
        syscall
        sw $v0, wage               # store in wage

        # Calculate regular salary: regular_hours * wage
        lw $t0, hours              # load hours
        lw $t1, wage               # load wage
        mul $t2, $t0, $t1          # t2 = hours * wage
        sw $t2, gross              # store in gross

      
     # Calculate OT pay: ot_hours * wage * 1.5
        lw $t3, ot_hours           # load ot_hours
        mul $t4, $t3, $t1          # t4 = ot_hours * wage
        mul $t4, $t4, 3            # t4 = ot_hours * wage * 3
        div $t4, $t4, 2            # t4 = (ot_hours * wage * 3) / 2
        add $t2, $t2, $t4          # gross = regular + OT pay
        sw $t2, gross              # store updated gross

        # Calculate tax (8%)
        li $t6, 8                # 8% tax
        div $t7, $t2, 100          # t7 = gross / 100
        mul $t7, $t7, $t6          # t7 = gross * 0.02
        sw $t7, tax                # store tax

        # Calculate insurance (16%)
        li $t8, 16              # 16% insurance
        div $t9, $t2, 100          # t9 = gross / 100
        mul $t9, $t9, $t8          # t9 = gross * 0.10
        sw $t9, insurance          # store insurance

        # Calculate total deductions
        add $t0, $t7, $t9          # total_deductions = tax + insurance
        sw $t0, deductions         # store deductions

        # Calculate net salary
        sub $t1, $t2, $t0          # net_salary = gross - deductions
        sw $t1, net_salary         # store net_salary

        # Display results
        li $v0, 4                  # syscall to print string
        la $a0, gross_msg          # load address of gross_msg
        syscall

        li $v0, 1                  # syscall to print integer
        lw $a0, gross              # load gross
        syscall

        li $v0, 4                  # print newline
        la $a0, newline
        syscall

        li $v0, 4                  # print deductions message
        la $a0, deductions_msg
        syscall

        li $v0, 1                  # print total deductions
        lw $a0, deductions
        syscall

        li $v0, 4                  # print newline
        la $a0, newline
        syscall

        li $v0, 4                  # print net salary message
        la $a0, net_salary_msg
        syscall

        li $v0, 1                  # print net salary
        lw $a0, net_salary
        syscall

        # Ask if the user wants to enter another employee
        li $v0, 4                  # print continue message
        la $a0, continue_msg
        syscall

        li $v0, 5                  # read user input (1 for yes, 0 for no)
        syscall
        sw $v0, continue           # store in continue

        lw $t2, continue
        bnez $t2, loop_start       # if continue != 0, loop again

    # Program termination
    li $v0, 10                     # syscall for exit
    syscall
