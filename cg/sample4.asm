$main1
	alloc	2
	enter	1
	pushs	"Enter an integer: "
	call	$printf, 2
	popI	4
	enter	1
	pushs	"%d"
	pusha	5
	call	$scanf, 2
	popI	4
	pusha	6
	enter	1
	pusha	5
	fetchI
	call	$factorial2, 1
	storeI
	popI	4
	enter	1
	pushs	"Factorial of %d "
	pusha	5
	fetchI
	call	$printf, 2
	popI	4
	enter	1
	pushs	"is %d\n"
	pusha	6
	fetchI
	call	$printf, 2
	popI	4
	pushcI	0
	setrvI
	returnf
$factorial2
	alloc	0
	pusha	5
	fetchI
	pushcI	1
	leI
	jumpz	$1
	pushcI	1
	setrvI
	returnf
	jump	$2
$1
	pusha	5
	fetchI
	enter	1
	pusha	5
	fetchI
	pushcI	1
	subI
	call	$factorial2, 1
	mulI
	setrvI
	returnf
$2
	returnf
main
	enter	0
	alloc	0
	enter	0
	call	$main1, 0
	return
