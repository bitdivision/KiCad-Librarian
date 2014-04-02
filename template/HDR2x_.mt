#version 1
#brief Through-hole pin array, dual row
#note Standard dual row pin headers (use HDRSIL for 2-pin header)
#pins 4 6 ...
#param 8 @PT 2.54 @PP 2.54 @SH 1.6 @PW 1.6 @PL 0.2 @BP 0.65 @TS 0.15 @TP 5.08 @BW 10.16 @BL
$MODULE {NAME}
AR HDR2x_
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw HDR DIL
Op 0 0 0
At STD
{BW 2 / @X2   X2 ~ @X1   BL 2 / @Y2   Y2 ~ @Y1}
T0 {X1 TS - BP 2 * -} 0 {TS} {TS} 900 {TP} N V 21 N "REF**"
T1 {X2 TS + BP +} 0 {TS} {TS} 900 {TP} N H 21 N "VAL**"
DS {X1} {Y1} {X2} {Y1} {BP} 21
DS {X1} {Y2} {X2} {Y2} {BP} 21
DS {X1} {Y1} {X1} {Y2} {BP} 21
DS {X2} {Y1} {X2} {Y2} {BP} 21
{PT -0.25 * 1 + PP * @YP}
DS {X1} {YP} 0 {YP} {BP} 21
DS 0 {Y1} 0 {YP} {BP} 21
$PAD
{? PN 1 =}Sh "{PN}" R {PW} {PL} 0 0 0
{? PN 1 >}Sh "{PN}" C {PW} {PL} 0 0 0
Dr 0.8 0 0
At STD N 00E0FFFF
Ne 0 ""
{? PN odd}Po {SH 2 / ~} {PT -0.25 * 0.5 - PN 2 / round + PP *}
{? PN even}Po {SH 2 /} {PT -0.25 * 0.5 - PN 2 / round + PP *}
$EndPAD
$EndMODULE {NAME}
