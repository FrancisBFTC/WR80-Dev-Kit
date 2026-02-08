;define label addr - 1

addr:
	ret
	ret
	ret
	; JP aponta para este 3ª RET
jp addr - 1

db 0, 0, 0, 0

