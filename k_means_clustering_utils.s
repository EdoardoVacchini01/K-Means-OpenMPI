	.file	"k_means_clustering_utils.c"
	.text
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	printPointLabels
	.type	printPointLabels, @function
printPointLabels:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movq	%rdx, -40(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	24(%rax), %edx
	movq	-40(%rbp), %rax
	movl	$.LC0, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	addl	$1, -4(%rbp)
.L2:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jb	.L3
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	printPointLabels, .-printPointLabels
	.section	.rodata
.LC1:
	.string	" "
.LC2:
	.string	"\n"
.LC3:
	.string	"%.10lf%s"
	.text
	.globl	printCentroid
	.type	printCentroid, @function
printCentroid:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L5
.L8:
	cmpl	$1, -4(%rbp)
	ja	.L6
	movl	$.LC1, %ecx
	jmp	.L7
.L6:
	movl	$.LC2, %ecx
.L7:
	movq	-24(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	(%rax,%rdx,8), %xmm0
	movq	-32(%rbp), %rax
	movq	%rcx, %rdx
	movl	$.LC3, %esi
	movq	%rax, %rdi
	movl	$1, %eax
	call	fprintf
	addl	$1, -4(%rbp)
.L5:
	cmpl	$2, -4(%rbp)
	jbe	.L8
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	printCentroid, .-printCentroid
	.globl	printCentroids
	.type	printCentroids, @function
printCentroids:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movq	%rdx, -40(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L10
.L11:
	movl	-4(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	printCentroid
	addl	$1, -4(%rbp)
.L10:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jb	.L11
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	printCentroids, .-printCentroids
	.globl	getSquaredDistance
	.type	getSquaredDistance, @function
getSquaredDistance:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movl	$0, -4(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L13
.L14:
	movq	-40(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	(%rax,%rdx,8), %xmm0
	movq	-48(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	(%rax,%rdx,8), %xmm1
	subsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-24(%rbp), %xmm0
	mulsd	-24(%rbp), %xmm0
	movsd	-16(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	addl	$1, -4(%rbp)
.L13:
	cmpl	$2, -4(%rbp)
	jbe	.L14
	movsd	-16(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	getSquaredDistance, .-getSquaredDistance
	.globl	initCentroids
	.type	initCentroids, @function
initCentroids:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movq	%rdx, -40(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L17
.L20:
	movl	$0, -8(%rbp)
	jmp	.L18
.L19:
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-4(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-8(%rbp), %eax
	movsd	(%rcx,%rax,8), %xmm0
	movl	-8(%rbp), %eax
	movsd	%xmm0, (%rdx,%rax,8)
	addl	$1, -8(%rbp)
.L18:
	cmpl	$2, -8(%rbp)
	jbe	.L19
	addl	$1, -4(%rbp)
.L17:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jb	.L20
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	initCentroids, .-initCentroids
	.globl	initPrototypes
	.type	initPrototypes, @function
initPrototypes:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L22
.L25:
	movl	$0, -8(%rbp)
	jmp	.L23
.L24:
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-8(%rbp), %eax
	pxor	%xmm0, %xmm0
	movsd	%xmm0, (%rdx,%rax,8)
	addl	$1, -8(%rbp)
.L23:
	cmpl	$2, -8(%rbp)
	jbe	.L24
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	$0, 24(%rax)
	addl	$1, -4(%rbp)
.L22:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jb	.L25
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	initPrototypes, .-initPrototypes
	.globl	updateCentroid
	.type	updateCentroid, @function
updateCentroid:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L27
.L30:
	movq	-32(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	(%rax,%rdx,8), %xmm1
	movq	-32(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, %eax
	testq	%rax, %rax
	js	.L28
	cvtsi2sdq	%rax, %xmm0
	jmp	.L29
.L28:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L29:
	divsd	%xmm0, %xmm1
	movapd	%xmm1, %xmm0
	movq	-24(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	%xmm0, (%rax,%rdx,8)
	addl	$1, -4(%rbp)
.L27:
	cmpl	$2, -4(%rbp)
	jbe	.L30
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	updateCentroid, .-updateCentroid
	.globl	updateCentroids
	.type	updateCentroids, @function
updateCentroids:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$40, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L32
.L33:
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-32(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-4(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	updateCentroid
	addl	$1, -4(%rbp)
.L32:
	movl	-4(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jb	.L33
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	updateCentroids, .-updateCentroids
	.globl	updatePrototype
	.type	updatePrototype, @function
updatePrototype:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L35
.L36:
	movq	-24(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	(%rax,%rdx,8), %xmm1
	movq	-32(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	(%rax,%rdx,8), %xmm0
	addsd	%xmm1, %xmm0
	movq	-24(%rbp), %rax
	movl	-4(%rbp), %edx
	movsd	%xmm0, (%rax,%rdx,8)
	addl	$1, -4(%rbp)
.L35:
	cmpl	$2, -4(%rbp)
	jbe	.L36
	movq	-24(%rbp), %rax
	movl	24(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, 24(%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	updatePrototype, .-updatePrototype
	.globl	kMeansIteration
	.type	kMeansIteration, @function
kMeansIteration:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -56(%rbp)
	movl	%esi, -60(%rbp)
	movq	%rdx, -72(%rbp)
	movq	%rcx, -80(%rbp)
	movl	%r8d, -64(%rbp)
	movl	$0, -4(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	movl	$0, -28(%rbp)
	movl	$0, -20(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -40(%rbp)
	movl	$0, -24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L38
.L44:
	movsd	.LC5(%rip), %xmm0
	movsd	%xmm0, -16(%rbp)
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	addq	%rdx, %rax
	movl	24(%rax), %eax
	movl	%eax, -28(%rbp)
	movl	$0, -20(%rbp)
	jmp	.L39
.L42:
	movl	-20(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rax, %rdx
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rcx
	movq	-56(%rbp), %rax
	addq	%rcx, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	getSquaredDistance
	movq	%xmm0, %rax
	movq	%rax, -40(%rbp)
	movsd	-16(%rbp), %xmm0
	comisd	-40(%rbp), %xmm0
	jbe	.L40
	movsd	-40(%rbp), %xmm0
	movsd	%xmm0, -16(%rbp)
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	addq	%rax, %rdx
	movl	-20(%rbp), %eax
	movl	%eax, 24(%rdx)
.L40:
	addl	$1, -20(%rbp)
.L39:
	movl	-20(%rbp), %eax
	cmpl	-64(%rbp), %eax
	jb	.L42
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	addq	%rdx, %rax
	movl	24(%rax), %eax
	cmpl	%eax, -28(%rbp)
	je	.L43
	movl	$1, -24(%rbp)
.L43:
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	addq	%rax, %rdx
	movl	-4(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rcx
	movq	-56(%rbp), %rax
	addq	%rcx, %rax
	movl	24(%rax), %eax
	movl	%eax, %eax
	salq	$5, %rax
	movq	%rax, %rcx
	movq	-80(%rbp), %rax
	addq	%rcx, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	updatePrototype
	addl	$1, -4(%rbp)
.L38:
	movl	-4(%rbp), %eax
	cmpl	-60(%rbp), %eax
	jb	.L44
	movl	-24(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	kMeansIteration, .-kMeansIteration
	.section	.rodata
.LC6:
	.string	"r"
.LC7:
	.string	"%d"
	.text
	.globl	readDataset
	.type	readDataset, @function
readDataset:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$4168, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -4168(%rbp)
	movq	%rsi, -4176(%rbp)
	movq	$0, -40(%rbp)
	movl	$0, -4148(%rbp)
	movl	$0, -20(%rbp)
	movl	$0, -24(%rbp)
	movq	$0, -32(%rbp)
	movw	$32, -4150(%rbp)
	movq	-4168(%rbp), %rax
	movl	$.LC6, %esi
	movq	%rax, %rdi
	call	fopen
	movq	%rax, -40(%rbp)
	cmpq	$0, -40(%rbp)
	jne	.L48
	movl	$0, %eax
	jmp	.L59
.L48:
	movq	-40(%rbp), %rdx
	leaq	-4144(%rbp), %rax
	movl	$4096, %esi
	movq	%rax, %rdi
	call	fgets
	leaq	-4148(%rbp), %rdx
	leaq	-4144(%rbp), %rax
	movl	$.LC7, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_sscanf
	movl	-4148(%rbp), %eax
	movl	%eax, %eax
	salq	$5, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, %rdx
	movq	-4176(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-4176(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L51
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	$0, %eax
	jmp	.L59
.L58:
	movl	$0, -24(%rbp)
	leaq	-4150(%rbp), %rdx
	leaq	-4144(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strtok
	movq	%rax, -32(%rbp)
	jmp	.L52
.L54:
	movq	-4176(%rbp), %rax
	movq	(%rax), %rax
	movl	-20(%rbp), %edx
	salq	$5, %rdx
	leaq	(%rax,%rdx), %rbx
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	atof
	movq	%xmm0, %rdx
	movl	-24(%rbp), %eax
	movq	%rdx, (%rbx,%rax,8)
	addl	$1, -24(%rbp)
	leaq	-4150(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	strtok
	movq	%rax, -32(%rbp)
.L52:
	cmpq	$0, -32(%rbp)
	je	.L53
	cmpl	$2, -24(%rbp)
	jbe	.L54
.L53:
	cmpl	$3, -24(%rbp)
	jne	.L55
	cmpq	$0, -32(%rbp)
	je	.L56
.L55:
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movq	-4176(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$0, %eax
	jmp	.L59
.L56:
	movq	-4176(%rbp), %rax
	movq	(%rax), %rax
	movl	-20(%rbp), %edx
	salq	$5, %rdx
	addq	%rdx, %rax
	movl	$0, 24(%rax)
	addl	$1, -20(%rbp)
.L51:
	movq	-40(%rbp), %rdx
	leaq	-4144(%rbp), %rax
	movl	$4096, %esi
	movq	%rax, %rdi
	call	fgets
	testq	%rax, %rax
	je	.L57
	movl	-4148(%rbp), %eax
	cmpl	%eax, -20(%rbp)
	jb	.L58
.L57:
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	-4148(%rbp), %eax
.L59:
	addq	$4168, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	readDataset, .-readDataset
	.section	.rodata
	.align 8
.LC5:
	.long	4294967295
	.long	2146435071
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-4)"
	.section	.note.GNU-stack,"",@progbits
