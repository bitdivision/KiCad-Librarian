#version 1
#brief Through-hole pin array, single row
#note Suitable for pin headers or resistor/capacitor arrays
#pins 2 3 ...
#param 5 @PT 2.54 @PP 1.6 @PW 1.6 @PL 0.2 @BP 0.65 @TS 0.15 @TP
$MODULE {NAME}
AR HDR1x_
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw HDR SIL
Op 0 0 0
At STD
{PP @BW   PT PP * @BL}
{BW 2 / @X2   X2 ~ @X1   BL 2 / @Y2   Y2 ~ @Y1}
T0 {X1 TS - BP 2 * -} 0 {TS} {TS} 900 {TP} N V 21 N "REF**"
T1 {X2 TS + BP +} 0 {TS} {TS} 900 {TP} N H 21 N "VAL**"
DS {X1} {Y1} {X2} {Y1} {BP} 21
DS {X1} {Y2} {X2} {Y2} {BP} 21
DS {X1} {Y1} {X1} {Y2} {BP} 21
DS {X2} {Y1} {X2} {Y2} {BP} 21
{?PT 2 >}DS {X1} {Y1 PP +} {X2} {Y1 PP +} {BP} 21
$PAD
{? PN 1 =}Sh "{PN}" R {PW} {PL} 0 0 0
{? PN 1 >}Sh "{PN}" C {PW} {PL} 0 0 0
Dr 0.8 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 0 {PT -0.5 * 0.5 - PN + PP *}
$EndPAD
$EndMODULE {NAME}
