; R0 = 3
st 3
ld r0

; DR = 200 ($C8)
st $c
shl 4
st 8

pushd
push r0

mul r0	; DR = 200 * 3 = 600 ($258) -> DR = $58, R0 = 2

pop r0
popd
div r0	; DR = 200 / 3 = 66,66 ($42) -> DR = $42, R0 = 2


