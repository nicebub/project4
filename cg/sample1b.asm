$main1
	alloc	3
	enter	1
	pushs	"%f"
	pusha	5
	call	$scanf, 2
	popI	4
	pusha	6
	pushcI	9
	pusha	5
	fetchR
	pushcI	8
	flt
	mulR
	fltb
	addR
	storeR
	popI	4
	pusha	7
	pushcI	9
	pusha	5
	fetchR
	pushcI	8
	flt
	mulR
	fltb
	addR
	int
	storeI
	popI	4
	enter	1
	pushs	"Result (int) is %d\n"
	pusha	7
	fetchI
	call	$printf, 2
	popI	4
	enter	1
	pushs	"Result (float) is %f\n"
	pusha	6
	fetchR
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
