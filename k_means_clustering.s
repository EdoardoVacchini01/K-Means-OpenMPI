	.file	"k_means_clustering.c"
	.text
	.section	.rodata
.LC0:
	.string	"Reading the dataset file..."
.LC1:
	.string	"dataset.txt"
	.align 8
.LC2:
	.string	"An error occurred while reading the dataset file."
	.align 8
.LC3:
	.string	"The number of clusters must be less than or equal to the number of data points."
	.align 8
.LC4:
	.string	"An error occurred while allocating the memory for the centroids."
	.align 8
.LC5:
	.string	"An error occurred while allocating the memory for the prototypes."
.LC6:
	.string	"Clustering the data points..."
	.align 8
.LC8:
	.string	"Clustering process completed in %.06lf s.\n\nCentroids:\n"
.LC9:
	.string	"clustering.txt"
.LC10:
	.string	"w"
	.align 8
.LC11:
	.string	"\nAn error occurred while opening the output file."
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movl	%edi, -100(%rbp)
	movq	%rsi, -112(%rbp)
	movq	$0, -56(%rbp)
	movl	$0, -8(%rbp)
	cmpl	$3, -100(%rbp)
	jle	.L2
	movq	-112(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi
	jmp	.L3
.L2:
	movl	$3, %eax
.L3:
	movl	%eax, -12(%rbp)
	movq	$0, -80(%rbp)
	movq	$0, -72(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -32(%rbp)
	movl	$0, -36(%rbp)
	movl	$0, -4(%rbp)
	cmpl	$4, -100(%rbp)
	jle	.L4
	movq	-112(%rbp), %rax
	addq	$32, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi
	jmp	.L5
.L4:
	movl	$100, %eax
.L5:
	movl	%eax, -40(%rbp)
	movq	$0, -96(%rbp)
	movq	$0, -88(%rbp)
	movq	$0, -48(%rbp)
	movl	$.LC0, %edi
	call	puts
	cmpl	$1, -100(%rbp)
	jle	.L6
	movq	-112(%rbp), %rax
	movq	8(%rax), %rax
	jmp	.L7
.L6:
	movl	$.LC1, %eax
.L7:
	leaq	-56(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	readDataset
	movl	%eax, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jne	.L8
	movl	$.LC2, %edi
	call	puts
	movl	$1, %eax
	jmp	.L19
.L8:
	movl	-12(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jbe	.L10
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$.LC3, %edi
	call	puts
	movl	$1, %eax
	jmp	.L19
.L10:
	leaq	-80(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	clock_gettime
	movl	-12(%rbp), %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L11
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$.LC4, %edi
	call	puts
	movl	$1, %eax
	jmp	.L19
.L11:
	movl	-12(%rbp), %eax
	salq	$5, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L12
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$.LC5, %edi
	call	puts
	movl	$1, %eax
	jmp	.L19
.L12:
	movl	$.LC6, %edi
	call	puts
	movq	-56(%rbp), %rdx
	movl	-12(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	initCentroids
.L14:
	movl	-12(%rbp), %edx
	movq	-32(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	initPrototypes
	movq	-56(%rbp), %rax
	movl	-12(%rbp), %edi
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movl	-8(%rbp), %esi
	movl	%edi, %r8d
	movq	%rax, %rdi
	call	kMeansIteration
	movl	%eax, -36(%rbp)
	movl	-12(%rbp), %edx
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	updateCentroids
	addl	$1, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	-40(%rbp), %eax
	jnb	.L13
	cmpl	$0, -36(%rbp)
	jne	.L14
.L13:
	leaq	-96(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	clock_gettime
	movq	-96(%rbp), %rdx
	movq	-80(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm1
	movq	-88(%rbp), %rdx
	movq	-72(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm2
	movsd	.LC7(%rip), %xmm0
	mulsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movl	$.LC8, %edi
	movl	$1, %eax
	call	printf
	movq	stdout(%rip), %rdx
	movl	-12(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	printCentroids
	cmpl	$2, -100(%rbp)
	jle	.L15
	movq	-112(%rbp), %rax
	movq	16(%rax), %rax
	jmp	.L16
.L15:
	movl	$.LC9, %eax
.L16:
	movl	$.LC10, %esi
	movq	%rax, %rdi
	call	fopen
	movq	%rax, -48(%rbp)
	cmpq	$0, -48(%rbp)
	je	.L17
	movq	-48(%rbp), %rdx
	movl	-12(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	printCentroids
	movq	-48(%rbp), %rax
	movq	%rax, %rsi
	movl	$10, %edi
	call	fputc
	movq	-56(%rbp), %rax
	movq	-48(%rbp), %rdx
	movl	-8(%rbp), %ecx
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	printPointLabels
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	jmp	.L18
.L17:
	movl	$.LC11, %edi
	call	puts
.L18:
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$0, %eax
.L19:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC7:
	.long	3894859413
	.long	1041313291
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-4)"
	.section	.note.GNU-stack,"",@progbits
