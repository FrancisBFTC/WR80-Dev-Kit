word length(word str){
	word count = 0;
	while(*str){
		count = count + 1;
		str = str + 1;
	}
	return count;
}

byte compare(word str1, word str2){
	while(*str1 || *str2){
		if(*str1 != *str2)
			return 1;
		str1 = str1 + 1;
		str2 = str2 + 1;
	}
	return (*str1 || *str2);
}