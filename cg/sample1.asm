$main1
	alloc	2
	enter	1
	pushs	"%d"
	pusha	5
	call	$scanf, 2
	popI	4
	pusha	6
	pushcI	9
	pusha	5
	fetchI
	pushcI	8
	mulI
	addI
	storeI
	popI	4
	enter	1
	pushs	"Result is %d\n"
	pusha	6
	fetchI
	call	$printf, 2
	popI	4
	pushcI	0
	setrvI
	returnf
main
	enter	0
	alloc	0
	enter	0
	call	$main1, 0
	return
