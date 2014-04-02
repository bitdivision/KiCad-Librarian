#version 1
#brief SMD pin array, dual row
#note Standard dual row pin headers, SMD version
#pins 4 6 ...
#param 8 @PT 2.54 @PP 5.08 @SH 3.0 @PW 1.4 @PL 5.08 @BW 10.16 @BL 0.2 @BP 0.65 @TS 0.15 @TP 0.2 @STP
$MODULE {NAME}
AR HDR2x_SMD
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw HDR DIL
Op 0 0 0
At SMD
{BW 2 / @X2   X2 ~ @X1   BL 2 / @Y2   Y2 ~ @Y1}
T0 {SH PW + -0.5 * TS - BP -} 0 {TS} {TS} 900 {TP} N V 21 N "REF**"
T1 {SH PW + 0.5 * TS +} 0 {TS} {TS} 900 {TP} N H 21 N "VAL**"
T2 {BW 2 / ~ BP 2 * -} {PT -0.25 * 0.5 + PP * PL - BP -} {BP 2 *} {BP 2 *} 0 {TP} N H 21 N "○"
{BL 2 / PT 4 / 0.5 - PP * - PL 2 / - STP - @OFFS}
DS {X1} {Y1} {X2} {Y1} {BP} 21
DS {X1} {Y1} {X1} {Y1 OFFS +} {BP} 21
DS {X2} {Y1} {X2} {Y1 OFFS +} {BP} 21
DS {X1} {Y2} {X2} {Y2} {BP} 21
DS {X1} {Y2} {X1} {Y2 OFFS -} {BP} 21
DS {X2} {Y2} {X2} {Y2 OFFS -} {BP} 21
$PAD
Sh "{PN}" O {PW} {PL} 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
{? PN odd}Po {SH 2 / ~} {PT -0.25 * 0.5 - PN 2 / round + PP *}
{? PN even}Po {SH 2 /} {PT -0.25 * 0.5 - PN 2 / round + PP *}
$EndPAD
$EndMODULE {NAME}
