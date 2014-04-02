#version 1
#brief Small Outline, No pins
#note Suitable for SON and WSON parts. Without exposed pad.
#pins 8 10 ...
#param 8 @PT 0.65 @PP 2.75 @SH 0.7 @PW 0.35 @PL 2.85 @BW 3.1 @BL 0.2 @STP 0.2 @BP 0.65 @TS 0.1 @TP
$MODULE {NAME}
AR SON
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw SON
Op 0 0 0
At SMD
T0 0 {BL 2 / TS + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {BL 2 / TS +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
T2 {BW 2 / ~ BP 2 * -} {PT -0.25 * 0.5 + PP * PL - BP -} {BP 2 *} {BP 2 *} 0 {TP} N H 21 N "○"
{BW 2 / @X2   X2 ~ @X1   BL 2 / @Y2   Y2 ~ @Y1}
{BL PL - 2 / PT 4 / 0.5 - PP * - STP - 0 max @SEGM} #SEGM must not be less than zero
DS {X1} {Y1} {X2} {Y1} {BP} 21
DS {X1} {Y2} {X2} {Y2} {BP} 21
DS {X1} {Y1} {X1} {Y1 SEGM +} {BP} 21
DS {X1} {Y2} {X1} {Y2 SEGM -} {BP} 21
DS {X2} {Y1} {X2} {Y1 SEGM +} {BP} 21
DS {X2} {Y2} {X2} {Y2 SEGM -} {BP} 21
{PL 0.4 * @RAD   RAD SH PW - 2 / - STP + @XC   RAD BL 2 / - STP + @YC}
DC {XC} {YC} {XC RAD +} {YC} {BP} 21
$PAD
Sh "{PN}" O {PW} {PL} 0 0 0
Dr 0 0 0
At SMD N 00888000
Ne 0 ""
{? PN PT 2 / 1 + <}Po {SH 2 / ~} {PT -0.25 * 0.5 - PN + PP *}
{? PN PT 2 / >}Po {SH 2 /} {PT 0.75 * 0.5 + PN - PP *}
$EndPAD
$EndMODULE {NAME}
