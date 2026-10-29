/*
word func(){
	return 0xFFEE;
}
*/

word func(){
	word A = 0xF02;
	return A + 1;
}

word var = func() + func();

