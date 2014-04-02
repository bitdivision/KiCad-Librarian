#version 1
#brief Through-hole DIP
#note Normal and wide versions.
#pins 8 10 ...
#param 8 @PT 2.54 @PP 7.62 @SH 1.6 @PW 1.6 @PL 5.7 @BW 9.8 @BL 0.2 @BP 0.65 @TS 0.1 @TP
$MODULE {NAME}
AR DIP
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw DIP
Op 0 0 0
At STD
T0 0 {BL 2 / TS + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {BL 2 / TS +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
DS {BW 2 / ~} {BL 2 / ~} {BW 2 /} {BL 2 / ~} {BP} 21
DS {BW 2 /} {BL 2 / ~} {BW 2 /} {BL 2 /} {BP} 21
DS {BW 2 /} {BL 2 /} {BW 2 / ~} {BL 2 /} {BP} 21
DS {BW 2 / ~} {BL 2 /} {BW 2 / ~} {BL 2 / ~} {BP} 21
{BW 4 / 0.1 - 0.5 min @RAD   0.5 @OFFS   RAD BW 2 / - OFFS + @XC   RAD BL 2 / - OFFS + @YC}
DC {XC} {YC} {XC RAD +} {YC} {BP} 21
$PAD
{? PN 1 =}Sh "{PN}" R {PW} {PL} 0 0 0
{? PN 1 >}Sh "{PN}" C {PW} {PL} 0 0 0
Dr 0.8 0 0
At STD N 00E0FFFF
Ne 0 ""
{? PN PT 2 / 1 + <}Po {SH 2 / ~} {PT -0.25 * 0.5 - PN + PP *}
{? PN PT 2 / >}Po {SH 2 /} {PT 0.75 * 0.5 + PN - PP *}
$EndPAD
$EndMODULE {NAME}
