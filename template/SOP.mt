#version 1
#brief Small Outline w/ Pins
#note Suitable for SOIC and SSOP parts. Without exposed pad.
#pins 8 10 ...
#param 8 @PT 1.27 @PP 5.6 @SH 1.75 @PW 0.65 @PL 3.6 @BW 5.0 @BL 0.2 @BP 0.65 @TS 0.1 @TP
$MODULE {NAME}
AR SOP
Po 0 0 0 15 00000000 00000000 ~~
Li {NAME}
Cd {DESCR}
Kw SOIC SSOP TSSOP
Op 0 0 0
At SMD
T0 0 {BL 2 / TS + ~} {TS} {TS} 0 {TP} N V 21 N "REF**"
T1 0 {BL 2 / TS +} {TS} {TS} 0 {TP} N H 21 N "VAL**"
T2 {BW 2 / ~ BP 2 * -} {PT -0.25 * 0.5 + PP * PL - BP -} {BP 2 *} {BP 2 *} 0 {TP} N H 21 N "○"
DS {BW 2 / ~} {BL 2 / ~} {BW 2 /} {BL 2 / ~} {BP} 21
DS {BW 2 /} {BL 2 / ~} {BW 2 /} {BL 2 /} {BP} 21
DS {BW 2 /} {BL 2 /} {BW 2 / ~} {BL 2 /} {BP} 21
DS {BW 2 / ~} {BL 2 /} {BW 2 / ~} {BL 2 / ~} {BP} 21
{BW 4 / 0.1 - 0.5 min @RAD   0.25 @OFFS   RAD BW 2 / - OFFS + @XC   RAD BL 2 / - OFFS + @YC}
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
