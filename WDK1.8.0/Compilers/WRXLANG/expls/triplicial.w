include "../../../Libraries/SYS8/WRX/iosys.w"
include "../../../Libraries/SYS8/WRX/math.w"

byte i = 5;
while(i > 0){
	byte f = factorial(i);
	byte p = primorial(i);
	byte t = termial(i);
	
	print("Fatorial de %d: %d\n", i, f);
	print("Primorial de %d: %d\n", i, p);
	print("Termial de %d: %d\n\n", i, t);
	
	if(f == p && p == t)
		print("%d e um numero FPT\n\n", i);
	i = i - 1;
}

byte x = 0;
while(x < 100){
	if(isPrime(x))
		print("%d ", x);
	x = x + 1;
}