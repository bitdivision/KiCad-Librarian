#version 1
#brief Aliminium Electrolytic Capacitor
#pins 2
#param 2 @PT 4 @PP 2.5 @PW 1.0 @PL 5.2 @BW 5.2 @BL 0.2 @STP 0.2 @BP 0.65 @TS 0.1 @TP
$MODULE {NAME}
AR CAE
Po 0 0 0 15 51D5CF28 00000000 ~~
Li {NAME}
Cd {DESCR}
Op 0 0 0
T0 0 {BL 2 / TS 1.2 * + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {BL 2 / TS 1.2 * +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
{BL 2 / 0.1 - @RAD   75 cos RAD * @DX   75 sin RAD * @DY}
DS {DX} {DY ~} {DX} {DY} {BP} 21
{20 cos RAD * @DX   20 sin RAD * @DY   PL 2 / STP + @OFF   BL 5 / @CUT}
DA 0 0 {DX} {DY} 1400 {BP} 21
DA 0 0 {DX ~} {DY ~} 1400 {BP} 21
DS {BW 2 / ~} {BL 2 / CUT - ~} {BW 2 / ~} {OFF ~} {BP} 21
DS {BW 2 / ~} {BL 2 / CUT -} {BW 2 / ~} {OFF} {BP} 21
DS {BW 2 / CUT - ~} {BL 2 / ~} {BW 2 /} {BL 2 / ~} {BP} 21
DS {BW 2 / CUT - ~} {BL 2 /} {BW 2 /} {BL 2 /} {BP} 21
DS {BW 2 / CUT - ~} {BL 2 / ~} {BW 2 / ~} {BL 2 / CUT - ~} {BP} 21
DS {BW 2 / CUT - ~} {BL 2 /} {BW 2 / ~} {BL 2 / CUT -} {BP} 21
DS {BW 2 /} {BL 2 / ~} {BW 2 /} {OFF ~} {BP} 21
DS {BW 2 /} {BL 2 /} {BW 2 /} {OFF} {BP} 21
$PAD
Sh "{PN}" R {PW} {PL} 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po {PN 1.5 - PP *} 0
$EndPAD
$EndMODULE {NAME}
