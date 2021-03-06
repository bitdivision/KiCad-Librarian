#version 1
#brief diode or LED in MELF/chip package
#note Polarized shape. Suitable for MELF, 2-pin DFN and some LED packages.
#pins 2
#param 2 @PT 1.9 @PP 0.7 @PW 1.3 @PL 0.2 @STP 0.2 @BP 0.65 @TS 0.1 @TP
$MODULE {NAME}
AR MELF
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw CHIP MELF
Op 0 0 0
At SMD
{PP PW + STP 2 * + @BW   PL STP 2 * + @BL}
T0 0 {BL 2 / TS + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {BL 2 / TS +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
T2 {BW 2 / ~ BP 2 * -} {BL 2 / ~ BP 2 / +} {BP 2 *} {BP 2 *} 0 {TP} N H 21 N "○"
{BW 2 / @X2   X2 ~ @X1   BL 2 / @Y2   Y2 ~ @Y1}
DS {X1} {Y1} {X2} {Y1} {BP} 21
DS {X2} {Y1} {X2} {Y2} {BP} 21
DS {X2} {Y2} {X1} {Y2} {BP} 21
DS {X1} {Y2} {X1} {Y1} {BP} 21
{BL 0.2 * @Y2   Y2 ~ @Y1   60 sin Y2 * @X2   X2 ~ @X1}
DS {X1} 0 {X2} {Y1} {BP} 21
DS {X2} {Y1} {X2} {Y2} {BP} 21
DS {X2} {Y2} {X1} 0 {BP} 21
DS {X1} {Y1} {X1} {Y2} {BP} 21
$PAD
Sh "{PN}" R {PW} {PL} 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
Po {PN 1.5 - PP *} 0
$EndPAD
$EndMODULE {NAME}
