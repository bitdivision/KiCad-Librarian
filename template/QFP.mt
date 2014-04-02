#version 1
#brief Quad Flat Package w/ Pins
#note Without exposed pad.
#pins 28 32 ...
#param 32 @PT 0.8 @PP 8.5 @SH 8.5 @SV 1.3 @PW 0.45 @PL 6.8 @BW 6.8 @BL 0.2 @BP 0.65 @TS 0.1 @TP
$MODULE {NAME}
AR QFP
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw QFP TQFP
Op 0 0 0
At SMD
T0 0 {SV PW + 2 / TS + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {SV PW + 2 / TS +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
T2 {SH 2 / ~ BP +} {SV 2 / ~ BP +} {BP 2 *} {BP 2 *} 0 {TP} N H 21 N "○"
{BW 2 / @X2   X2 ~ @X1   BL 2 / @Y2   Y2 ~ @Y1}
{BW 4 / 0.1 - 0.5 min @RAD   0.4 @OFFS   RAD BW 2 / - OFFS + @XC   RAD BL 2 / - OFFS + @YC}
DS {X1 OFFS +} {Y1} {X2} {Y1} {BP} 21
DS {X2} {Y1} {X2} {Y2} {BP} 21
DS {X2} {Y2} {X1} {Y2} {BP} 21
DS {X1} {Y2} {X1} {Y1 OFFS +} {BP} 21
DS {X1} {Y1 OFFS +} {X1 OFFS +} {Y1} {BP} 21
DC {XC} {YC} {XC RAD +} {YC} {BP} 21
{PT 4 / round @PINS}
$PAD
{PN 1 - PINS / floor @ROW}
{? ROW 0 = ROW 2 = |}Sh "{PN}" O {PW} {PL} 0 0 0
{? ROW 1 = ROW 3 = |}Sh "{PN}" O {PW} {PL} 0 0 900
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
{? ROW 0 =}Po {SH -0.5 *} {PINS -0.5 * 0.5 - PN + PP *}
{? ROW 1 =}Po {PINS -1.5 * 0.5 - PN + PP *} {SV 0.5 *}
{? ROW 2 =}Po {SH 0.5 *} {PINS 2.5 * 0.5 + PN - PP *}
{? ROW 3 =}Po {PINS 3.5 * 0.5 + PN - PP *} {SV -0.5 *}
$EndPAD
$EndMODULE {NAME}
